import FWCore.ParameterSet.Config as cms

process = cms.Process('ANA')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load('RecoEcal/EgammaClusterProducers/geometryForClustering_cff')

process.load("PhysicsTools.HepMCCandAlgos.allMuonsGenParticlesMatch_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

# Input source
process.source = cms.Source("PoolSource",
     
    fileNames = cms.untracked.vstring(
      ### Jpsi prompt 0-20
     ##  '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/2013DB5B-F6C4-DD11-B170-000423D9970C.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/2806E882-EFC4-DD11-9235-001D09F250AF.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/2CAEF484-EEC4-DD11-B2F8-0030487D0D3A.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/3C4E2F85-EFC4-DD11-B8E4-001D09F24FE7.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/44EA5097-EFC4-DD11-8FD1-001D09F241B9.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/4C59813C-F8C4-DD11-AD34-000423D992A4.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/64222E76-EFC4-DD11-B906-001D09F24EAC.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/7873D78F-EFC4-DD11-A494-001D09F23A84.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/D838EF84-EFC4-DD11-A7FE-001D09F2424A.root',
##       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_0_20/GEN-SIM-RECO/IDEAL_V9_v1/0003/E0036983-EFC4-DD11-B294-001D09F24F1F.root'

      ### Jpsi prompt 20-inf
       '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/24A8E425-CBC4-DD11-9807-001D09F23E53.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/42291295-CBC4-DD11-AC40-0030487D1BCC.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/4824D94F-CBC4-DD11-9A41-001D09F24DA8.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/7C8D5991-CBC4-DD11-8FC7-000423D99660.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/9A51C782-CBC4-DD11-A81A-001617C3B710.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/B6535C70-CBC4-DD11-B69B-0030487D1BCC.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/BEB95B9E-CBC4-DD11-96CD-000423D9939C.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/DC12A178-CBC4-DD11-98F6-001D09F241D2.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/DE1A649C-CBC4-DD11-8099-000423D6CA6E.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0002/F04FAB7C-CBC4-DD11-8070-000423D991F0.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/0CD9824F-EFC4-DD11-A792-001D09F29619.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/44606A5D-EFC4-DD11-A3D0-000423D6A6F4.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/4895C079-CBC4-DD11-986F-001D09F23D04.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/4C18B363-EFC4-DD11-A595-001D09F25325.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/66B80D4A-EFC4-DD11-AB7A-001D09F24353.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/70F3448A-F6C4-DD11-83AD-000423D9970C.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/8C28454A-EFC4-DD11-B9CC-000423D94534.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/CEF23D68-EFC4-DD11-80A9-001D09F251FE.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0003/D4217164-EFC4-DD11-B434-001D09F2546F.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0004/0246A5EF-05C5-DD11-B522-000423D98F98.root',
        '/store/relval/CMSSW_2_2_1/RelValJpsiMM_Pt_20_inf/GEN-SIM-RECO/IDEAL_V9_v1/0004/3C00FA80-FEC4-DD11-83BB-001617DBCF6A.root'
     
    )
)


process.MuonAnalysis = cms.EDAnalyzer("Onia2MuMu",
    OutputFileName       = cms.string('InclusiveppToMu_pt25_225.root'),
    OniaType             = cms.int32(443),
    DebugLevel           = cms.int32(0),
    genParticlesLabel    = cms.InputTag("genParticles"),
    StandAloneMuonsLabel = cms.InputTag("standAloneMuons"),
    GlobalMuonsLabel     = cms.InputTag("globalMuons"),
    MuonsLabel           = cms.InputTag("muons"),
    CaloMuonsLabel       = cms.InputTag("calomuons"),
    BeamSpotLabel        = cms.InputTag("offlineBeamSpot"),
    PrimaryVertexLabel   = cms.InputTag("offlinePrimaryVertices"),
    TrackLabel           = cms.InputTag("generalTracks"),
    PhotonLabel          = cms.InputTag("particleFlow"),
    triggerEventLabel    = cms.InputTag("hltTriggerSummaryAOD::HLT"),
    L1GTReadoutRec       = cms.InputTag("gtDigis"),
    HLTriggerResults     = cms.InputTag("TriggerResults::HLT"),
    L1MuonLabel          = cms.InputTag("hltL1extraParticles"),
    PATMuonsLabel        = cms.InputTag("selectedLayer1Muons"),
    # PATPhotonsLabel      = cms.InputTag("selectedLayer1Photons"),
    StoreGenFlag         = cms.bool(True),
    StoreHLTFlag         = cms.bool(True),
    StoreL1Flag          = cms.bool(True),
    StoreTrkFlag         = cms.bool(False),
    StorePhotonFlag      = cms.bool(False),
    # StorePFMuonFlag      = cms.bool(True),
    StoreSTAMuonFlag     = cms.bool(True),
    StoreGLBMuonFlag     = cms.bool(True),
    StoreAllMuonFlag     = cms.bool(True),
    StoreBeamSpotFlag    = cms.bool(True),
    StorePriVtxFlag      = cms.bool(True),
    StoreOniaFlag        = cms.bool(True),
    StoreChicFlag        = cms.bool(False),
    StoreBpFlag          = cms.bool(False),                     
    StoreWSOnia          = cms.bool(False),                                  
    StoreOniaRadiation   = cms.bool(False),
    UsingBeamSpot        = cms.bool(True),
    minimumFlag          = cms.bool(False),
    UsingAOD             = cms.bool(True),
    StorePATFlag         = cms.bool(False)
)

process.ana_step = cms.EndPath(process.MuonAnalysis)

# Schedule definition
process.schedule = cms.Schedule(process.ana_step)

