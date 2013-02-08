#ifndef TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt3_h
#define TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt3_h

/** \class NSVfitEventLikelihoodMEt3
 *
 * Plugin for computing likelihood for neutrinos produced in tau lepton decays
 * to match missing transverse momentum reconstructed in the event
 *
 * New version using covariance matrix of (PF)MET significance calculation
 * (CMS AN-10/400) to compute the likehood
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.9 $
 *
 * $Id: NSVfitEventLikelihoodMEt3.h,v 1.9 2012/08/28 15:00:22 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TH2.h>
#include <TAxis.h>
#include <TRandom3.h>

#include <string>
#include <list>

class NSVfitEventLikelihoodMEt3 : public NSVfitEventLikelihood
{
 public:
  NSVfitEventLikelihoodMEt3(const edm::ParameterSet&);
  ~NSVfitEventLikelihoodMEt3();

  void beginJob(NSVfitAlgorithmBase*);
  void beginEvent(const edm::Event&, const edm::EventSetup&);
  void beginCandidate(const NSVfitEventHypothesis*) const;

  double operator()(const NSVfitEventHypothesis*) const;

 private:

  double power_;

  edm::InputTag srcPFJets_;
  edm::InputTag srcPFCandidates_;

  double pfCandPtThreshold_;
  double pfJetPtThreshold_;
  TH2* lut_;
  mutable TAxis* xAxis_;
  mutable int numBinsX_;
  mutable TAxis* yAxis_;
  mutable int numBinsY_;

  unsigned numToys_;

  std::list<const reco::PFJet*> pfJetListForCovMatrix_;
  std::list<const reco::PFCandidate*> pfCandidateListForCovMatrix_;
  std::list<const reco::PFJet*> pfJetListForToys_;
  std::list<const reco::PFCandidate*> pfCandidateListForToys_;
  
  double dRoverlapPFJet_;
  double dRoverlapPFCandidate_;

  PFMEtSignInterfaceBase* pfMEtSign_;

  mutable TRandom3 rnd_;

  bool monitorMEtUncertainty_;
  std::string monitorFilePath_;
  std::string monitorFileName_;
};

#endif
