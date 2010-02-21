import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt


maxevents=100
isMC=False
#isMC=True


process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("L1Trigger/Configuration/L1RawToDigi_cff")
process.load("RecoMET/Configuration/RecoMET_BeamHaloId_cff")
process.load("RecoMET/METProducers/BeamHaloSummary_cfi")
process.load("RecoMET/METProducers/CSCHaloData_cfi")
process.load("RecoMET/METProducers/EcalHaloData_cfi")
process.load("RecoMET/METProducers/HcalHaloData_cfi")
process.load("RecoMET/METProducers/GlobalHaloData_cfi")


#MC (Summer09-V16D_900GeV-v1)
if (isMC):
    #process.GlobalTag.globaltag ='STARTUP31X_V7::All'
    process.GlobalTag.globaltag ='START3X_V16D::All'


#DATA (Feb9ReReco)
else:
    #process.GlobalTag.globaltag ='GR09_R_V4::All'
    process.GlobalTag.globaltag ='GR09_R_34X_V5::All'



process.load("Configuration/StandardSequences/ReconstructionCosmics_cff")

process.load("RecoMuon/Configuration/RecoMuon_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'your_output_PMTNoise.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(maxevents) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
    "/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec14thSkim_v1/0102/BABAF8C3-71EA-DE11-9D8C-0024E8768446.root"
    #'/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8D_900GeV-v1/0005/E4590360-4CD7-DE11-8CB4-002618943896.root'
    #'/store/data/BeamCommissioning09/MinimumBias/RECO/rereco_GR09_P_V7_v1/0099/DABD5D6D-D4E2-DE11-8FFD-00261894387A.root'
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/596/F82DED93-36E2-DE11-9316-000423D9870C.root"
    #"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/123/151/0E45A7CE-F5DD-DE11-9B2E-001617E30CC8.root"
    ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# jet corrections
process.load("JetMETCorrections.Configuration.L2L3Corrections_900GeV_cff")
#process.load("JetMETCorrections.Configuration.L2L3Corrections_2360GeV_cff")

# summary
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
#process.bit40OR41 = hltLevel1GTSeed.clone(L1TechTriggerSeeding = cms.bool(True), L1SeedsLogicalExpression = cms.string('40 OR 41'))

#from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
#process.physDecl = hltHighLevelDev.clone(HLTPaths = ['HLT_PhysicsDeclared'], HLTPathsPrescales = [1])

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    #'keep *_promptanamet_*_*',
    #'keep *_promptanatcmet_*_*',
    #'keep *_promptanapfmet_*_*',
    #'keep *_promptananohf_*_*',
    #'keep *_promptanaic5calojet_*_*',
    #'keep *_promptanasc5calojet_*_*',
    #'keep *_promptanakt4calojet_*_*',
    #'keep *_promptanaak5calojet_*_*',
    #'keep *_promptanaJPTak5_*_*',
    #'keep *_promptanaak5pfjet_*_*',
    #'keep *_promptanahalo_*_*',
    #'keep *_promptanacalotowers_*_*',
    #'keep *_promptanatrigger_*_*',
    #'keep *_promptanavtx_*_*',
    #'keep *_promptanatrack_*_*',
    #'keep *_promptanacleanup_*_*',
    'keep *_promptanaPMTnoise_*_*'
    ))

process.promptanaPMTnoise.isMC=isMC
process.promptanaPMTnoise.debug=0

process.theBigNtuple=cms.Path(#process.physDecl
                              process.promptanaevent
                              *process.promptanaPMTnoise
                              *process.promptanaTree)

