import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("PhysicsTools.HepMCCandAlgos.genParticleCandidates_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("MagneticField.Engine.uniformMagneticField_cfi")

process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_2_1_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_v1/0006/203B0D51-4B76-DD11-907B-001D09F253D4.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.printTree = cms.EDFilter("ParticleTreeDrawer",
    status = cms.untracked.vint32(3),
    src = cms.InputTag("genParticleCandidates"),
    printIndex = cms.untracked.bool(True)
)

process.myanalysis = cms.EDAnalyzer("GammaJetAnalyzer",
    recoProducer = cms.string('ecalRecHit'),
    HistOutFile = cms.untracked.string('output.root'),
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMet"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    jets = cms.untracked.InputTag("iterativeCone5CaloJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgen = cms.untracked.InputTag("iterativeCone5GenJets")
)

process.p = cms.Path(process.myanalysis)


