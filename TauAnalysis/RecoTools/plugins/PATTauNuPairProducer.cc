#include "TauAnalysis/CandidateTools/interface/CompositePtrCandidateTMEtProducer.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateTMEtProducer<pat::Tau> PATTauNuPairProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATTauNuPairProducer);
