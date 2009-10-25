//
// $Id: ZllHypothesisT1T2EventSelector.h,v 1.1 2009/10/10 12:32:16 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_ZllHypothesisT1T2EventSelector_h
#define TauAnalysis_RecoTools_ZllHypothesisT1T2EventSelector_h

#include "CommonTools/Utils/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MaxNumberSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"

#include <vector>

namespace reco {

  typedef ObjectCountEventSelector<std::vector<ZllHypothesisElecTau>, AnySelector, MinNumberSelector> ZllHypothesisElecTauMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisMuTau>, AnySelector, MinNumberSelector> ZllHypothesisMuTauMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisDiTau>, AnySelector, MinNumberSelector> ZllHypothesisDiTauMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisElecMu>, AnySelector, MinNumberSelector> ZllHypothesisElecMuMinEventSelector;
  
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisElecTau>, AnySelector, MaxNumberSelector> ZllHypothesisElecTauMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisMuTau>, AnySelector, MaxNumberSelector> ZllHypothesisMuTauMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisDiTau>, AnySelector, MaxNumberSelector> ZllHypothesisDiTauMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<ZllHypothesisElecMu>, AnySelector, MaxNumberSelector> ZllHypothesisElecMuMaxEventSelector;
  
}

#endif
