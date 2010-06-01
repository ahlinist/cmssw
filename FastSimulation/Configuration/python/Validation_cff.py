import FWCore.ParameterSet.Config as cms
from Validation.Generator.BasicGenValidation_cff import *
from FastSimulation.Validation.globalValidation_cff import *
from HLTriggerOffline.Common.HLTValidation_cff import *

association = cms.Sequence(association_fastsim)
validation = cms.Sequence(basicGenTest_seq+globalValidation+hltvalidation_fastsim) 
validation_preprod = cms.Sequence(basicGenTest_seq+globalValidation_preprod+hltvalidation_preprod_fastsim) 
validation_prod = cms.Sequence(basicGenTest_seq+hltvalidation_prod) 
