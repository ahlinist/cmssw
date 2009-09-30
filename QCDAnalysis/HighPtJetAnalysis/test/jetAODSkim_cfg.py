import FWCore.ParameterSet.Config as cms
process = cms.Process("SKIM")
process.load('FWCore.MessageService.MessageLogger_cfi')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Summer09/QCD_BCtoE_Pt30to80/GEN-SIM-RECO/MC_31X_V3-v1/0021/8A504EC5-FC84-DE11-9DF8-00151796D680.root',
        '/store/mc/Summer09/QCD_BCtoE_Pt30to80/GEN-SIM-RECO/MC_31X_V3-v1/0020/76BC9A89-F384-DE11-BB94-0015179EDC2C.root',
        '/store/mc/Summer09/QCD_BCtoE_Pt30to80/GEN-SIM-RECO/MC_31X_V3-v1/0015/E49D4ED3-C781-DE11-B39F-001D096B0C83.root',
        '/store/mc/Summer09/QCD_BCtoE_Pt30to80/GEN-SIM-RECO/MC_31X_V3-v1/0015/B48A2C0B-FD83-DE11-BB2F-001D0967D567.root',
)
)
#############   Trigger selection  ###################
process.hltBit = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag  = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths           = cms.vstring("HLT_L1Jet15"),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(True)
)
#############   Path       ###########################
process.skimPath = cms.Path(process.hltBit)

#############   output module ########################
process.compress = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *',
        #------- CaloJet collections ------ 
        'keep *_sisCone5CaloJets_*_*',
        'keep *_sisCone7CaloJets_*_*',
        'keep *_kt4CaloJets_*_*',
        'keep *_kt6CaloJets_*_*',
        'keep *_antikt5CaloJets_*_*',
        'keep *_iterativeCone5CaloJets_*_*',  
        #------- PFJet collections --------
        'keep *_sisCone5PFJets_*_*',
        'keep *_sisCone7PFJets_*_*',
        'keep *_kt4PFJets_*_*',
        'keep *_kt6PFJets_*_*',
        'keep *_antikt5PFJets_*_*',
        'keep *_iterativeCone5PFJets_*_*',
        #------- Trigger collections ------
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        #------- Tracks collection --------
        'keep *_generalTracks_*_*',
        #------- CaloTower collection -----
        'keep *_towerMaker_*_*',
        #------- Various collections ------
        'keep *_EventAuxilary_*_*',
        'keep *_pixelVertices_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_hcalnoise_*_*', 
        #------- MET collections ----------
        'keep *_metHO_*_*',
        'keep *_metNoHF_*_*',
        'keep *_metNoHFHO_*_*', 
        'keep *_met_*_*'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('skimPath')), 
    fileName = cms.untracked.string('JetAOD2.root')
)

process.p = cms.EndPath(process.compress)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10
