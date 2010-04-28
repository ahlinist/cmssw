//
// $Id: PATTauNuPairSelector.h,v 1.1 2009/07/09 12:16:16 veelken Exp $
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
            std::vector<PATTauNuPair>,
            StringCutObjectSelector<PATTauNuPair>
        > PATTauNuPairSelector;

typedef SingleObjectSelector<
            std::vector<PATTauNuPair>,
            StringCutObjectSelector<PATTauNuPair>,
            edm::RefVector<std::vector<PATTauNuPair> >
        > PATTauNuPairRefSelector;

#endif
