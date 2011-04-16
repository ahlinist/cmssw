import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about distribution of energy deposits in the event 
# with respect to direction of missing Et vector
#--------------------------------------------------------------------------------

htRatios = cms.EDProducer("HtRatioProducer",
    srcJet = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
    srcTau = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

selectedHtRatio = cms.EDFilter("HtRatioSelector",
    src = cms.InputTag('htRatios'),
    cut = cms.string('Ratio() > 0.65'),
    filter = cms.bool(False)
)



produceHtRatio = cms.Sequence(htRatios)

selectHtRatio = cms.Sequence( selectedHtRatio )
