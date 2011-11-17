import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim W --> e nu candidate events passing VTBF selection
#--------------------------------------------------------------------------------

process = cms.Process("skimTightWenu")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

isMC = True
##isMC = False

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
# (only relevant for HPS tau reconstruction algorithm)
if isMC:
    process.GlobalTag.globaltag = cms.string('START42_V13::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All')
#--------------------------------------------------------------------------------    

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data2/veelken/CMSSW_4_2_x/skims/TauTriggerStudy/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/tauIdEffSample_Ztautau_powheg_2011Jul23_RECO_9_1_n5R.root'
    )
)

# import event content definition:
# keep full FEVT (RAW + RECO) event content
# plus collections of selectedElectrons and WenuCandidates
from TauAnalysis.Skimming.tightWenuEventContent_cff import *

# load definition of VBTF W --> e nu event selection
from TauAnalysis.Skimming.tightWenuSelectionVBTFrelPFIsolation_cfi import filterTightWenuCandidates
filterTightWenuCandidates(process, isMC)

# load definitions of data-quality filters
process.load("TauAnalysis.TauIdEfficiency.filterDataQuality_cfi")
if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# select events passing VTBF W --> e nu selection
#--------------------------------------------------------------------------------

process.tightWenuSkimPath = cms.Path(
    process.dataQualityFilters 
   + process.tightWenuSelectionSequence
)

# add event counter for Mauro's "self baby-sitting" technology
process.processedEventsSkimming = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.processedEventsSkimming)

#--------------------------------------------------------------------------------
# save events passing VTBF W --> e nu selection
#--------------------------------------------------------------------------------

process.tightWenuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tightWenuEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tightWenuSkimPath')
    ),
    fileName = cms.untracked.string('tightWenuEvents_AOD.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.tightWenuSkimOutputModule)

processDumpFile = open('skimTightWenu.dump' , 'w')
print >> processDumpFile, process.dumpPython()
