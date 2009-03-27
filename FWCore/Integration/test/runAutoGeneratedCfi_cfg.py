# Runs using configuration cfi file that was autogenerated
# The purpose is to test the generation of cfi files

import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.source = cms.Source("EmptySource")

# expects this cfi file to be in the current directory
# usually run from the tmp/slc4_ia32_gcc345 directory
# where the cfi was just created by the test script
from testProducerWithPsetDesc_cfi import *

process.testProducerWithPsetDesc = testProducerWithPsetDesc

process.p1 = cms.Path(process.testProducerWithPsetDesc)
