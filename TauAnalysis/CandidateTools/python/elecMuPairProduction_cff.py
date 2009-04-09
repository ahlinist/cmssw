import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# produce combinations of electron + muons pairs
#--------------------------------------------------------------------------------

allElecMuPairs = cms.EDProducer("PATElecMuPairProducer",
  useLeadingTausOnly = cms.bool(False),
  srcLeg1 = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
  srcLeg2 = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
  dRmin12 = cms.double(-1.),
  srcMET = cms.InputTag('layer1METs'),
  recoMode = cms.string(""),
  verbosity = cms.untracked.int32(0)
)

produceElecMuPairs = cms.Sequence( allElecMuPairs )


