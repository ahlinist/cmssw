#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveConeJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/DijetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(InclusiveConeJetAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(InclusiveJetAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(DijetTreeProducer);
