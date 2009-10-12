import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.elecTauPairZeeHypothesis_cff import *

#--------------------------------------------------------------------------------  
# produce collections of electron + tau-jet pairs for data-driven background estimation methods
#--------------------------------------------------------------------------------

elecTauPairsForBgEstZeeEnriched = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausChargeCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

elecTauPairZeeHypothesesForBgEstZeeEnriched = copy.deepcopy(elecTauPairZeeHypotheses)
elecTauPairZeeHypothesesForBgEstZeeEnriched.diCandidatePairSource = cms.InputTag('elecTauPairsForBgEstZeeEnriched')

elecTauPairVisMassHypothesesForBgEstZeeEnriched = copy.deepcopy(elecTauPairVisMassHypotheses)
elecTauPairVisMassHypothesesForBgEstZeeEnriched.diCandidatePairSource = elecTauPairZeeHypothesesForBgEstZeeEnriched.diCandidatePairSource
elecTauPairVisMassHypothesesForBgEstZeeEnriched.ZllHypotheses[0].src = cms.InputTag('elecTauPairZeeHypothesesForBgEstZeeEnriched')

elecTauPairsForBgEstWplusJetsEnriched = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('tausAntiOverlapWithElectronsVetoLooseIsolationForBgEst'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

elecTauPairsForBgEstTTplusJetsEnriched = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

elecTauPairsForBgEstQCDenriched = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsTrkLooseIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForElecTauEcalIsoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

produceElecTauPairsForBgEst = cms.Sequence(
    elecTauPairsForBgEstZeeEnriched * elecTauPairZeeHypothesesForBgEstZeeEnriched * elecTauPairVisMassHypothesesForBgEstZeeEnriched
   * elecTauPairsForBgEstWplusJetsEnriched
   * elecTauPairsForBgEstTTplusJetsEnriched
   * elecTauPairsForBgEstQCDenriched
)
