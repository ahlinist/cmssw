import FWCore.ParameterSet.Config as cms
import copy
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent

def addDiTauSkimm(process):
   #--------------------------------------------------------------------------------
   # select tau-jets 
   #--------------------------------------------------------------------------------

   process.selectedPFTausLeadPionPt8 = cms.EDFilter("PFTauSelector",
                                                    src = cms.InputTag('shrinkingConePFTauProducer'),
                                                    discriminators = cms.VPSet(
       cms.PSet(
       discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
       selectionCut = cms.double(0.5)
       )
       ),
                                                    cut = cms.string("abs(eta) < 2.5 & leadPFCand().isNonnull() & leadPFCand().pt() > 8."),
                                                    filter = cms.bool(True)
                                                    )

   process.selectedPFTausLeadPionPt12 = copy.deepcopy(process.selectedPFTausLeadPionPt8)
   process.selectedPFTausLeadPionPt12.cut = cms.string("abs(eta) < 2.5 & leadPFCand().isNonnull() & leadPFCand().pt() > 12.")

   process.selectedCaloTausLeadTrackPt8 = cms.EDFilter("CaloTauSelector",
                                                       src = cms.InputTag('caloRecoTauProducer'),
                                                       discriminators = cms.VPSet(
       cms.PSet(
       discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
       selectionCut = cms.double(0.5)
       )
       ),
                                                       cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 8."),                                    
                                                       filter = cms.bool(True)
                                                       )
   
   process.selectedCaloTausLeadTrackPt12 = copy.deepcopy(process.selectedCaloTausLeadTrackPt8)
   process.selectedCaloTausLeadTrackPt12.cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 12.")

   #--------------------------------------------------------------------------------
   # combine selected tau-jets into pairs;
   # DO NOT require the two tau-jets to be separated by dR > 0.3 in eta-phi
   #--------------------------------------------------------------------------------

   process.caloTauPairs = cms.EDProducer("DiCandidatePairProducer",
                                         useLeadingTausOnly = cms.bool(False),
                                         srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt8'),
                                         srcLeg2 = cms.InputTag('selectedCaloTausLeadTrackPt12'),
                                         dRmin12 = cms.double(0.),
                                         srcMET = cms.InputTag(''),
                                         recoMode = cms.string(""),
                                         scaleFuncImprovedCollinearApprox = cms.string('1'),
                                         verbosity = cms.untracked.int32(0)                                       
                                         )

   process.selectedCaloTauPairs = cms.EDFilter("DiCandidatePairSelector",
                                               src = cms.InputTag('caloTauPairs'),
                                               cut = cms.string("dR12 > - 0.3"),
                                               filter = cms.bool(True)                                     
                                               )

   process.pfTauPairs = cms.EDProducer("DiCandidatePairProducer",
                                       useLeadingTausOnly = cms.bool(False),
                                       srcLeg1 = cms.InputTag('selectedPFTausLeadPionPt8'),
                                       srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt12'),
                                       dRmin12 = cms.double(0.),
                                       srcMET = cms.InputTag(''),
                                       recoMode = cms.string(""),
                                       scaleFuncImprovedCollinearApprox = cms.string('1'),
                                       verbosity = cms.untracked.int32(0)
                                       )

   process.selectedPFTauPairs = cms.EDFilter("DiCandidatePairSelector",
                                             src = cms.InputTag('pfTauPairs'),
                                             cut = cms.string("dR12 > - 0.3"),
                                             filter = cms.bool(True)                                     
                                             )

   process.mixedTauPairsI = cms.EDProducer("DiCandidatePairProducer",
                                           useLeadingTausOnly = cms.bool(False),
                                           srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt8'),
                                           srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt12'),
                                           dRmin12 = cms.double(0.),
                                           srcMET = cms.InputTag(''),
                                           scaleFuncImprovedCollinearApprox = cms.string('1'),
                                           recoMode = cms.string(""),
                                           verbosity = cms.untracked.int32(0)
                                           )

   process.selectedMixedTauPairsI = cms.EDFilter("DiCandidatePairSelector",
                                                 src = cms.InputTag('mixedTauPairsI'),
                                                 cut = cms.string("dR12 > - 0.3"),
                                                 filter = cms.bool(True)                                     
                                                 )
   
   process.mixedTauPairsII = cms.EDProducer("DiCandidatePairProducer",
                                            useLeadingTausOnly = cms.bool(False),
                                            srcLeg1 = cms.InputTag('selectedCaloTausLeadTrackPt12'),
                                            srcLeg2 = cms.InputTag('selectedPFTausLeadPionPt8'),
                                            dRmin12 = cms.double(0.),
                                            srcMET = cms.InputTag(''),
                                            scaleFuncImprovedCollinearApprox = cms.string('1'),
                                            recoMode = cms.string(""),
                                            verbosity = cms.untracked.int32(0)
                                            )
   
   process.selectedMixedTauPairsII = cms.EDFilter("DiCandidatePairSelector",
                                                  src = cms.InputTag('mixedTauPairsII'),
                                                  cut = cms.string("dR12 > - 0.3"),
                                                  filter = cms.bool(True)                                     
                                                  )

   #--------------------------------------------------------------------------------
   # keep event in case it passed either the caloTau + caloTau, pfTau + pfTau
   # or mixed caloTau + pfTau selection
   #--------------------------------------------------------------------------------

   process.caloTauSkimPath = cms.Path(
       process.selectedCaloTausLeadTrackPt8 * process.selectedCaloTausLeadTrackPt12 
       * process.caloTauPairs
       * process.selectedCaloTauPairs
       )
  
   process.pfTauSkimPath = cms.Path(
       process.selectedPFTausLeadPionPt8 * process.selectedPFTausLeadPionPt12
       * process.pfTauPairs
       * process.selectedPFTauPairs
       )
  
   process.mixedTauSkimPathI = cms.Path(
       (process.selectedCaloTausLeadTrackPt8 + process.selectedPFTausLeadPionPt12)
       * process.mixedTauPairsI
       * process.selectedMixedTauPairsI
       )

   process.mixedTauSkimPathII = cms.Path(
       (process.selectedCaloTausLeadTrackPt12 + process.selectedPFTausLeadPionPt8)
       * process.mixedTauPairsII
       * process.selectedMixedTauPairsII
       )

   diTauEventSelection = cms.untracked.PSet(
       SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring('pfTauSkimPath', 'caloTauSkimPath', 'mixedTauSkimPathI', 'mixedTauSkimPathII')
       )
       )



   #--------------------------------------------------------------------------------
   # use HLT to count events passing OR.
   #--------------------------------------------------------------------------------
   process.diTauFilter = cms.EDFilter("HLTHighLevel",
                                       TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
                                       HLTPaths = diTauEventSelection.SelectEvents.SelectEvents,
                                       eventSetupPathsKey = cms.string(''), 
                                       andOr = cms.bool(True),   
                                       throw = cms.bool(True)
                                       )

   #--------------------------------------------------------------------------------
   # save events passing either the caloTau + caloTau, pfTau + pfTau
   # or mixed caloTau + pfTau selection
   #--------------------------------------------------------------------------------

   process.diTauSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
                                                    tauAnalysisEventContent,
                                                    diTauEventSelection,
                                                    dataset = cms.untracked.PSet(
       filterName = cms.untracked.string('diTauSkim')
       ),                                                 
                                                    fileName = cms.untracked.string('diTauSkim.root')
                                                    )
   

   process.diTauCounter = cms.EndPath(process.diTauSkimOutputModule + process.diTauFilter) 
