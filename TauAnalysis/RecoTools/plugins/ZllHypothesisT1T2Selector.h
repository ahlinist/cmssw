//
// $Id: ZllHypothesisT1T2Selector.h,v 1.1 2009/07/29 13:03:38 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_ZllHypothesisT1T2Selector_h
#define TauAnalysis_RecoTools_ZllHypothesisT1T2Selector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<ZllHypothesisElecTau>,
            StringCutObjectSelector<ZllHypothesisElecTau>
        > ZllHypothesisElecTauSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisMuTau>,
            StringCutObjectSelector<ZllHypothesisMuTau>
        > ZllHypothesisMuTauSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisDiTau>,
            StringCutObjectSelector<ZllHypothesisDiTau>
        > ZllHypothesisDiTauSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisElecMu>,
            StringCutObjectSelector<ZllHypothesisElecMu>
        > ZllHypothesisElecMuSelector;

typedef SingleObjectSelector<
            std::vector<ZllHypothesisElecTau>,
            StringCutObjectSelector<ZllHypothesisElecTau>,
            edm::RefVector<std::vector<ZllHypothesisElecTau> >
        > ZllHypothesisElecTauRefSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisMuTau>,
            StringCutObjectSelector<ZllHypothesisMuTau>,
            edm::RefVector<std::vector<ZllHypothesisMuTau> >
        > ZllHypothesisMuTauRefSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisDiTau>,
            StringCutObjectSelector<ZllHypothesisDiTau>,
            edm::RefVector<std::vector<ZllHypothesisDiTau> >
        > ZllHypothesisDiTauRefSelector;
typedef SingleObjectSelector<
            std::vector<ZllHypothesisElecMu>,
            StringCutObjectSelector<ZllHypothesisElecMu>,
            edm::RefVector<std::vector<ZllHypothesisElecMu> >
        > ZllHypothesisElecMuRefSelector;

#endif
