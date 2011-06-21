#ifndef AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3Fwd_h
#define AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3Fwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"

#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of VBFEvent objects
typedef std::vector<DiCandidatePairVBFEvent> DiCandidatePairVBFEventCollection;
typedef std::vector<PATDiElecPairVBFEvent> PATDiElecPairVBFEventCollection;
typedef std::vector<PATDiMuPairVBFEvent> PATDiMuPairVBFEventCollection;
typedef std::vector<PATElecTauPairVBFEvent> PATElecTauPairVBFEventCollection;
typedef std::vector<PATMuTauPairVBFEvent> PATMuTauPairVBFEventCollection;
typedef std::vector<PATDiTauPairVBFEvent> PATDiTauPairVBFEventCollection;
typedef std::vector<PATElecMuPairVBFEvent> PATElecMuPairVBFEventCollection;

/// persistent reference to a VBFEvent object
typedef edm::Ref<DiCandidatePairVBFEventCollection> DiCandidatePairVBFEventRef;
typedef edm::Ref<PATDiElecPairVBFEventCollection> PATDiElecPairVBFEventRef;
typedef edm::Ref<PATDiMuPairVBFEventCollection> PATDiMuPairVBFEventRef;
typedef edm::Ref<PATElecTauPairVBFEventCollection> PATElecTauPairVBFEventRef;
typedef edm::Ref<PATMuTauPairVBFEventCollection> PATMuTauPairVBFEventRef;
typedef edm::Ref<PATDiTauPairVBFEventCollection> PATDiTauPairVBFEventRef;
typedef edm::Ref<PATElecMuPairVBFEventCollection> PATElecMuPairVBFEventRef;

/// references to VBFEvent collection
typedef edm::RefProd<DiCandidatePairVBFEventCollection> DiCandidatePairVBFEventRefProd;
typedef edm::RefProd<PATDiElecPairVBFEventCollection> PATDiElecPairVBFEventRefProd;
typedef edm::RefProd<PATDiMuPairVBFEventCollection> PATDiMuPairVBFEventRefProd;
typedef edm::RefProd<PATElecTauPairVBFEventCollection> PATElecTauPairVBFEventRefProd;
typedef edm::RefProd<PATMuTauPairVBFEventCollection> PATMuTauPairVBFEventRefProd;
typedef edm::RefProd<PATDiTauPairVBFEventCollection> PATDiTauPairVBFEventRefProd;
typedef edm::RefProd<PATElecMuPairVBFEventCollection> PATElecMuPairRefVBFEventProd;

/// vector of references to VBFEvent objects all in the same collection
typedef edm::RefVector<DiCandidatePairVBFEventCollection> DiCandidatePairVBFEventRefVector;
typedef edm::RefVector<PATDiElecPairVBFEventCollection> PATDiElecPairVBFEventRefVector;
typedef edm::RefVector<PATDiMuPairVBFEventCollection> PATDiMuPairVBFEventRefVector;
typedef edm::RefVector<PATElecTauPairVBFEventCollection> PATElecTauPairVBFEventRefVector;
typedef edm::RefVector<PATMuTauPairVBFEventCollection> PATMuTauPairVBFEventRefVector;
typedef edm::RefVector<PATDiTauPairVBFEventCollection> PATDiTauPairVBFEventRefVector;
typedef edm::RefVector<PATElecMuPairVBFEventCollection> PATElecMuPairVBFEventRefVector;

#endif
