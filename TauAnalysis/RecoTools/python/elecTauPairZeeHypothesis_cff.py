import FWCore.ParameterSet.Config as cms
import copy

#from SimGeneral.HepPDTESSource.pdt_cfi import *

#--------------------------------------------------------------------------------
# produce data-formats providing information about compatibility of elecron + tau-jet pairs
# with hypothesis of being a pair of electrons resulting from a Z --> e+ e- decay
#--------------------------------------------------------------------------------

elecTauPairZeeHypotheses = cms.EDProducer("ZllHypothesisElecTauProducer",
    diCandidatePairSource = cms.InputTag('selectedElecTauPairsPzetaDiffCumulative'),

    genLeptonsFromZsSource = cms.InputTag('genElectronsFromZs'),
                                          
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

selectedElecTauPairZeeHypotheses = cms.EDFilter("ZllHypothesisElecTauSelector",
     src = cms.InputTag('elecTauPairZeeHypotheses'),
     cut = cms.string('p4ZbestMatch.mass > 85. & p4ZbestMatch.mass < 100.'),
     filter = cms.bool(False)
)

elecTauPairVisMassHypotheses = cms.EDProducer("ZtautauVisMassHypothesisElecTauProducer",
    diCandidatePairSource = elecTauPairZeeHypotheses.diCandidatePairSource,

    ZllHypotheses = cms.VPSet(
        cms.PSet(
            src = cms.InputTag('selectedElecTauPairZeeHypotheses'),
            minZllMass = cms.double(85.),
            maxZllMass = cms.double(100.)
        )
    )
)

elecTauPairZeeHypothesesLooseElectronIsolation = copy.deepcopy(elecTauPairZeeHypotheses)
elecTauPairZeeHypothesesLooseElectronIsolation.diCandidatePairSource = cms.InputTag('selectedElecTauPairsPzetaDiffLooseElectronIsolationCumulative')

selectedElecTauPairZeeHypothesesLooseElectronIsolation = copy.deepcopy(selectedElecTauPairZeeHypotheses)
selectedElecTauPairZeeHypothesesLooseElectronIsolation.src = cms.InputTag('elecTauPairZeeHypothesesLooseElectronIsolation')

elecTauPairVisMassHypothesesLooseElectronIsolation = copy.deepcopy(elecTauPairVisMassHypotheses)
elecTauPairVisMassHypothesesLooseElectronIsolation.diCandidatePairSource = elecTauPairZeeHypothesesLooseElectronIsolation.diCandidatePairSource
elecTauPairVisMassHypothesesLooseElectronIsolation.ZllHypotheses[0].src = cms.InputTag('selectedElecTauPairZeeHypothesesLooseElectronIsolation')

produceElecTauPairZeeHypotheses = cms.Sequence(
    elecTauPairZeeHypotheses * selectedElecTauPairZeeHypotheses
   * elecTauPairVisMassHypotheses
   * elecTauPairZeeHypothesesLooseElectronIsolation * selectedElecTauPairZeeHypothesesLooseElectronIsolation
   * elecTauPairVisMassHypothesesLooseElectronIsolation
)
                                                  
