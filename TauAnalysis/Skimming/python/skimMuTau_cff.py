import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent
from TauAnalysis.Skimming.ewkMuTauValHistManager_cfi import *


def addMuTauSkimm(process):
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
    # DO NOT require muons and tau-jets to be separated by dR > 0.3 in eta-phi,
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
                                                  cut = cms.string("dR12 > - 0.3"),
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
                                                cut = cms.string("dR12 > - 0.3"),
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
    # fill validation histograms for events passing either the muon + pfTau or muon + caloTau selection
    #--------------------------------------------------------------------------------

    process.DQMStore = cms.Service("DQMStore")

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
    
      
    #--------------------------------------------------------------------------------
    # save events passing either the muon + pfTau or muon + caloTau selection
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
