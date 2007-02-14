#ifndef RecoEgamma_MaterialConversionModules_Pi0MaterialConversion_h
#define RecoEgamma_MaterialConversionModules_Pi0MaterialConversion_h

/** \class Pi0MaterialConversion
 *  
 * Data class for pi0 conversions in material
 *
 * \author Andrew Askew, Florida State University
 *
 *
 */
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversionFwd.h"
#include <vector>


class Pi0MaterialConversion {
 public:
 
  // default constructor. It avails you not.
  Pi0MaterialConversion();
  
  // constructor from values
  Pi0MaterialConversion(double photonE, double photoneta, double photonphi,
			double ele1E, double ele1eta, double ele1phi,
			double ele2E, double ele2eta, double ele2phi,
			double RConv, double PhiConv, double coneIso, 
			double mass//, double pi0E, double pi0eta, double pi0phi
			); 
  // destructor
  virtual ~Pi0MaterialConversion();
  //Getters
  double getIso(){return coneIso_;}
  //Derived quantities from the three clusters
  double getPi0E(){return pi0energy_;}
  double getPi0phi(){return pi0phi_;}
  double getPi0eta(){return pi0eta_;}
  double getMass(){return mass_;}

  //Cluster quantities
  double getE1E(){return e1energy_;}
  double getE1phi(){return e1phi_;}
  double getE1eta(){return e1eta_;}
  double getE2E(){return e2energy_;}
  double getE2phi(){return e2phi_;}
  double getE2eta(){return e2eta_;}
  double getGamE(){return phoenergy_;}
  double getGamphi(){return phophi_;}
  double getGameta(){return phoeta_;}
  //Estimated conversion position
  double getREst(){return ConversionREst_;}
  double getPhiEst(){return ConversionPhiEst_;}
  ////////////////////////////////////////
  //Now quantities that may or may not be here
  //track1 quantities
  double getTrk1PTIS(){return track1ptIS_;}
  double getTrk1PhiIS(){return track1phiIS_;}
  double getTrk1EtaIS(){return track1etaIS_;}
  double getTrk1PTFS() {return track1ptFS_;}
  double getTrk1PhiFS(){return track1phiFS_;}
  double getTrk1EtaFS(){return track1etaFS_;}
  int getTrk1NHIT(){return nHits1_;}
  double getTrk1Chi2(){return track1chi2_;}
  double getTrk1SmallRHit(){return track1SmallRhit_;}
  //track2 quantities
  double getTrk2PTIS(){return track2ptIS_;}
  double getTrk2PhiIS(){return track2phiIS_;}
  double getTrk2EtaIS(){return track2etaIS_;}
  double getTrk2PTFS() {return track2ptFS_;}
  double getTrk2PhiFS(){return track2phiFS_;}
  double getTrk2EtaFS(){return track2etaFS_;}
  int getTrk2NHIT(){return nHits2_;}
  double getTrk2Chi2(){return track2chi2_;}
  double getTrk2SmallRHit(){return track2SmallRhit_;}
  //////////////
  //vertex quantities
  double getVtxXPos(){return VtxXpos_;}
  double getVtxYpos(){return VtxYpos_;}
  double getVtxZpos(){return VtxZpos_;}
  double getVtxRpos(){return VtxRpos_;}
  double getVtxPhipos(){return VtxPhipos_;}
  double getVtxChi2(){return VtxChi2_;}

  //Setters
  void setTrk1PTIS(double PTIS){track1ptIS_ = PTIS;}
  void setTrk1PhiIS(double PhiIS){track1phiIS_ = PhiIS;}
  void setTrk1EtaIS(double EtaIS){track1etaIS_ = EtaIS;}
  void setTrk1PTFS(double PTFS) {track1ptFS_ = PTFS;}
  void setTrk1PhiFS(double PhiFS){track1phiFS_ = PhiFS;}
  void setTrk1EtaFS(double EtaFS){track1etaFS_ = EtaFS;}
  void setTrk1NHIT(int nhit){nHits1_=nhit;}
  void setTrk1Chi2(double Chi2){track1chi2_=Chi2;}
  void setTrk1SmallRHit(double rhit){track1SmallRhit_=rhit;}
  //track2 quantities
  void setTrk2PTIS(double PTIS){track2ptIS_ = PTIS;}
  void setTrk2PhiIS(double PhiIS){track2phiIS_ = PhiIS;}
  void setTrk2EtaIS(double EtaIS){track2etaIS_ = EtaIS;}
  void setTrk2PTFS(double PTFS) {track2ptFS_ = PTFS;}
  void setTrk2PhiFS(double PhiFS){track2phiFS_ = PhiFS;}
  void setTrk2EtaFS(double EtaFS){track2etaFS_ = EtaFS;}
  void setTrk2NHIT(int nhit){nHits2_=nhit;}
  void setTrk2Chi2(double Chi2){track2chi2_=Chi2;}
  void setTrk2SmallRHit(double rhit){track2SmallRhit_=rhit;}
  //////////////
  //vertex quantities
  void setVtxXPos(double VtxX){VtxXpos_=VtxX;}
  void setVtxYpos(double VtxY){VtxYpos_=VtxY;}
  void setVtxZpos(double VtxZ){VtxZpos_=VtxZ;}
  void setVtxRpos(double VtxR){VtxRpos_=VtxR;}
  void setVtxPhipos(double VtxPhi){VtxPhipos_=VtxPhi;}
  void setVtxChi2(double VtxChi2){VtxChi2_=VtxChi2;}


 private:
  
  //////////////////// ALL CONVERSIONCANDIDATES HAVE THESE QUANTITIES
  // pi0 energy
  double mass_;
  double coneIso_;
  //pi0 candidate quantities
  //may want to add some individual shape quantities here for
  //later use.
  double pi0energy_;
  double pi0phi_;
  double pi0eta_;
  //electron quantities
  double e1energy_;
  double e1phi_;
  double e1eta_;
  double e2energy_;
  double e2phi_;
  double e2eta_;
  //photon quantities
  double phoenergy_;
  double phoeta_;
  double phophi_;
  //Estimated conversion position in R/Phi
  double ConversionREst_;
  double ConversionPhiEst_;
  ////////////////////////////////////////
  //Now quantities that may or may not be here
  //track1 quantities
  double track1ptIS_;
  double track1phiIS_;
  double track1etaIS_;
  double track1ptFS_;
  double track1phiFS_;
  double track1etaFS_;
  int nHits1_;
  double track1chi2_;
  double track1SmallRhit_;
  //track2 quantities
  double track2ptIS_;
  double track2phiIS_;
  double track2etaIS_;
  double track2ptFS_;
  double track2phiFS_;
  double track2etaFS_;
  int nHits2_;
  double track2chi2_;
  double track2SmallRhit_;

  //////////////
  //vertex quantities
  double VtxXpos_;
  double VtxYpos_;
  double VtxZpos_;
  double VtxRpos_;
  double VtxPhipos_;
  double VtxChi2_;

};
 
 
 
#endif
