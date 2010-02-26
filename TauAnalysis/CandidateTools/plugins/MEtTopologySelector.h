
#ifndef TauAnalysis_CandidateTools_MEtTopologySelector_h
#define TauAnalysis_CandidateTools_MEtTopologySelector_h

#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include <vector>

typedef SingleObjectSelector<std::vector<MEtTopology>,StringCutObjectSelector<MEtTopology> > MEtTopologySelector;

#endif
