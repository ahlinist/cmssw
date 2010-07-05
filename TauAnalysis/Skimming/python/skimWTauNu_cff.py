import FWCore.ParameterSet.Config as cms
from TauAnalysis.Skimming.EventContent_cff import tauAnalysisEventContent


def addWTauNuSkimm(process):
    #--------------------------------------------------------------------------------
    # select met and tau-jets
    #--------------------------------------------------------------------------------
    process.selectedPFMet = cms.EDFilter("PFMEtSelector",
                                         src = cms.InputTag('pfMet'),
                                         cut = cms.string("pt > 15"),
                                         filter = cms.bool(True)
                                         )

    process.selectedCaloMet = cms.EDFilter("CaloMEtSelector",
                                           src = cms.InputTag('met'),
                                           cut = cms.string("pt > 15"),
                                           filter = cms.bool(True)
                                           )

    process.selectedPFTaus = cms.EDFilter("PFTauSelector",
                                          src = cms.InputTag('shrinkingConePFTauProducer'),
                                          discriminators = cms.VPSet(
        cms.PSet(
        discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackPtCut"),
        selectionCut = cms.double(0.5)
        )
        ),
                                          cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 15."),
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
                                            cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 15."),
                                            filter = cms.bool(True)
                                            )

    #--------------------------------------------------------------------------------
    # keep event in case it passed either the MET + pfTau or MET + caloTau selection
    #--------------------------------------------------------------------------------

    process.PFTauNuSkimPath = cms.Path(
        (process.selectedPFTaus + process.selectedPFMet)
        )
    
    process.CaloTauNuSkimPath = cms.Path(
        (process.selectedCaloTaus + process.selectedCaloMet)
        )

    wTauNuEventSelection = cms.untracked.PSet(
        SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('PFTauNuSkimPath', 'CaloTauNuSkimPath')
        )
        )

    #--------------------------------------------------------------------------------
    # use HLT to count events passing OR.
    #--------------------------------------------------------------------------------
    process.wTauNuFilter = cms.EDFilter("HLTHighLevel",
                                        TriggerResultsTag = cms.InputTag('TriggerResults','',process.name_()),
                                        HLTPaths = wTauNuEventSelection.SelectEvents.SelectEvents,
                                        eventSetupPathsKey = cms.string(''), 
                                        andOr = cms.bool(True),   
                                        throw = cms.bool(True),
                                        filter = cms.bool(False)
                                        )

    process.wTauNuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
                                                      tauAnalysisEventContent,                                               
                                                      wTauNuEventSelection,
                                                      dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('wTauNuSkim')
        ),
                                                      fileName = cms.untracked.string('wTauNuSkim.root')
                                                      )

    process.wTauNuCounter = cms.EndPath(process.wTauNuSkimOutputModule + process.wTauNuFilter) 
    process.o = cms.EndPath( process.wTauNuSkimOutputModule )

