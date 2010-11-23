import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim Z --> mu+ mu- candidate events passing loose selections
# used for measuting muon reconstruction, id., isolation and trigger efficiencies
#--------------------------------------------------------------------------------

process = cms.Process("skimLooseZmumu")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = cms.string('GR_R_38X_V13A::All')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V13::All')

# load definition of loose Z --> mu+ mu- event selection
process.load("TauAnalysis.Skimming.looseZmmSelection_cfi")

# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/friis/Run24/skim_AHtoMuTau_data_Mu_Run2010B_Prompt_Run24.root'
        #'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_1_1_adj.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_2_1_3uZ.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_3_1_PXh.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_4_1_yHg.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_5_1_I5Q.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_6_1_9m7.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_7_1_y8p.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_8_1_6fE.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_9_1_ejQ.root',
	##'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_10_1_Kst.root'
    )
)

#--------------------------------------------------------------------------------
# select events passing loose VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.selZmumuSkimPathForMuonInnerTrackRecoEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonInnerTrackRecoEff
)

process.selZmumuSkimPathForMuonStandAloneTrackRecoEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonStandAloneTrackRecoEff
)

process.selZmumuSkimPathForMuonGlobalTrackRecoEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonGlobalTrackRecoEff
)

process.selZmumuSkimPathForMuonIdEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonIdEff
)

process.selZmumuSkimPathForMuonIsoEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonIsoEff
)

process.selZmumuSkimPathForMuonTriggerEff = cms.Path(
    process.dataQualityFilters
   + process.looseZmumuProductionSequence
   + process.selZmumuFilterForMuonTriggerEff
)

#--------------------------------------------------------------------------------
# select events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimPath = cms.Path(
    process.dataQualityFilters
   + process.goldenZmumuSelectionSequence
)

#--------------------------------------------------------------------------------
# save events passing loose Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.looseZmumuSkimOutputModule = cms.OutputModule("PoolOutputModule",
    #FEVTEventContent,
    FEVTSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring(
            'goldenZmumuSkimPath',
            'selZmumuSkimPathForMuonInnerTrackRecoEff',
            'selZmumuSkimPathForMuonStandAloneTrackRecoEff',
            'selZmumuSkimPathForMuonGlobalTrackRecoEff',
            'selZmumuSkimPathForMuonIdEff',
            'selZmumuSkimPathForMuonIsoEff',
            'selZmumuSkimPathForMuonTriggerEff'
        )
    ),
    fileName = cms.untracked.string('looseZmumuSkim.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.looseZmumuSkimOutputModule)

# print-out all python configuration parameter information
#print process.dumpPython()
