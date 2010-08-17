import FWCore.ParameterSet.Config as cms

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
heavyIon = cms.EDProducer("GenHIEventProducer",
                            doReco     = cms.bool(True),
                            doMC       = cms.bool(True),
                            generators = cms.vstring("generator"),

                            ptCut      = cms.double(0),
                            DoGetData  = cms.bool(False)
                          )



