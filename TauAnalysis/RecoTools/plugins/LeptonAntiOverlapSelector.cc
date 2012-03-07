#include "TauAnalysis/CandidateTools/interface/ParticleAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/MuonReco/interface/Muon.h"

typedef ObjectSelector<ParticleAntiOverlapSelector<reco::GsfElectron> > GsfElectronAntiOverlapSelector;
typedef ObjectSelector<ParticleAntiOverlapSelector<reco::Muon> > MuonAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GsfElectronAntiOverlapSelector);
DEFINE_FWK_MODULE(MuonAntiOverlapSelector);
