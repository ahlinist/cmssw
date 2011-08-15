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
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V12::All')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/skimGenZtoMuTauWithinAcc_Ztautau_2011Jun30v2_AOD.root'
    )
)

##isMC = True
isMC = False

# import event content definition:
# keep full FEVT (RAW + RECO) event content
# plus collections of goodMuons, goldenZmumuCandidates and "tag" & "probe" flags
from TauAnalysis.Skimming.goldenZmmEventContent_cff import *

# load definition of VBTF Z --> mu+ mu- event selection
# (with no isolation cuts applied on one of the two muons)
process.load("TauAnalysis.Skimming.goldenZmmSelectionVBTFnoMuonIsolation_cfi")

# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")
if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# select events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimPath = cms.Path(
    process.dataQualityFilters 
   + process.goldenZmumuSelectionSequence
)

#--------------------------------------------------------------------------------
# save events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.goldenZmumuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    goldenZmumuEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('goldenZmumuSkimPath')
    ),
    fileName = cms.untracked.string('goldenZmumuEvents_runs165071to167913_RECO.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.goldenZmumuSkimOutputModule)

# print-out all python configuration parameter information
#print process.dumpPython()
