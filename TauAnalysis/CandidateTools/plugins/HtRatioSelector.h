#ifndef TauAnalysis_CandidateTools_HtRatioSelector_h
#define TauAnalysis_CandidateTools_HtRatioSelector_h

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/HtRatio.h"
#include <vector>

typedef SingleObjectSelector<std::vector<HtRatio>,StringCutObjectSelector<HtRatio> > HtRatioSelector;

#endif
