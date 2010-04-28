//
// $Id: PATTauPairEventSelector.h,v 1.2 2009/10/25 12:38:23 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATTauPairEventSelector_h
#define TauAnalysis_RecoTools_PATTauPairEventSelector_h

#include "CommonTools/UtilAlgos/interface/AnySelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "CommonTools/UtilAlgos/interface/MinNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/MaxNumberSelector.h"
#include "CommonTools/UtilAlgos/interface/AndSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

typedef ObjectCountEventSelector<std::vector<PATElecTauPair>, AnySelector, MinNumberSelector> PATElecTauPairMinEventSelector;
typedef ObjectCountEventSelector<std::vector<PATMuTauPair>, AnySelector, MinNumberSelector> PATMuTauPairMinEventSelector;
typedef ObjectCountEventSelector<std::vector<PATDiTauPair>, AnySelector, MinNumberSelector> PATDiTauPairMinEventSelector;
typedef ObjectCountEventSelector<std::vector<PATElecMuPair>, AnySelector, MinNumberSelector> PATElecMuPairMinEventSelector;

typedef ObjectCountEventSelector<std::vector<PATElecTauPair>, AnySelector, MaxNumberSelector> PATElecTauPairMaxEventSelector;
typedef ObjectCountEventSelector<std::vector<PATMuTauPair>, AnySelector, MaxNumberSelector> PATMuTauPairMaxEventSelector;
typedef ObjectCountEventSelector<std::vector<PATDiTauPair>, AnySelector, MaxNumberSelector> PATDiTauPairMaxEventSelector;
typedef ObjectCountEventSelector<std::vector<PATElecMuPair>, AnySelector, MaxNumberSelector> PATElecMuPairMaxEventSelector;

typedef ObjectCountEventSelector<std::vector<PATElecTauPair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATElecTauPairCountEventSelector;
typedef ObjectCountEventSelector<std::vector<PATMuTauPair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATMuTauPairCountEventSelector;
typedef ObjectCountEventSelector<std::vector<PATDiTauPair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATDiTauPairCountEventSelector;
typedef ObjectCountEventSelector<std::vector<PATElecMuPair>, AnySelector, AndSelector<MinNumberSelector, MaxNumberSelector> > PATElecMuPairCountEventSelector;

#endif
