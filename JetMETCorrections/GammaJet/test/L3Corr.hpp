// Adapted from D0 Experiment jetcorr/jetcorr/RjetCorr.hpp

#ifndef __L3CORR__
#define __L3CORR__

//
// Purpose: low level routines for L3 absolute jet response correction
//



#include "JetDefs.hpp"

#include <iostream>

class L3Corr {
public:
  enum JetAlg {IC5_DATA, SIS5_DATA, SIS7_DATA, KT5_DATA, KT7_DATA,
	       IC5_MC, SIS5_MC, SIS7_MC, KT5_MC, KT7_MC};  

  enum PhotonID {kLoose, kMedium, kTight, kNN,// min pT 8/10
		 kMedium005, kMedium010, kMedium020}; // min pT 2.5/10

  L3Corr(const JetAlg& jetalg = L3Corr::IC5_DATA, 
	 const jec::ErrorTypes& errType = jec::kAll,
	 const PhotonID& id = L3Corr::kMedium);
  ~L3Corr(){};
  
  double Rjet(const double pTprime, double& err);

  void SetJetAlg(const JetAlg& jetalg = L3Corr::IC5_DATA);
  void SetErrType(const jec::ErrorTypes& errType = jec::kAll);
  void SetPhoID(const PhotonID& id = L3Corr::kMedium);

  jec::ErrorTypes GetErrType();

  // Half-internal calculations
  // Keep accessible for plotting scripts

  // Jet response
  double _Rjet(const double pTprime) const;

  // Statistical and systematic uncertainties
  double _SystErr(const double pTprime) const;
  double _StatErr(const double pTprime) const;

  // Purity and purity systematics broken into pieces
  double _purity(const double pTprime, const PhotonID& id) const;
  double _SystPurity(const double pTprime, const PhotonID& id) const;
  double _SystPurityID(const double pTprime, const PhotonID& id) const;
  double _SystPurityXsec(const double pTprime, const PhotonID& id) const;
  //double _SystPurity2ndJet(const double pTprime, const PhotonID& id) const;
  double _StatPurity(const double pTprime, const PhotonID& id) const;

  // Parton correction systematics
  double _parton(const double pTprime) const;
  double _partonFrag(const double pTprime) const;
  double _partonUE(const double pTprime) const;

  // Flavor mapping or ratio of dijet and photon+jet jet responses
  double _flavorMap(const double pTprime) const;

  // Background response shift
  double _deltaC(const double pTprime,
		 double& syserr,
		 const PhotonID& id, // = L3Corr::kMedium,
		 double (*syserrs)[6] = 0) const;

  // Background correction, deltaR = deltaC * (1 - P)
  double _deltaRjet(const double pTprime, const PhotonID& id) const;

  //double _SystParton(const double pTprime) const;

  // Bias corrections
  // Bkg: Difference between mixed dijet/photon+jet and pure photon+jet MC reco.
  //      This should equal _deltaRjet, but is taken directly from fits.
  //      Calculated for arithmetic means so _deltaRjet factorization works.
  // Phot: Residual photon energy scale (summer08), make sure <Rphot>==1.000.
  //       Calibrates arithmetic mean, and accounts for tails shifting it.
  // Topo: Difference between photon+jet MC truth and reco, after taking out
  //       all the other know effects. Seems to be practically all due to
  //       ISR combined with bin-to-bin migrations for pTgamma binning. Best
  //       seen by comparing pT,jetparton/pT,photonparton vs pThat and pTgamma
  // Parton: Ratio of pT,GenJet/pT,jetparton. This accounts for the parton
  //         correction, which folds underlying event, hadronization (physics
  //         showering) and final state radiation
  // Balance: Ratio of pT,jetparton/pT,photonparton in pThat bins. Very small,
  //          so currently folded as part of Topo
  // Peak: Difference between peak and mean in pure photon+jet MC truth.
  //       Currently done in pThat bins, could change slightly for pT,GenJet
  double _Rbias(const double pTprime, const PhotonID& id) const;
  double _RbiasBkg(const double pTprime, const PhotonID& id) const;
  double _RbiasPhot(const double pTprime, const PhotonID& id) const;
  double _RbiasTopo(const double pTprime, const PhotonID& id) const;
  double _RbiasParton(const double pTprime, const PhotonID& id) const;
  double _RbiasBalance(const double pTprime) const;
  double _RbiasPeak(const double pTprime) const;
  //
  double _RjetMix(const double pTprime, const PhotonID& id) const;
  double _RjetReco(const double pTprime, const PhotonID& id) const;
  double _RjetTruth(const double pTprime) const;
  double _RjetTruthQCD(const double pTprime) const;
  double _Rphoton(const double pTprime, const PhotonID& id) const;

private:
  JetAlg _jetalg;
  jec::ErrorTypes _errType;
  PhotonID _phoID;
  bool _isMC;

  static const int npar = 3;
  double _E0;
  double _par[npar];

  double _chi2;
  double _pstat[npar];
  double _eig[npar][npar];

  double _powerlaw(const double& pTprime, const double* par) const;
  double _eigpowerlaw(const double& pTprime, const double* par,
		      const double (*eig)[npar], const int& ieig) const;

};

#endif /* __L3CORR__ */
