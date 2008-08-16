
import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")
# "old-style" ECAL scale
process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

# "old-style" HCAL scale
process.load("SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff")

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

#keep the logging output to a nice level
process.MessageLogger = cms.Service("MessageLogger")

process.write = cms.EDFilter("L1CaloInputScalesGenerator")

process.p = cms.Path(process.write)


