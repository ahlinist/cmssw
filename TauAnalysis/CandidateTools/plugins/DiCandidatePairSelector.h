#ifndef TauAnalysis_CandidateTools_DiCandidatePairSelector_h
#define TauAnalysis_CandidateTools_DiCandidatePairSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<DiCandidatePair>,
            StringCutObjectSelector<DiCandidatePair>
        > DiCandidatePairSelector;
typedef SingleObjectSelector<
            std::vector<PATElecTauPair>,
            StringCutObjectSelector<PATElecTauPair>
        > PATElecTauPairSelector;
typedef SingleObjectSelector<
            std::vector<PATMuTauPair>,
            StringCutObjectSelector<PATMuTauPair>
        > PATMuTauPairSelector;
typedef SingleObjectSelector<
            std::vector<PATDiTauPair>,
            StringCutObjectSelector<PATDiTauPair>
        > PATDiTauPairSelector;
typedef SingleObjectSelector<
            std::vector<PATElecMuPair>,
            StringCutObjectSelector<PATElecMuPair>
        > PATElecMuPairSelector;
typedef SingleObjectSelector<
            std::vector<PATDiElecPair>,
            StringCutObjectSelector<PATDiElecPair>
        > PATDiElecPairSelector;
typedef SingleObjectSelector<
            std::vector<PATDiMuPair>,
            StringCutObjectSelector<PATDiMuPair>
        > PATDiMuPairSelector;

typedef SingleObjectSelector<
            std::vector<DiCandidatePair>,
            StringCutObjectSelector<DiCandidatePair>,
            edm::RefVector<std::vector<DiCandidatePair> >
        > DiCandidatePairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATElecTauPair>,
            StringCutObjectSelector<PATElecTauPair>,
            edm::RefVector<std::vector<PATElecTauPair> >
        > PATElecTauPairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATMuTauPair>,
            StringCutObjectSelector<PATMuTauPair>,
            edm::RefVector<std::vector<PATMuTauPair> >
        > PATMuTauPairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiTauPair>,
            StringCutObjectSelector<PATDiTauPair>,
            edm::RefVector<std::vector<PATDiTauPair> >
        > PATDiTauPairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATElecMuPair>,
            StringCutObjectSelector<PATElecMuPair>,
            edm::RefVector<std::vector<PATElecMuPair> >
        > PATElecMuPairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiElecPair>,
            StringCutObjectSelector<PATDiElecPair>,
            edm::RefVector<std::vector<PATDiElecPair> >
        > PATDiElecPairRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiMuPair>,
            StringCutObjectSelector<PATDiMuPair>,
            edm::RefVector<std::vector<PATDiMuPair> >
        > PATDiMuPairRefSelector;

#endif
