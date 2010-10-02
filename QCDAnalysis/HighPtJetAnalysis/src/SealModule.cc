#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "QCDAnalysis/HighPtJetAnalysis/interface/GenDijetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/SimpleInclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/DijetAnalyzer.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
using namespace reco;

//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(InclusiveJetTreeProducer);
DEFINE_FWK_MODULE(InclusiveJetAnalyzer);
DEFINE_FWK_MODULE(DijetAnalyzer);
DEFINE_FWK_MODULE(GenDijetTreeProducer);

typedef SimpleInclusiveJetAnalyzer<CaloJet> SimpleInclusiveCaloJetAnalyzer;
DEFINE_FWK_MODULE(SimpleInclusiveCaloJetAnalyzer);

typedef SimpleInclusiveJetAnalyzer<GenJet> SimpleInclusiveGenJetAnalyzer;
DEFINE_FWK_MODULE(SimpleInclusiveGenJetAnalyzer);

