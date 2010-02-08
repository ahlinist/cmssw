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

#process.load("MagneticField.Engine.uniformMagneticField_cfi")

process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

# produce JPT jets
#rocess.load("Configuration.StandardSequences.Geometry_cff")
#rocess.load("Configuration.StandardSequences.MagneticField_cff")
#rocess.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#rocess.GlobalTag.globaltag = cms.string('MC_31X_V3::All')
#rocess.load("JetMETCorrections.Configuration.ZSPJetCorrections219_cff")
#rocess.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
# fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_0_pre11/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_31X_V1_LowLumiPileUp-v1/0001/FE549F7D-2C65-DE11-9B82-001D09F2AF1E.root')
#    fileNames = cms.untracked.vstring('file:/cmsrm/pc17/pandolf/eventi_PhotonJetPt170_Summer09_10TeV.root')
#    fileNames = cms.untracked.vstring('file:/cmsrm/pc17/delre/7A62C541-37A0-DE11-BF0C-00215E221098.root')
# fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_1_0_pre11/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_31X_V1_LowLumiPileUp-v1/0001/FE549F7D-2C65-DE11-9B82-001D09F2AF1E.root')
#    fileNames = cms.untracked.vstring('file:/tmp/voutila/Cern/data/summer09/raw/PhotonJet_Pt80to120_Summer09-MC_31X_V3-v1_x100.root')
    fileNames = cms.untracked.vstring(
'file:/cmsrm/pc18/pandolf/CMSSW_3_3_3/src/JetMETCorrections/GammaJet/test/events_QCDDiJets_Pt0_15_Summer09.root'
#'file:/cmsrm/pc18/pandolf/data/DiJetFilter5/DiJetSkim_124009.root',
#'file:/cmsrm/pc18/pandolf/data/DiJetFilter5/DiJetSkim_124020.root',
#'file:/cmsrm/pc18/pandolf/data/DiJetFilter5/DiJetSkim_124022.root',
#'file:/cmsrm/pc18/pandolf/data/DiJetFilter5/DiJetSkim_124023_124024.root'
)
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



#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorAK5PF") 


process.myanalysis = cms.EDAnalyzer("GammaJetAnalyzer",
    debug = cms.bool(False),
    recoProducer = cms.string('ecalRecHit'),
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    JetCorrectionService_pfakt5 = cms.string('L2L3JetCorrectorAK5PF'),
    JetCorrectionService_pfakt7 = cms.string('L2L3JetCorrectorAK5PF'),
    jetsite = cms.untracked.InputTag("iterativeCone5CaloJets"),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetsakt5 = cms.untracked.InputTag("ak5CaloJets"),
    jetssis5 = cms.untracked.InputTag("sisCone5CaloJets"),
    jetssis7 = cms.untracked.InputTag("sisCone7CaloJets"),
    #jetsjptak5 = cms.untracked.InputTag("JetPlusTrackZSPCorJetIcone5"),
    jetspfite = cms.untracked.InputTag("iterativeCone5PFJets"),
    jetspfkt4 = cms.untracked.InputTag("kt4PFJets"),
    jetspfkt6 = cms.untracked.InputTag("kt6PFJets"),
    jetspfakt5 = cms.untracked.InputTag("ak5PFJets"),
    jetspfakt7 = cms.untracked.InputTag("ak5PFJets"),
    jetspfsis5 = cms.untracked.InputTag("sisCone5PFJets"),
    jetspfsis7 = cms.untracked.InputTag("sisCone7PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgenite = cms.untracked.InputTag("iterativeCone5GenJets"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgenakt5 = cms.untracked.InputTag("ak5GenJetsptmin1"),
    jetsgenakt7 = cms.untracked.InputTag("sisCone5GenJets"),
    jetsgensis5 = cms.untracked.InputTag("sisCone5GenJets"),
    jetsgensis7 = cms.untracked.InputTag("sisCone7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::HLT"),
    vertices = cms.untracked.InputTag("offlinePrimaryVertices")
)

from RecoJets.JetProducers.ak5GenJets_cfi import ak5GenJets
process.ak5GenJetsptmin1 = ak5GenJets.clone()
process.ak5GenJetsptmin1.jetPtMin = cms.double(1.0)

process.genParticlesForJets = cms.EDFilter("InputGenJetsParticleSelector",
    src = cms.InputTag("genParticles"),
    ignoreParticleIDs = cms.vuint32(
         1000022,
         1000012, 1000014, 1000016,
         2000012, 2000014, 2000016,
         1000039, 5100039,
         4000012, 4000014, 4000016,
         9900012, 9900014, 9900016,
         39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    tausAsJets = cms.bool(False)
)

newGenJets = cms.Sequence(process.genParticlesForJets* process.ak5GenJetsptmin1)

# histogram service
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('output.root')
)
#process.p = cms.Path(process.myanalysis)
# produce JPT jets before running analysis
#process.p = cms.Path(process.ZSPJetCorrections*process.JetPlusTrackCorrections*process.myanalysis)
process.p = cms.Path(newGenJets*process.myanalysis)
