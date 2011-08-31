#include "TauAnalysis/RecoTools/plugins/PATObjectEfficiencyCorrectionProducer.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"

typedef PATObjectEfficiencyCorrectionProducer<pat::Electron> PATElectronEfficiencyCorrectionProducer;
typedef PATObjectEfficiencyCorrectionProducer<pat::Muon> PATMuonEfficiencyCorrectionProducer;
typedef PATObjectEfficiencyCorrectionProducer<pat::Tau> PATTauEfficiencyCorrectionProducer;
typedef PATObjectEfficiencyCorrectionProducer<pat::MET> PATMEtEfficiencyCorrectionProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronEfficiencyCorrectionProducer);
DEFINE_FWK_MODULE(PATMuonEfficiencyCorrectionProducer);
DEFINE_FWK_MODULE(PATTauEfficiencyCorrectionProducer);
DEFINE_FWK_MODULE(PATMEtEfficiencyCorrectionProducer);

