import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# shift/smear jet collection
#--------------------------------------------------------------------------------

patJetsJECshiftUp = cms.EDProducer("SmearedJetProducer",
    src                         = cms.InputTag('cleanPatJets'),
    ##jecUncertaintyInputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/START38_V13_AK5PF_Uncertainty.txt"),
    jetCorrPayloadName          = cms.string('AK5PF'),
    jetCorrUncertaintyTag       = cms.string('Uncertainty'),
    shiftByJECuncertainty       = cms.double(+1.)
)

patJetsJECshiftDown = patJetsJECshiftUp.clone(
    shiftByJECuncertainty = cms.double(-1.)
)

prodSmearedJets = cms.Sequence(patJetsJECshiftUp * patJetsJECshiftDown)



