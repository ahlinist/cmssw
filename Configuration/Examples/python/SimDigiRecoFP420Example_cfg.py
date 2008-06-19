# The following comments couldn't be translated into the new config version:

#  "keep PSimHits_*_FP420SI_*",
#  "keep DigiCollectionFP420_*_*_*",
#  "keep ClusterCollectionFP420_*_*_*",
#  "keep TrackCollectionFP420_*_*_*",

import FWCore.ParameterSet.Config as cms

process = cms.Process("GSDRFP420")
# 
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#
#   include "Geometry/FP420CommonData/data/cmsFP420GeometryXML.cfi"
process.load("Geometry.CMSCommonData.cmsAllGeometryXML_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")


#
#VX smearing
process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")

#
#SimTransport
process.load("SimTransport.HectorProducer.HectorTransport_cfi")

process.load("SimG4Core.Configuration.SimG4Core_cff")

#
#digi-rec for FP420
process.load("SimGeneral.MixingModule.mixNoPU_cfi")

process.load("SimRomanPot.SimFP420.FP420Digi_cfi")

process.load("RecoRomanPot.RecoFP420.FP420Cluster_cfi")

process.load("RecoRomanPot.RecoFP420.FP420Track_cfi")

process.load("RecoRomanPot.RecoFP420.FP420Reco_cfi")

#
process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:Exhume_Hbb.root')
)

process.o1 = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_VtxSmeared_*_*', 
        'keep *_source_*_*', 
        'keep edmHepMCProduct_*_*_*', 
        'keep SimTracks_*_*_*', 
        'keep SimVertexs_*_*_*', 
        'keep RecoCollectionFP420_*_*_*'),
    fileName = cms.untracked.string('Hec_Exhume_Hbb.root')
)

process.Timing = cms.Service("Timing")

process.Tracer = cms.Service("Tracer")

process.p1 = cms.Path(process.VtxSmeared*process.LHCTransport*process.g4SimHits*process.mix*process.FP420Digi*process.FP420Cluster*process.FP420Track*process.FP420Reco)
process.outpath = cms.EndPath(process.o1)
process.schedule = cms.Schedule(process.p1,process.outpath)

process.MessageLogger.cerr.default.limit = 10
process.LHCTransport.ZDCTransport = False
process.HectorEtaCut.EtaCutForHector = 5.0
process.g4SimHits.Generator.ApplyPtCuts = False
process.g4SimHits.Generator.ApplyEtaCuts = False
process.g4SimHits.Generator.HepMCProductLabel = 'LHCTransport'

