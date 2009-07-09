//
// $Id: PATTauNuPairSelector.h,v 1.1 2009/06/10 09:33:53 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATTauNuPairSelector_h
#define TauAnalysis_RecoTools_PATTauNuPairSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

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
