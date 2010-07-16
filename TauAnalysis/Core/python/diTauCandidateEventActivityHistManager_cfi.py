import FWCore.ParameterSet.Config as cms

from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *

diTauCandidateEventActivityHistManager = cms.PSet(    
    pluginName = cms.string('diTauCandidateEventActivityHistManager'),
    pluginType = cms.string('DiCandidatePairEventActivityHistManager'),
      
    diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),

    dRveto = cms.double(0.5),

    etaGaps = cms.vdouble(0.5, 1.0, 1.5, 2.0),

    particlePtThresholds = cms.vdouble(0.5, 1.0, 1.5, 2.0),
    chargedParticlePtThresholds = cms.vdouble(0.5, 1.0, 1.5, 2.0),
    jetPtThresholds = cms.vdouble(10.0, 15.0, 20.0),

    pfCandidateSource  = cms.InputTag('particleFlow'),
    patJetSource = cms.InputTag('patJets'),

    dqmDirectory_store = cms.string('DiTauCandidateEventActivityQuantities'),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events")
)
