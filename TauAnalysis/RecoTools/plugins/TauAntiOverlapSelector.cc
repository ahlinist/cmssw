#include "TauAnalysis/RecoTools/interface/PATObjectAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::CaloTau> > CaloTauAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::PFTau> > PFTauAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CaloTauAntiOverlapSelector);
DEFINE_FWK_MODULE(PFTauAntiOverlapSelector);
