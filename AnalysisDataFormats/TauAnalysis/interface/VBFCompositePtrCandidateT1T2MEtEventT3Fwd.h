#ifndef AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3Fwd_h
#define AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3Fwd_h

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"

#include <vector>

/// collection of VBFEvent objects
typedef std::vector<PATDiElecPairVBFEvent> PATDiElecPairVBFEventCollection;
typedef std::vector<PATDiMuPairVBFEvent> PATDiMuPairVBFEventCollection;
typedef std::vector<PATElecTauPairVBFEvent> PATElecTauPairVBFEventCollection;
typedef std::vector<PATMuTauPairVBFEvent> PATMuTauPairVBFEventCollection;
typedef std::vector<PATDiTauPairVBFEvent> PATDiTauPairVBFEventCollection;
typedef std::vector<PATElecMuPairVBFEvent> PATElecMuPairVBFEventCollection;

#endif
