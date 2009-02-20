import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('')
#process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.threshold = 'WARNING'
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Castor FastSimulation 
process.load('FastSimulation.ForwardDetectors.CastorTowerProducer_cfi') 
# Castor Reconstruction 
process.load('RecoLocalCalo.Castor.Castor_cfi')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SingleDiffractiveWmunuPlus_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.wmunuSelFilter = cms.EDFilter("WMuNuSelector",
    # Defaults follow...
    #
    # Input collections ->
    MuonTag = cms.untracked.InputTag("muons"),
    METTag = cms.untracked.InputTag("met"),
    JetTag = cms.untracked.InputTag("iterativeCone5CaloJets"),
    #UseOnlyGlobalMuons = cms.bool(True),
    #
    # Main cuts ->
    PtCut = cms.untracked.double(15.0),
    EtaCut = cms.untracked.double(2.1),
    IsRelativeIso = cms.untracked.bool(True),
    IsoCut03 = cms.untracked.double(0.1),
    MassTMin = cms.untracked.double(30.0),
    MassTMax = cms.untracked.double(200.0),
    #
    # To suppress Zmm ->
    #PtThrForZCount = cms.double(20.0),
    #
    # To suppress ttbar ->
    #AcopCut = cms.double(999999.),
    #EJetMin = cms.double(999999.),
    #NJetMax = cms.int32(999999)
)

process.genParticlesCalo = cms.EDProducer(
    "InputGenJetsParticleSelector",
    src = cms.InputTag("genParticles"),
    partonicFinalState = cms.bool(False),
    excludeResonances = cms.bool(False),
    excludeFromResonancePids = cms.vuint32(),
    tausAsJets = cms.bool(False),
    ignoreParticleIDs = cms.vuint32(
    1000022, 2000012, 2000014,
    2000016, 1000039, 5000039,
    4000012, 9900012, 9900014,
    9900016, 39, 12, 13, 14, 16)  ###These ID's will contribute to MET 
)

process.castorGen = cms.EDProducer("CASTORGenInfoDumper",
    GenParticlesLabel = cms.InputTag("genParticlesCalo"),
    CASTORParticleEnergyThreshold = cms.untracked.double(10.0),
    CASTORPhiSliceEnergyThreshold = cms.untracked.double(10.0),
    CASTOREtaMin = cms.untracked.double(5.2),
    CASTOREtaMax = cms.untracked.double(6.6)
)

process.castorTower = cms.EDProducer("CastorTowerEdmNtupleDumper",
    CastorTowersTag = cms.InputTag("CastorTowerReco"),
    TowerEnergyTresholdMin = cms.untracked.double(0.0),
    TowerEnergyTresholdMax = cms.untracked.double(20.0),
    NumberOfTresholds = cms.untracked.uint32(50)
)

process.hfTower = cms.EDProducer("HFTowerEdmNtupleDumperAOD",
    CaloTowersTag = cms.InputTag("towerMaker"),
    TowerEnergyTresholdMin = cms.untracked.double(0.0),
    TowerEnergyTresholdMax = cms.untracked.double(10.0),
    NumberOfTresholds = cms.untracked.uint32(50)
)

process.xiTower = cms.EDProducer("XiTowerDumper",
    CaloTowersTag = cms.InputTag("towerMaker"),
    MuonTag = cms.InputTag("muons"),
    UseMETInfo = cms.bool(True),
    CaloMETTag = cms.InputTag("met"),
    HFPlusTowerThreshold = cms.double(1.0),
    HFMinusTowerThreshold = cms.double(1.0),
    HEPlusTowerThreshold = cms.double(1.0),
    HEMinusTowerThreshold = cms.double(1.0),
    HBTowerThreshold = cms.double(1.0) 
)

process.xiTowerNoMET = cms.EDProducer("XiTowerDumper",
    CaloTowersTag = cms.InputTag("towerMaker"),
    MuonTag = cms.InputTag("muons"),
    UseMETInfo = cms.bool(False),
    CaloMETTag = cms.InputTag("met"),
    HFPlusTowerThreshold = cms.double(1.0),
    HFMinusTowerThreshold = cms.double(1.0),
    HEPlusTowerThreshold = cms.double(1.0),
    HEMinusTowerThreshold = cms.double(1.0),
    HBTowerThreshold = cms.double(1.0)
)

process.pileUpInfo = cms.EDProducer("PileUpEdmNtupleDumper")

process.MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
process.MyEventContent.outputCommands.append('keep *_*_*_Analysis')
process.MyEventContent.outputCommands.append('keep recoMuons_muons_*_*')
process.MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVerticesWithBS_*_*')

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.MyEventContent.outputCommands,
    fileName = cms.untracked.string('POMWIG_SDPlusWmunu_EdmDump_noPU.root'),
    #fileName = cms.untracked.string('POMWIG_SDPlusWmunu_EdmDump_InitialLumPU.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    )
)

process.p1 = cms.Path(process.wmunuSelFilter*process.CastorTowerReco*process.genParticlesCalo*process.castorGen*process.castorTower*process.hfTower*process.xiTower*process.xiTowerNoMET*process.pileUpInfo) 
process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.p1,process.out_step)
