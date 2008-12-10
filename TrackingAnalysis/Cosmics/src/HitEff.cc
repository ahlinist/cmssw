////////////////////////////////////////////////////////////////////////////////
// Package:          TrackingAnalysis/Cosmics
// Class:            HitEff
// Original Author:  Daniele Benedetti-INFN perugia
//   Update for P5:  Keith Ulmer--University of Colorado
//
//*************** Important Notes ***********************************
//  To run the code is necessary change the number of maxLostHit and maxConsecLostHit
//
//  replace ckfBaseTrajectoryFilterP5.filterPset.maxLostHits = 6
//  replace ckfBaseTrajectoryFilterP5.filterPset.maxConsecLostHits = 4
//  
///////////////////////////////////////////////////////////////////////////////

// system include files
#include <memory>
#include <string>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TrackingAnalysis/Cosmics/interface/HitEff.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "TrackingAnalysis/Cosmics/interface/TrajectoryAtValidHit.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/StripClusterParameterEstimator.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h" 

#include "TMath.h"
#include "TH1F.h"

//
// constructors and destructor
//

using namespace std;
HitEff::HitEff(const edm::ParameterSet& conf) : 
  conf_(conf)
{
  layers =conf_.getParameter<int>("Layer");
  DEBUG = conf_.getParameter<bool>("Debug");
}

// Virtual destructor needed.
HitEff::~HitEff() { }

void HitEff::beginJob(const edm::EventSetup& c){

  edm::Service<TFileService> fs;

  //Tree
  rechits = fs->make<TTree>("rechits","tree of recHit positions");
  rechits->Branch("RHNum",&RHNum,"RHNum/I");
  rechits->Branch("RHgpx",RHgpx,"RHgpx[RHNum]/F");
  rechits->Branch("RHgpy",RHgpy,"RHgpy[RHNum]/F");
  rechits->Branch("RHgpz",RHgpz,"RHgpz[RHNum]/F");
  rechits->Branch("RHMod",RHMod,"RHMod[RHNum]/I");
  rechits->Branch("RHLay",RHLay,"RHLay[RHNum]/i");

  rechits->Branch("TrajNum",&TrajNum,"TrajNum/I");
  rechits->Branch("Trajgpx",Trajgpx,"Trajgpx[TrajNum]/F");
  rechits->Branch("Trajgpy",Trajgpy,"Trajgpy[TrajNum]/F");
  rechits->Branch("Trajgpz",Trajgpz,"Trajgpz[TrajNum]/F");
  rechits->Branch("TrajLay",TrajLay,"TrajLay[TrajNum]/i");
  rechits->Branch("TrajMod",TrajMod,"TrajMod[TrajNum]/I");

  traj = fs->make<TTree>("traj","tree of trajectory positions");
  traj->Branch("TrajGlbX",&TrajGlbX,"TrajGlbX/F");
  traj->Branch("TrajGlbY",&TrajGlbY,"TrajGlbY/F");
  traj->Branch("TrajGlbZ",&TrajGlbZ,"TrajGlbZ/F");
  traj->Branch("trackAngle",&trackAngle,"trackAngle/F");
  traj->Branch("TrajLocX",&TrajLocX,"TrajLocX/F");
  traj->Branch("TrajLocY",&TrajLocY,"TrajLocY/F");
  traj->Branch("TrajLocErrX",&TrajLocErrX,"TrajLocErrX/F");
  traj->Branch("TrajLocErrY",&TrajLocErrY,"TrajLocErrY/F");
  traj->Branch("ClusterLocX",&ClusterLocX,"ClusterLocX/F");
  traj->Branch("ClusterLocY",&ClusterLocY,"ClusterLocY/F");
  traj->Branch("ClusterLocErrX",&ClusterLocErrX,"ClusterLocErrX/F");
  traj->Branch("ClusterLocErrY",&ClusterLocErrY,"ClusterLocErrY/F");
  traj->Branch("ResX",&ResX,"ResX/F");
  traj->Branch("ResXSig",&ResXSig,"ResXSig/F");
  traj->Branch("ModIsBad",&ModIsBad,"ModIsBad/i");
  traj->Branch("SiStripQualBad",&SiStripQualBad,"SiStripQualBad/i");
  traj->Branch("rhX",RHX,"RHX[500]/F");
  traj->Branch("rhY",RHY,"RHY[500]/F");
  traj->Branch("rhZ",RHZ,"RHZ[500]/F");
  traj->Branch("rhID",RHID,"RHID[500]/F");
  traj->Branch("Id",&Id,"Id/i");
  traj->Branch("run",&run,"run/i");
  traj->Branch("event",&event,"event/i");
  traj->Branch("layer",&layers,"layer/i");

  events = 0;
  EventTrackCKF = 0;
  
}


void HitEff::analyze(const edm::Event& e, const edm::EventSetup& es){

  //  bool DEBUG = false;

  if (DEBUG)  cout << "beginning analyze from HitEff" << endl;

  using namespace edm;
  // Step A: Get Inputs 

  int run_nr = e.id().run();
  int ev_nr = e.id().event();

  //CombinatorialSeed
  edm::Handle<TrajectorySeedCollection> seedcollCKF;
  edm::InputTag seedTagCKF = conf_.getParameter<edm::InputTag>("combinatorialSeeds");
  e.getByLabel(seedTagCKF,seedcollCKF);
  
  //CombinatoriaTrack
  edm::Handle<reco::TrackCollection> trackCollectionCKF;
  edm::InputTag TkTagCKF = conf_.getParameter<edm::InputTag>("combinatorialTracks");
  e.getByLabel(TkTagCKF,trackCollectionCKF);
  
  edm::Handle<std::vector<Trajectory> > TrajectoryCollectionCKF;
  edm::InputTag TkTrajCKF = conf_.getParameter<edm::InputTag>("trajectories");
  e.getByLabel(TkTrajCKF,TrajectoryCollectionCKF);
  
  // RecHit
  //rphi
  edm::Handle<SiStripRecHit2DCollection> rphirecHits;
  edm::InputTag rphirecHitsTag = conf_.getParameter<edm::InputTag>("rphirecHits");
  e.getByLabel( rphirecHitsTag ,rphirecHits);

  //stereo
  edm::Handle<SiStripRecHit2DCollection> stereorecHits;
  edm::InputTag stereorecHitsTag = conf_.getParameter<edm::InputTag>("stereorecHits");
  e.getByLabel( stereorecHitsTag ,stereorecHits);

  // Clusters
  // get the SiStripClusters from the event
  edm::Handle< edmNew::DetSetVector<SiStripCluster> > theClusters;
  e.getByLabel("siStripClusters", theClusters);

  //get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  const TrackerGeometry * tkgeom=&(* tracker);

  //get Cluster Parameter Estimator
  //std::string cpe = conf_.getParameter<std::string>("StripCPE");
  edm::ESHandle<StripClusterParameterEstimator> parameterestimator;
  es.get<TkStripCPERecord>().get("StripCPEfromTrackAngle", parameterestimator); 
  const StripClusterParameterEstimator &stripcpe(*parameterestimator);

  // get the SiStripQuality records
  edm::ESHandle<SiStripQuality> SiStripQuality_;
  es.get<SiStripQualityRcd>().get(SiStripQuality_);

  edm::ESHandle<SiStripQuality> SiStripQualityForCluster_;
  es.get<SiStripQualityRcd>().get("forCluster",SiStripQualityForCluster_);

  // take from eventSetup the SiStripDetCabling object
  edm::ESHandle<SiStripDetCabling> SiStripDetCabling_;
  es.get<SiStripDetCablingRcd>().get(SiStripDetCabling_);  

  theAngleFinder = new TrackLocalAngle( tracker.product() );
  
  
  events++;
  
  
  for (int i=0;i<500;i++){
    RHgpx[i] = 0.;
    RHgpy[i] = 0.;
    RHgpz[i] = 0.;
    RHMod[i] = 0;
    RHLay[i] = 0;
    Trajgpx[i] = 0.;
    Trajgpy[i] = 0.;
    Trajgpz[i] = 0.;
    TrajLay[i] = 0;
    TrajMod[i] = 0;
  }
  
  // *************** SiStripCluster Collection
  const edmNew::DetSetVector<SiStripCluster>& input = *theClusters;

  int ContRH = 0;
  for (edmNew::DetSetVector<SiStripCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++) {
    // DSViter is a vector of SiStripClusters located on a single module
    unsigned int ClusterId = DSViter->id();
    DetId ClusterDetId(ClusterId);
    const StripGeomDetUnit * stripdet=(const StripGeomDetUnit*)tkgeom->idToDetUnit(ClusterDetId);
    
    edmNew::DetSet<SiStripCluster>::const_iterator begin=DSViter->begin();
    edmNew::DetSet<SiStripCluster>::const_iterator end  =DSViter->end();
    for(edmNew::DetSet<SiStripCluster>::const_iterator iter=begin;iter!=end;++iter) {
      //iter is a single SiStripCluster
      StripClusterParameterEstimator::LocalValues parameters=stripcpe.localParameters(*iter,*stripdet);

      const Surface* surface;
      surface = &(tracker->idToDet(ClusterDetId)->surface());
      LocalPoint lp = parameters.first;
      GlobalPoint gp = surface->toGlobal(lp);

      uint layer=0;
      unsigned int subid=ClusterDetId.subdetId();

      if    (subid==  StripSubdetector::TIB) {
	layer = TIBDetId(ClusterDetId).layer();
      }
      if    (subid==  StripSubdetector::TOB) {
	layer = TOBDetId(ClusterDetId).layer() + 4;
      }
      if    (subid==  StripSubdetector::TID) {
	layer = TIDDetId(ClusterDetId).wheel() + 10;
      }
      if    (subid==  StripSubdetector::TEC) {
	layer = TECDetId(ClusterDetId).wheel() + 13;
      }

      if (ContRH < 500){
	RHgpx[ContRH] = gp.x();
	RHgpy[ContRH] = gp.y();
	RHgpz[ContRH] = gp.z();
	RHMod[ContRH] = ClusterId;
	RHLay[ContRH] = layer;
      }
      ContRH++;

      if (DEBUG)      cout << "Found hit in cluster collection layer = " << layer << " with id = " << ClusterId << "   local X position = " << lp.x() << " +- " << sqrt(parameters.second.xx()) << "   matched/stereo/rphi = " << ((ClusterId & 0x3)==0) << "/" << ((ClusterId & 0x3)==1) << "/" << ((ClusterId & 0x3)==2) << endl;

    }
  }
	

  // ************ Stereo RecHit Collection
  SiStripRecHit2DCollection::const_iterator istripSt;
  SiStripRecHit2DCollection stripcollSt=*stereorecHits;
  
  if (stereorecHits.product()->size() > 0) {
    if (DEBUG) cout << "size of stereo recHit collection = " << stereorecHits.product()->size() << endl;
    for(istripSt=stripcollSt.begin();istripSt!=stripcollSt.end();istripSt++) {
      //const SiStripRecHit2D *hitSt = &*(istripSt);
      //do something with stereo rechits if desired
    }
  } 
  // ************ END Stereo RecHit Collection
  
  // ************ RPhi RecHit Collection
  SiStripRecHit2DCollection::const_iterator istrip;
  SiStripRecHit2DCollection stripcoll=*rphirecHits;

  if (rphirecHits.product()->size() > 0) {
    if (DEBUG) cout << "size of rphi recHit collection = " << rphirecHits.product()->size() << endl;
    for(istrip=stripcoll.begin();istrip!=stripcoll.end();istrip++) {

      /*
      bool FakeArt = false;
      if (FakeArt == false) {
	vSiStripRecHit2D.push_back(hit);
      }
      else {
	// Simulation of Artificial RPhi Inefficiency in layers, modules
	if(layer  == layers ) {
	  double prob;
	  prob = RanGen2.Rndm();
	  if (prob > 0.05) {
	    vSiStripRecHit2D.push_back(hit);
	  }
	}
	else {
	  vSiStripRecHit2D.push_back(hit);
	}
      }
      */
    }
  } //end fill rphi vector
  
  RHNum = ContRH;
  if (RHNum > 500 ) RHNum = 499;
  
  // Tracking 
  const   reco::TrackCollection *tracksCKF=trackCollectionCKF.product();
  if (DEBUG)  cout << "number ckf tracks found = " << tracksCKF->size() << endl;
  if (tracksCKF->size() == 1 ){
  if (DEBUG)    cout << "starting checking good single track event" << endl;
    reco::TrackCollection::const_iterator iCKF=trackCollectionCKF.product()->begin();
    EventTrackCKF++;  
    
    int ContTraj = 0;      
      
    const Trajectory traject = *(TrajectoryCollectionCKF.product()->begin());
    std::vector<TrajectoryMeasurement> TMeas=traject.measurements();
    vector<TrajectoryMeasurement>::iterator itm;
    double xloc = 0.;
    double yloc = 0.;
    double xErr = 0.;
    double yErr = 0.;
    double xglob,yglob,zglob;
    
    if (DEBUG)      cout << "lenght of TMeas = " << TMeas.size() << endl;      
    for (itm=TMeas.begin();itm!=TMeas.end();itm++){
      ConstReferenceCountingPointer<TransientTrackingRecHit> theInHit;
      theInHit = (*itm).recHit();
      
      if(DEBUG) cout << "theInHit is valid = " << theInHit->isValid() << endl;
      
      uint iidd = theInHit->geographicalId().rawId();
      
      StripSubdetector strip=StripSubdetector(iidd);
      unsigned int subid=strip.subdetId();
      uint TKlayers = 0;
      if (subid ==  StripSubdetector::TIB) { 
	TIBDetId tibid(iidd);
	TKlayers = tibid.layer();
      }
      if (subid ==  StripSubdetector::TOB) { 
	TOBDetId tobid(iidd);
	TKlayers = tobid.layer() + 4 ; 
      }
      if (subid ==  StripSubdetector::TID) { 
	TIDDetId tidid(iidd);
	TKlayers = tidid.wheel() + 10;
      }
      if (subid ==  StripSubdetector::TEC) { 
	TECDetId tecid(iidd);
	TKlayers = tecid.wheel() + 13 ; 
      }
      if (DEBUG)	cout << "TKlayer from trajectory: " << TKlayers << "  from module = " << iidd <<  "   matched/stereo/rphi = " << ((iidd & 0x3)==0) << "/" << ((iidd & 0x3)==1) << "/" << ((iidd & 0x3)==2) << endl;
      
      // Modules Constraints
      TrajectoryAtValidHit*  TM = new TrajectoryAtValidHit(*itm,tkgeom);
      
      // --> Get trajectory from combinatedState 
      iidd = TM->monodet_id();
      
      xloc = TM->localX();
      yloc = TM->localY();
      
      xErr =  TM->localErrorX();
      yErr =  TM->localErrorY();
      
      xglob = TM->globalX();
      yglob = TM->globalY();
      zglob = TM->globalZ();
      
      float YINTCons = 0.;
      if(TKlayers < 5) {
	YINTCons = -1.;
      }
      if(TKlayers >= 5) {
	YINTCons = 1.0;  // Bonding Region
      }
      
      
      // Tracking position (just to draw) 
      if (ContTraj < 500) {
	Trajgpx[ContTraj] = xglob;
	Trajgpy[ContTraj] = yglob;
	Trajgpz[ContTraj] = zglob;
	TrajLay[ContTraj] = TKlayers;
	TrajMod[ContTraj] = iidd;
      }
      ContTraj++;
    
	
      if (layers == TKlayers) {   // Look at the layer not used to reconstruct the track
	  
	if (DEBUG)	  cout << "Looking at layer under study" << endl;
	TrajGlbX = 0.0; TrajGlbY = 0.0; TrajGlbZ = 0.0; trackAngle = 0.0; ModIsBad = 0; Id = 0; SiStripQualBad = 0; 
	run = 0; event = 0; TrajLocX = 0.0; TrajLocY = 0.0; TrajLocErrX = 0.0; TrajLocErrY = 0.0; ResX = 0.0; ResXSig = 0.0;
	ClusterLocX = 0.0; ClusterLocY = 0.0; ClusterLocErrX = 0.0; ClusterLocErrY = 0.0;
	for (int i=0;i<500;i++){
	  RHX[i] = 0; RHY[i] = 0; RHZ[i] = 0; RHID[i] = 0;
	}
	// RPhi RecHit Efficiency 
	
	if (input.size() > 0 ) {  
	  if (DEBUG) cout << "Checking clusters with size = " << input.size() << endl;
	  int nClusters = 0;
	  std::vector< std::vector<float> > VCluster_info; //fill with X residual, X residual pull, local X, sig(X), local Y, sig(Y)
	  for (edmNew::DetSetVector<SiStripCluster>::const_iterator DSViter = input.begin(); DSViter != input.end(); DSViter++) {
	    // DSViter is a vector of SiStripClusters located on a single module
	    if (DEBUG)      cout << "the ID from the DSViter = " << DSViter->id() << endl; 
	    unsigned int ClusterId = DSViter->id();
	    if (ClusterId == iidd) {
	      DetId ClusterDetId(ClusterId);
	      const StripGeomDetUnit * stripdet=(const StripGeomDetUnit*)tkgeom->idToDetUnit(ClusterDetId);
	      
	      edmNew::DetSet<SiStripCluster>::const_iterator begin=DSViter->begin();
	      edmNew::DetSet<SiStripCluster>::const_iterator end  =DSViter->end();
	      for(edmNew::DetSet<SiStripCluster>::const_iterator iter=begin;iter!=end;++iter) {
		//iter is a single SiStripCluster
		StripClusterParameterEstimator::LocalValues parameters=stripcpe.localParameters(*iter,*stripdet);
		float res = (parameters.first.x() - xloc);
		float sigma = checkConsistency(parameters , xloc, xErr);
		//float cluster_info[6] = {res, sigma, parameters.first.x(), sqrt(parameters.second.xx()), parameters.first.y(), sqrt(parameters.second.yy())};
		std::vector< float > cluster_info;
		cluster_info.push_back(res); 
		cluster_info.push_back(sigma);
		cluster_info.push_back(parameters.first.x()); 
		cluster_info.push_back(sqrt(parameters.second.xx()));
		cluster_info.push_back(parameters.first.y());
		cluster_info.push_back(sqrt(parameters.second.yy()));
		VCluster_info.push_back(cluster_info);
		nClusters++;
		if (DEBUG) cout << "Have ID match. residual = " << VCluster_info.back()[0] << "  res sigma = " << VCluster_info.back()[1] << endl;
		cout << "trajectory measurement compatability estimate = " << (*itm).estimate() << endl;
		cout << "hit position = " << parameters.first.x() << "  hit error = " << sqrt(parameters.second.xx()) << "  trajectory position = " << xloc << "  traj error = " << xErr << endl;
	      }
	    }
	  }
	  float FinalResSig = 1000.0;
	  float FinalCluster[6];
	  if (nClusters > 0) {
	    if (nClusters > 1) {
	      //get the smallest one
	      vector< vector<float> >::iterator ires;
	      for (ires=VCluster_info.begin(); ires!=VCluster_info.end(); ires++){
		if ( abs((*ires)[1]) < abs(FinalResSig)) {
		  FinalResSig = (*ires)[1];
		  for (int i = 0; i<6; i++) {
		    cout << "filling final cluster. i = " << i << " before fill FinalCluster[i]=" << FinalCluster[i] << " and (*ires)[i] =" << (*ires)[i] << endl;
		    FinalCluster[i] = (*ires)[i];
		    cout << "filling final cluster. i = " << i << " after fill FinalCluster[i]=" << FinalCluster[i] << " and (*ires)[i] =" << (*ires)[i] << endl;
		  }
		}
		if (DEBUG) cout << "iresidual = " << (*ires)[0] << "  isigma = " << (*ires)[1] << "  and FinalRes = " << FinalCluster[0] << endl;
	      }
	    }
	    else {
	      FinalResSig = VCluster_info.at(0)[1];
	      for (int i = 0; i<6; i++) {
		FinalCluster[i] = VCluster_info.at(0)[i];
	      }
	    }
	  }
	  
	  if (DEBUG) cout << "Final residual in X = " << FinalCluster[0] << "+-" << (FinalCluster[0]/FinalResSig) << endl;
	  if (DEBUG) cout << "Checking location of trajectory: abs(yloc) = " << abs(yloc) << "  abs(xloc) = " << abs(xloc) << endl;
	  if (DEBUG) cout << "Checking location of cluster hit: yloc = " << FinalCluster[4] << "+-" << FinalCluster[5] << "  xloc = " << FinalCluster[2] << "+-" << FinalCluster[3] << endl;
	  
	  // YINTCons is the bonding region absolute cut, leave this for now
	  // xloc cut is to keep out tracks that don't pass within the study acceptance region
	  
	  if ( abs(yloc) > YINTCons && abs(xloc) < 1500 ) { 
	    
	    // fill ntuple varibles
	    //get global position from module id number iidd
	    TrajGlbX = xglob;
	    TrajGlbY = yglob;
	    TrajGlbZ = zglob;	  
	    Id = iidd;
	    run = run_nr;
	    event = ev_nr;
	    //if ( SiStripQuality_->IsModuleBad(iidd) ) {
	    if ( SiStripQuality_->getBadApvs(iidd)!=0 ) {
	      SiStripQualBad = 1; 
	      if(DEBUG) cout << "strip is bad from SiStripQuality" << endl;
	    } else {
	      SiStripQualBad = 0; 
	      if(DEBUG) cout << "strip is good from SiStripQuality" << endl;
	    }

	    //if ( SiStripQualityForCluster_->getBadApvs(iidd)==0 ) {
	    
	    if(DEBUG) cout << "Known bad modules from Dom IsUsable = " << SiStripQuality_->IsModuleUsable(436281908) << "  and IsBad = " << SiStripQuality_->IsModuleBad(436281908) << "  and getBadAPvs = " << SiStripQuality_->getBadApvs(436281908) << endl;
	    
	    std::pair<float,float> monoAng = theAngleFinder->findtrackangle(*itm);
	    trackAngle = monoAng.first;
	    TrajLocX = xloc;
	    TrajLocY = yloc;
	    TrajLocErrX = xErr;
	    TrajLocErrY = yErr;
	    ResX = FinalCluster[0];
	    ResXSig = FinalResSig;
	    if (FinalResSig != FinalCluster[1]) cout << "Problem with best cluster selection" << endl;
	    ClusterLocX = FinalCluster[2];
	    ClusterLocY = FinalCluster[4];
	    ClusterLocErrX = FinalCluster[3];
	    ClusterLocErrY = FinalCluster[5];
	    int j = 0;
	    for (int i = 0; i < 500; i++){
	      if (RHLay[i] == layers){
		RHX[j] = RHgpx[i];
		RHY[j] = RHgpy[i];
		RHZ[j] = RHgpz[i];
		RHID[j] = RHMod[i];
		j++;
	      }
	    }
	    
	    if (DEBUG)	      cout << "before check good" << endl;
	    
	    if ( FinalResSig < 999.0) {  //could make requirement on track/hit consistency, but for
	      //now take anything with a hit on the module
	      cout << "hit being counted as good" << endl;
	      ModIsBad = 0;
	      traj->Fill();
	    }
	    else {
	      cout << "hit being counted as bad   ######### Invalid RPhi FinalResX " << FinalCluster[0] << " FinalRecHit " << 
		iidd << "   TKlayers  "  <<  TKlayers  << " xloc " <<  xloc << " yloc  " << yloc << " module " << iidd << 
		"   matched/stereo/rphi = " << ((iidd & 0x3)==0) << "/" << ((iidd & 0x3)==1) << "/" << ((iidd & 0x3)==2) << endl;
	      ModIsBad = 1;
	      traj->Fill();
	      
	      cout << " RPhi Error " << sqrt(xErr*xErr + yErr*yErr) << " ErrorX " << xErr << " yErr " <<  yErr <<  endl;
	    } if (DEBUG) cout << "after check good" << endl;
	  } if (DEBUG) cout << "after good location check" << endl;
	} if (DEBUG) cout << "after list of clusters" << endl;
      } if (DEBUG) cout << "After layers=TKLayers if" << endl;
    } if (DEBUG) cout << "end TMeasurement loop" << endl;
    TrajNum = ContTraj;
    if (TrajNum > 500 ) TrajNum = 499;
    rechits->Fill();   // Fill the tree only if there are tracks. 
  }
}

void HitEff::endJob(){
  rechits->GetDirectory()->cd();
  rechits->Write();
  traj->GetDirectory()->cd();
  traj->Write();  
  
  cout << " Events Analysed             " <<  events          << endl;
  cout << " Number Of Tracked events    " <<  EventTrackCKF   << endl;
}

double HitEff::checkConsistency(StripClusterParameterEstimator::LocalValues parameters, double xx, double xerr) {
  double error = sqrt(parameters.second.xx() + xerr*xerr);
  double separation = abs(parameters.first.x() - xx);
  double consistency = separation/error;
  return consistency;
}

double HitEff::checkConsistency(const SiStripRecHit2D* rechit, double xx, double xerr)
{
  double error = sqrt(rechit->localPositionError().xx() + xerr*xerr);
  double separation = abs(rechit->localPosition().x() - xx);
  double consistency = separation/error;
  return consistency;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HitEff);
