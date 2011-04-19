#ifndef TauAnalysis_CandidateTools_PFMEtSignInterface_h
#define TauAnalysis_CandidateTools_PFMEtSignInterface_h

/** \class PFMEtSignInterface
 *
 * Auxiliary class interfacing the TauAnalysis software to 
 *  RecoMET/METAlgorithms/interface/significanceAlgo.h 
 * for computing (PF)MEt significance
 * (see CMS AN-10/400 for description of the (PF)MEt significance computation)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PFMEtSignInterface.h,v 1.1 2011/04/15 16:51:24 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"
#include "RecoMET/METAlgorithms/interface/SigInputObj.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TMatrixD.h>

#include <list>

class PFMEtSignInterface
{
 public:
  PFMEtSignInterface(const edm::ParameterSet&);
  ~PFMEtSignInterface();

  void beginEvent(const edm::Event&, const edm::EventSetup&);

  TMatrixD operator()(const std::list<const reco::Candidate*>&) const; 

 private:
  template <typename T>
  void addPFMEtSignObjects(std::vector<metsig::SigInputObj>&, const std::list<const T*>&) const;

  edm::InputTag srcPFJets_;
  edm::InputTag srcPFCandidates_;

  std::list<const reco::PFJet*> pfJetList_;
  std::list<const reco::PFCandidate*> pfCandidateList_;

  metsig::SignAlgoResolutions* pfMEtResolution_;

  double dRoverlapPFJet_;
  double dRoverlapPFCandidate_;

  int verbosity_;
};

#endif
