
#ifndef TauAnalysis_CandidateTools_MEtTopologyEventSelector_h
#define TauAnalysis_CandidateTools_MEtTopologyEventSelector_h

#include "CommonTools/Utils/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<MEtTopology>, AnySelector, MinNumberSelector> MEtTopologyMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<MEtTopology>, AnySelector, MaxNumberSelector> MEtTopologyMaxEventSelector;
}

#endif 
