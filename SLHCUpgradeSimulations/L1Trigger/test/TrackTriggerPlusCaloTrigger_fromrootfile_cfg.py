import FWCore.ParameterSet.Config as cms


process = cms.Process("RunFromFile")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.source = cms.Source("PoolSource", 
				fileNames = cms.untracked.vstring(
					'file:TrackTriggerPlusCaloTrigger.root'
				)
			)


# from std full sim (eventsetup for digis?)
process.load("Configuration.StandardSequences.FakeConditions_cff")

# Famos sequences (fake conditions)
process.load("FastSimulation.Configuration.CommonInputsFake_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")
# replace with strawmanb geometry
process.load("SLHCUpgradeSimulations.Geometry.strawmanb_cmsIdealGeometryXML_cff")


# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")
#process.StackedTrackerGeometryESModule.make_debug_file = True



process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )
process.TFileService = cms.Service("TFileService", fileName = cms.string('TrackTriggerPlusCaloTrigger_fromrootfile.root'), closeFileFast = cms.untracked.bool(True))
process.demo = cms.EDAnalyzer("TrackTriggerPlusCaloTrigger")
process.p = cms.Path(process.demo)

# Keep output to a nice level
#process.Timing =  cms.Service("Timing")
#process.load("FWCore/MessageService/MessageLogger_cfi")
#process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo.txt")

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )

# Famos with tracks
process.schedule = cms.Schedule(process.p)
#,process.p3,process.p4
