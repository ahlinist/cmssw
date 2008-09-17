import FWCore.ParameterSet.Config as cms

IncludedHLTs = cms.EDFilter("HLTFilter",
#       //HLTSelection = cms.VInputTag(cms.InputTag("HLT1Tau"),
#                                      cms.InputTag("HLT1MuonIso"),
#                                      cms.InputTag("HLT1MET"),
#				       cms.InputTag("HLT1Tau1MET"))
        HLTSelection = cms.VInputTag(cms.InputTag("pathALCARECORpcCalHLT"))
)
