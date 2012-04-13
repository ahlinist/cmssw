import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim Z --> mu+ mu- candidate events passing "golden" VTBF selection
#--------------------------------------------------------------------------------

process = cms.Process("skimGoldenZmumu2")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

#--------------------------------------------------------------------------------
# define configuration parameter default values

isMC = True # use for MC
##isMC = False # use for Data
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
if isMC:
    process.GlobalTag.globaltag = cms.string('START52_V9::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_52_V7::All')
#--------------------------------------------------------------------------------    

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/ZplusJets/simZplusJets_AOD_1_1_ZkM.root'
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/data/data2012runA_doubleMu_AOD_1_1_Fzg.root'                        
    )
)

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

# add event counter for Mauro's "self baby-sitting" technology
process.processedEventsSkimming = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.processedEventsSkimming)

#--------------------------------------------------------------------------------
# save events passing "golden" VTBF Z --> mu+ mu- selection
#--------------------------------------------------------------------------------

process.skimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    goldenZmumuEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('goldenZmumuSkimPath')
    ),
    fileName = cms.untracked.string('goldenZmumuEvents_AOD.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.skimOutputModule)

processDumpFile = open('skimGoldenZmumu.dump' , 'w')
print >> processDumpFile, process.dumpPython()

