
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATElecTauPairZeeHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PATElecTauPairZeeHypothesisFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergyFwd.h"

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

    /// create dictionaries for 
    ///  o edm::Ptr<pat::Electron> 
    ///  o edm::Ptr<pat::Muon> 
    ///  o edm::Ptr<pat::Tau>
    ///  o edm::Ptr<pat::Jet>
    /// as these dictionaries are not yet created in DataFormats/PatCandidates/src/classes.h
    edm::Ptr<pat::Electron> dummyPATElectronPtr;
    edm::Ptr<pat::Muon> dummyPATMuonPtr;
    edm::Ptr<pat::Tau> dummyPATTauPtr;
    edm::Ptr<pat::Jet> dummyPATJetPtr;

    /// create dictionaries for 
    ///  o edm::Ptr<reco::CaloJet>
    ///  o edm::Ptr<reco::PFJet>
    /// as these dictionaries are not yet created in DataFormats/JetReco/src/classes.h
    edm::Ptr<reco::CaloJet> dummyCaloJetPtr;
    edm::Ptr<reco::PFJet> dummyPFJetPtr;

    /// create dictionaries for
    ///  o edm::Ptr<reco::GenParticle>
    /// as these dictionaries are not yet created in DataFormats/HepMCCandidate/src/classes.h
    edm::Ptr<reco::GenParticle> dummyGenParticlePtr;

    /// create dictionaries for
    ///  o edm::Ptr<reco::Track>
    ///  o edm::Ptr<reco::GsfTrack>
    /// as these dictionaries are not yet created in DataFormats/TrackReco/src/classes.h (DataFormats/GsfTrackReco/src/classes.h)
    edm::Ptr<reco::Track> dummyTrackPtr;
    edm::Ptr<reco::GsfTrack> dummyGsfTrackPtr;

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
    edm::Ptr<PATElecTauPair> dummyPATElecTauPairPtr;

    /// create dictionaries for PATElecTauPairZeeHypothesis objects
    PATElecTauPairZeeHypothesis dummyPATElecTauPairZeeHypothesis;
    edm::Wrapper<PATElecTauPairZeeHypothesis> dummyPATElecTauPairZeeHypothesisWrapper;
    PATElecTauPairZeeHypothesisCollection dummyPATElecTauPairZeeHypothesisCollection;
    edm::Wrapper<PATElecTauPairZeeHypothesisCollection> dummyPATElecTauPairCollectionZeeHypothesisWrapper;
    
    /// create dictionaries for PATMuTauPair objects
    PATMuTauPair dummyPATMuTauPair;
    edm::Wrapper<PATMuTauPair> dummyPATMuTauPairWrapper;
    PATMuTauPairCollection dummyPATMuTauPairCollection;
    edm::Wrapper<PATMuTauPairCollection> dummyPATMuTauPairCollectionWrapper;
    edm::Ptr<PATMuTauPair> dummyPATMuTauPairPtr;
    
    /// create dictionaries for PATDiTauPair objects
    PATDiTauPair dummyPATDiTauPair;
    edm::Wrapper<PATDiTauPair> dummyPATDiTauPairWrapper;
    PATDiTauPairCollection dummyPATDiTauPairCollection;
    edm::Wrapper<PATDiTauPairCollection> dummyPATDiTauPairCollectionWrapper;
    edm::Ptr<PATDiTauPair> dummyPATDiTauPairPtr;
    
    /// create dictionaries for PATElecMuPair objects
    PATElecMuPair dummyPATElecMuPair;
    edm::Wrapper<PATElecMuPair> dummyPATElecMuPairWrapper;
    PATElecMuPairCollection dummyPATElecMuPairCollection;
    edm::Wrapper<PATElecMuPairCollection> dummyPATElecMuPairCollectionWrapper;
    edm::Ptr<PATElecMuPair> dummyPATElecMuPairPtr;

    /// create dictionaries for PATTauNuPair objects
    PATTauNuPair dummyPATTauNuPair;
    edm::Wrapper<PATTauNuPair> dummyPATTauNuPairWrapper;
    PATTauNuPairCollection dummyPATTauNuPairCollection;
    edm::Wrapper<PATTauNuPairCollection> dummyPATTauNuPairCollectionWrapper;
    edm::Ptr<PATTauNuPair> dummyPATTauNuPairPtr;

    /// create dictionaries for PATLeptonRecoilEnergyFromJets objects
    PATTauRecoilEnergyFromJets dummyPATTauRecoilEnergyFromJets;
    edm::Wrapper<PATTauRecoilEnergyFromJets> dummyPATTauRecoilEnergyFromJetsWrapper;
    PATTauRecoilEnergyFromJetsCollection dummyPATTauRecoilEnergyFromJetsCollection;
    edm::Wrapper<PATTauRecoilEnergyFromJetsCollection> dummyPATTauRecoilEnergyFromJetsCollectionWrapper;

    /// create dictionaries for PATLeptonRecoilEnergyFromCaloTowers objects
    PATTauRecoilEnergyFromCaloTowers dummyPATTauRecoilEnergyFromCaloTowers;
    edm::Wrapper<PATTauRecoilEnergyFromCaloTowers> dummyPATTauRecoilEnergyFromCaloTowersWrapper;
    PATTauRecoilEnergyFromCaloTowersCollection dummyPATTauRecoilEnergyFromCaloTowersCollection;
    edm::Wrapper<PATTauRecoilEnergyFromCaloTowersCollection> dummyPATTauRecoilEnergyFromCaloTowersCollectionWrapper;

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
