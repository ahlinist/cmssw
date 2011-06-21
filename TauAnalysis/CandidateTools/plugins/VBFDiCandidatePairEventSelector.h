#ifndef TauAnalysis_CandidateTools_VBFDiCandidatePairEventSelector_h
#define TauAnalysis_CandidateTools_VBFDiCandidatePairEventSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"

#include <vector>

typedef SingleObjectSelector<
            std::vector<DiCandidatePairVBFEvent>,
            StringCutObjectSelector<DiCandidatePairVBFEvent>
        > DiCandidatePairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATElecTauPairVBFEvent>,
            StringCutObjectSelector<PATElecTauPairVBFEvent>
        > PATElecTauPairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATMuTauPairVBFEvent>,
            StringCutObjectSelector<PATMuTauPairVBFEvent>
        > PATMuTauPairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATDiTauPairVBFEvent>,
            StringCutObjectSelector<PATDiTauPairVBFEvent>
        > PATDiTauPairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATElecMuPairVBFEvent>,
            StringCutObjectSelector<PATElecMuPairVBFEvent>
        > PATElecMuPairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATDiElecPairVBFEvent>,
            StringCutObjectSelector<PATDiElecPairVBFEvent>
        > PATDiElecPairVBFEventSelector;
typedef SingleObjectSelector<
            std::vector<PATDiMuPairVBFEvent>,
            StringCutObjectSelector<PATDiMuPairVBFEvent>
        > PATDiMuPairVBFEventSelector;

typedef SingleObjectSelector<
            std::vector<DiCandidatePairVBFEvent>,
            StringCutObjectSelector<DiCandidatePairVBFEvent>,
            edm::RefVector<std::vector<DiCandidatePairVBFEvent> >
        > DiCandidatePairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATElecTauPairVBFEvent>,
            StringCutObjectSelector<PATElecTauPairVBFEvent>,
            edm::RefVector<std::vector<PATElecTauPairVBFEvent> >
        > PATElecTauPairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATMuTauPairVBFEvent>,
            StringCutObjectSelector<PATMuTauPairVBFEvent>,
            edm::RefVector<std::vector<PATMuTauPairVBFEvent> >
        > PATMuTauPairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiTauPairVBFEvent>,
            StringCutObjectSelector<PATDiTauPairVBFEvent>,
            edm::RefVector<std::vector<PATDiTauPairVBFEvent> >
        > PATDiTauPairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATElecMuPairVBFEvent>,
            StringCutObjectSelector<PATElecMuPairVBFEvent>,
            edm::RefVector<std::vector<PATElecMuPairVBFEvent> >
        > PATElecMuPairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiElecPairVBFEvent>,
            StringCutObjectSelector<PATDiElecPairVBFEvent>,
            edm::RefVector<std::vector<PATDiElecPairVBFEvent> >
        > PATDiElecPairVBFEventRefSelector;
typedef SingleObjectSelector<
            std::vector<PATDiMuPairVBFEvent>,
            StringCutObjectSelector<PATDiMuPairVBFEvent>,
            edm::RefVector<std::vector<PATDiMuPairVBFEvent> >
        > PATDiMuPairVBFEventRefSelector;

#endif
