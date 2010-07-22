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
'file:Wenu_copy/eventi_1_1_Jx6.root'
)

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.options = cms.untracked.PSet(
#    SkipEvent = cms.untracked.vstring('ProductNotFound')
#    #wantSummary = cms.untracked.bool(True)
#)

process.MessageLogger.cerr.FwkReport.reportEvery = 10



#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")


#monster track event cleaning
process.monster = cms.EDFilter(
   "FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.2)
)



## produce JPT jets
#process.load('RecoJets.Configuration.RecoJPTJets_cff')



process.myanalysis = cms.EDAnalyzer("GammaJetAnalyzer",
    debug = cms.bool(False),
    recoProducer = cms.string('ecalRecHit'),
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Photonsrc = cms.untracked.InputTag("photons"),
    recoCollection = cms.string('EcalRecHitsEB'),
    JetCorrectionService_akt5 = cms.string('ak5CaloL2L3'),
    JetCorrectionService_akt7 = cms.string('ak7CaloL2L3'),
    JetCorrectionService_pfakt5 = cms.string('ak5PFL2L3'),
    JetCorrectionService_pfakt7 = cms.string('ak7PFL2L3'),
    #jetsite = cms.untracked.InputTag("iterativeCone5CaloJets"),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetsakt5 = cms.untracked.InputTag("ak5CaloJets"),
    jetsakt7 = cms.untracked.InputTag("ak7CaloJets"),
    jetsjptak5 = cms.untracked.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    #jetspfite = cms.untracked.InputTag("iterativeCone5PFJets"),
    jetspfkt4 = cms.untracked.InputTag("kt4PFJets"),
    jetspfkt6 = cms.untracked.InputTag("kt6PFJets"),
    jetspfakt5 = cms.untracked.InputTag("ak5PFJets"),
    jetspfakt7 = cms.untracked.InputTag("ak7PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    #jetsgenite = cms.untracked.InputTag("iterativeCone5GenJets"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgenakt5 = cms.untracked.InputTag("ak5GenJetsptmin2"),
    jetsgenakt7 = cms.untracked.InputTag("ak7GenJetsptmin2"),
    #jetsgenakt5 = cms.untracked.InputTag("new_ak5GenJets"),
    #jetsgenakt7 = cms.untracked.InputTag("new_ak7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::REDIGI37X"),
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

#process.genParticlesForJets = cms.EDFilter("InputGenJetsParticleSelector",
#    src = cms.InputTag("genParticles"),
#    ignoreParticleIDs = cms.vuint32(
#         1000022,
#         1000012, 1000014, 1000016,
#         2000012, 2000014, 2000016,
#         1000039, 5100039,
#         4000012, 4000014, 4000016,
#         9900012, 9900014, 9900016,
#         39),
#    partonicFinalState = cms.bool(False),
#    excludeResonances = cms.bool(True),
#    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
#    tausAsJets = cms.bool(False)
#)


from RecoJets.JetProducers.ak5GenJets_cfi import ak5GenJets
process.ak5GenJetsptmin2 = ak5GenJets.clone()
process.ak5GenJetsptmin2.jetPtMin = cms.double(2.0)

process.new_ak5GenJets = cms.Sequence(process.genParticlesForJets* process.ak5GenJetsptmin2)
#process.new_ak5GenJets = cms.Sequence(process.ak5GenJetsptmin2)

# --- to recover the ak7 GenJets:
process.ak7GenJetsptmin2 = ak5GenJets.clone()
process.ak7GenJetsptmin2.rParam = cms.double(0.7)
process.ak7GenJetsptmin2.jetPtMin = cms.double(2.0)

process.new_ak7GenJets = cms.Sequence(process.genParticlesForJets* process.ak7GenJetsptmin2)
#process.new_ak7GenJets = cms.Sequence(process.ak7GenJetsptmin2)

process.p = cms.Path(process.new_ak5GenJets*process.new_ak7GenJets*process.monster*process.myanalysis)
