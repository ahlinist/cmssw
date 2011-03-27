#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/ProcessedTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"
using namespace reco;

//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(InclusiveJetTreeProducer);
DEFINE_FWK_MODULE(ProcessedTreeProducer);
DEFINE_FWK_MODULE(InclusiveJetAnalyzer);

