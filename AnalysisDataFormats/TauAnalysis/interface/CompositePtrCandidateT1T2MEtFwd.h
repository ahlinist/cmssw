#ifndef AnalysisDataFormats_TauAnalysis_CompositePtrCandidateT1T2MEtFwd_h
#define AnalysisDataFormats_TauAnalysis_CompositePtrCandidateT1T2MEtFwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of CompositeRefCandidateT1T2MEt objects
typedef std::vector<DiCandidatePair> DiCandidatePairCollection;
typedef std::vector<PATElecPair> PATElecPairCollection;
typedef std::vector<PATMuPair> PATMuPairCollection;
typedef std::vector<PATElecTauPair> PATElecTauPairCollection;
typedef std::vector<PATMuTauPair> PATMuTauPairCollection;
typedef std::vector<PATDiTauPair> PATDiTauPairCollection;
typedef std::vector<PATElecMuPair> PATElecMuPairCollection;

/// persistent reference to a CompositeRefCandidateT1T2MEt object
typedef edm::Ref<DiCandidatePairCollection> DiCandidatePairRef;
typedef edm::Ref<PATElecPairCollection> PATElecPairRef;
typedef edm::Ref<PATMuPairCollection> PATMuPairRef;
typedef edm::Ref<PATElecTauPairCollection> PATElecTauPairRef;
typedef edm::Ref<PATMuTauPairCollection> PATMuTauPairRef;
typedef edm::Ref<PATDiTauPairCollection> PATDiTauPairRef;
typedef edm::Ref<PATElecMuPairCollection> PATElecMuPairRef;

/// references to CompositeRefCandidateT1T2MEt collection
typedef edm::RefProd<DiCandidatePairCollection> DiCandidatePairRefProd;
typedef edm::RefProd<PATElecPairCollection> PATElecPairRefProd;
typedef edm::RefProd<PATMuPairCollection> PATMuPairRefProd;
typedef edm::RefProd<PATElecTauPairCollection> PATElecTauPairRefProd;
typedef edm::RefProd<PATMuTauPairCollection> PATMuTauPairRefProd;
typedef edm::RefProd<PATDiTauPairCollection> PATDiTauPairRefProd;
typedef edm::RefProd<PATElecMuPairCollection> PATElecMuPairRefProd;

/// vector of references to CompositeRefCandidateT1T2MEt objects all in the same collection
typedef edm::RefVector<DiCandidatePairCollection> DiCandidatePairRefVector;
typedef edm::RefVector<PATElecPairCollection> PATElecPairRefVector;
typedef edm::RefVector<PATMuPairCollection> PATMuPairRefVector;
typedef edm::RefVector<PATElecTauPairCollection> PATElecTauPairRefVector;
typedef edm::RefVector<PATMuTauPairCollection> PATMuTauPairRefVector;
typedef edm::RefVector<PATDiTauPairCollection> PATDiTauPairRefVector;
typedef edm::RefVector<PATElecMuPairCollection> PATElecMuPairRefVector;

#endif
