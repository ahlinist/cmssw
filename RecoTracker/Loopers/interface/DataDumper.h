#ifndef Looper_DataDumper_H
#define Looper_DataDumper_H

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TAxis.h"


class fastRecHit {
public:
  fastRecHit(TransientTrackingRecHit::RecHitPointer h,
	     GlobalPoint & origin,
	     bool single=false );

  float & v0(){    return v0_;  }
  float & v1(){    return v1_;  }
  std::string print(){
    std::stringstream ss;
    ss<<"["<<v0_<<" ~ 2*R="<<2./v0_<<" , fi="<<v1_<<" , z="<< position_.z()<<", x="<<position_.x()<<", y="<<position_.y()<<"] detId: "<<id_<<" ptr "<<this<<" used "<<used_<<"\n";
    return ss.str();
  }

  // private:
  TransientTrackingRecHit::RecHitPointer hit_;
  GlobalPoint position_;
  uint id_;

  float v0_,v1_;
  bool single_;
  bool used_;
};

bool sameDetId(fastRecHit * h1, fastRecHit * h2);
bool sortByDetId( fastRecHit * h1, fastRecHit * h2);
bool sortByZ( fastRecHit * h1, fastRecHit * h2);

class aCell{
public:
  aCell();
  void orderInZ();
  int unique();
  uint countUsable();
  uint count() const {   return elements_.size();  }
  void increment( fastRecHit * e, 
		  bool upLeg,
		  const float & annularCut
		  );
  void center();
  void suite();
  void resuite();
  void resize();
  void reverse(){
      std::reverse(elements_.begin(),elements_.end());
      std::reverse(inCercle_.begin(),inCercle_.end());
      std::reverse(upLeg_.begin(),upLeg_.end());
  }
  bool equilibrate(uint eachSide);
  void truncateForZ(float & maxZ);
  bool isHelix() const { return isHelix_;}
  std::string printElements(uint itab=0);
  std::string print(uint itab=0);

  // private:
  std::vector< fastRecHit *> elements_;
  std::vector< bool > upLeg_;
  int i_,j_;
  float phi_,overR_,R_,x_,y_; //values from the histoset binning centers

  class fastHitInCercle{
  public:
    fastHitInCercle():
      phi(0),
      phiTurn(0),
      dPhi(0),
      x(0),
      y(0),
      r(0),
      use(true){}
    float phi,phiTurn,dPhi,x,y,r;
    bool use;
    
  };
  std::vector<fastHitInCercle> inCercle_;
  uint zcenter_;
  bool helixCache_;
  bool isHelix_;
  bool zUp_;
  bool phiUp_;

  float aveR_,aveX_,aveY_;
  // physical values after fitting
  bool calculateKinematic(double Bz);
  float px_,py_,pz_,pt_;
  float refx_,refy_,refz_;
  int charge_;

  void allHitsUsed();
};


class DataDumper {

public:
  DataDumper(edm::ParameterSet & pset);
  void resize();
  inline  int binX(float &x){return X_.FindBin(x);}
  int binY(float &y);

  inline aCell * cell(float & x, float & y){return &container_[X_.FindBin(x) + binY(y)*offset_];}
  inline aCell * cellI(int i,int j){return &container_[i+offset_*j];}

  std::string print(aCell * cell){    return cell->print();  }

  void collect( fastRecHit & hit);

  bool peakMade_;
  std::vector<aCell*>::iterator peak_begin() {
    if (!peakMade_) makePeaks();
    return peaks_.begin();
  }
  std::vector<aCell*>::iterator peak_end()   {
    if (!peakMade_) makePeaks();
    return peaks_.end();
  }

  unsigned int nPeaks() { return peaks_.size();}

  void makePeaks();

  std::map <std::string,uint> countfail_;

  bool setHelix(aCell * c,bool v, const std::string txt="");
  bool isHelix(aCell * c);

  uint cellImage(aCell * cell,std::string mark="");

  std::string image(std::string stage="");

  void setRBound(float R){
    RBound_=R;
    ioverRBound_ = X_.FindBin(1./R);
  }
  float RBound_;
  int ioverRBound_;

  std::vector<aCell> container_;
  uint minHitPerPeak_;
  int baseLineCut_;
  std::vector<aCell*> peaks_;

  bool linkPoints_;
  int edgeOff_;
  float annularCut_;
  uint symetryTopologySelection_;
  float maxZForTruncation_;
  float deltaSlopeCut_;
  float phiSlopeEpsilon_;
  float phiSpreadCut_;
  
  double maximumTime_;

  TAxis X_,Y_;
  uint offset_;

  uint tfileDir;
};

bool sortByNumber( aCell* cell1, aCell* cell2);

#endif
