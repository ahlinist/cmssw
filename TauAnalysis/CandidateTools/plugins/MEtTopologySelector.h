#ifndef TauAnalysis_CandidateTools_MEtTopologySelector_h
#define TauAnalysis_CandidateTools_MEtTopologySelector_h

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include <vector>

typedef SingleObjectSelector<std::vector<MEtTopology>,StringCutObjectSelector<MEtTopology> > MEtTopologySelector;

#endif
