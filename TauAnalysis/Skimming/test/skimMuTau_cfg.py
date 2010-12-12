import FWCore.ParameterSet.Config as cms

process = cms.Process("muTauSkim")

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
# rerun tau reconstruction algorithm
#--------------------------------------------------------------------------------

process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

#--------------------------------------------------------------------------------
# select muons and tau-jets
#--------------------------------------------------------------------------------

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > 13.5 & abs(eta) < 2.3 & isGlobalMuon"),
    filter = cms.bool(True)
)

process.selectedTaNCtaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('hpsTancTaus'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("hpsTancTausDiscriminationByVLooseIsolation"),
            selectionCut = cms.double(0.5)
        )
    ),
    filter = cms.bool(True)
)

process.selectedHPStaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('hpsTancTaus'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("hpsTancTausDiscriminationByTancVLoose"),
            selectionCut = cms.double(0.5)
        )
    ),
    filter = cms.bool(True)
)

#--------------------------------------------------------------------------------
# combine selected muons and tau-jets into pairs;
# require muons and tau-jets to be separated by dR > 0.3 in eta-phi,
# in order to reject events in which the tau-jet refers to the same physical
# particle as the muon (note that almost all muons get selected as tau-jets !!)
#--------------------------------------------------------------------------------

process.muTaNCtauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedTaNCtaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)                                       
)

process.selectedMuTaNCtauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muTaNCtauPairs'),
    cut = cms.string("dR12 > 0."),
    filter = cms.bool(True)                                     
)

process.muHPStauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedHPStaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedMuHPStauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muHPStauPairs'),
    cut = cms.string("dR12 > 0."),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the muon + pfTau or muon + caloTau selection
#--------------------------------------------------------------------------------

process.muTaNCtauSkimPath = cms.Path(
    process.PFTau
   * (process.selectedTaNCtaus + process.selectedMuons)
   * process.muTaNCtauPairs
   * process.selectedMuTaNCtauPairs
)

process.muHPStauSkimPath = cms.Path(
    process.PFTau
   * (process.selectedHPStaus + process.selectedMuons)
   * process.muHPStauPairs
   * process.selectedMuHPStauPairs
)

muTauEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('muTaNCtauSkimPath', 'muHPStauSkimPath')
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
# save events passing either the muon + TaNC tau or muon + HPS tau selection
#--------------------------------------------------------------------------------

process.muTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    muTauEventSelection,
    fileName = cms.untracked.string('muTauSkim.root')
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.muTauSkimOutputModule)

