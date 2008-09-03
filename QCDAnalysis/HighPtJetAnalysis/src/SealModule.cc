#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "QCDAnalysis/HighPtJetAnalysis/interface/DijetTreeProducer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/SimpleInclusiveJetAnalyzer.h"
#include "QCDAnalysis/HighPtJetAnalysis/interface/METFilter.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/GenMET.h"
using namespace reco;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(InclusiveJetAnalyzer);

typedef SimpleInclusiveJetAnalyzer<CaloJet> SimpleInclusiveCaloJetAnalyzer;
DEFINE_ANOTHER_FWK_MODULE(SimpleInclusiveCaloJetAnalyzer);

typedef SimpleInclusiveJetAnalyzer<GenJet> SimpleInclusiveGenJetAnalyzer;
DEFINE_ANOTHER_FWK_MODULE(SimpleInclusiveGenJetAnalyzer);

typedef METFilter<CaloMET> CaloMETFilter;
DEFINE_ANOTHER_FWK_MODULE(CaloMETFilter);

typedef METFilter<GenMET> GenMETFilter;
DEFINE_ANOTHER_FWK_MODULE(GenMETFilter);

typedef DijetTreeProducer<CaloJet> CaloDijetTreeProducer;
DEFINE_ANOTHER_FWK_MODULE(CaloDijetTreeProducer);

typedef DijetTreeProducer<GenJet> GenDijetTreeProducer;
DEFINE_ANOTHER_FWK_MODULE(GenDijetTreeProducer);
