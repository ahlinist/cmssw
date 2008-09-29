import FWCore.ParameterSet.Config as cms

process = cms.Process("TestMassTag")

#Input Events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/0A366D53-7C6C-DD11-8D69-000423D94A20.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/1AFF04B0-7C6C-DD11-822A-000423D98C20.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/1C34D986-7C6C-DD11-9471-001617DBD5AC.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/62244544-7C6C-DD11-BC1A-000423D98E6C.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/6C8EE0B6-7C6C-DD11-9697-000423D99AAE.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/76591936-7C6C-DD11-9AE3-001617E30E28.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8047F04D-7C6C-DD11-8B43-001617C3B77C.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/90F05115-7C6C-DD11-AF42-001617C3B6CC.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/94113FF3-7C6C-DD11-BAD9-001617E30CE8.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/BE4CA84D-7C6C-DD11-9185-001617E30D4A.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/DA550D7D-7C6C-DD11-945C-001617C3B6C6.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/E2B86924-7C6C-DD11-960C-000423D99E46.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/EE9C82AC-7C6C-DD11-A1E9-000423D99E46.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F8AB2A46-7C6C-DD11-8CA0-001617E30E28.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/FA6B0D56-7C6C-DD11-BD3F-000423D9970C.root',
      '/store/relval/CMSSW_2_1_4/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0005/967F66ED-8B6C-DD11-B4B9-000423D6B358.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.default.limit = -1
process.MessageLogger.suppressInfo = ['ckfTrackCandidates', 'IterativeConeJetProducer']

# Magnetic Field 
process.load("MagneticField.Engine.uniformMagneticField_cfi")
process.UniformMagneticFieldESProducer.ZFieldInTesla = cms.double(4.0)
process.prefer("UniformMagneticFieldESProducer")

# Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

#Condition
process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.load("RecoParticleFlow.Configuration.RecoParticleFlow_cff")
process.load("RecoJets.JetAssociationProducers.ic5PFJetTracksAssociatorAtVertex_cfi")

process.load("RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolation_cfi")
process.pfRecoTauDiscriminationByIsolation.ApplyDiscriminationByECALIsolation = True

process.load("RecoTauTag.RecoTau.PFRecoTauTagInfoProducer_cfi")
process.load("RecoTauTag.RecoTau.PFRecoTauProducer_cfi")
process.pfRecoTauProducer.ECALSignalConeSizeFormula = cms.string('0.1')

process.load("RecoJets.JetProducers.iterativeCone5GenJetsNoNuBSM_cff")
process.load("RecoJets.Configuration.GenJetParticles_cff")

process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")

process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")

process.prefer("UniformMagneticFieldESProducer")
process.massTagTest = cms.EDFilter("InvariantMassTagTest",
    LeadingTrackPt = cms.untracked.double(6.0),
    PFSimParticleProducer = cms.untracked.string('particleFlowSimParticle'),
    ClusterTrackMatchingCone = cms.untracked.double(0.08),
    PFTauProducer = cms.untracked.string('pfRecoTauProducer'),
    ClusterJetMatchingCone = cms.untracked.double(0.4),
    SignalCone = cms.untracked.double(0.1),
    TrackingTruthProducer = cms.untracked.InputTag("mergedtruth","MergedTrackTruth"),
    MassTagProd = cms.untracked.string('invariantMass'),
    PFTauDiscriminatorByIsolationProducer = cms.untracked.string('pfRecoTauDiscriminationByIsolation'),
    InvariantMassCutoff = cms.untracked.double(100.0),
    MatchingCone = cms.untracked.double(0.1),
    IsolationCone = cms.untracked.double(0.4),
    GenJetProducer = cms.untracked.InputTag("iterativeCone5GenJetsNoNuBSM"),
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

process.tauGenJet = cms.Sequence(process.genParticles*process.genParticlesAllStableNoNuBSM*process.iterativeCone5GenJetsNoNuBSM)
process.pfTauReco = cms.Sequence(process.particleFlowSimParticle*process.ic5PFJetTracksAssociatorAtVertex*process.pfRecoTauTagInfoProducer*process.pfRecoTauProducer*process.pfRecoTauDiscriminationByIsolation)
process.p = cms.Path(process.mix*process.trackingParticles*process.tauGenJet*process.tauFilter*process.pfTauReco*process.massTagTest)


