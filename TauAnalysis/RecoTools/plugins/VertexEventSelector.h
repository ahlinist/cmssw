//
// $Id: VertexEventSelector.h,v 1.1.2.1 2009/08/04 10:04:55 mbluj Exp $
//

#ifndef TauAnalysis_RecoTools_VertexEventSelector_h
#define TauAnalysis_RecoTools_VertexEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<Vertex>, AnySelector, MinNumberSelector> VertexMinEventSelector;

  typedef ObjectCountEventSelector<std::vector<Vertex>, AnySelector, MaxNumberSelector> VertexMaxEventSelector;

}

#endif
