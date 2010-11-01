#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ObjectSelector<ParticlePFIsolationSelector<pat::Electron> > PATElectronPFIsolationSelector;
typedef ObjectSelector<ParticlePFIsolationSelector<pat::Muon> > PATMuonPFIsolationSelector;
typedef ObjectSelector<ParticlePFIsolationSelector<pat::Tau> > PATTauPFIsolationSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronPFIsolationSelector);
DEFINE_FWK_MODULE(PATMuonPFIsolationSelector);
DEFINE_FWK_MODULE(PATTauPFIsolationSelector);
