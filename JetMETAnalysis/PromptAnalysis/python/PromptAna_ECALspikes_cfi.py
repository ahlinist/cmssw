import FWCore.ParameterSet.Config as cms

promptanaecalspikes = cms.EDProducer( "PromptAna_ECALspikes"
                            , InputTag  = cms.InputTag('')
                            , Prefix    = cms.string('ECALnoise')
                            , Suffix    = cms.string('')
                            )
