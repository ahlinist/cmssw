//
// $Id: DiCandidatePairEventSelector.h,v 1.2 2009/08/03 12:02:15 mbluj Exp $
//

#ifndef TauAnalysis_CandidateTools_DiCandidatePairEventSelector_h
#define TauAnalysis_CandidateTools_DiCandidatePairEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, MinNumberSelector> DiCandidatePairMinEventSelector;
typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, MaxNumberSelector> DiCandidatePairMaxEventSelector;

typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > DiCandidatePairCountEventSelector;

#endif
