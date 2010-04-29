import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")

# ###########
# process.load('Configuration.StandardSequences.Services_cff')
# process.load('Configuration/StandardSequences/GeometryExtended_cff')
# process.load('Configuration/StandardSequences/Reconstruction_cff')
# process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")

######################
# HF PMT cleaning; VERSION 2 is the currently recommended version, as of 22 April 2010.
version = 2   # version 1 = default (loose), version 2 = (medium), version 3 = (tight)

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
#process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.GlobalTag.globaltag = 'GR_R_35X_V6::All'##make sure to check the GT from DBS

# New SeverityLevelComputer that forces RecHits with UserDefinedBit0 set to be excluded from new rechit collection
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")
process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)

# HF RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
if version==1:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
elif version==2:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif version==3:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = cms.InputTag("hfrecoReflagged")
process.towerMakerWithHO.hfInput = cms.InputTag("hfrecoReflagged")

# Path and EndPath definitions
process.reflagging_step = cms.Path(process.hfrecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec*(process.recoJets*process.recoJetIds+process.recoTrackJets)*process.recoJetAssociations*process.metreco) # re-reco jets and met
#process.rereco_step = cms.Path(process.towerMaker*process.ak5CaloJets*process.met) # a simpler use case
#process.out_step = cms.EndPath(process.output)
######################

process.add_( cms.Service( "TFileService",
fileName = cms.string("WZ.root"), ##give a name to the output file
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
#         "/store/caf/user/meridian/InterestingEvents/133874/EGMExpressFilterInteresting_133874_358_1_1.root",
#         "/store/caf/user/meridian/InterestingEvents/133885/EGMExpressFilterInteresting_133885_62_1_1.root",
        "/store/caf/user/meridian/InterestingEvents/133877/EGMExpressFilterInteresting_133877_443_1_1.root"
    #'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F65A94F7-4141-DF11-9F4E-003048D47A80.root'
    #'/store/data/Commissioning10/MinimumBias/RECO/Apr1ReReco-v2/0140/FEAE8844-6E40-DF11-92E9-0026189438E8.root'
    #"/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec14thSkim_v1/0102/BABAF8C3-71EA-DE11-9D8C-0024E8768446.root"
    #'/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/E4590360-4CD7-DE11-8CB4-002618943896.root'
    #'/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/DABD5D6D-D4E2-DE11-8FFD-00261894387A.root'
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/596/F82DED93-36E2-DE11-9316-000423D9870C.root"
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/0E45A7CE-F5DD-DE11-9B2E-001617E30CC8.root"
    ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# jet corrections
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
#process.load("JetMETCorrections.Configuration.L2L3Corrections_2360GeV_cff")

# summary
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
process.bit40OR41 = hltLevel1GTSeed.clone(L1TechTriggerSeeding = cms.bool(True), L1SeedsLogicalExpression = cms.string('40 OR 41'))

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
process.physDecl = hltHighLevelDev.clone(HLTPaths = ['HLT_PhysicsDeclared'], HLTPathsPrescales = [1])

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
    'keep *_promptanatcmet_*_*',
    'keep *_promptanapfmet_*_*',
    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    #'keep *_promptanasc5calojet_*_*',
    #'keep *_promptanakt4calojet_*_*',
    'keep *_promptanaak5calojet_*_*',
    'keep *_promptanaJPTak5_*_*',
    'keep *_promptanaak5pfjet_*_*',
#    'keep *_promptanahalo_*_*',
    'keep *_promptanacalotowers_*_*',
    'keep *_promptanatrigger_*_*',
    'keep *_promptanavtx_*_*',
    'keep *_promptanatrack_*_*',
    'keep *_promptanacleanup_*_*',
    'keep *_promptanaecalspikes_*_*',
    'keep *_promptanaPMTnoise_*_*'
    ))

process.theBigNtuple = cms.Path(
    process.physDecl *
    process.bit40OR41 *
    #process.BeamHaloId *
    (
    process.promptanaevent +
    process.promptanamet   +
    process.promptanatcmet   +
    process.promptanapfmet   +
    process.promptananohf  +
    process.promptanaic5calojet +
    #process.promptanasc5calojet +
    #process.promptanakt4calojet +
    process.promptanaak5calojet +
    process.promptanaJPTak5 +
    process.promptanaak5pfjet +
    #process.promptanahalo +
    process.promptanacalotowers +
    process.promptanatrigger +
    process.promptanavtx +
    process.promptanatrack +
    process.promptanacleanup +
    process.promptanaecalspikes+
    process.promptanaPMTnoise
    )
    * process.promptanaTree )

process.schedule = cms.Schedule(process.reflagging_step,process.rereco_step,process.theBigNtuple)

