#ifndef JetMETCorrections_METPUSubtraction_noPileUpMEtUtilities_h
#define JetMETCorrections_METPUSubtraction_noPileUpMEtUtilities_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/MVAMEtData.h"
#include "DataFormats/METReco/interface/MVAMEtDataFwd.h"

#include <vector>

namespace noPileUpMEtUtilities
{
  //-------------------------------------------------------------------------------
  // general auxiliary functions
  void finalizeMEtData(CommonMETData&);
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  // auxiliary functions for jets
  reco::MVAMEtJetInfoCollection cleanJets(const reco::MVAMEtJetInfoCollection&, 
					  const std::vector<reco::Candidate::LorentzVector>&, 
					  double, bool);
  reco::MVAMEtJetInfoCollection selectJets(const reco::MVAMEtJetInfoCollection&,
					   double, double, int);
  CommonMETData computeJetSum(const reco::MVAMEtJetInfoCollection&);
  CommonMETData computeJetSum_neutral(const reco::MVAMEtJetInfoCollection&);
  reco::MVAMEtJetInfo jet(const reco::MVAMEtJetInfoCollection&, unsigned);
  reco::MVAMEtJetInfo leadJet(const reco::MVAMEtJetInfoCollection&);
  reco::MVAMEtJetInfo subleadJet(const reco::MVAMEtJetInfoCollection&);
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  // auxiliary functions for PFCandidates
  reco::MVAMEtPFCandInfoCollection cleanPFCandidates(const reco::MVAMEtPFCandInfoCollection&,
						     const std::vector<reco::Candidate::LorentzVector>&,
						     double, bool);
  reco::MVAMEtPFCandInfoCollection selectPFCandidates(const reco::MVAMEtPFCandInfoCollection&,
						      double, double, int, int);
  CommonMETData computePFCandidateSum(const reco::MVAMEtPFCandInfoCollection&);
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  // auxiliary functions to compute different types of MEt/hadronic recoils
  //
  // NOTE: all pfCandidates and jets passed as function arguments
  //       need to be cleaned wrt. leptons
  //
  CommonMETData computeTrackRecoil(const reco::MVAMEtPFCandInfoCollection&);
  CommonMETData computeTrackRecoilNoPU(const reco::MVAMEtPFCandInfoCollection&);
  CommonMETData computeTrackRecoilPU(const reco::MVAMEtPFCandInfoCollection&);
  CommonMETData computeNeutralRecoil_unclustered(const reco::MVAMEtPFCandInfoCollection&);
  CommonMETData computeHadRecoilNoPU(const reco::MVAMEtJetInfoCollection&,
				     const reco::MVAMEtPFCandInfoCollection&);
  CommonMETData computeHadRecoilPU(const reco::MVAMEtJetInfoCollection&,
				   const reco::MVAMEtPFCandInfoCollection&);
  //-------------------------------------------------------------------------------
}

#endif
