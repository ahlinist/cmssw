#include "TauAnalysis/RecoTools/interface/PATObjectAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::CaloJet> > CaloJetAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::PFJet> > PFJetAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CaloJetAntiOverlapSelector);
DEFINE_FWK_MODULE(PFJetAntiOverlapSelector);
