import FWCore.ParameterSet.Config as cms

electronSelection = cms.EDProducer('GsfElectronViewPtrSelector',
                                   cut = cms.string('et > 0')
                                   )
