#include "TauAnalysis/RecoTools/interface/PATObjectAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Electron> > PATElectronAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Muon> > PATMuonAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Tau> > PATTauAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Jet> > PATJetAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronAntiOverlapSelector);
DEFINE_FWK_MODULE(PATMuonAntiOverlapSelector);
DEFINE_FWK_MODULE(PATTauAntiOverlapSelector);
DEFINE_FWK_MODULE(PATJetAntiOverlapSelector);
