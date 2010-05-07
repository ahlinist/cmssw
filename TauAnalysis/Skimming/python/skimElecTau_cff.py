import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent
from TauAnalysis.Skimming.ewkElecTauValHistManager_cfi import *


def addElecTauSkimm(process):

  #--------------------------------------------------------------------------------
  # select electrons and tau-jets
  #--------------------------------------------------------------------------------

  process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
                                           src = cms.InputTag("gsfElectrons"),
                                           cut = cms.string("abs(eta) < 2.5 & pt > 8."),
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
  # combine selected electrons and tau-jets into pairs;
  # DO NOT require electrons and tau-jets to be separated by dR > 0.3 in eta-phi,
  # in order to reject events in which the tau-jet refers to the same physical
  # particle as the electron (note that almost all electrons get selected as tau-jets !!)
  #--------------------------------------------------------------------------------

  process.elecCaloTauPairs = cms.EDProducer("DiCandidatePairProducer",
                                            useLeadingTausOnly = cms.bool(False),
                                            srcLeg1 = cms.InputTag('selectedCaloTaus'),
                                            srcLeg2 = cms.InputTag('selectedElectrons'),
                                            dRmin12 = cms.double(0.),
                                            srcMET = cms.InputTag(''),
                                            recoMode = cms.string(""),
                                            scaleFuncImprovedCollinearApprox = cms.string('1'),
                                            verbosity = cms.untracked.int32(0)                                       
                                            )

  process.selectedElecCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
                                                  src = cms.InputTag('elecCaloTauPairs'),
                                                  cut = cms.string("dR12 > - 0.3"),
                                                  filter = cms.bool(True)                                     
                                                  )

  process.elecPFTauPairs = cms.EDProducer("DiCandidatePairProducer",
                                          useLeadingTausOnly = cms.bool(False),
                                          srcLeg1 = cms.InputTag('selectedPFTaus'),
                                          srcLeg2 = cms.InputTag('selectedElectrons'),
                                          dRmin12 = cms.double(0.),
                                          srcMET = cms.InputTag(''),
                                          recoMode = cms.string(""),
                                          scaleFuncImprovedCollinearApprox = cms.string('1'),
                                          verbosity = cms.untracked.int32(0)
                                          )
  
  process.selectedElecPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
                                                src = cms.InputTag('elecPFTauPairs'),
                                                cut = cms.string("dR12 > - 0.3"),
                                                filter = cms.bool(True)                                     
                                                )

  #--------------------------------------------------------------------------------
  # keep event in case it passed either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

  process.elecPFTauSkimPath = cms.Path(
    (process.selectedPFTaus + process.selectedElectrons)
    * process.elecPFTauPairs
    * process.selectedElecPFTauPairs
    )
  
  process.elecCaloTauSkimPath = cms.Path(
      (process.selectedCaloTaus + process.selectedElectrons)
      * process.elecCaloTauPairs
      * process.selectedElecCaloTauPairs
      )
  
  elecTauEventSelection = cms.untracked.PSet(
      SelectEvents = cms.untracked.PSet(
      SelectEvents = cms.vstring('elecPFTauSkimPath','elecCaloTauSkimPath')
      )
      )

  #--------------------------------------------------------------------------------
  # use HLT to count events passing OR.
  #--------------------------------------------------------------------------------
  process.elecTauFilter = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
                                       HLTPaths = elecTauEventSelection.SelectEvents.SelectEvents,
                                       eventSetupPathsKey = cms.string(''), 
                                       andOr = cms.bool(True),   
                                       throw = cms.bool(True)
                                       )


  #--------------------------------------------------------------------------------
  # fill validation histograms for events passing either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

  process.DQMStore = cms.Service("DQMStore")

  process.fillElecTauValPlots = cms.EDAnalyzer("EwkTauValidation",
                                               
                                               # list of individual channels                           
                                               channels = cms.VPSet(
      ewkElecTauValHistManager
      ),
                                               
                                               # disable all warnings
                                               maxNumWarnings = cms.int32(1)                      
                                               )
  
  process.saveElecTauValPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
                                               outputFileName = cms.string('elecTauValPlots.root')
                                               )
  
  #--------------------------------------------------------------------------------
  # save events passing either the electron + pfTau or electron + caloTau selection
  #--------------------------------------------------------------------------------

  process.elecTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
                                                     tauAnalysisEventContent,
                                                     elecTauEventSelection,
                                                     dataset = cms.untracked.PSet(
      filterName = cms.untracked.string('elecTauSkim')
      ),
                                                     fileName = cms.untracked.string('skimElecTau.root')
                                                     )


  process.elecTauCounter = cms.EndPath(process.elecTauSkimOutputModule + process.elecTauFilter) 
