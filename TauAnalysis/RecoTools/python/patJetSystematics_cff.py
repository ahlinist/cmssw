import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# shift/smear jet collection
#--------------------------------------------------------------------------------

patJetsJECshiftUp = cms.EDProducer("ShiftedPATJetProducer",
    src = cms.InputTag('cleanPatJets'),
    jetCorrPayloadName = cms.string('AK5PF'),
    jetCorrUncertaintyTag = cms.string('Uncertainty'),
    addResidualJES = cms.bool(False),                               
    shiftBy = cms.double(+1.)
)

patJetsJECshiftDown = patJetsJECshiftUp.clone(
    shiftBy = cms.double(-1.)
)

prodSmearedJets = cms.Sequence(patJetsJECshiftUp * patJetsJECshiftDown)



