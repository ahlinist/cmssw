#ifndef AnalysisDataFormats_TauAnalysis_MEtTopologyFwd_h
#define AnalysisDataFormats_TauAnalysis_MEtTopologyFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of CompositeRefCandidateTMEt objects
typedef std::vector<MEtTopology> MEtTopologyCollection;

/// persistent reference to a CompositeRefCandidateTMEt object
typedef edm::Ref<MEtTopologyCollection> MEtTopologyRef;

/// references to CompositeRefCandidateTMEt collection
typedef edm::RefProd<MEtTopologyCollection> MEtTopologyRefProd;

/// vector of references to CompositeRefCandidateTMEt objects all in the same collection
typedef edm::RefVector<MEtTopologyCollection> MEtTopologyRefVector;

#endif
