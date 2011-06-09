/** \class CandViewCountRangeFilter
 *
 *  \brief Filters events based on number of candidates
 *         Unlike the CandViewCountFilter, it also tests the maximum number.
 *         Works also with classes derived from reco:Candidate
 *
 *  \author Jan Veverka, Caltech
 *
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountFilter.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"

typedef ObjectCountFilter<
           reco::CandidateView,
           AnySelector,
           AndSelector<MinNumberSelector, MaxNumberSelector>
         >::type CandViewCountRangeFilter;

DEFINE_FWK_MODULE( CandViewCountRangeFilter );
