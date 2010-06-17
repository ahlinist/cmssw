import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

process.load("FWCore.MessageLogger.MessageLogger_cfi")


process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticleCandidates_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")



#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(
#'file:/cmsrm/pc18/pandolf/CMSSW_3_5_7/src/JetMETCorrections/GammaJet/test/eventi_136097.root'
'file:/cmsrm/pc18/pandolf/CMSSW_3_6_3/src/JetMETCorrections/GammaJet/test/events_136100.root'
)

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.options = cms.untracked.PSet(
#    SkipEvent = cms.untracked.vstring('ProductNotFound')
#    #wantSummary = cms.untracked.bool(True)
#)

process.MessageLogger.cerr.FwkReport.reportEvery = 100


#############   Include the jet corrections ##########
#from JetMETCorrections.Configuration.JetCorrectionEra_cff import *
#JetCorrectionEra.era = 'Summer09_7TeV_ReReco332' # applies to L2 & L3 only
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")


#monster track event cleaning
process.monster = cms.EDFilter(
   "FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.2)
)


###########  EB SPIKE CLEANING BEGIN #####################

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
#process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
#process.load('Configuration/StandardSequences/Reconstruction_cff')
#process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.load('Configuration/EventContent/EventContent_cff')
#process.load('TrackingTools/Configuration/TrackingTools_cff')

process.GlobalTag.globaltag = cms.string('GR_R_36X_V12::All')
#process.GlobalTag.globaltag = cms.string('GR_R_35X_V8::All')
#process.GlobalTag.globaltag = cms.string('START36_V10::All')

#from RecoEcal.EgammaClusterProducers.ecalRecHitFlags_cfi import *
#from RecoEcal.EgammaClusterProducers.hybridSuperClusters_cfi import *
#from RecoEgamma.EgammaPhotonProducers.photons_cfi import *
#hybridSuperClusters.RecHitFlagToBeExcluded = ( ecalRecHitFlag_kFaultyHardware,
#                                             ecalRecHitFlag_kPoorCalib,
#                                             #ecalRecHitFlag_kOutOfTime,
#                                             ecalRecHitFlag_kDead)
#hybridSuperClusters.RecHitSeverityToBeExcluded = (3,4)
#hybridSuperClusters.severityRecHitThreshold = 4.
#hybridSuperClusters.severitySpikeId = 2
#hybridSuperClusters.severitySpikeThreshold = 0.95
#hybridSuperClusters.excludeFlagged = True

#process.ecalCleanClustering = cms.Sequence(process.hybridClusteringSequence*process.photonSequence*process.photonIDSequence)

#process.load('EGamma/EGammaSkims/promptRecoTrackCorrections_cff')
process.load('EGamma/EGammaSkims/cleanReRecoSequence_cff')

#process.ecalCleanClustering = cms.Sequence(process.cleanedEcalClusters*process.cleanedEgammaSkimReco)
process.ecalCleanClustering = cms.Sequence(process.cleanedEgammaSkimReco)

###########  EB SPIKE CLEANING END   #####################

## produce JPT jets
process.load('RecoJets.Configuration.RecoJPTJets_cff')



process.myanalysis = cms.EDAnalyzer("GammaJetAnalyzer",
    debug = cms.bool(False),
    recoProducer = cms.string('ecalRecHit'),
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    #JetCorrectionService_akt5 = cms.string('L2L3JetCorrectorAK5Calo'),
    #JetCorrectionService_pfakt5 = cms.string('L2L3JetCorrectorAK5PF'),
    #JetCorrectionService_pfakt7 = cms.string('L2L3JetCorrectorAK7PF'),
    JetCorrectionService_akt5 = cms.string('ak5CaloJetsL2L3'),
    JetCorrectionService_pfakt5 = cms.string('ak5PFJetsL2L3'),
    JetCorrectionService_pfakt7 = cms.string('ak7PFJetsL2L3'),
    jetsite = cms.untracked.InputTag("iterativeCone5CaloJets"),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetsakt5 = cms.untracked.InputTag("ak5CaloJets"),
    jetssis5 = cms.untracked.InputTag("sisCone5CaloJets"),
    jetssis7 = cms.untracked.InputTag("sisCone7CaloJets"),
    jetsjptak5 = cms.untracked.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    jetspfite = cms.untracked.InputTag("iterativeCone5PFJets"),
    jetspfkt4 = cms.untracked.InputTag("kt4PFJets"),
    jetspfkt6 = cms.untracked.InputTag("kt6PFJets"),
    jetspfakt5 = cms.untracked.InputTag("ak5PFJets"),
    jetspfakt7 = cms.untracked.InputTag("ak7PFJets"),
    jetspfsis5 = cms.untracked.InputTag("sisCone5PFJets"),
    jetspfsis7 = cms.untracked.InputTag("sisCone7PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgenite = cms.untracked.InputTag("iterativeCone5GenJets"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgenakt5 = cms.untracked.InputTag("ak5GenJets"),
    jetsgenakt7 = cms.untracked.InputTag("ak7GenJets"),
    jetsgensis5 = cms.untracked.InputTag("sisCone5GenJets"),
    jetsgensis7 = cms.untracked.InputTag("sisCone7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::HLT"),
    vertices = cms.untracked.InputTag("offlinePrimaryVertices"),
    genjetptthr = cms.double(5.),
    calojetptthr = cms.double(3.),
    pfjetptthr = cms.double(4.),
    jptjetptthr = cms.double(4.),
    genjetnmin = cms.int32(10),
    pfjetnmin = cms.int32(10),
    jptjetnmin = cms.int32(10)
)


process.p = cms.Path(process.monster*process.ecalCleanClustering*process.recoJPTJets*process.myanalysis)
