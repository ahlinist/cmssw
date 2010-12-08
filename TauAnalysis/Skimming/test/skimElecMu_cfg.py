import FWCore.ParameterSet.Config as cms

process = cms.Process("elecMuSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V13::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'   
    )
)

#--------------------------------------------------------------------------------
# select electrons and muons
#--------------------------------------------------------------------------------

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("pt > 8 & abs(eta) < 2.5"),
    filter = cms.bool(True)
)

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > 8 & abs(eta) < 2.5 & isGlobalMuon"),
    filter = cms.bool(True)
)

process.elecMuPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedElectrons'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                 
    verbosity = cms.untracked.int32(0)
)

process.selectedElecMuPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('elecMuPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed the muon + electron selection
#--------------------------------------------------------------------------------

process.elecMuSkimPath = cms.Path(
    (process.selectedElectrons + process.selectedMuons)
   * process.elecMuPairs
   * process.selectedElecMuPairs
)

elecMuEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('elecMuSkimPath')
    )
)

#--------------------------------------------------------------------------------
#Dummy filter (always false) necessary to get correct efficiency report.
#--------------------------------------------------------------------------------

process.dummyFilter = cms.EDFilter("HLTHighLevel",
     TriggerResultsTag = cms.InputTag("TriggerResults","",process.name_()),
     HLTPaths = cms.vstring(''),# provide list of HLT paths (or patterns) you want
     eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
     andOr = cms.bool(True),   # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
     throw = cms.bool(True)    # throw exception on unknown path names
)

process.p = cms.Path(process.dummyFilter)

#--------------------------------------------------------------------------------
# save events passing the electron + muon selection
#--------------------------------------------------------------------------------

process.elecMuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    elecMuEventSelection,
    fileName = cms.untracked.string('elecMuSkim.root')                                                  
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.elecMuSkimOutputModule)

