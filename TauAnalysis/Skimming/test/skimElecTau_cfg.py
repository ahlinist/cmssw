import FWCore.ParameterSet.Config as cms

process = cms.Process("elecTauSkim")

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
		'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_5/RelValZEE/GEN-SIM-RECO/MC_38Y_V12-v1/0040/406EA972-22D2-DF11-9052-002618943918.root'
    )
)

#--------------------------------------------------------------------------------
# rerun tau reconstruction algorithm
#--------------------------------------------------------------------------------

process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

#--------------------------------------------------------------------------------
# select electrons and tau-jets
#--------------------------------------------------------------------------------

#  select electrons with 
#    - abs(eta) < 2.5
#    - pt > 12
#    - WP95 ID (no conversion rejection or isolation)
#
process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("abs(eta) < 2.5 & pt > 12. & ((abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.8 & hcalOverEcal < 0.15 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.01 & abs(deltaPhiSuperClusterTrackAtVtx) <0.7 & hcalOverEcal < 0.07 & sigmaIetaIeta < 0.03))"),
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
# combine selected electrons and tau-jets into pairs;
# require electrons and tau-jets to be separated by dR > 0.3 in eta-phi,
# in order to reject events in which the tau-jet refers to the same physical
# particle as the electron (note that almost all electrons get selected as tau-jets !!)
#--------------------------------------------------------------------------------

process.elecTaNCtauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedTaNCtaus'),
    srcLeg2 = cms.InputTag('selectedElectrons'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)                                       
)

process.selectedElecTaNCtauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('elecTaNCtauPairs'),
    cut = cms.string("dR12 > 0."),
    filter = cms.bool(True)                                     
)

process.elecHPStauPairs = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedHPStaus'),
    srcLeg2 = cms.InputTag('selectedElectrons'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

process.selectedElecHPStauPairs = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('elecHPStauPairs'),
    cut = cms.string("dR12 > 0."),
    filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the electron + TaNC tau or electron + hpsTau selection
#--------------------------------------------------------------------------------

process.elecTaNCtauSkimPath = cms.Path(
    process.PFTau
   * (process.selectedTaNCtaus + process.selectedElectrons)
   * process.elecTaNCtauPairs
   * process.selectedElecTaNCtauPairs
)

process.elecHPStauSkimPath = cms.Path(
    process.PFTau
   * (process.selectedHPStaus + process.selectedElectrons)
   * process.elecHPStauPairs
   * process.selectedElecHPStauPairs
)

elecTauEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('elecTaNCtauSkimPath', 'elecHPStauSkimPath')
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
# save events passing either the electron + TaNC tau or electron + HPS tau selection
#--------------------------------------------------------------------------------

process.elecTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    elecTauEventSelection,
    fileName = cms.untracked.string('elecTauSkim.root')
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.elecTauSkimOutputModule)



