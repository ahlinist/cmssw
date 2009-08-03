import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

#process.MessageLogger = cms.Service("MessageLogger",
#    cout = cms.untracked.PSet(
#        threshold = cms.untracked.string('WARNING'),
#        noLineBreaks = cms.untracked.bool(True),
#        noTimeStamps = cms.untracked.bool(True),
#        default = cms.untracked.PSet(
#            limit = cms.untracked.int32(0)
#        ),
#        EcalPositionFromTrack = cms.untracked.PSet(
#            limit = cms.untracked.int32(0)
#        )
#    ),
#    categories = cms.untracked.vstring('EcalPositionFromTrack'),
#    destinations = cms.untracked.vstring('cout')
#)


process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("PhysicsTools.HepMCCandAlgos.genParticleCandidates_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("MagneticField.Engine.uniformMagneticField_cfi")

process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/mc/CSA08/GammaJets/GEN-SIM-RECO/1PB_V2_RECO_v1/0028/00298B8A-DB24-DD11-AD4B-0019B9F72D71.root')
#    fileNames = cms.untracked.vstring('file:/cmsrm/pc21/emanuele/data/VecbosMADGRAPH/Wjets_2_1_X_RECO.root')
 fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_0_pre11/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_31X_V1_LowLumiPileUp-v1/0001/FE549F7D-2C65-DE11-9B82-001D09F2AF1E.root')
#    fileNames = cms.untracked.vstring('file:/tmp/delre/0AB64B48-BFCB-DD11-868B-000423D6B358.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    jetsite = cms.untracked.InputTag("iterativeCone5CaloJets"),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetssis5 = cms.untracked.InputTag("sisCone5CaloJets"),
    jetssis7 = cms.untracked.InputTag("sisCone7CaloJets"),
    pfjets = cms.untracked.InputTag("iterativeCone5PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgenite = cms.untracked.InputTag("iterativeCone5GenJets"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgensis5 = cms.untracked.InputTag("sisCone5GenJets"),
    jetsgensis7 = cms.untracked.InputTag("sisCone7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::HLT")
)

process.p = cms.Path(process.myanalysis)

