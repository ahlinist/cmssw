#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "JetMETCorrections/DijetBalance/interface/DijetBalanceTreeProducer.h"
#include "JetMETCorrections/DijetBalance/interface/CaloTreeProducer.h"
#include "JetMETCorrections/DijetBalance/interface/PFTreeProducer.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
using namespace cms;
using namespace reco;
DEFINE_SEAL_MODULE();
typedef DijetBalanceTreeProducer<CaloJet> CaloDBTreeProducer;
DEFINE_ANOTHER_FWK_MODULE(CaloDBTreeProducer);
typedef DijetBalanceTreeProducer<GenJet> GenDBTreeProducer;
DEFINE_ANOTHER_FWK_MODULE(GenDBTreeProducer);
typedef DijetBalanceTreeProducer<PFJet> PFDBTreeProducer;
DEFINE_ANOTHER_FWK_MODULE(PFDBTreeProducer);
DEFINE_ANOTHER_FWK_MODULE(CaloTreeProducer);
DEFINE_ANOTHER_FWK_MODULE(PFTreeProducer);
