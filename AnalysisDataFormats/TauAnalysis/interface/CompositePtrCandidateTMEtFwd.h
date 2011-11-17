#ifndef AnalysisDataFormats_TauAnalysis_CompositePtrCandidateTMEtFwd_h
#define AnalysisDataFormats_TauAnalysis_CompositePtrCandidateTMEtFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of CompositeRefCandidateTMEt objects
typedef std::vector<PATElecNuPair> PATElecNuPairCollection;
typedef std::vector<PATMuNuPair> PATMuNuPairCollection;
typedef std::vector<PATTauNuPair> PATTauNuPairCollection;

/// persistent reference to a CompositeRefCandidateTMEt object
typedef edm::Ref<PATElecNuPairCollection> PATElecNuPairRef;
typedef edm::Ref<PATMuNuPairCollection> PATMuNuPairRef;
typedef edm::Ref<PATTauNuPairCollection> PATTauNuPairRef;

/// references to CompositeRefCandidateTMEt collection
typedef edm::RefProd<PATElecNuPairCollection> PATElecNuPairRefProd;
typedef edm::RefProd<PATMuNuPairCollection> PATMuNuPairRefProd;
typedef edm::RefProd<PATTauNuPairCollection> PATTauNuPairRefProd;

/// vector of references to CompositeRefCandidateTMEt objects all in the same collection
typedef edm::RefVector<PATElecNuPairCollection> PATElecNuPairRefVector;
typedef edm::RefVector<PATMuNuPairCollection> PATMuNuPairRefVector;
typedef edm::RefVector<PATTauNuPairCollection> PATTauNuPairRefVector;

#endif
