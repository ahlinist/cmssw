#ifndef TauAnalysis_CandidateTools_MEtTopologyAlgorithm_h
#define TauAnalysis_CandidateTools_MEtTopologyAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/METReco/interface/MET.h"

#include "DataFormats/Candidate/interface/Candidate.h" 

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"

class MEtTopologyAlgorithm
{
  typedef edm::View<reco::Candidate> energyDepositCollectionType;

 public:

  MEtTopologyAlgorithm(const edm::ParameterSet&);
  ~MEtTopologyAlgorithm();

  MEtTopology buildMEtTopology(const edm::Ptr<reco::MET>&, const energyDepositCollectionType&);

 private:

  void computeVproj(MEtTopology&, const edm::Ptr<reco::MET>&, const energyDepositCollectionType&);

  double globalThreshold_;
  int verbosity_;
};

#endif 

