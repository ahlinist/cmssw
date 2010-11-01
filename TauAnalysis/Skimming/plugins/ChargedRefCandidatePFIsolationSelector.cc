#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"

typedef ObjectSelector<ParticlePFIsolationSelector<reco::RecoChargedRefCandidate> > ChargedRefCandidatePFIsolationSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ChargedRefCandidatePFIsolationSelector);
