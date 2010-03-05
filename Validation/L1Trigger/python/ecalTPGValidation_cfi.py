import FWCore.ParameterSet.Config as cms

ecalTPGValidation = cms.EDAnalyzer('EcalTPGValidation',
    #outputFile = cms.string('histosEcalTrigPrimValidation.root'),
    outputFile = cms.string(''),
    Producer = cms.string(''),
    Label = cms.string('simEcalTriggerPrimitiveDigis')
)
