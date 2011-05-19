import FWCore.ParameterSet.Config as cms

process = cms.Process("SUSY")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_3_8_0_pre8/RelValTTbar/GEN-SIM-RECO/START38_V6-v1/0004/847D00B0-608E-DF11-A37D-003048678FA0.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.register('GlobalTag', "MC_42_V10::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')
options.register('doL1FastJet', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off L1FastJet correction")

options.register('hltName', 'HLT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5Calo', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
options.jetTypes.append('AK5PF')
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('doValidation', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Include the validation histograms from SusyDQM (needs extra tags)")
options.register('doExtensiveMatching', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Matching to simtracks (needs extra tags)")
options.register('doSusyTopProjection', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Apply Susy selection in PF2PAT to obtain lepton cleaned jets (needs validation)")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('test', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('type', 'Zinv', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('WJetsSpecEvtFilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "specific option for WJets signal -- currently filter nonIsoMuonMatchToGenMuon or not")

options.register('doSpecialTPBEpaths', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "append special TP||BE filter paths")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

options.parseArguments()
options._tagOrder =[]

print options
 
#-- Message Logger ------------------------------------------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.debug:
   process.MessageLogger.cerr.FwkReport.reportEvery = 1

#-- Input Source --------------------------------------------------------------
if options.files:
   process.source.fileNames = options.files
else:
   process.source.fileNames = [
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/AODSIM/MC_42_V10-v1/0030/507509ED-6D67-E011-B21F-0018F3D096E0.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/AODSIM/MC_42_V10-v1/0026/D8EB529A-D866-E011-A8E0-003048D3FC94.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/AODSIM/MC_42_V10-v1/0026/52BB6A4C-CA66-E011-9FE4-00304867C1BA.root'
        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0030/7C1055F3-6D67-E011-B40D-003048678D6C.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0026/E491EBC2-C966-E011-AB81-001A92971B04.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0026/DAEDD9B6-CC66-E011-B631-003048678BAA.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0026/CCFAFBAE-CC66-E011-96E7-0026189438B3.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0026/1C333D47-CA66-E011-B961-00248C55CC3C.root',
#        '/store/relval/CMSSW_4_2_1/RelValProdQCD_Pt_3000_3500/GEN-SIM-RECO/MC_42_V10-v1/0026/102A8BB8-CA66-E011-B3E6-002354EF3BE4.root'
   ]

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_MEtoEDMConverter_*_*" )
process.maxEvents.input = options.maxEvents

#-- Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
if options.doL1FastJet == True: options.jetCorrections.insert(0, 'L1FastJet')

print "jetCorrections: "
print options.jetCorrections

etValToBeFlagged = 63.75

process.load('JetMETAnalysis.ecalDeadCellTools.RA2TPfilter_cff')
process.ecalDeadCellTPfilter.debug = True
process.ecalDeadCellTPfilter.makeProfileRoot = False
process.ecalDeadCellTPfilter.etValToBeFlagged = etValToBeFlagged
#process.ecalDeadCellTPfilter.tpDigiCollection = cms.InputTag("ecalTPSkim")
process.ecalDeadCellTPfilter.tpDigiCollection = cms.InputTag("dummy")
#process.ecalDeadCellTPfilter.ebReducedRecHitCollection = cms.InputTag("ecalRecHit", "EcalRecHitsEB")
#process.ecalDeadCellTPfilter.eeReducedRecHitCollection = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
#process.ecalDeadCellTPfilter.ebReducedRecHitCollection = cms.InputTag("dummy")
#process.ecalDeadCellTPfilter.eeReducedRecHitCollection = cms.InputTag("dummy")

process.ecalDeadfilters_seq = cms.Sequence(process.ecalDeadCellTPfilter)

process.comb_seq = cms.Sequence(
# dead filters
   process.ecalDeadfilters_seq
)

process.analysis_path = cms.Path(process.comb_seq)

process.schedule = cms.Schedule( process.analysis_path )
