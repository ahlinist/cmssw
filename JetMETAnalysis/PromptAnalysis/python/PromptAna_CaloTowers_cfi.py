import FWCore.ParameterSet.Config as cms

promptanacalotowers = cms.EDProducer( "PromptAna_CaloTowers"
                            , InputTag  = cms.InputTag('towerMaker')
                            , Prefix    = cms.string('CaloTowers')
                            , Suffix    = cms.string('')
                            )
