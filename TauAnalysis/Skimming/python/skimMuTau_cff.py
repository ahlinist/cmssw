import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent
from TauAnalysis.Skimming.ewkMuTauValHistManager_cfi import *


def addMuTauSkimm(process):
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
    # use HLT to count events passing OR.
    #--------------------------------------------------------------------------------
    process.muTauFilter = cms.EDFilter("HLTHighLevel",
        TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
        HLTPaths = muTauEventSelection.SelectEvents.SelectEvents,
        eventSetupPathsKey = cms.string(''), 
        andOr = cms.bool(True),   
        throw = cms.bool(True),
        filter = cms.bool(False)
    )

    #--------------------------------------------------------------------------------
    # save events passing either the muon + TaNC tau or muon + HPS tau selection
    #--------------------------------------------------------------------------------

    process.muTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
        tauAnalysisEventContent,                                               
        muTauEventSelection,
        dataset = cms.untracked.PSet(
            filterName = cms.untracked.string('muTauSkim')
        ),                                             
        fileName = cms.untracked.string('muTauSkim.root')
    )

    process.muTauCounter = cms.EndPath(process.muTauSkimOutputModule + process.muTauFilter) 
