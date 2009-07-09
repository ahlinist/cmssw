#ifndef AnalysisDataFormats_TauAnalysis_CompositePtrCandidateTMEtFwd_h
#define AnalysisDataFormats_TauAnalysis_CompositePtrCandidateTMEtFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of CompositeRefCandidateTMEt objects
typedef std::vector<PATTauNuPair> PATTauNuPairCollection;

/// persistent reference to a CompositeRefCandidateTMEt object
typedef edm::Ref<PATTauNuPairCollection> PATTauNuPairRef;

/// references to CompositeRefCandidateTMEt collection
typedef edm::RefProd<PATTauNuPairCollection> PATTauNuPairRefProd;

/// vector of references to CompositeRefCandidateTMEt objects all in the same collection
typedef edm::RefVector<PATTauNuPairCollection> PATTauNuPairRefVector;

#endif
