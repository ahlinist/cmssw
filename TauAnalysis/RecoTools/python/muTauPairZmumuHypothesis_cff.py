import FWCore.ParameterSet.Config as cms
import copy

#from SimGeneral.HepPDTESSource.pdt_cfi import *

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about compatibility of an muon + tau-jet pair
# with the hypothesis of being a pair of muons,
# resulting from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

muTauPairZmumuHypotheses = cms.EDProducer("ZllHypothesisMuTauProducer",
    diCandidatePairSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),

    genLeptonsFromZsSource = cms.InputTag('genMuonsFromZs'),

    # try all possible combinations
    caloJetSource = cms.InputTag('iterativeCone5CaloJets'),
    pfJetSource = cms.InputTag('iterativeCone5PFJets'),                                       
    trackSource = cms.InputTag('generalTracks'),
    gsfElectronSource = cms.InputTag('pixelMatchGsfElectrons'),
    gsfTrackSource = cms.InputTag('pixelMatchGsfFit'),

    # combined (inner) tracks only                                      
    #trackSource = cms.InputTag('generalTracks'),
                                          
    tkminPixelHits = cms.int32(1),
    tkminTrackerHits = cms.int32(8),	
    tkmaxChi2 = cms.double(100.),

    dRmatch = cms.double(0.5),

    verbosity = cms.untracked.int32(0)                                      
)

muTauPairZmumuHypothesesLooseMuonIsolation = copy.deepcopy(muTauPairZmumuHypotheses)
muTauPairZmumuHypothesesLooseMuonIsolation.diCandidatePairSource = cms.InputTag('selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative')

produceMuTauPairZmumuHypotheses = cms.Sequence( muTauPairZmumuHypotheses
                                               * muTauPairZmumuHypothesesLooseMuonIsolation )
                                                  
