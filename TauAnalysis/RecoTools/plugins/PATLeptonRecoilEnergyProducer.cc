#include "TauAnalysis/RecoTools/plugins/PATLeptonRecoilEnergyProducer.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

typedef PATLeptonRecoilEnergyProducer<pat::Tau, reco::Jet> PATTauRecoilEnergyFromJetsProducer;
typedef PATLeptonRecoilEnergyProducer<pat::Tau, CaloTower> PATTauRecoilEnergyFromCaloTowersProducer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATTauRecoilEnergyFromJetsProducer);
DEFINE_ANOTHER_FWK_MODULE(PATTauRecoilEnergyFromCaloTowersProducer);
