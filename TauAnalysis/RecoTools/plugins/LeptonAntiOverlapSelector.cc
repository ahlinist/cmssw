#include "TauAnalysis/RecoTools/interface/PATObjectAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/MuonReco/interface/Muon.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::GsfElectron> > GsfElectronAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::Muon> > MuonAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GsfElectronAntiOverlapSelector);
DEFINE_FWK_MODULE(MuonAntiOverlapSelector);
