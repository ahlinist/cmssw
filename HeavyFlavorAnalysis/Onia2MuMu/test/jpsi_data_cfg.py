import FWCore.ParameterSet.Config as cms

process = cms.Process('ANA')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("PhysicsTools.HepMCCandAlgos.allMuonsGenParticlesMatch_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

# Input source
process.source = cms.Source("PoolSource",
                            
    fileNames = cms.untracked.vstring(
    # first data
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/0A71AE7F-4DE2-DE11-8B2F-001D09F251CC.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/263E80C6-41E2-DE11-A194-001617C3B66C.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/26EC6965-4CE2-DE11-ABCF-003048D373AE.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/4693BF9A-40E2-DE11-BDBD-000423D944F8.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/54A3A373-4CE2-DE11-8658-000423D99AAA.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/68ED95B5-50E2-DE11-B4C8-001D09F27003.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/7E34865F-45E2-DE11-896A-000423D98F98.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/E432BCD7-55E2-DE11-B670-001617C3B6CC.root',
    'rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/596/F67BCF17-48E2-DE11-98B1-000423D94534.root'
    )
)

process.GlobalTag.globaltag = 'GR09_P_V7::All'

process.MuonAnalysis = cms.EDAnalyzer("Onia2MuMu",
    OutputFileName       = cms.string('jpsi_data09.root'),
    OniaType             = cms.int32(443),
    OniaMaxCat           = cms.int32(6),
    skimOnOniaMaxCat     = cms.bool(False),
    DebugLevel           = cms.int32(0),
    genParticlesLabel    = cms.InputTag("genParticles"),
    # StandAloneMuonsLabel = cms.InputTag("standAloneMuons"),
    GlobalMuonsLabel     = cms.InputTag("globalMuons"),
    MuonsLabel           = cms.InputTag("muons"),
    CaloMuonsLabel       = cms.InputTag("calomuons"),
    BeamSpotLabel        = cms.InputTag("offlineBeamSpot"),
    PrimaryVertexLabel   = cms.InputTag("offlinePrimaryVerticesWithBS"),
    UsePrimaryNoMuons    = cms.bool(True),
    TrackLabel           = cms.InputTag("generalTracks"),
    PhotonLabel          = cms.InputTag("particleFlow"),
    PhotonMinEnergy      = cms.double(2.0),
    triggerEventLabel    = cms.string("hltTriggerSummaryAOD"),
    triggerResultsLabel  = cms.string("TriggerResults"),
    HLTprocessName8e29   = cms.string("HLT"),
    HLTprocessName1e31   = cms.string("HLT"),  
    L1GTReadoutRec       = cms.InputTag("gtDigis"),
    L1MuonLabel          = cms.InputTag("l1extraParticles"),
    PATMuonsLabel        = cms.InputTag("selectedLayer1Muons"),
    # PATPhotonsLabel      = cms.InputTag("selectedLayer1Photons"),
    useKinFit            = cms.bool(False),
    StoreGenFlag         = cms.bool(False),
    StoreHLTFlag         = cms.bool(True),
    StoreL1Flag          = cms.bool(True),
    StoreTrkFlag         = cms.bool(True),
    StorePhotonFlag      = cms.bool(False),
    # StorePFMuonFlag      = cms.bool(True),
    StoreTRKMuonFlag     = cms.bool(True),
    StoreGLBMuonFlag     = cms.bool(True),
    StoreCALMuonFlag     = cms.bool(True),
    StoreBeamSpotFlag    = cms.bool(True),
    StorePriVtxFlag      = cms.bool(True),
    StoreOniaFlag        = cms.bool(True),
    StoreChicFlag        = cms.bool(False),
    StoreBpFlag          = cms.bool(False),                     
    StoreWSOnia          = cms.bool(True),                                  
    StoreOniaRadiation   = cms.bool(False),
    UsingBeamSpot        = cms.bool(False),
    minimumFlag          = cms.bool(False),
    UsingAOD             = cms.bool(False),
    StorePATFlag         = cms.bool(False)
)

# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# different choices of trigger conditions:
# bsc minbias
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41)')
# bsc minbias and veto on beam halo
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
# bsc minbias in coincidence with bptx
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41)')
# bsc minbias in coinidence with bptx and veto on beam halo
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


# this is for filtering on HLT physics bit
process.hltPhysicsDeclared = cms.EDFilter("HLTHighLevel",
                                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                 HLTPaths = cms.vstring("HLT_PhysicsDeclared"
                                                        ),
                                 eventSetupPathsKey = cms.string(''),
                                 andOr = cms.bool(True),
                                 throw = cms.bool(True)
                                 )


# Beamspot temporary fix
from CondCore.DBCommon.CondDBSetup_cfi import *
process.firstCollBeamspot = cms.ESSource("PoolDBESSource",
                                         CondDBSetup,
                                         connect = cms.string("frontier://PromptProd/CMS_COND_31X_BEAMSPOT"),
                                         # connect = cms.string("sqlite_file:/afs/cern.ch/user/y/yumiceva/public/BeamSpotObjects_2009_v1_express@a35f2218-e25f-11de-9d9b-0018f34695d6.db"),
                                         toGet = cms.VPSet(cms.PSet(record = cms.string("BeamSpotObjectsRcd"),
                                                                    tag =cms.string("BeamSpotObjects_2009_v2_offline"))
                                                           )
                                         )
process.es_prefer_firstCollBeamspot = cms.ESPrefer("PoolDBESSource","firstCollBeamspot")
process.load('RecoVertex.BeamSpotProducer.BeamSpot_cfi')

# filter on lumisections
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('123592:2-123592:14','123596:2-123596:144','123615:33-123615:91')

process.ana_step = cms.Path(process.hltLevel1GTSeed + process.hltPhysicsDeclared + process.offlineBeamSpot + process.MuonAnalysis)
#process.ana_step = cms.EndPath(process.MuonAnalysis)

# Schedule definition
process.schedule = cms.Schedule(process.ana_step)

