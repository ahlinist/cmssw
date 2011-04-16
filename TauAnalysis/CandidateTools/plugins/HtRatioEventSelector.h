
#ifndef TauAnalysis_CandidateTools_HtRatioEventSelector_h
#define TauAnalysis_CandidateTools_HtRatioEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/HtRatio.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<HtRatio>, AnySelector, MinNumberSelector> HtRatioMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<HtRatio>, AnySelector, MaxNumberSelector> HtRatioMaxEventSelector;
}

#endif 
