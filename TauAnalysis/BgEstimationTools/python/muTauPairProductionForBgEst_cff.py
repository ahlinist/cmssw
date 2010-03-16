import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.muTauPairZmumuHypothesis_cff import *

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs for data-driven background estimation methods
#--------------------------------------------------------------------------------

muTauPairsForBgEstZmumuEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausChargeCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                             
    verbosity = cms.untracked.int32(0)
)

muTauPairZmumuHypothesesForBgEstZmumuEnriched = copy.deepcopy(muTauPairZmumuHypotheses)
muTauPairZmumuHypothesesForBgEstZmumuEnriched.diCandidatePairSource = cms.InputTag('muTauPairsForBgEstZmumuEnriched')

muTauPairVisMassHypothesesForBgEstZmumuEnriched = copy.deepcopy(muTauPairVisMassHypotheses)
muTauPairVisMassHypothesesForBgEstZmumuEnriched.diCandidatePairSource = muTauPairZmumuHypothesesForBgEstZmumuEnriched.diCandidatePairSource
muTauPairVisMassHypothesesForBgEstZmumuEnriched.ZllHypotheses[0].src = cms.InputTag('muTauPairZmumuHypothesesForBgEstZmumuEnriched')

muTauPairsForBgEstWplusJetsEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('tausAntiOverlapWithMuonsVetoLooseIsolationForBgEst'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                 
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstTTplusJetsEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                  
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstQCDenrichedLooseMuonIsolation = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkLooseIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauEcalIsoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                             
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstQCDenrichedNoMuonIsolation = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkNoIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauEcalIsoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                          
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstQCDenrichedNoIsolation = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkNoIsolationForBgEst'),
    srcLeg2 = cms.InputTag('tausNoIsolationForBgEst'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                      
    verbosity = cms.untracked.int32(0)
)

produceMuTauPairsForBgEst = cms.Sequence(
    muTauPairsForBgEstZmumuEnriched * muTauPairZmumuHypothesesForBgEstZmumuEnriched * muTauPairVisMassHypothesesForBgEstZmumuEnriched
   * muTauPairsForBgEstWplusJetsEnriched
   * muTauPairsForBgEstTTplusJetsEnriched
   * muTauPairsForBgEstQCDenrichedLooseMuonIsolation * muTauPairsForBgEstQCDenrichedNoMuonIsolation * muTauPairsForBgEstQCDenrichedNoIsolation
)
