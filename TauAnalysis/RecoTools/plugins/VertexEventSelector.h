//
// $Id: VertexEventSelector.h,v 1.2 2009/10/25 12:38:23 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_VertexEventSelector_h
#define TauAnalysis_RecoTools_VertexEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<Vertex>, AnySelector, MinNumberSelector> VertexMinEventSelector;

  typedef ObjectCountEventSelector<std::vector<Vertex>, AnySelector, MaxNumberSelector> VertexMaxEventSelector;

}

#endif
