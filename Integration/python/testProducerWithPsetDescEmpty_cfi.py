
# The main purpose of this cfi is test that the validation will insert
# missing parameters that are required by the ParameterSetDescription.
# It also tests that missing optional parameters are not inserted
# and that added parameters without defaults are validated OK.

import FWCore.ParameterSet.Config as cms

testProducerWithPsetDescEmpty = cms.EDProducer('ProducerWithPSetDesc',
  testingAutoGeneratedCfi = cms.untracked.bool(False),
  p_int_opt_nd = cms.int32(11),
  p_int_optuntracked_nd = cms.untracked.int32(12)
  #, bars = cms.VPSet(
  #  cms.PSet(
  #    ndouDrinks = cms.untracked.uint32(5)
  #  ),
  #  cms.PSet(
  #  )
  #)
)
