import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim Z --> mu+ mu- candidate events passing "golden" VTBF selection
#--------------------------------------------------------------------------------

process = cms.Process("reskimGoldenZmumu")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V13A::All')

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

# load definition of VBTF Z --> mu+ mu- event selection
process.load("ElectroWeakAnalysis.Utilities.goldenZmmSelectionVBTF_cfi")

# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ##'file:/data1/veelken/CMSSW_3_6_x/skims/pseudoData_Ztautau.root'
        'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_1_1_adj.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_2_1_3uZ.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_3_1_PXh.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_4_1_yHg.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_5_1_I5Q.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_6_1_9m7.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_7_1_y8p.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_8_1_6fE.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_9_1_ejQ.root',
	'rfio:/castor/cern.ch/user/s/smaruyam/ZmmSkim/smaruyam/Mu/CMSSW_3_8_5_ZmmSkimV4/88174dd9b4b05e756497e260fa3e4f76/goldenZmumuEvents_RAW_RECO_10_1_Kst.root'
    )
)

#--------------------------------------------------------------------------------
# select events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimPath = cms.Path(
    process.dataQualityFilters 
   + process.goldenZmmSelectionSequence
)

goldenZmumuEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('goldenZmumuSkimPath')
    )
)

#--------------------------------------------------------------------------------
# save events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    FEVTEventContent,                                               
    goldenZmumuEventSelection,
    fileName = cms.untracked.string('goldenZmumuEvents_RAW_RECO.root')                                                  
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.goldenZmumuSkimOutputModule)

