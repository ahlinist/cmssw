import FWCore.ParameterSet.Config as cms

promptanavtx = cms.EDProducer( "PromptAna_Vertex"
                            , InputTag  = cms.InputTag('offlinePrimaryVertices')
                            , Prefix    = cms.string('vertex')
                            , Suffix    = cms.string('')
                            )
