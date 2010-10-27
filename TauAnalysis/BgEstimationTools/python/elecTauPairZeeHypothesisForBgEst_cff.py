import FWCore.ParameterSet.Config as cms
import copy

#from SimGeneral.HepPDTESSource.pdt_cfi import *

#--------------------------------------------------------------------------------
# produce data-formats providing information about compatibility of elecron + tau-jet pairs
# with hypothesis of being a pair of electrons resulting from a Z --> e+ e- decay
# (for data-driven background estimation methods)
#--------------------------------------------------------------------------------

elecTauPairZeeHypothesesForBgEstWplusJetsEnriched = cms.EDProducer("ZllHypothesisElecTauProducer",
    diCandidatePairSource = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),

    genLeptonsFromZsSource = cms.InputTag('genElectronsFromZs'),
                                          
    caloJetSource = cms.InputTag('ak5CaloJets'),
    pfJetSource = cms.InputTag('ak5PFJets'),                                       
    trackSource = cms.InputTag('generalTracks'),
    gsfElectronSource = cms.InputTag('pixelMatchGsfElectrons'),
    gsfTrackSource = cms.InputTag('pixelMatchGsfFit'),
                                          
    tkminPixelHits = cms.int32(1),
    tkminTrackerHits = cms.int32(8),	
    tkmaxChi2 = cms.double(100.),

    dRmatch = cms.double(0.5),

    verbosity = cms.untracked.int32(0)                                      
)

produceElecTauPairZeeHypothesesForBgEst = cms.Sequence( elecTauPairZeeHypothesesForBgEstWplusJetsEnriched )
                                                  
