# The following comments couldn't be translated into the new config version:

#

import FWCore.ParameterSet.Config as cms

process = cms.Process("CaloFP420")
#
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#
process.load("Geometry.CMSCommonData.cmsAllGeometryXML_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")


# Reconstruction geometry services
#  Calorimeters
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.EcalMapping.EcalMapping_cfi")

process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")

#  Calo only conditions
process.load("CalibCalorimetry.Configuration.Hcal_FakeConditions_cff")

process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")

#
process.load("Configuration.StandardSequences.Generator_cff")

#VX smearing
process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")

#SimTransport
process.load("SimTransport.HectorProducer.HectorTransport_cfi")

#Simulation
process.load("SimG4Core.Configuration.SimG4Core_cff")

#
#calo digitization with no pile up
process.load("SimGeneral.MixingModule.mixNoPU_cfi")

process.load("SimCalorimetry.Configuration.SimCalorimetry_cff")

#local reconstruction
process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")

# Global  reco
process.load("RecoJets.Configuration.RecoJets_cff")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.load("RecoJets.JetProducers.CaloTowerSchemeB_cfi")

#jet corrections
#include "JetMETCorrections/Configuration/data/MCJetCorrectionsSpring07.cff"
#select generator level particles
process.load("RecoJets.Configuration.GenJetParticles_cff")

#GenJets reconstruction
process.load("RecoJets.Configuration.RecoGenJets_cff")

#sequence reconstruction = {localreco, globalreco, MCJetCorrections}
# digi for FP420
process.load("SimRomanPot.Configuration.SimChargeDigiFP420_cff")

# rec for FP420
process.load("RecoRomanPot.Configuration.RecClusterTrackMomentumFP420_cff")

#
process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:Exhume_Hbb.root')
)

process.CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz')
)

process.caloTowers = cms.EDFilter("CaloTowerCandidateCreator",
    src = cms.InputTag("towerMaker"),
    minimumEt = cms.double(-1.0),
    minimumE = cms.double(-1.0)
)

process.o1 = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('CaloFP420_Exhume_Hbb.root')
)

process.localreco = cms.Sequence(process.calolocalreco)
process.caloTowersRec = cms.Sequence(process.towerMaker*process.caloTowers)
process.globalreco = cms.Sequence(process.caloTowersRec*process.recoJets)
process.reconstruction = cms.Sequence(process.localreco*process.globalreco)
process.p1 = cms.Path(process.pgen*process.LHCTransport*process.g4SimHits*process.mix*process.calDigi*process.FP420Digi*process.FP420Cluster*process.FP420Track*process.FP420Reco*process.reconstruction)
process.outpath = cms.EndPath(process.o1)
process.schedule = cms.Schedule(process.p1,process.outpath)

process.MessageLogger.cerr.default.limit = 10
process.LHCTransport.ZDCTransport = False
process.HectorEtaCut.EtaCutForHector = 5.0
process.g4SimHits.Generator.ApplyPtCuts = False
process.g4SimHits.Generator.ApplyEtaCuts = False
process.g4SimHits.Generator.HepMCProductLabel = 'LHCTransport'

