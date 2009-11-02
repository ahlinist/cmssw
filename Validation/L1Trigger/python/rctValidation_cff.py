import FWCore.ParameterSet.Config as cms

from Validation.L1Trigger.rctValidation_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import * 


rctValidation = cms.Sequence(rctValParticlesFromZ*
                             rctValElectronsFromZ*
                             rctValEGammaAll*
                             rctZeeVal*
                             rctInclusiveVal)


rctHarvesting = cms.Sequence(rctValHarvesting)


rctQualityTests = cms.Sequence(rctValQuality)
