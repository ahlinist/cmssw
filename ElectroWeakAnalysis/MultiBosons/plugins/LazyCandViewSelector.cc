#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/View.h"

typedef SingleObjectSelector<edm::View<reco::Candidate>,
			     StringCutObjectSelector<reco::Candidate,true>,
			     reco::CandidateCollection
> LazyCandViewSelector;

DEFINE_FWK_MODULE( LazyCandViewSelector );
