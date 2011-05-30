#ifndef AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisBaseFwd_h
#define AnalysisDataFormats_TauAnalysis_NSVfitEventHypothesisBaseFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of NSVfitEventHypothesis objects
typedef std::vector<NSVfitEventHypothesisBase> NSVfitEventHypothesisBaseCollection;

/// persistent reference to a NSVfitEventHypothesis object
typedef edm::Ref<NSVfitEventHypothesisBaseCollection> NSVfitEventHypothesisBaseRef;

/// references to NSVfitEventHypothesis collection
typedef edm::RefProd<NSVfitEventHypothesisBaseCollection> NSVfitEventHypothesisBaseRefProd;

/// vector of references to NSVfitEventHypothesis objects all in the same collection
typedef edm::RefVector<NSVfitEventHypothesisBaseCollection> NSVfitEventHypothesisBaseRefVector;

#endif
