import FWCore.ParameterSet.Config as cms

#from SimGeneral.HepPDTESSource.pdt_cfi import *

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about compatibility of an elecron + tau-jet pair
# with the hypothesis of being a pair of electron,
# resulting from a Z --> e+ e- decay
#--------------------------------------------------------------------------------

elecTauPairZeeHypotheses = cms.EDProducer("PATElecTauPairZeeHypothesisProducer",
    elecTauPairSource = cms.InputTag('selectedElecTauPairsPzetaDiffCumulative'),

    genElectronsFromZsSource = cms.InputTag('genElectronsFromZs'),
                                          
    caloJetSource = cms.InputTag('iterativeCone5CaloJets'),
    pfJetSource = cms.InputTag('iterativeCone5PFJets'),                                       
    trackSource = cms.InputTag('generalTracks'),
    gsfElectronSource = cms.InputTag('pixelMatchGsfElectrons'),
    gsfTrackSource = cms.InputTag('pixelMatchGsfFit'),
                                          
    tkminPixelHits = cms.int32(1),
    tkminTrackerHits = cms.int32(8),	
    tkmaxChi2 = cms.double(100.),

    dRmatch = cms.double(0.5),

    verbosity = cms.untracked.int32(0)                                      
)

selectedElecTauPairZeeHypotheses = cms.EDFilter("PATElecTauPairZeeHypothesisSelector",
     src = cms.InputTag('elecTauPairZeeHypotheses'),
     cut = cms.string('p4Z0bestMatch.mass > 85. & p4Z0bestMatch.mass < 100.'),
     filter = cms.bool(False)
)

elecTauPairZeeHypothesesLooseElectronIsolation = cms.EDProducer("PATElecTauPairZeeHypothesisProducer",
    elecTauPairSource = cms.InputTag('selectedElecTauPairsPzetaDiffLooseElectronIsolationCumulative'),

    genElectronsFromZsSource = cms.InputTag('genElectronsFromZs'),
                                          
    caloJetSource = cms.InputTag('iterativeCone5CaloJets'),
    pfJetSource = cms.InputTag('iterativeCone5PFJets'),                                       
    trackSource = cms.InputTag('generalTracks'),
    gsfElectronSource = cms.InputTag('pixelMatchGsfElectrons'),
    gsfTrackSource = cms.InputTag('pixelMatchGsfFit'),
                                          
    tkminPixelHits = cms.int32(1),
    tkminTrackerHits = cms.int32(8),	
    tkmaxChi2 = cms.double(100.),

    dRmatch = cms.double(0.5),

    verbosity = cms.untracked.int32(0)                                      
)

selectedElecTauPairZeeHypothesesLooseElectronIsolation = cms.EDFilter("PATElecTauPairZeeHypothesisSelector",
     src = cms.InputTag('elecTauPairZeeHypothesesLooseElectronIsolation'),
     cut = cms.string('p4Z0bestMatch.mass > 85. & p4Z0bestMatch.mass < 100.'),
     filter = cms.bool(False)
)

produceElecTauPairZeeHypotheses = cms.Sequence( elecTauPairZeeHypotheses
                                               * selectedElecTauPairZeeHypotheses
                                               * elecTauPairZeeHypothesesLooseElectronIsolation
                                               * selectedElecTauPairZeeHypothesesLooseElectronIsolation )
                                                  
