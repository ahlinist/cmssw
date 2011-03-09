#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisFwd_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of NSVfitEventHypothesis objects
typedef std::vector<NSVfitEventHypothesis> NSVfitEventHypothesisCollection;

/// persistent reference to a NSVfitEventHypothesis object
typedef edm::Ref<NSVfitEventHypothesisCollection> NSVfitEventHypothesisRef;

/// references to NSVfitEventHypothesis collection
typedef edm::RefProd<NSVfitEventHypothesisCollection> NSVfitEventHypothesisRefProd;

/// vector of references to NSVfitEventHypothesis objects all in the same collection
typedef edm::RefVector<NSVfitEventHypothesisCollection> NSVfitEventHypothesisRefVector;

#endif
