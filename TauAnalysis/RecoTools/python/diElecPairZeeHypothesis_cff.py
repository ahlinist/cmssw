import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patElectronPFIsolationSelector

#--------------------------------------------------------------------------------
# produce collection of PF charged hadrons that satisfy:
#  deltaXY(inner hit,vertex) < 0.1
#--------------------------------------------------------------------------------

selectedPfCandidatesIpCut = cms.EDFilter("PfCandidateIpSelector",
	vertexSrc = cms.InputTag("selectedPrimaryVertexHighestPtTrackSum"),
	src = cms.InputTag("pfNoPileUp"),
	rhoMax = cms.double(10)
)

selectPfCandidatesIpCut = cms.Sequence( selectedPfCandidatesIpCut )

#--------------------------------------------------------------------------------
# produce combinations of electron + electron pairs,
# the hypothesis being that the pair of electrons results from a Z --> e+ e- decay
#--------------------------------------------------------------------------------

# VBTF WP95 ID 
selectedPatElectronsForZeeHypothesesElectronTrack = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("cleanPatElectrons"),
    cut = cms.string('(abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.8 & hcalOverEcal < 0.15 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.01 & abs(deltaPhiSuperClusterTrackAtVtx) <0.7 & hcalOverEcal < 0.07 & sigmaIetaIeta < 0.03)'),
    filter = cms.bool(False)
)

selectedPatElectronsForZeeHypothesesEta = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("selectedPatElectronsForZeeHypothesesElectronTrack"),
    cut = cms.string('abs(eta) < 2.5'),
    filter = cms.bool(False)
)

selectedPatElectronsForZeeHypothesesPt = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("selectedPatElectronsForZeeHypothesesEta"),
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

selectedPatElectronsForZeeHypothesesLoosePFRelIso = cms.EDFilter("PATElectronPFIsolationSelector",
    patElectronPFIsolationSelector.clone(
        sumPtMax = cms.double(0.30),
		pfCandidateSource = cms.InputTag("pfNoPileUp")  # we no longer use the IP-selected PF candidates
    ),
    src = cms.InputTag("selectedPatElectronsForZeeHypothesesPt"),  
    filter = cms.bool(False)
)

selectedPatElectronsForZeeHypotheses = cms.Sequence(
    selectedPatElectronsForZeeHypothesesElectronTrack
   * selectedPatElectronsForZeeHypothesesEta
   * selectedPatElectronsForZeeHypothesesPt
   * selectedPatElectronsForZeeHypothesesLoosePFRelIso
)    

allDiElecPairZeeHypothesesByLooseIsolation = cms.EDProducer("PATDiElecPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatElectronsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatElectronsForZeeHypothesesLoosePFRelIso'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

selectedDiElecPairZeeHypothesesByLooseIsolation = cms.EDFilter("PATDiElecPairSelector",
    src = cms.InputTag("allDiElecPairZeeHypothesesByLooseIsolation"),                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

produceDiElecPairs = cms.Sequence(
    selectedPatElectronsForZeeHypotheses
   * allDiElecPairZeeHypothesesByLooseIsolation * selectedDiElecPairZeeHypothesesByLooseIsolation
)
