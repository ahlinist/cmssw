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
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")



process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(
'file:Spring10_TTbar_preprod.root'
)

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    #wantSummary = cms.untracked.bool(True)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.printTree = cms.EDFilter("ParticleTreeDrawer",
    status = cms.untracked.vint32(3),
    src = cms.InputTag("genParticleCandidates"),
    printIndex = cms.untracked.bool(True)
)



#############   Include the jet corrections ##########
#from JetMETCorrections.Configuration.JetCorrectionEra_cff import *
#JetCorrectionEra.era = 'Summer09_7TeV_ReReco332' # applies to L2 & L3 only
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorAK5PF") 



###########  EB SPIKE CLEANING BEGIN #####################

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')
#process.load('TrackingTools/Configuration/TrackingTools_cff')
#process.load('RecoEgamma/EgammaElectronProducers/gsfElectronSequence_cff')

process.GlobalTag.globaltag = cms.string('GR_R_35X_V8::All')
#process.GlobalTag.globaltag = cms.string('START3X_V26::All')

from RecoEcal.EgammaClusterProducers.ecalRecHitFlags_cfi import *
from RecoEcal.EgammaClusterProducers.hybridSuperClusters_cfi import *
from RecoEgamma.EgammaPhotonProducers.photons_cfi import *
hybridSuperClusters.RecHitFlagToBeExcluded = ( ecalRecHitFlag_kFaultyHardware,
                                             ecalRecHitFlag_kPoorCalib,
                                             #ecalRecHitFlag_kOutOfTime,
                                             ecalRecHitFlag_kDead)
hybridSuperClusters.RecHitSeverityToBeExcluded = (3,4)
hybridSuperClusters.severityRecHitThreshold = 4.
hybridSuperClusters.severitySpikeId = 2
hybridSuperClusters.severitySpikeThreshold = 0.95
hybridSuperClusters.excludeFlagged = True

process.ecalCleanClustering = cms.Sequence(process.hybridClusteringSequence*process.photonSequence*process.photonIDSequence)


###########  EB SPIKE CLEANING END   #####################

## produce JPT jets
process.load('JetMETCorrections.Configuration.ZSPJetCorrections332_cff')
process.load('JetMETCorrections.Configuration.JetPlusTrackCorrections_cff')
process.ak5JPTJetsSequence = cms.Sequence(
   process.ZSPJetCorrectionsAntiKt5*
   process.JetPlusTrackCorrectionsAntiKt5
   )



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
    JetCorrectionService_pfakt7 = cms.string('L2L3JetCorrectorAK7PF'),
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
    jetsgenakt5 = cms.untracked.InputTag("ak5GenJetsptmin2"),
    jetsgenakt7 = cms.untracked.InputTag("ak7GenJetsptmin2"),
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

# --- to recover the ak5 GenJets:
process.load("RecoJets.Configuration.GenJetParticles_cff")
#process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.load("RecoJets.Configuration.RecoGenJets_cff")


from RecoJets.JetProducers.ak5GenJets_cfi import ak5GenJets
process.ak5GenJetsptmin2 = ak5GenJets.clone()
process.ak5GenJetsptmin2.jetPtMin = cms.double(2.0)

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

process.new_ak5GenJets = cms.Sequence(process.genParticlesForJets* process.ak5GenJetsptmin2)

# --- to recover the ak7 GenJets:
process.ak7GenJetsptmin2 = ak5GenJets.clone()
process.ak7GenJetsptmin2.rParam = cms.double(0.7)
process.ak7GenJetsptmin2.jetPtMin = cms.double(2.0)

process.new_ak7GenJets = cms.Sequence(process.genParticlesForJets* process.ak7GenJetsptmin2)

process.p = cms.Path(process.new_ak5GenJets*process.new_ak7GenJets*process.ecalCleanClustering*process.ak5JPTJetsSequence*process.myanalysis)
