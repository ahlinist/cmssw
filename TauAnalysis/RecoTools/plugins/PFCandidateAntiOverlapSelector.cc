#include "TauAnalysis/RecoTools/interface/PATObjectAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<reco::PFCandidate> > PFCandidateAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFCandidateAntiOverlapSelector);
