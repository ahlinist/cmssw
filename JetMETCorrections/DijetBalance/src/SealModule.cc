#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "JetMETCorrections/DijetBalance/interface/DijetBalanceTreeProducer.h"
#include "JetMETCorrections/DijetBalance/interface/RelJECTreeProducer.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
using namespace cms;
using namespace reco;

DEFINE_FWK_MODULE(RelJECTreeProducer);

typedef DijetBalanceTreeProducer<CaloJet> CaloDBTreeProducer;
DEFINE_FWK_MODULE(CaloDBTreeProducer);
typedef DijetBalanceTreeProducer<PFJet> PFDBTreeProducer;
DEFINE_FWK_MODULE(PFDBTreeProducer);
typedef DijetBalanceTreeProducer<JPTJet> JPTDBTreeProducer;
DEFINE_FWK_MODULE(JPTDBTreeProducer);
