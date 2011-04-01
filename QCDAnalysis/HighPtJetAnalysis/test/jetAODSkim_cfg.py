import FWCore.ParameterSet.Config as cms
process = cms.Process("SKIM")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011A/Jet/AOD/PromptReco-v1/000/161/311/AE2177F9-DF57-E011-BD15-001D09F26509.root',
    )
)
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
#############   output module ########################
process.slim = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *',
        #------- CaloJet collections ------ 
        'keep *_ak5CaloJets_*_*',
        'keep *_ak5JetID_*_*',
        'keep *_ak5JetExtender_*_*',
        'keep *_ak7CaloJets_*_*',
        'keep *_ak7JetID_*_*',
        'keep *_ak7JetExtender_*_*',
        #------- PFJet collections --------
        'keep *_kt6PFJets_rho_SKIM',
        'keep *_kt6PFJets_sigma_SKIM',
        'keep *_ak5PFJets_*_SKIM',  
        'keep *_ak7PFJets_*_SKIM',
        #------- Trigger collections ------
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        #------- Various collections ------
        'keep *_EventAuxilary_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_offlinePrimaryVerticesWithBS_*_*',
        #------- MET collections ----------
        'keep *_met_*_*',
        'keep *_pfMet_*_*'),
    fileName = cms.untracked.string('JetAOD.root')
)
############# turn-on the fastjet area calculation needed for the L1Fastjet ##############
############# applied only to PFJets because if CaloJets are re-recoed the JetID map will be lost #####
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.doAreaFastjet = True
process.ak7PFJets.Rho_EtaMax = cms.double(5.0)
############# slimming the PFJet collection by raising the pt cut #################
process.ak5PFJets.jetPtMin = cms.double(15.0)
process.ak7PFJets.jetPtMin = cms.double(15.0)

process.p = cms.EndPath(
############# first run the kt6PFJets reconstruction to calculate the fastjet density rho #####
process.kt6PFJets * 
############# then reconstruct the PFJets with jet area calculation ############
process.ak5PFJets *
process.ak7PFJets * 
############# keep only the collections of interest ###############################
process.slim)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
