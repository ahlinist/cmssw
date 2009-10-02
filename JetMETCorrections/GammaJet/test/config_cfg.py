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
    skipEvents = cms.untracked.uint32(0),
# fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_0_pre11/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_31X_V1_LowLumiPileUp-v1/0001/FE549F7D-2C65-DE11-9B82-001D09F2AF1E.root')
#    fileNames = cms.untracked.vstring('file:/cmsrm/pc17/pandolf/eventi_PhotonJetPt170_Summer09_10TeV.root')
    fileNames = cms.untracked.vstring('file:/cmsrm/pc17/delre/7A62C541-37A0-DE11-BF0C-00215E221098.root')
# fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_0_pre11/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_31X_V1_LowLumiPileUp-v1/0001/FE549F7D-2C65-DE11-9B82-001D09F2AF1E.root')
#    fileNames = cms.untracked.vstring('file:/tmp/voutila/Cern/data/summer09/raw/PhotonJet_Pt80to120_Summer09-MC_31X_V3-v1_x100.root')
#    fileNames = cms.untracked.vstring('file:/tmp/voutila/Cern/data/summer09/raw/QCD_Pt80_Summer09-MC_31X_V3-v1_x100.root')
#    fileNames = cms.untracked.vstring('file:/tmp/voutila/Cern/data/summer09/raw/QCD_EMEnriched_Pt80to170_Summer09-MC_31X_V3-v1_x100.root')
#    fileNames = cms.untracked.vstring('file:/tmp/voutila/Cern/data/summer09/raw/QCD_BCtoE_Pt80to170_Summer09-MC_31X_V3-v1_x100.root')

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
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    jetsite = cms.untracked.InputTag("iterativeCone5CaloJets"),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetsakt5 = cms.untracked.InputTag("antikt5CaloJets"),
    jetssis5 = cms.untracked.InputTag("sisCone5CaloJets"),
    jetssis7 = cms.untracked.InputTag("sisCone7CaloJets"),
    jetspfite = cms.untracked.InputTag("iterativeCone5PFJets"),
    jetspfkt4 = cms.untracked.InputTag("kt4PFJets"),
    jetspfkt6 = cms.untracked.InputTag("kt6PFJets"),
    jetspfakt5 = cms.untracked.InputTag("antikt5PFJets"),
    jetspfsis5 = cms.untracked.InputTag("sisCone5PFJets"),
    jetspfsis7 = cms.untracked.InputTag("sisCone7PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgenite = cms.untracked.InputTag("iterativeCone5GenJets"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgenakt5 = cms.untracked.InputTag("antikt5GenJets"),
    jetsgensis5 = cms.untracked.InputTag("sisCone5GenJets"),
    jetsgensis7 = cms.untracked.InputTag("sisCone7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::HLT"),
    vertices = cms.untracked.InputTag("offlinePrimaryVertices")
)

# histogram service
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('output.root')
)
process.p = cms.Path(process.myanalysis)

