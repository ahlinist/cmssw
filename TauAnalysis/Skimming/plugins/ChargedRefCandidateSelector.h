#ifndef TauAnalysis_Skimming_ChargedRefCandidateSelector_h
#define TauAnalysis_Skimming_ChargedRefCandidateSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedRefCandidate.h"

#include <vector>

namespace reco {

  typedef SingleObjectSelector<
              std::vector<reco::RecoChargedRefCandidate>,
              StringCutObjectSelector<reco::RecoChargedRefCandidate>
          > ChargedRefCandidateSelector;

  typedef SingleObjectSelector<
              std::vector<reco::RecoChargedRefCandidate>,
              StringCutObjectSelector<reco::RecoChargedRefCandidate>,
              edm::RefVector<std::vector<reco::RecoChargedRefCandidate> >
    > ChargedRefCandidateRefSelector;

}

#endif
