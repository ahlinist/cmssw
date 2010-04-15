import FWCore.ParameterSet.Config as cms

electronSelection = cms.EDProducer('GsfElectronRefSelector',
                                   cut = cms.string('et > 0')
                                   )
