
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/GenPhaseSpaceEventInfo.h"

#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopology.h"
#include "AnalysisDataFormats/TauAnalysis/interface/MEtTopologyFwd.h"

#include "DataFormats/Common/interface/Wrapper.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

namespace {
  struct AnalysisDataFormats_TauAnalysis_dictionary {
    /// create dictionaries for DiCandidatePair objects
    DiCandidatePair dummyDiCandidatePair;
    edm::Wrapper<DiCandidatePair> dummyDiCandidatePairWrapper;
    DiCandidatePairCollection dummyDiCandidatePairCollection;
    edm::Wrapper<DiCandidatePairCollection> dummyDiCandidatePairCollectionWrapper;

    /// create dictionaries for edm::Ptr<pat::Electron>, edm::Ptr<pat::Muon> and edm::Ptr<pat::Tau>,
    /// as these dictionaries are not yet created in DataFormats/PatCandidates/src/classes.h
    edm::Ptr<pat::Electron> dummyPATElectronPtr;
    edm::Ptr<pat::Muon> dummyPATMuonPtr;
    edm::Ptr<pat::Tau> dummyPATTauPtr;

    /// create dictionaries for PATElecPair objects
    PATElecPair dummyPATElecPair;
    edm::Wrapper<PATElecPair> dummyPATElecPairWrapper;
    PATElecPairCollection dummyPATElecPairCollection;
    edm::Wrapper<PATElecPairCollection> dummyPATElecPairCollectionWrapper;

    /// create dictionaries for PATMuPair objects
    PATMuPair dummyPATMuPair;
    edm::Wrapper<PATMuPair> dummyPATMuPairWrapper;
    PATMuPairCollection dummyPATMuPairCollection;
    edm::Wrapper<PATMuPairCollection> dummyPATMuPairCollectionWrapper;

    /// create dictionaries for PATElecTauPair objects
    PATElecTauPair dummyPATElecTauPair;
    edm::Wrapper<PATElecTauPair> dummyPATElecTauPairWrapper;
    PATElecTauPairCollection dummyPATElecTauPairCollection;
    edm::Wrapper<PATElecTauPairCollection> dummyPATElecTauPairCollectionWrapper;
    
    /// create dictionaries for PATMuTauPair objects
    PATMuTauPair dummyPATMuTauPair;
    edm::Wrapper<PATMuTauPair> dummyPATMuTauPairWrapper;
    PATMuTauPairCollection dummyPATMuTauPairCollection;
    edm::Wrapper<PATMuTauPairCollection> dummyPATMuTauPairCollectionWrapper;
    
    /// create dictionaries for PATDiTauPair objects
    PATDiTauPair dummyPATDiTauPair;
    edm::Wrapper<PATDiTauPair> dummyPATDiTauPairWrapper;
    PATDiTauPairCollection dummyPATDiTauPairCollection;
    edm::Wrapper<PATDiTauPairCollection> dummyPATDiTauPairCollectionWrapper;
    
    /// create dictionaries for PATElecMuPair objects
    PATElecMuPair dummyPATElecMuPair;
    edm::Wrapper<PATElecMuPair> dummyPATElecMuPairWrapper;
    PATElecMuPairCollection dummyPATElecMuPairCollection;
    edm::Wrapper<PATElecMuPairCollection> dummyPATElecMuPairCollectionWrapper;

    /// create dictionaries for PATTauNuPair objects
    PATTauNuPair dummyPATTauNuPair;
    edm::Wrapper<PATTauNuPair> dummyPATTauNuPairWrapper;
    PATTauNuPairCollection dummyPATTauNuPairCollection;
    edm::Wrapper<PATTauNuPairCollection> dummyPATTauNuPairCollectionWrapper;

    /// create dictionaries for GenPhaseSpaceEventInfo objects
    GenPhaseSpaceEventInfo dummyGenPhaseSpaceEventInfo;
    edm::Wrapper<GenPhaseSpaceEventInfo> dummyGenPhaseSpaceEventInfoWrapper;

    /// create dictionaries for MEtTopology objects
    MEtTopology dummyMEtTopology;
    edm::Wrapper<MEtTopology> dummyMEtTopologyWrapper;
    MEtTopologyCollection dummyMEtTopologyCollection;
    edm::Wrapper<MEtTopologyCollection> dummyMEtTopologyCollectionWrapper;
  };
}
