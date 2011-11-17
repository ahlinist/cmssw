#include "TauAnalysis/CandidateTools/interface/CompositePtrCandidateTMEtProducer.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateTMEtProducer<pat::Electron> PATElecNuPairProducer;
typedef CompositePtrCandidateTMEtProducer<pat::Muon> PATMuNuPairProducer;
typedef CompositePtrCandidateTMEtProducer<pat::Tau> PATTauNuPairProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElecNuPairProducer);
DEFINE_FWK_MODULE(PATMuNuPairProducer);
DEFINE_FWK_MODULE(PATTauNuPairProducer);
