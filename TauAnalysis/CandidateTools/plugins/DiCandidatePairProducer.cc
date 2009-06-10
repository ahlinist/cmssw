#include "TauAnalysis/CandidateTools/interface/CompositePtrCandidateT1T2MEtProducer.h"

#include "DataFormats/Candidate/interface/Candidate.h" 

typedef CompositePtrCandidateT1T2MEtProducer<reco::Candidate, reco::Candidate> DiCandidatePairProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(DiCandidatePairProducer);
