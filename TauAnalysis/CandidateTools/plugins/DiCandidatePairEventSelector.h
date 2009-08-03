//
// $Id: DiCandidatePairEventSelector.h,v 1.1 2009/06/10 09:33:09 veelken Exp $
//

#ifndef TauAnalysis_CandidateTools_DiCandidatePairEventSelector_h
#define TauAnalysis_CandidateTools_DiCandidatePairEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/AndSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, MinNumberSelector> DiCandidatePairMinEventSelector;
typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, MaxNumberSelector> DiCandidatePairMaxEventSelector;

typedef ObjectCountEventSelector<std::vector<DiCandidatePair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > DiCandidatePairCountEventSelector;

#endif
