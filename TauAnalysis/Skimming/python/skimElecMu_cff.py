import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent
from TauAnalysis.Skimming.ewkElecMuValHistManager_cfi import *


def addElecMuSkimm(process):
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
        cut = cms.string("dR12 > -0.3"),
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
    # use HLT to count events passing OR.
    #--------------------------------------------------------------------------------
    process.elecMuFilter = cms.EDFilter("HLTHighLevel",
        TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
        HLTPaths = elecMuEventSelection.SelectEvents.SelectEvents,
        eventSetupPathsKey = cms.string(''), 
        andOr = cms.bool(True),   
        throw = cms.bool(True)
    )

    #--------------------------------------------------------------------------------
    # save events passing the electron + muon selection
    #--------------------------------------------------------------------------------

    process.elecMuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
        tauAnalysisEventContent,                                               
        elecMuEventSelection,
        dataset = cms.untracked.PSet(
            filterName = cms.untracked.string('elecMuSkim')
        ),                                              
        fileName = cms.untracked.string('elecMuSkim.root')                                                  
    )

    process.elecMuCounter = cms.EndPath(process.elecMuSkimOutputModule + process.elecMuFilter) 
