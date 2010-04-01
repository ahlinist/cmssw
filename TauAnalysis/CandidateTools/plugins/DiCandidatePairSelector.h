//
// $Id: DiCandidatePairSelector.h,v 1.1 2009/06/10 09:33:09 veelken Exp $
//

#ifndef TauAnalysis_CandidateTools_DiCandidatePairSelector_h
#define TauAnalysis_CandidateTools_DiCandidatePairSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<DiCandidatePair>,
            StringCutObjectSelector<DiCandidatePair>
        > DiCandidatePairSelector;

typedef SingleObjectSelector<
            std::vector<DiCandidatePair>,
            StringCutObjectSelector<DiCandidatePair>,
            edm::RefVector<std::vector<DiCandidatePair> >
        > DiCandidatePairRefSelector;

#endif
