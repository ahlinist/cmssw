#include "TauAnalysis/RecoTools/plugins/PATObjectLUTvalueEmbedder.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorFromTH1.h"
#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorFromKNN.h"

typedef PATObjectLUTvalueEmbedder<pat::Electron, PATElectronLUTvalueExtractorFromTH1> PATElectronLUTvalueEmbedderFromTH1;
typedef PATObjectLUTvalueEmbedder<pat::Electron, PATElectronLUTvalueExtractorFromKNN> PATElectronLUTvalueEmbedderFromKNN;
typedef PATObjectLUTvalueEmbedder<pat::Muon, PATMuonLUTvalueExtractorFromTH1> PATMuonLUTvalueEmbedderFromTH1;
typedef PATObjectLUTvalueEmbedder<pat::Muon, PATMuonLUTvalueExtractorFromKNN> PATMuonLUTvalueEmbedderFromKNN;
typedef PATObjectLUTvalueEmbedder<pat::Tau, PATTauLUTvalueExtractorFromTH1> PATTauLUTvalueEmbedderFromTH1;
typedef PATObjectLUTvalueEmbedder<pat::Tau, PATTauLUTvalueExtractorFromKNN> PATTauLUTvalueEmbedderFromKNN;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronLUTvalueEmbedderFromTH1);
DEFINE_FWK_MODULE(PATElectronLUTvalueEmbedderFromKNN);
DEFINE_FWK_MODULE(PATMuonLUTvalueEmbedderFromTH1);
DEFINE_FWK_MODULE(PATMuonLUTvalueEmbedderFromKNN);
DEFINE_FWK_MODULE(PATTauLUTvalueEmbedderFromTH1);
DEFINE_FWK_MODULE(PATTauLUTvalueEmbedderFromKNN);



