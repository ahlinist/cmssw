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
#include "Geometry/Vector/interface/GlobalPoint.h"

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
  double getHCALEnergy(){return HCALEnergy_;}

  //Cluster quantities
  double getE1E(){return e1energy_;}
  double getE1phi(){return e1phi_;}
  double getE1eta(){return e1eta_;}
  double getE1Emax(){return e1Emax_;}
  double getE1E2nd(){return e1E2nd_;}
  double getE1e2x2(){return e1E2x2_;}
  double getE1e3x3(){return e1E3x3_;}
  double getE1etaeta(){return e1etaeta_;}
  double getE1phiphi(){return e1phiphi_;}
  double getE1etaphi(){return e1etaphi_;}

  double getE2E(){return e2energy_;}
  double getE2phi(){return e2phi_;}
  double getE2eta(){return e2eta_;}
  double getE2Emax(){return e2Emax_;}
  double getE2E2nd(){return e2E2nd_;}
  double getE2e2x2(){return e2E2x2_;}
  double getE2e3x3(){return e2E3x3_;}
  double getE2etaeta(){return e2etaeta_;}
  double getE2phiphi(){return e2phiphi_;}
  double getE2etaphi(){return e2etaphi_;}

  double getGamE(){return phoenergy_;}
  double getGamphi(){return phophi_;}
  double getGameta(){return phoeta_;}
  double getGamEmax(){return phoEmax_;}
  double getGamE2nd(){return phoE2nd_;}
  double getGame2x2(){return phoE2x2_;}
  double getGame3x3(){return phoE3x3_;}
  double getGametaeta(){return phoetaeta_;}
  double getGamphiphi(){return phophiphi_;}
  double getGametaphi(){return phoetaphi_;}


  void setE1Emax(double E1Emax){e1Emax_=E1Emax;}
  void setE1E2nd(double E1E2nd){e1E2nd_=E1E2nd;}
  void setE1e2x2(double E1E2x2){e1E2x2_=E1E2x2;}
  void setE1e3x3(double E1E3x3){e1E3x3_=E1E3x3;}
  void setE1etaeta(double E1etaeta){e1etaeta_=E1etaeta;}
  void setE1phiphi(double E1phiphi){e1phiphi_=E1phiphi;}
  void setE1etaphi(double E1etaphi){e1etaphi_=E1etaphi;}

  void setE2Emax(double E2Emax){e2Emax_=E2Emax;}
  void setE2E2nd(double E2E2nd){e2E2nd_=E2E2nd;}
  void setE2e2x2(double E2E2x2){e2E2x2_=E2E2x2;}
  void setE2e3x3(double E2E3x3){e2E3x3_=E2E3x3;}
  void setE2etaeta(double E2etaeta){e2etaeta_=E2etaeta;}
  void setE2phiphi(double E2phiphi){e2phiphi_=E2phiphi;}
  void setE2etaphi(double E2etaphi){e2etaphi_=E2etaphi;}

  void setGamEmax(double phoEmax){phoEmax_=phoEmax;}
  void setGamE2nd(double phoE2nd){phoE2nd_=phoE2nd;}
  void setGame2x2(double phoE2x2){phoE2x2_=phoE2x2;}
  void setGame3x3(double phoE3x3){phoE3x3_=phoE3x3;}
  void setGametaeta(double phoetaeta){phoetaeta_=phoetaeta;}
  void setGamphiphi(double phophiphi){phophiphi_=phophiphi;}
  void setGametaphi(double phoetaphi){phoetaphi_=phoetaphi;}
  void setHCALEnergy(double HCALEnergy){HCALEnergy_ =  HCALEnergy;}

  //Estimated conversion position
  double getREst(){return ConversionREst_;}
  double getPhiEst(){return ConversionPhiEst_;}
  // Stub quantities
  int getNStub1Hits(){return NStub1Hits_;}
  int getNStub2Hits(){return NStub2Hits_;}
  int getStub1Char(){return stubchar1_;}
  int getStub2Char(){return stubchar2_;}
  std::vector<GlobalPoint> getStub1Hits(){return stub1Hits;}
  std::vector<GlobalPoint> getStub2Hits(){return stub2Hits;}


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
  int getTrk1Charge(){return track1Charge_;}
  std::vector<GlobalPoint> getTrack1Hits(){return trk1Hits;}
  std::vector<GlobalPoint> getTrack2Hits(){return trk2Hits;}


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
  int getTrk2Charge(){return track2Charge_;}

  //////////////
  //vertex quantities
  double getVtxXPos(){return VtxXpos_;}
  double getVtxYpos(){return VtxYpos_;}
  double getVtxZpos(){return VtxZpos_;}
  double getVtxRpos(){return VtxRpos_;}
  double getVtxPhipos(){return VtxPhipos_;}
  double getVtxChi2(){return VtxChi2_;}

  //Setters
  void setNStub1Hits(int stub1){NStub1Hits_ = stub1;}
  void setNStub2Hits(int stub2){NStub2Hits_=stub2;}
  void setStub1Charge(int char1){stubchar1_ = char1;}
  void setStub2Charge(int char2){stubchar2_ = char2;}
  void setStub1Hits(std::vector<GlobalPoint> stb){stub1Hits=stb;}
  void setStub2Hits(std::vector<GlobalPoint> stb){stub2Hits=stb;}
  void setTrack1Hits(std::vector<GlobalPoint> stb){trk1Hits=stb;}
  void setTrack2Hits(std::vector<GlobalPoint> stb){trk2Hits=stb;}

  void setTrk1PTIS(double PTIS){track1ptIS_ = PTIS;}
  void setTrk1PhiIS(double PhiIS){track1phiIS_ = PhiIS;}
  void setTrk1EtaIS(double EtaIS){track1etaIS_ = EtaIS;}
  void setTrk1PTFS(double PTFS) {track1ptFS_ = PTFS;}
  void setTrk1PhiFS(double PhiFS){track1phiFS_ = PhiFS;}
  void setTrk1EtaFS(double EtaFS){track1etaFS_ = EtaFS;}
  void setTrk1NHIT(int nhit){nHits1_=nhit;}
  void setTrk1Chi2(double Chi2){track1chi2_=Chi2;}
  void setTrk1SmallRHit(double rhit){track1SmallRhit_=rhit;}
  void setTrk1Charge(int charge){track1Charge_=charge;}
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
  void setTrk2Charge(int charge){track2Charge_=charge;}

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
  double HCALEnergy_;
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
  double e1Emax_;
  double e1E2nd_;
  double e1E2x2_;
  double e1E3x3_;
  double e1etaeta_;
  double e1phiphi_;
  double e1etaphi_;

  double e2energy_;
  double e2phi_;
  double e2eta_;
  double e2Emax_;
  double e2E2nd_;
  double e2E2x2_;
  double e2E3x3_;
  double e2etaeta_;
  double e2phiphi_;
  double e2etaphi_;
  //photon quantities
  double phoenergy_;
  double phoeta_;
  double phophi_;
  double phoEmax_;
  double phoE2nd_;
  double phoE2x2_;
  double phoE3x3_;
  double phoetaeta_;
  double phophiphi_;
  double phoetaphi_;
  //Estimated conversion position in R/Phi
  double ConversionREst_;
  double ConversionPhiEst_;
  int NStub1Hits_;
  int NStub2Hits_;
  int stubchar1_;
  int stubchar2_;
  std::vector<GlobalPoint> stub1Hits;
  std::vector<GlobalPoint> stub2Hits;

  std::vector<GlobalPoint> trk1Hits;
  std::vector<GlobalPoint> trk2Hits;
  
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
  int track1Charge_;
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
  int track2Charge_;

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
