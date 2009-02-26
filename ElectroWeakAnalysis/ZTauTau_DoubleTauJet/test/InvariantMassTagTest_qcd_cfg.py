import FWCore.ParameterSet.Config as cms

process = cms.Process("TestMassTag")

#Input Events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/106AA2BA-89BA-DD11-965F-001617C3B710.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/386984B1-89BA-DD11-8FC4-000423D98EC8.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/50D9A0BB-8ABA-DD11-98FB-001617C3B70E.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/52F5B5A7-89BA-DD11-BC49-001D09F25442.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/683D740B-8CBA-DD11-9305-000423D99AAA.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/74F8EBAB-89BA-DD11-B24D-001D09F24259.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/8AB2EAA5-89BA-DD11-9446-001617C3B5F4.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/C0AF0449-8ABA-DD11-BDD4-001D09F251E0.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/E86B681A-8ABA-DD11-9AAA-000423D98F98.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0001/EA70E261-8ABA-DD11-A42A-0019B9F70468.root',
     '/store/relval/CMSSW_2_2_0/RelValQCD_Pt_50_80/GEN-SIM-RECO/IDEAL_V9_v3/0002/D6420006-00BB-DD11-94E1-000423D98DD4.root'
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
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('./mass_tag_qcd50_80.root')
)


process.tauGenJet = cms.Sequence(process.genJetParticles*process.genParticlesAllStableNoNuBSM*process.iterativeCone5GenJets)
process.p = cms.Path(process.mix*process.trackingParticles*process.tauGenJet*process.PFTau*process.massTagTest)


