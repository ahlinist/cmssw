import FWCore.ParameterSet.Config as cms

process = cms.Process("TestMassTag")

#Input Events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0000/024D51A2-1EBA-DD11-B114-000423D6B48C.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0000/3EB23861-2CBA-DD11-9798-000423D944FC.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0000/48AC084C-2CBA-DD11-B886-000423D944F8.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/68AF79B4-89BA-DD11-AB31-000423D98FBC.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/8CF32D6A-8ABA-DD11-B002-000423D95220.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/CC4986A8-8ABA-DD11-AED0-000423D94908.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/D84ED9C2-89BA-DD11-A280-000423D94990.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/E4BA9EDF-89BA-DD11-A245-001617C3B6CC.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/FA03B176-8BBA-DD11-9D8D-000423D6B444.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/FABD890B-8CBA-DD11-B88F-000423D99BF2.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0001/FAF7515B-8ABA-DD11-88CF-001617E30D40.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0002/CA4525DB-FFBA-DD11-9C18-001617C3B65A.root',
    '/store/relval/CMSSW_2_2_0/RelValZTT/GEN-SIM-RECO/IDEAL_V9_v3/0002/E007D363-19BB-DD11-BC98-000423D992A4.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.default.limit = -1
process.MessageLogger.suppressInfo = ['ckfTrackCandidates', 'IterativeConeJetProducer']

# Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.prefer("VolumeBasedMagneticFieldESProducer")

# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

#Condition
process.load("Configuration.StandardSequences.FakeConditions_cff")

# PF Tau and discrimination
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

# Gen Jet
process.load("RecoJets.JetProducers.iterativeCone5GenJets_cff")
process.load("RecoJets.Configuration.GenJetParticles_cff")

process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

###process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi") ! does not work for 2_2_0
process.load("RecoJets.Configuration.GenJetParticles_cff")

process.massTagTest = cms.EDFilter("InvariantMassTagTest",
    LeadingTrackPt = cms.untracked.double(6.0),
    PFSimParticleProducer = cms.untracked.string('particleFlowSimParticle'),
    ClusterTrackMatchingCone = cms.untracked.double(0.08),
    PFTauProducer = cms.untracked.string('pfRecoTauProducerHighEfficiency'),
    ClusterJetMatchingCone = cms.untracked.double(0.4),
    SignalCone = cms.untracked.double(0.1),
    TrackingTruthProducer = cms.untracked.InputTag("mergedtruth","MergedTrackTruth"),
    MassTagProd = cms.untracked.string('invariantMass'),
    PFTauDiscriminatorByIsolationProducer = cms.untracked.string('pfRecoTauDiscriminationByIsolationHighEfficiency'),
    InvariantMassCutoff = cms.untracked.double(100.0),
    MatchingCone = cms.untracked.double(0.1),
    IsolationCone = cms.untracked.double(0.4),
    GenJetProducer = cms.untracked.InputTag("iterativeCone5GenJets"),
    GenParticleProducer = cms.untracked.InputTag("source")
)

process.genParticlesAllStableNoNuBSM = cms.EDFilter("InputGenJetsParticleSelector",
    src = cms.InputTag('genParticles'),
    ignoreParticleIDs = cms.vuint32(12, 14, 16, 1000022, 2000012, 
        2000014, 2000016, 1000039, 5000039, 4000012, 
        9900012, 9900014, 9900016, 39),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(True),
    excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
    stableOnly = cms.bool(True),
    tausAsJets = cms.bool(True),
    verbose = cms.untracked.bool(False)
)
process.tauFilter = cms.EDFilter("TauJetMCFilter",
    PtMuon = cms.double(0.0),
    EtaTauMax = cms.double(2.5),
    doPrintOut = cms.bool(False),
    PtElec = cms.double(0.0),
    #  "etau" "mutau" "tautau" "emu" "ee" "mumu". "tau" corresponds to hadronic decays
    includeList = cms.vstring('tautau'),
    fillHistos = cms.bool(False),
    EtaTauMin = cms.double(0.0),
    EtaMuonMax = cms.double(2.5),
    EtaElecMax = cms.double(2.5),
    EtTau = cms.double(0.0),
    GenParticles = cms.InputTag("source")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('./mass_tag_sig_ztt.root')
)

#process.tauGenJet = cms.Sequence(process.genJetParticles*process.iterativeCone5GenJets)
process.tauGenJet = cms.Sequence(process.genJetParticles*process.genParticlesAllStableNoNuBSM*process.iterativeCone5GenJets)
process.p = cms.Path(process.mix*process.trackingParticles*process.tauGenJet*process.tauFilter*process.PFTau*process.massTagTest)


