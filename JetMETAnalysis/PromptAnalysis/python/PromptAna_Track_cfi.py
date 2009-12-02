import FWCore.ParameterSet.Config as cms

promptanatrack = cms.EDProducer("PromptAna_Track",
                                InputTag          = cms.InputTag('generalTracks'),
                                PrimaryVertexTag  = cms.InputTag('offlinePrimaryVertices'),
                                Prefix            = cms.string('tracks'),
                                Suffix            = cms.string(''),
                                MinPT             = cms.double(0.3),
                                )


