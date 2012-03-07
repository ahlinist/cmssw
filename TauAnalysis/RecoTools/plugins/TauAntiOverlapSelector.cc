#include "TauAnalysis/CandidateTools/interface/ParticleAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

typedef ObjectSelector<ParticleAntiOverlapSelector<reco::CaloTau> > CaloTauAntiOverlapSelector;
typedef ObjectSelector<ParticleAntiOverlapSelector<reco::PFTau> > PFTauAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CaloTauAntiOverlapSelector);
DEFINE_FWK_MODULE(PFTauAntiOverlapSelector);
