import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *

#--------------------------------------------------------------------------------
# produce combinations of muon + tau-jet pairs
#--------------------------------------------------------------------------------

allMuTauPairs = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauElectronVetoCumulative'),                               
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('layer1METs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),
    srcGenParticles = cms.InputTag('genParticles'),                  
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                           
    verbosity = cms.untracked.int32(0)
)

muTauPairProdConfigurator = objProdConfigurator(
    allMuTauPairs,
    pyModuleName = __name__
)

produceMuTauPairs = muTauPairProdConfigurator.configure(pyNameSpace = locals())

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

allMuTauPairsLooseMuonIsolation = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedLayer1MuonsTrkIPlooseIsolationCumulative'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauElectronVetoCumulative'),                                                  
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('layer1METs'),
    srcGenParticles = cms.InputTag('genParticles'),                                              
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                             
    verbosity = cms.untracked.int32(0)
)

muTauPairProdConfiguratorLooseMuonIsolation = objProdConfigurator(
    allMuTauPairsLooseMuonIsolation,
    pyModuleName = __name__
)

produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(pyNameSpace = locals())

produceMuTauPairsAll = cms.Sequence( produceMuTauPairs * produceMuTauPairsLooseMuonIsolation )
