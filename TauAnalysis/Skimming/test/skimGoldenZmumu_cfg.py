import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim Z --> mu+ mu- candidate events passing "golden" VTBF selection
#--------------------------------------------------------------------------------

process = cms.Process("skimGoldenZmumu")

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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_3_6_x/skims/pseudoData_Ztautau.root'
    )
)

#--------------------------------------------------------------------------------
# select events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimPath = cms.Path(
    process.goldenZmmSelectionSequence
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

