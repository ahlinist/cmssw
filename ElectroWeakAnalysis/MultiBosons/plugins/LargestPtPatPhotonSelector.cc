/* \class LargestPtPatPhotonSelector
 * Based on CommonTools/CandAlgos/plugins/LargestPtCandSelector.cc
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "CommonTools/Utils/interface/PtComparator.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

typedef ObjectSelector<
          SortCollectionSelector<
            pat::PhotonCollection,
            GreaterByPt<reco::Candidate>
          >
        > LargestPtPatPhotonSelector;

DEFINE_FWK_MODULE( LargestPtPatPhotonSelector );
