import FWCore.ParameterSet.Config as cms

process = cms.Process("Fastsimwdigi2")

process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring('file:out.root')
)

process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("FastSimulation.Configuration.CommonInputsFake_cff")
process.load("SLHCUpgradeSimulations.Geometry.strawmanb_cmsIdealGeometryXML_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")


process.demo2 = cms.EDFilter("stubproductiontest")
process.p = cms.Path(process.demo2)

process.schedule = cms.Schedule(process.p)
