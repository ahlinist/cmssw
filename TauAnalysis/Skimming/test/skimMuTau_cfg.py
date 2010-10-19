import FWCore.ParameterSet.Config as cms

process = cms.Process("muTauSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START36_V7::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root',
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/BA1ED3DD-375D-DF11-8298-001A92810AE6.root',
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/AACDC475-375D-DF11-8F4D-0018F3D09612.root',
        '/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/48A2071B-3C5D-DF11-B231-002618FDA25B.root'
    )
)

#--------------------------------------------------------------------------------
# select muons and tau-jets
#--------------------------------------------------------------------------------

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > 8 & abs(eta) < 2.5"),
    filter = cms.bool(True)
)

process.selectedPFTaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('shrinkingConePFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    filter = cms.bool(True)
)

process.selectedCaloTaus = cms.EDFilter("CaloTauSelector",
    src = cms.InputTag('caloRecoTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
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

process.muCaloTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedCaloTaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                    
    verbosity = cms.untracked.int32(0)                                       
)

process.selectedMuCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muCaloTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

process.muPFTauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPFTaus'),
    srcLeg2 = cms.InputTag('selectedMuons'),
    dRmin12 = cms.double(0.),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                  
    verbosity = cms.untracked.int32(0)
)

process.selectedMuPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('muPFTauPairs'),
    cut = cms.string("dR12 > 0.3"),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the muon + pfTau or muon + caloTau selection
#--------------------------------------------------------------------------------

process.muPFTauSkimPath = cms.Path(
    (process.selectedPFTaus + process.selectedMuons)
   * process.muPFTauPairs
   * process.selectedMuPFTauPairs
)

process.muCaloTauSkimPath = cms.Path(
    (process.selectedCaloTaus + process.selectedMuons)
   * process.muCaloTauPairs
   * process.selectedMuCaloTauPairs
)

muTauEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('muPFTauSkimPath', 'muCaloTauSkimPath')
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

#--------------------------------------------------------------------------------
# fill validation histograms for events passing either the muon + pfTau or muon + caloTau selection
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

from TauAnalysis.Skimming.ewkMuTauValHistManager_cfi import *

process.fillMuTauValPlots = cms.EDAnalyzer("EwkTauValidation",

    # list of individual channels                           
    channels = cms.VPSet(
        ewkMuTauValHistManager
    ),

    # disable all warnings
    maxNumWarnings = cms.int32(1)                    
)

process.saveMuTauValPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('muTauValPlots.root')
)

process.p = cms.Path(
    process.fillMuTauValPlots
   + process.saveMuTauValPlots
   + process.dummyFilter
)

#--------------------------------------------------------------------------------
# save events passing either the muon + pfTau or muon + caloTau selection
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

