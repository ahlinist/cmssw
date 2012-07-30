#include "RecoTracker/Loopers/interface/DataDumper.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TH2F.h"
#include "TGraph.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <fstream>
#include <set>
#include "TStopwatch.h"

bool sameDetId(fastRecHit * h1, fastRecHit * h2){ return (h1->id_ == h2->id_); }
bool sortByDetId( fastRecHit * h1, fastRecHit * h2){return (h1->id_ > h2->id_);}
bool sortByZ( fastRecHit * h1, fastRecHit * h2){return (h1->position_.z() < h2->position_.z());}

bool sortByNumber (aCell* cell1, aCell* cell2){ return cell1->count() > cell2->count(); }

fastRecHit::fastRecHit(TransientTrackingRecHit::RecHitPointer h,
		       GlobalPoint & origin,
		       bool single):
  hit_(h)
  ,id_(h->geographicalId())
  ,single_(single)
  ,used_(false)
{
  //the origine is ~the BS, and therefore the BS ordinate is (0,0,0)
    position_=GlobalPoint((h->globalPosition()-origin).basicVector());
    v0_=2./position_.perp();
    v1_=position_.phi();
}

aCell::aCell(){ 
    elements_.reserve(30); 
    upLeg_.reserve(30);
    inCercle_.reserve(30);
    isHelix_=false;
    helixCache_=false;
  }

void aCell::orderInZ(){
    //    LogTrace("DataDumper")<<printElements();
    std::sort(elements_.begin(),elements_.end(),sortByZ);
    //    LogTrace("DataDumper")<<printElements(1);
  }

int aCell::unique(){
    std::vector< fastRecHit *>::iterator  newEnd;
    uint s=count();
    if (s==0) return -1;
    LogDebug("Collect")<<"uniquing a cell";
    LogTrace("Collect")<<printElements();
    newEnd=std::unique(elements_.begin(),elements_.end());
    if (newEnd!=elements_.end())edm::LogError("DataDumper")<<"unique() is necessary !!!";
    elements_.resize(newEnd-elements_.begin());
    return (count()-s);
  }

uint aCell::countUsable(){ 
    uint r=0;
    for (uint iC=0;iC!=elements_.size();++iC)
      if (inCercle_[iC].use) ++r;
    return r;
  }

void aCell::increment( fastRecHit * e, 
		  bool upLeg,
		  const float & annularCut
		  ){

    // cut in RIncircle-Rhelix
    float dR=fabs(sqrt((e->position_.x() - x_)*(e->position_.x() - x_)+(e->position_.y() - y_)*(e->position_.y() - y_))
		  - R_);
    
    //to put in parameter
    if (dR > annularCut ) {
      LogDebug("Collect")<<" does not pass the annular cut.\n"<<e->print();
      return;
    }

    //    std::cout<<"[*] add pointer: "<<e<<" to cell at: "<<i_<<":"<<j_<<std::endl;
    elements_.push_back(e);
    upLeg_.push_back(upLeg);
  }
  
void aCell::center(){
    int ci=-1;
    if (elements_.size()==0) return ;
    float zcenter=fabs(elements_[0]->position_.z());        
    for (uint iC=0;iC!=count();++iC){
      if (!inCercle_[iC].use) continue;
      float az=fabs(elements_[iC]->position_.z());
      if ( az < zcenter){
	zcenter=az;
	ci=iC;
      }
    }
    zcenter_=ci;
  }

void aCell::suite(){
    if (inCercle_.size()==elements_.size())
      return;
    
    inCercle_.resize(count());
    for (uint iC=0;iC!=count();++iC){
      inCercle_[iC].use=true;
      inCercle_[iC].x=elements_[iC]->position_.x() - x_;
      inCercle_[iC].y=elements_[iC]->position_.y() - y_;
      inCercle_[iC].r=sqrt(inCercle_[iC].x*inCercle_[iC].x+
			   inCercle_[iC].y*inCercle_[iC].y);
      
      inCercle_[iC].dPhi=reco::deltaPhi(phi_,elements_[iC]->v1());
      inCercle_[iC].phi=atan2(inCercle_[iC].y,inCercle_[iC].x);
      inCercle_[iC].phiTurn=inCercle_[iC].phi;
      
    }
    center();
  }
void aCell::resuite(){
    for (uint iC=0;iC!=count();++iC)
      inCercle_[iC].phiTurn=inCercle_[iC].phi;
  }
void aCell::resize(){
    uint iIn=0;
    for (uint iC=0;iC!=count();++iC){
      //take out the one currently being not used and the one used in general
      if (inCercle_[iC].use && !elements_[iC]->used_){
	upLeg_[iIn]=upLeg_[iC];
	elements_[iIn]=elements_[iC];
	inCercle_[iIn]=inCercle_[iC];
	//if (mediatrices_.size()!=0)
	//  mediatrices_[iIn]=mediatrices_[iC];
	++iIn;
      }
    }
    upLeg_.resize(iIn);
    elements_.resize(iIn);
    inCercle_.resize(iIn);
    //if (mediatrices_.size()!=0)
    //  mediatrices_.resize(iIn);
  }

bool aCell::equilibrate(uint eachSide){
    uint nPos=0,nNeg=0;
    uint nUp=0,nDo=0;
    for (uint iC=0;iC!=count();++iC){
      if (!inCercle_[iC].use) continue;
      //count element on each side of the diameter.
      if (inCercle_[iC].dPhi<0)	++nNeg;
      else	++nPos;
      if (upLeg_[iC]) ++nUp;
      else ++nDo;
    }
    
    LogDebug("PeakFinder|Equilibrate")<<"Get n+: "<<nPos<<" n-: "<<nNeg<<" n^: " <<nUp<<"nv: "<<nDo<<" with set of fast hit :\n"<<printElements();
    
    if ( (nPos>=eachSide && nNeg>=eachSide) || (nUp>=eachSide && nDo>=eachSide))
      return true;
    else
      return false;
}

void aCell::truncateForZ(float & maxZ){
    center();
    float  zmin=elements_.front()->position_.z();
    float  zmax=elements_.back()->position_.z();
    if ( zmax > maxZ && zmin < -maxZ )
      {
	//truncate part of the set that is below/above the center
	uint middle=elements_.size()/2;
	if (zcenter_ >= middle){
	  if (zcenter_==middle){
	    if (elements_[zcenter_]->position_.z()>0) zUp_=true;
	    for (uint m=0;m<zcenter_;++m) inCercle_[m].use=false;
	  }
	  else {
	    zUp_=false;
	    for (uint m=zcenter_+1;m<elements_.size();++m) inCercle_[m].use=false;
	  }
	}
	else 
	  {
	    zUp_=true;
	    for (uint m=0;m<zcenter_;++m) inCercle_[m].use=false;
	  }
      }
     else if ( zmax > maxZ )
       zUp_=true;
     else if ( zmin < -maxZ )
       zUp_=false;
     else{
       uint middle=elements_.size()/2;
       if (zcenter_ >= middle)
	 zUp_=false;
       else
	 zUp_=true;
     }

    //actually remove all masked
    resize();
    
    // and recalculate center
    center();

    //reorder from outward to center
    if (zUp_) reverse();

    LogDebug("PeakFinder|Truncate")<<"after truncation\n"<<printElements();
  }



bool aCell::calculateKinematic( double Bz ){
  //option 1: calculate from the cell position in the histoset
  pt_=0.3 * Bz * (R_/100.); //cm->m
  
  //option 2: calculate from the first two points of the helix (plus third for charge sign)
  pt_=0.3 * Bz * (aveR_/100.); //cm->m
  
  uint secondpoint=1;
  const GlobalPoint & p0= elements_[0]->hit_->globalPosition();
  GlobalPoint p1= elements_[1]->hit_->globalPosition();
  //option 3: get a z0 in input : just change p0 and p1

  //find tow different radiuses
  while ( (p1.perp()-p0.perp())==0 and secondpoint<elements_.size()){
    p1= elements_[++secondpoint]->hit_->globalPosition();
  }
  if (secondpoint==elements_.size()){
    //something bad happened
    throw;
  }
    
  //BS = (0,0,0) in the current frame
  GlobalPoint atBeam(0,0, (p1.perp()*p0.z() - p0.perp()*p1.z()) / (p1.perp()-p0.perp()) );

  float tx,ty,t,tdir;
  tx=aveY_-atBeam.y();
  ty=atBeam.x()-aveX_;

  t=sqrt( ( tx * tx ) + ( ty * ty ) ) ;
  tx/=t;
  ty/=t;
  

  GlobalVector dist(p0-atBeam);
  tdir=(tx*dist.x()) + (ty*dist.y());
  if (tdir<0){
    tx*=-1;
    ty*=-1;
  }
   
  px_=tx*pt_;
  py_=ty*pt_;
  pz_=pt_*dist.z() / dist.perp();

  //FIXME
  //get the charge from the three point we have now
  charge_=-1;

  refx_=atBeam.x();
  refy_=atBeam.y();
  refz_=atBeam.z();
  
  
  LogDebug("PeakFinder|Kinematics")<<printKinematics();

  return true;
}


void aCell::allHitsUsed(){
  for (uint iC=0;iC!=count();++iC){ 
    elements_[iC]->used_=true;
  }
}

std::string aCell::printElements(uint itab){
    std::string tab="";
    for (uint iC=0;iC!=itab;++iC)      tab+="\t";
    std::stringstream ss;
    ss<<print(itab);


    for (uint iC=0;iC!=count();++iC){
      ss<<tab<<iC<<")";
      if (inCercle_.size()!=0)
	{
	  ss
	    <<" phiCercle="<<inCercle_[iC].phi
	    <<" phiCercl++="<<inCercle_[iC].phiTurn
	    <<" dPhi(hit,centre)="<<inCercle_[iC].dPhi;
	}
      ss<<" leg: "<<upLeg_[iC];
      if (mediatrices_.size()!=0)
	{
	  ss<<"\n"<<tab<<"\t"
	    <<" x."<<mediatrices_[iC].u<<"+"
	    <<" y."<<mediatrices_[iC].v<<"+ "
	    <<mediatrices_[iC].w<<"=0"
	    <<" inter. ("<<mediatrices_[iC].x<<", "<<mediatrices_[iC].y<<")"
	    <<" rad. "<<mediatrices_[iC].r;
	}
      ss<<"\n"<<tab<<"\t"
	<<elements_[iC]->print();
    }

    return ss.str();
  }
std::string aCell::print(uint itab){
    std::string tab="";
    for (uint iC=0;iC!=itab;++iC)      tab+="\t";
    std::stringstream ss;
    ss<<tab<<count()<<" elements at: "<<i_<<":"<<j_<<" "//<<"\n"
      <<" centered @ "<<overR_<<" ~ "<<1./overR_ <<" : "<<phi_<<"\n";
    return ss.str();
  }

std::string aCell::printKinematics(){
  std::stringstream ss;
  
  ss<<"momentum: ("
    <<px_<<", "
    <<py_<<", "
    <<pz_<<"). "
    <<"pT "<<pt_
    <<" reference point: ("
    <<refx_<<", "
    <<refy_<<", "
    <<refz_<<")";
  return ss.str();
}


DataDumper::DataDumper(edm::ParameterSet & pset){
    peakMade_=false;
    std::vector<double> in=pset.getParameter<std::vector<double> >("xAxis");
    uint nBins=in[0];
    if (pset.getParameter<bool>("invertX")){
      X_.Set(nBins,1./in[2],1./in[1]);
      //invert the x axis
      float * bins =new float[nBins+1];
      for (uint iBins=0;iBins<=nBins;++iBins){
	bins[iBins]=1./X_.GetBinUpEdge(nBins-iBins);
      }
      X_.Set(nBins,bins);
      delete bins;
    }
    else
      X_.Set(nBins,in[1],in[2]);

    Y_.Set(pset.getParameter<unsigned int>("nPhi"),0,TMath::TwoPi());
    offset_=X_.GetNbins()+2;
    minHitPerPeak_=pset.getParameter<unsigned int>("peakAbove");
    if (minHitPerPeak_ < 3)
      {
	//	this is not possible
	throw;
      }
    baseLineCut_=pset.getParameter<int>("baseLineCut");
    setRBound(pset.getParameter<double>("RBound"));
    resize();
    
    linkPoints_=pset.getParameter<bool>("linkPoints");
    edgeOff_=pset.getParameter<int>("offEdge");
    annularCut_=pset.getParameter<double>("annularCut");
    symetryTopologySelection_=pset.getParameter<unsigned int>("symetryTopologySelection");
    maxZForTruncation_=pset.getParameter<double>("maxZForTruncation");
    deltaSlopeCut_ = pset.getParameter<double>("deltaSlopeCut");
    phiSlopeEpsilon_ = pset.getParameter<double>("phiSlopeEpsilon");
    phiSpreadCut_ = pset.getParameter<double>("phiSpread");
    maximumTime_ = pset.getParameter<double>("maximumTime");
  }

void DataDumper::resize(){
    container_.resize((X_.GetNbins()+2)*(Y_.GetNbins()+2));
    for (int i=0;i<=X_.GetNbins()+1;++i){
      for (int j=0;j<=Y_.GetNbins()+1;++j){
	container_[i+offset_*j].i_=i;
	container_[i+offset_*j].j_=j;
	container_[i+offset_*j].phi_=Y_.GetBinCenter(j);
	container_[i+offset_*j].overR_=X_.GetBinCenter(i);
	container_[i+offset_*j].R_=1./X_.GetBinCenter(i);
	container_[i+offset_*j].x_=container_[i+offset_*j].R_ * cos ( container_[i+offset_*j].phi_ );
	container_[i+offset_*j].y_=container_[i+offset_*j].R_ * sin ( container_[i+offset_*j].phi_ ); 
      }}
  }

int DataDumper::binY(float &y){
    //this method actually changes the value of the float passed in argument. this is risky but what the hell !
    int b=Y_.FindBin(y);
    if (b==0) {
      y+=TMath::TwoPi();
      return binY(y);
    }
    if (b==Y_.GetNbins()+1){
      y-=TMath::TwoPi();
      return binY(y);
    }
    return b;
  }

void DataDumper::collect( fastRecHit & hit){
    aCell * seed = cell(hit.v0(),hit.v1());
    if (seed->i_<=ioverRBound_){
      LogDebug("Collect")<<"the seed is below boundary";
      return;
    }
    LogDebug("Collect")<<"starting from hit: "<<hit.print()<<" make the seed: "<<print(seed);

    //do not mark the seed as good, it's the grazing circle, and not worth adding
    if (edgeOff_)    seed->increment(&hit,true,annularCut_);
    
    float & phi0=hit.v1();
    float & overRh=hit.v0();

    aCell * previousCellUp=seed;
    aCell * previousCellDo=seed;
    
    //go in both directions
    unsigned int iCount=0;
    while (iCount++<container_.size()){
      LogTrace("Collect")<<"\tprevious cell up : "<<print(previousCellUp);
      float phiC = Y_.GetBinCenter(previousCellUp->j_+1);
      float overR = cos(phi0-phiC)*overRh;
      aCell * nextCell = cell(overR,phiC);
      LogTrace("Collect")<<"\tnext cell up is "<<print(nextCell);


      if ((seed->i_-nextCell->i_) > edgeOff_){ 
      if (nextCell->i_!=previousCellUp->i_ && linkPoints_){
	//construct interval of cells 
	float phiLowEdge = Y_.GetBinLowEdge(nextCell->j_);
	float overLowEdge = cos(phi0-phiLowEdge)*overRh;
	int ioverRcross=X_.FindBin(overLowEdge);
	LogTrace("Collect")<<"\t\t\tfilling intervals: "<<nextCell->i_<<"->"<<ioverRcross <<" @ "<<nextCell->j_<<"\n";
	for (int iLink=nextCell->i_; iLink<=ioverRcross;++iLink) cellI(iLink,nextCell->j_)->increment(&hit,true,annularCut_);
	LogTrace("Collect")<<"\t\t\tfilling intervals: "<<ioverRcross<<"->"<<previousCellUp->i_ <<"-- @ "<<previousCellUp->j_<<"\n";
	for (int iLink=ioverRcross; iLink<previousCellUp->i_;++iLink) cellI(iLink,previousCellUp->j_)->increment(&hit,true,annularCut_);
      }
      else
	nextCell->increment(&hit,true,annularCut_);
      }
      
      previousCellUp=nextCell;

      LogTrace("Collect")<<"\tprevious cell down : "<<print(previousCellDo);
      phiC = Y_.GetBinCenter(previousCellDo->j_-1);
      overR = cos(phi0-phiC)*overRh;
      nextCell = cell(overR,phiC);
      LogTrace("Collect")<<"\t\tnext cell down is "<<print(nextCell);

      if ((seed->i_-nextCell->i_) > edgeOff_){ //arbitrary
      if (nextCell->i_!=previousCellDo->i_ && linkPoints_){
	float phiUpEdge = Y_.GetBinUpEdge(nextCell->j_);
	float overUpEdge = cos(phi0-phiUpEdge)*overRh;
	int ioverRcross=X_.FindBin(overUpEdge);
	LogTrace("Collect")<<"\t\t\tfilling intervals: "<<nextCell->i_<<"->"<<ioverRcross <<" @ "<<nextCell->j_<<"\n";
	for (int iLink=nextCell->i_; iLink<=ioverRcross;++iLink) cellI(iLink,nextCell->j_)->increment(&hit,false,annularCut_);
	LogTrace("Collect")<<"\t\t\tfilling intervals: "<<ioverRcross<<"->"<<previousCellDo->i_ <<"-- @ "<<previousCellDo->j_<<"\n";
	for (int iLink=ioverRcross; iLink<previousCellDo->i_;++iLink) cellI(iLink,previousCellDo->j_)->increment(&hit,false,annularCut_);
      }
      else
	nextCell->increment(&hit,false,annularCut_);
      }

      previousCellDo=nextCell;

      //don't scan too much down in 1/R.
      if (nextCell->i_<= ioverRBound_){
	LogTrace("Collect")<<" reaching maximum radius ";
	break;
      }
      //boundary stopping condition here
      if (nextCell->i_==0 || nextCell->i_==(X_.GetNbins()+1) || nextCell->j_==0 || nextCell->j_==(Y_.GetNbins()+1)){
	LogTrace("Collect")<<" reaching a boarder"<<std::endl;
	break;}
    }
    if (iCount>=container_.size())
      edm::LogError("Collect")<<"went over board in collecting :"<<hit.print();
  }

void DataDumper::makePeaks(){
  float nTotalBins=0;
  const uint numberOfMax=5;    
  uint averageOccupancy=0;
  std::vector<uint> maxBins(numberOfMax,0);
  for(std::vector<aCell>::iterator iCell=container_.begin();
      iCell!=container_.end();++iCell){
    uint n=iCell->count();
    if (n==0) continue;
    if ( n > maxBins[0]) maxBins[0]=n;
    for (uint ib=1;ib<numberOfMax;++ib)
      if (n > maxBins[ib] && n < maxBins[ib-1])	
	maxBins[ib]=n;
    averageOccupancy+=n;
    ++nTotalBins;
  }
  averageOccupancy/=nTotalBins;
  uint baseLineCut = averageOccupancy;
  if (baseLineCut_ > 0)
    {
      if (uint(baseLineCut_)<= maxBins.back())
	baseLineCut = maxBins.back() - baseLineCut_;
      else
	baseLineCut = 0;
    }

  LogDebug("PeakFinder|CollectPeak")<<"The average occupancy of the histoset is "<<averageOccupancy<<" the "<<numberOfMax<<"th max is: "<<maxBins.back()
				    <<".\n"<<baseLineCut<<" is used as a baseline cut";

  //create an image of the container, to not sort in place. although we could in principle, since the container is not accessed anymore afterwards as is.
  std::vector<aCell*> sortedImage;
  sortedImage.reserve(container_.size());

  for(std::vector<aCell>::iterator iCell=container_.begin();
      iCell!=container_.end();++iCell){
    sortedImage.push_back( & *iCell);
  }
  
  // let's order it to start with the highest cell
  std::sort(sortedImage.begin(),sortedImage.end(),sortByNumber);
  
  uint above=0;
  std::map<uint,uint> counts;
  double timeSpent=0;
  TStopwatch myWatch;
  myWatch.Reset();
  bool abort=false;
  for(std::vector<aCell*>::iterator iiCell=sortedImage.begin();
      iiCell!=sortedImage.end();++iiCell){
    if (maximumTime_>0 && !abort && timeSpent>maximumTime_){
      edm::LogError("AbortedLooperReco")<<"it is taking too much time to compute: "<< timeSpent<<" > "<<maximumTime_<<". Let's stop !";
      abort=true;
    }

    //first remove possible duplicates
    //this is a test, which should be removed later
    //      iCell->unique();
    aCell * iCell=*iiCell;
    if (abort){
      setHelix(&*iCell,false,"Time Abort");
      continue;}
    if (iCell->count() < baseLineCut) {
      setHelix(&*iCell,false,"Under Baseline");
      continue;}
    if (iCell->count() < minHitPerPeak_) {
      setHelix(&*iCell,false,"Not enough hits");
      continue;}

    //triggers the computation
    myWatch.Start(false);
    if (isHelix(&*iCell)) 
      { 
	above++;
	counts[iCell->count()]++;
      }
    //stop the clock
    myWatch.Stop();
    LogDebug("PeakFinder|CollectPeak")<<"Timer: Real time "<< myWatch.RealTime()<<", CP time"<<myWatch.CpuTime();
    timeSpent+=myWatch.CpuTime();
  }
  myWatch.Stop();
  
  LogDebug("PeakFinder|CollectPeak")<<"pushing the peaks. computed in: "<<timeSpent;
  
  peaks_.reserve(above);
  for(std::vector<aCell*>::iterator iiCell=sortedImage.begin();
      iiCell!=sortedImage.end();++iiCell){
    
    aCell * iCell=*iiCell;
    if (iCell->isHelix()){
      LogDebug("PeakFinder|CollectPeak")<<"As a peak cell: "<< iCell->printElements();
      peaks_.push_back(&*iCell);
    }
  }

  //it's a non order list of peaks. but has been processed from largest to smallest

  LogDebug("PeakFinder|CollectPeak")<<" List of failures count:\n"<<printFail();

  peakMade_=true;
  //make plots for debugging
  LogDebug("PeakFinder|CollectPeak")<<image("endOfmakePeaks");
}


bool DataDumper::setHelix(aCell * c,bool v,
			  const std::string & txt){
    c->helixCache_=true;
    c->isHelix_=v;
    if (!c->isHelix_) 
      LogDebug("PeakFinder|CollectPeak")<<"set is not helix-like :["<<txt<<"] = "<<countfail(txt)<<"\n"<<c->print();
    return v;
  }

bool DataDumper::isHelix(aCell * c){
    if (c->helixCache_) return c->isHelix_;

    //create values with respect to the center of the cell (not using more refined value, because CPU intensive to compute anything else
    c->suite();

    LogDebug("PeakFinder|CollectPeak")<<"removing already used fast rechits\n"<<c->print();
    c->resize();

    if (c->count() < minHitPerPeak_ ){
      LogDebug("PeakFinder|CollectPeak")<<" not enough hits left after removal of already used hits";
      return setHelix(c,false,"previous hits removal");
    }

    LogDebug("PeakFinder|CollectPeak")<<"check on helix hypothesis for a cell at\n"<<c->print();

    c->orderInZ();


    if ( symetryTopologySelection_!=0 && !c->equilibrate(symetryTopologySelection_)){
      LogDebug("PeakFinder|CollectPeak")<<" not equilibrated set of hits";
      return setHelix(c,false,"not equilibrated set");
    }
      
    //mask part of the set according to max |z| : resize the set
    c->truncateForZ(maxZForTruncation_);
    
    //check on the number of hits left
    if (c->count() < minHitPerPeak_)
      {
	LogDebug("PeakFinder|CollectPeak")<<" not enough hits left after truncation for Z";
	return setHelix(c,false,"truncation in Z");
      }
    
    //initialization of phiUp
    if ((c->inCercle_.end()-2)->phi > (c->inCercle_.end()-1)->phi)	   
      c->phiUp_=true;
    else
      c->phiUp_=false;

    LogDebug("PeakFinder|CollectPeak")<<" this is :"<<(c->phiUp_? "phi up":"phi down");
	
     // --- search for valid segment
     //reset the phi in turn
    c->resuite();

    // change phiTurn under phiUp hypothesis
    uint phiSpread=0;
    float phiThisPoint=0,phiPreviousPoint=c->inCercle_[0].phi;
    float signMe=(c->phiUp_ ? 1 : -1 );
    //if phiUp, we need to remove 2Pi at each round while the next phi is bigger than the previous phi
    for (uint iC=1;iC<c->count();++iC){
      phiThisPoint=c->inCercle_[iC].phi;
      while(  (phiThisPoint - phiPreviousPoint)*signMe > 0){
	phiThisPoint-=signMe*TMath::TwoPi();
	/*
	     float dPhi = reco::deltaPhi(phiThisPoint,phiPreviousPoint);
	     if (fabs(dPhi) > phiSlopeEpsilon_) phiThisPoint=phiThisPoint - signMe*TMath::TwoPi();
	     else break;
	*/
      }
      c->inCercle_[iC].phiTurn = phiThisPoint;
      if (reco::deltaPhi(phiThisPoint,phiPreviousPoint) > phiSpreadCut_)
	++phiSpread;
      phiPreviousPoint=phiThisPoint;
    }

    if (phiSpread < 2.){
      LogDebug("PeakFinder|PhiInHelix")<<" There is not enough spread (" << phiSpread <<")in phi in this looper\n"<<c->printElements();
      return setHelix(c,false,"Phi spread");
    }
    
    LogDebug("PeakFinder|PhiInHelix")<<" done for phi turn initialisation\n"<<c->printElements();
    LogTrace("PeakFinder")<<cellImage(c,"_test");
    
    //compute the slope between two points once only, and not in a while loop
    std::vector<float> & slopes = c->slopes_;
    slopes.resize(c->count()-1);
    LogDebug("PeakFinder|SlopeCheck")<<"Calculating slopes two by two:";
    for (uint iC=0;iC<slopes.size();++iC){
      slopes[iC]=atan2(c->elements_[iC+1]->position_.z() - c->elements_[iC]->position_.z(),
		       c->inCercle_[iC+1].phiTurn - c->inCercle_[iC].phiTurn);
      LogTrace("PeakFinder|SlopeCheck")<<"Between: "<<iC<<" and "<<iC+1<<" the slope is: "<<slopes[iC];
    }
    int firstPointRemoved=0;
    int cannotRemoveMoreThan=c->count()-minHitPerPeak_;
    int point0=0;
    int point1=0;
    uint point2=0;
    float dSlope,slope0to1,slope1to2;
    
    if (!(firstPointRemoved <= cannotRemoveMoreThan ))
      LogDebug("PeakFinder|SlopeCheck")<<"Slopes are not tested";

    while (firstPointRemoved <= cannotRemoveMoreThan ){
      LogDebug("PeakFinder|SlopeCheck")<<"Test slopes starting from :"<<point0;
      point1=point0+1;
      slope0to1=slopes[point0];

      //check slopes, two by two
      for (point2=point1+1;point2<c->count();++point2,++point1){
	slope1to2=slopes[point1];
	
	dSlope=fabs(slope1to2 - slope0to1);
	if (dSlope > deltaSlopeCut_ ){
	  //skip the first point and start back
	  c->inCercle_[point0].use=false;
	  LogTrace("PeakFinder|SlopeCheck")<<"slope not compatible "<<slope1to2<<" and "<<slope0to1;
	  break; //from the for loop
	}
	else {
	  LogTrace("PeakFinder|SlopeCheck")<<"slope compatible :"<<slope1to2<<" and "<<slope0to1;
	  slope0to1=slope1to2;
	}
      }//loop on next points

      if (!c->inCercle_[point0].use){
	//means we broke out because of incompatible hit in the set
	++firstPointRemoved;
	++point0;
      }else{
	LogTrace("PeakFinder|SlopeCheck")<<"segment search ends "<<point0<<" "<<point1<<" "<<point2;
	//mask all remaining inside hits
	for (uint iC=point2;iC<c->count();++iC)  c->inCercle_[iC].use=false;
	break;
      }
      
    }// end of while

    //remove anything not used
    c->resize();
    LogTrace("PeakFinder|SlopeCheck")<<"removed hits with incompatible slopes\n"<<c->printElements();

    //are there enough hits left in the set
    if ( c->count() < minHitPerPeak_ ){
      LogDebug("PeakFinder|CollectPeak")<<" not enough hits left after slope check.\n"<<c->print();
      return setHelix(c,false,"Slope check");
    }
 
    // take out the double hits : too close in x,y and z
    for (uint iC1=1;iC1<c->count();++iC1){
      for (uint iC2=iC1+1;iC2<c->count();++iC2){
	GlobalVector dist( c->elements_[iC1]->hit_->globalPosition() - c->elements_[iC2]->hit_->globalPosition() );
	if (fabs( dist.x() ) < 0.1 )
	  c->inCercle_[iC1].use=false;
	else if (fabs( dist.y() ) < 0.1 )	  
	  c->inCercle_[iC1].use=false;
	else if (fabs( dist.z() ) < 0.1 )	 
	  c->inCercle_[iC1].use=false;
      }
    }
    c->resize();
    LogDebug("PeakFinder|DoubleHits")<<"removed double hits\n"<<c->printElements();
    if ( c->count() < minHitPerPeak_ ){
      LogDebug("PeakFinder|CollectPeak")<<" not enough hits left after removing double hits.\n"<<c->print();
      return setHelix(c,false,"Double Hits");     
    }

    // now compute better helix center from mediatrice intersections

    std::vector<aCell::Line> & mediatrices = c->mediatrices_;
    mediatrices.resize(c->count());
    GlobalPoint zero(0,0,0);
    GlobalPoint p0=zero;
    float minRatio=1000000.,rForMinRatio=-1;
    for (uint iC=0;iC<mediatrices.size();++iC){ 
      if (iC!=0)
	p0=c->elements_[iC-1]->hit_->globalPosition();
      const GlobalPoint & p1=c->elements_[iC]->hit_->globalPosition();

      //mediatrice coordinates
      aCell::Line & l1=mediatrices[iC];
      
      //mid-point
      l1.mx=(p1.x()+p0.x())/2.;
      l1.my=(p1.y()+p0.y())/2.;

      GlobalVector d(p1-p0);
      //mediatrice equation u.x+v.y+w=0
      l1.u=d.x();
      l1.v=d.y();
      l1.w=-( (l1.u*l1.mx) + (l1.v*l1.my) );

      //intersection with previous
      //protect for NAN !!!!
      if (iC!=0){
	aCell::Line & l0=mediatrices[iC-1];
	l1.y=( ( l1.u * l0.w ) - ( l1.w * l0.u  ) )  / ( ( l1.v * l0.u ) - ( l1.u * l0.v ) ) ;
	l1.x=( -l0.w -( l0.v * l1.y ) ) / l0.u; 
	//radius obtained: from center to one of the two points
	l1.r = sqrt( (l1.x-p0.x())*(l1.x-p0.x()) + (l1.y-p0.y())*(l1.y-p0.y()));
	
	l1.rr = abs( 1. - (l0.r / l1.r));
	if (l1.rr < minRatio){ 
	  minRatio=l1.rr;
	  rForMinRatio=l1.r;
	}
      }
      else{
	//set the value to the cell value
	l1.x=c->x_;
	l1.y=c->y_;
	l1.rr = 1.;
      }
    }
    
    //flag out the outliers in terms of circle radius ratio
    uint nAve=0;
    for (uint iC=0;iC<mediatrices.size();++iC){
      aCell::Line & l1=mediatrices[iC];
      if (l1.r / rForMinRatio > 2.0)
	c->inCercle_[iC].use=false;
      else{
	c->aveR_+=l1.r;
	c->aveX_+=l1.x;
	c->aveY_+=l1.y;
	nAve++;
      }
    }
    if (nAve!=0){
      c->aveR_/=nAve;
      c->aveX_/=nAve;
      c->aveY_/=nAve;
    }

    //remove anything not used
    c->resize();
    LogDebug("PeakFinder|RadiusCheck")<<"removed hits with incompatible radius with respect to intersection derived centers \n"<<c->printElements();
    
    //are there enough hits left in the set
    if ( c->count() < minHitPerPeak_ ){
      LogDebug("PeakFinder|CollectPeak")<<" not enough hits left after checking on radius spread.\n"<<c->print();
      return setHelix(c,false,"Radius Check");
    }

    LogDebug("PeakFinder|CollectPeak")<<" this is an helix."<<c->printElements();
    LogTrace("PeakFinder|CollectPeak")<<cellImage(c,"peak_");   

    //setting all surviving hits to have been used/masked already, to not re-use them anywhere else.
    c->allHitsUsed();
    return setHelix(c,true);      
  }

uint DataDumper::cellImage(aCell * cell,std::string mark){
    edm::Service<TFileService> fs;
    
    TGraph * gr = fs->make<TGraph>(cell->count());
    TGraph * grC = fs->make<TGraph>(cell->count());
    TGraph * grCt = fs->make<TGraph>(cell->count());
    TGraph * grG = fs->make<TGraph>(cell->count());
    gr->SetName(Form("aCell_zphiC_%s%d_%d",
		     mark.c_str(),cell->i_,cell->j_));
    grCt->SetName(Form("aCell_zphiCt_%s%d_%d",
		       mark.c_str(),cell->i_,cell->j_));
    grC->SetName(Form("aCell_xyC_%s%d_%d",
		      mark.c_str(),cell->i_,cell->j_));
    grG->SetName(Form("aCell_xyG_%s%d_%d",
		      mark.c_str(),cell->i_,cell->j_));
    gr->SetMarkerStyle(7);
    grCt->SetMarkerStyle(7);
    grC->SetMarkerStyle(7);
    grG->SetMarkerStyle(7);

    grG->SetPoint(0,cell->x_,cell->y_);

    for (uint iC=0;iC!=cell->count();++iC){
      gr->SetPoint(iC,
		   cell->elements_[iC]->position_.z(),
		   cell->inCercle_[iC].phiTurn);
      grCt->SetPoint(iC,
		     cell->elements_[iC]->position_.z(),
		     cell->inCercle_[iC].phi);
      grC->SetPoint(iC,
		    cell->inCercle_[iC].x,
		    cell->inCercle_[iC].y);
      grG->SetPoint(iC+1, 
		    cell->elements_[iC]->position_.x(),
		    cell->elements_[iC]->position_.y());
    }
    return cell->count();
  }
std::string DataDumper::image(std::string stage){
  static std::set<std::string> used;
  if (used.find(stage)!=used.end())
    edm::LogError("DataDumperImage")<<"duplicating image "<<stage;
  used.insert(stage);
    
    edm::Service<TFileService> fs;
    TH2F * image=fs->make<TH2F>(Form("HS_%s",stage.c_str()),"image",
				X_.GetNbins()+2,-0.5,X_.GetNbins()+1.5,
				Y_.GetNbins()+2,-0.5,Y_.GetNbins()+1.5);
    TH2F * imageD=fs->make<TH2F>(Form("HS_rphi_%s",stage.c_str()),"imageD",
				 X_.GetNbins(),X_.GetXbins()->GetArray(),
				 Y_.GetNbins(),Y_.GetXmin(),Y_.GetXmax());
    TH2F * imageH=fs->make<TH2F>(Form("HS_H_rphi_%s",stage.c_str()),"imageH",
				 X_.GetNbins(),X_.GetXbins()->GetArray(),
				 Y_.GetNbins(),Y_.GetXmin(),Y_.GetXmax());
  


    //make a picture of the container
    //    fstream ss("image.txt",fstream::out);
    //    std::stringstream ss;
    for (int j=0;j<=Y_.GetNbins()+1;++j){
      for (int i=0;i<=X_.GetNbins()+1;++i){
	const aCell * cell = cellI(i,j);

	//	ss.width(3);
	//	ss<<container_[i+offset_*j].count();
	image->SetBinContent(i,j,cell->count());

	if (i==0 || j==0 || i==X_.GetNbins()+1 || j==Y_.GetNbins()+1) continue;

	imageD->SetBinContent(i,j,cell->count());
	if (cell->count() >= minHitPerPeak_ && 
	    cell->isHelix())
	  imageH->SetBinContent(i,j,cell->count());
      }
      //      ss<<"\n";
    }
    //    return ss.str();
    //    return "look at image.txt for text details";
    return "store in TFile";
  }
