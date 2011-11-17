//
// $Id: PATTauNuPairSelector.h,v 1.2 2010/04/28 14:51:15 jkolb Exp $
//

#ifndef TauAnalysis_RecoTools_PATTauNuPairSelector_h
#define TauAnalysis_RecoTools_PATTauNuPairSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<PATElecNuPair>,
            StringCutObjectSelector<PATElecNuPair>
        > PATElecNuPairSelector;

typedef SingleObjectSelector<
            std::vector<PATElecNuPair>,
            StringCutObjectSelector<PATElecNuPair>,
            edm::RefVector<std::vector<PATElecNuPair> >
        > PATElecNuPairRefSelector;

typedef SingleObjectSelector<
            std::vector<PATMuNuPair>,
            StringCutObjectSelector<PATMuNuPair>
        > PATMuNuPairSelector;

typedef SingleObjectSelector<
            std::vector<PATMuNuPair>,
            StringCutObjectSelector<PATMuNuPair>,
            edm::RefVector<std::vector<PATMuNuPair> >
        > PATMuNuPairRefSelector;

typedef SingleObjectSelector<
            std::vector<PATTauNuPair>,
            StringCutObjectSelector<PATTauNuPair>
        > PATTauNuPairSelector;

typedef SingleObjectSelector<
            std::vector<PATTauNuPair>,
            StringCutObjectSelector<PATTauNuPair>,
            edm::RefVector<std::vector<PATTauNuPair> >
        > PATTauNuPairRefSelector;

#endif
