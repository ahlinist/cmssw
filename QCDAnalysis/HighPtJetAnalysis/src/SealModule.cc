#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "QCDAnalysis/HighPtJetAnalysis/interface/CaloDijetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/GenDijetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/SimpleInclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/MultiJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/DijetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/METFilter.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/JetTriggerFilter.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/MultiJetFilter.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"
using namespace reco;

//DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(InclusiveJetTreeProducer);
DEFINE_FWK_MODULE(InclusiveJetAnalyzer);
DEFINE_FWK_MODULE(DijetAnalyzer);
DEFINE_FWK_MODULE(JetTriggerFilter);
DEFINE_FWK_MODULE(CaloDijetTreeProducer);
DEFINE_FWK_MODULE(GenDijetTreeProducer);

typedef SimpleInclusiveJetAnalyzer<CaloJet> SimpleInclusiveCaloJetAnalyzer;
DEFINE_FWK_MODULE(SimpleInclusiveCaloJetAnalyzer);

typedef SimpleInclusiveJetAnalyzer<GenJet> SimpleInclusiveGenJetAnalyzer;
DEFINE_FWK_MODULE(SimpleInclusiveGenJetAnalyzer);

typedef MultiJetAnalyzer<CaloJet> MultiCaloJetAnalyzer;
DEFINE_FWK_MODULE(MultiCaloJetAnalyzer);

typedef MultiJetAnalyzer<GenJet> MultiGenJetAnalyzer;
DEFINE_FWK_MODULE(MultiGenJetAnalyzer);

typedef METFilter<CaloMET> CaloMETFilter;
DEFINE_FWK_MODULE(CaloMETFilter);

typedef METFilter<GenMET> GenMETFilter;
DEFINE_FWK_MODULE(GenMETFilter);

typedef MultiJetFilter<CaloJet> MultiCaloJetFilter;
DEFINE_FWK_MODULE(MultiCaloJetFilter);

typedef MultiJetFilter<GenJet> MultiGenJetFilter;
DEFINE_FWK_MODULE(MultiGenJetFilter);
