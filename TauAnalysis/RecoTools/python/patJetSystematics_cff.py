import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# shift/smear jet collection
#--------------------------------------------------------------------------------

patJetsJECshiftUp = cms.EDProducer("SmearedJetProducer",
    src                   = cms.InputTag('cleanPatJets'),
    jetCorrPayloadName    = cms.string('AK5PF'),
    jetCorrUncertaintyTag = cms.string('L2L3Residual'),
    shiftByJECuncertainty = cms.double(+1.)
)

patJetsJECshiftDown = patJetsJECshiftUp.clone(
    shiftByJECuncertainty = cms.double(-1.)
)

prodSmearedJets = cms.Sequence(patJetsJECshiftUp * patJetsJECshiftDown)



