import FWCore.ParameterSet.Config as cms

process = cms.Process("TestMassTag")

#Input Events
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/0A130B61-806C-DD11-895F-000423D944DC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/0ADAA215-816C-DD11-B9C8-000423D94E70.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/109EA1CB-7F6C-DD11-B5C8-001617E30CD4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/127D2783-7F6C-DD11-8FD5-000423D9970C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/1C62B6EB-7F6C-DD11-A6D8-000423D99896.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/1E11203E-816C-DD11-9394-000423D991D4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/20D85878-816C-DD11-AB96-001617C3B78C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/2447C580-806C-DD11-8975-000423D95220.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/245F3C79-806C-DD11-93FC-000423D98844.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/249317B0-806C-DD11-9935-001617E30CA4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/280E2A78-816C-DD11-ADFF-000423D98634.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/2E33D147-806C-DD11-9A02-000423D94E1C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/308E1E2A-806C-DD11-9D52-000423D991F0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/32E11150-806C-DD11-A7CE-000423D944FC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/3EE99BA6-816C-DD11-9A76-001617C3B79A.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/40AE459B-806C-DD11-A4CE-001617DBD316.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/42567A16-816C-DD11-8F66-000423D986C4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/4612CB0F-816C-DD11-9EBF-000423D98BC4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/46B86E25-816C-DD11-9573-000423D99EEE.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/5ADAB89B-806C-DD11-BC52-001617E30D52.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/5CB0BC47-806C-DD11-8C5E-000423D99F1E.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/5E45554E-816C-DD11-882B-000423D9970C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/64604CE6-806C-DD11-A60E-0016177CA7A0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/688BDA3C-816C-DD11-8553-001617E30D12.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/6AD9D870-816C-DD11-8F1E-001617E30D00.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/7052C1E7-7F6C-DD11-A1E0-000423D9517C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/7287D681-806C-DD11-A942-001617C3B6E8.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/7437276C-7F6C-DD11-AD44-001617C3B66C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/7C3D937D-806C-DD11-9147-000423D94990.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8450A978-816C-DD11-9E5E-001617E30F58.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/86EA637C-816C-DD11-8133-000423D9939C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/86F8E83E-816C-DD11-9ECC-000423D98A44.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/882760AA-816C-DD11-90F8-000423D99AAA.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/887A80E6-816C-DD11-981C-001617DBD472.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8896F0B4-816C-DD11-919B-000423D944FC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8AF620E1-816C-DD11-9BEA-001617E30E28.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8C44C129-806C-DD11-BA61-000423D99658.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/8E1C9B2A-806C-DD11-9FCD-000423D944FC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/908AD48B-7F6C-DD11-BF8C-000423D98BC4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/98A427B6-806C-DD11-ADA0-001617C3B66C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/9C1DE43F-816C-DD11-B2DB-000423D94494.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/9CECB35B-806C-DD11-BF59-001617C3B778.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/A4758E76-816C-DD11-ACA5-000423D9517C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/A492582B-816C-DD11-9617-000423D9997E.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/A8C7931B-816C-DD11-A8A4-001617DBCF6A.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/AA8F81C3-806C-DD11-A12F-001617E30D12.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/AAF174A9-816C-DD11-AA01-001617C3B6E8.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B067B7E1-806C-DD11-AB56-001617E30F50.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B4D33DA9-816C-DD11-BA5A-0016177CA7A0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B6BA4DBC-7F6C-DD11-AFBD-000423D9A212.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/B6E3E2B1-806C-DD11-A983-000423D174FE.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/BEB7D5B5-806C-DD11-AFFC-0019DB29C614.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/BEC577AA-816C-DD11-B61A-000423D9A2AE.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/C01878B5-806C-DD11-BA96-000423D94E1C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/C0603CC7-806C-DD11-B0BB-001617E30E28.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/C496C07C-806C-DD11-B368-000423D98B08.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/C6C74452-806C-DD11-B4AA-001617C3B70E.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/CAAF16D4-816C-DD11-9705-000423D99E46.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D022A62C-806C-DD11-B59E-001617C3B70E.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D058BC99-806C-DD11-9484-001617C3B6CC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/D2751DA5-816C-DD11-89BA-000423D60FF6.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/E095AB5D-806C-DD11-B214-001617C3B6CC.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/E8A256A1-816C-DD11-AE80-000423D99658.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/E8B2E193-806C-DD11-A9A4-001617E30CE8.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/EA2ECE7D-816C-DD11-A150-001617C3B66C.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F0DEA243-816C-DD11-9C2B-001617C3B6C6.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F0E4B733-806C-DD11-BE60-0016177CA7A0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F256E7AC-816C-DD11-8296-001617DBD316.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F2C849C1-7F6C-DD11-9640-0016177CA7A0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F4947B15-816C-DD11-87EB-000423D987E0.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/F4E05DE2-806C-DD11-8E9B-001617E30CD4.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0004/FADE973F-816C-DD11-9E02-000423D95030.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0005/32943684-886C-DD11-989D-000423D6BA18.root',
      '/store/relval/CMSSW_2_1_4/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V5_v1/0005/FCA8E4D7-AC6C-DD11-BDF0-001617C3B76E.root'
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

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('./mass_tag_qcd_leadTk.root')
)

process.tauGenJet = cms.Sequence(process.genParticles*process.genParticlesAllStableNoNuBSM*process.iterativeCone5GenJetsNoNuBSM)
process.pfTauReco = cms.Sequence(process.particleFlowSimParticle*process.ic5PFJetTracksAssociatorAtVertex*process.pfRecoTauTagInfoProducer*process.pfRecoTauProducer*process.pfRecoTauDiscriminationByIsolation)
process.p = cms.Path(process.mix*process.trackingParticles*process.tauGenJet*process.pfTauReco*process.massTagTest)


