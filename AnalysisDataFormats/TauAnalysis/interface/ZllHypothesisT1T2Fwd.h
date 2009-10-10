#ifndef AnalysisDataFormats_TauAnalysis_ZllHypothesisT1T2Fwd_h
#define AnalysisDataFormats_TauAnalysis_ZllHypothesisT1T2Fwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include <vector>

/// collection of ZllHypothesisT1T2 objects
typedef std::vector<ZllHypothesisElecTau> ZllHypothesisElecTauCollection;
typedef std::vector<ZllHypothesisMuTau> ZllHypothesisMuTauCollection;
typedef std::vector<ZllHypothesisElecMu> ZllHypothesisElecMuCollection;
typedef std::vector<ZllHypothesisDiTau> ZllHypothesisDiTauCollection;

/// persistent reference to a ZllHypothesisT1T2 object
typedef edm::Ref<ZllHypothesisElecTauCollection> ZllHypothesisElecTauRef;
typedef edm::Ref<ZllHypothesisMuTauCollection> ZllHypothesisMuTauRef;
typedef edm::Ref<ZllHypothesisElecMuCollection> ZllHypothesisElecMuRef;
typedef edm::Ref<ZllHypothesisDiTauCollection> ZllHypothesisDiTauRef;

/// references to ZllHypothesisT1T2 collection
typedef edm::RefProd<ZllHypothesisElecTauCollection> ZllHypothesisElecTauRefProd;
typedef edm::RefProd<ZllHypothesisMuTauCollection> ZllHypothesisMuTauRefProd;
typedef edm::RefProd<ZllHypothesisElecMuCollection> ZllHypothesisElecMuRefProd;
typedef edm::RefProd<ZllHypothesisDiTauCollection> ZllHypothesisDiTauRefProd;

/// vector of references to ZllHypothesisT1T2 objects all in the same collection
typedef edm::RefVector<ZllHypothesisElecTauCollection> ZllHypothesisElecTauRefVector;
typedef edm::RefVector<ZllHypothesisMuTauCollection> ZllHypothesisMuTauRefVector;
typedef edm::RefVector<ZllHypothesisElecMuCollection> ZllHypothesisElecMuRefVector;
typedef edm::RefVector<ZllHypothesisDiTauCollection> ZllHypothesisDiTauRefVector;

#endif
