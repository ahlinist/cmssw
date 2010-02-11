import FWCore.ParameterSet.Config as cms

ewkMuTauValHistManager = cms.PSet(
    pluginName = cms.string("ewkMuTauValHistManager"),
    pluginType = cms.string("EwkMuTauValHistManager"),
    
    # name of DQM (sub)directory in which histograms for Z --> tau+ tau- --> muon + tau-jet channel get stored
    dqmDirectory = cms.string("MuTauChannel"),
    
    # names of input collections
    triggerResultsSource = cms.InputTag("TriggerResults::HLT"),
    vertexSource = cms.InputTag("offlinePrimaryVertices"),
    beamSpotSource = cms.InputTag("offlineBeamSpot"),
    muonSource = cms.InputTag("muons"),
    tauJetSource = cms.InputTag("shrinkingConePFTauProducer"),
    caloMEtSource = cms.InputTag("met"),
    pfMEtSource = cms.InputTag("pfMet"),
    
    tauDiscrByLeadTrackFinding = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackFinding"),
    tauDiscrByLeadTrackPtCut = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackPtCut"),
    tauDiscrByTrackIso = cms.InputTag("shrinkingConePFTauDiscriminationByTrackIsolation"),
    tauDiscrByEcalIso = cms.InputTag("shrinkingConePFTauDiscriminationByECALIsolation"),
    tauDiscrAgainstMuons = cms.InputTag("shrinkingConePFTauDiscriminationAgainstMuon"),
    
    # high-level trigger paths
    # (at least one of the paths specified in the list is required to be passed)
    hltPaths = cms.vstring(
        #"HLT_Mu3"
    ),

    # event selection criteria
    muonEtaCut = cms.double(2.5),
    muonPtCut = cms.double(15.),
    muonTrackIsoCut = cms.double(2.),
    muonEcalIsoCut = cms.double(3.),
    muonIsoMode = cms.string("absoluteIso"),
    
    tauJetEtaCut = cms.double(2.5),
    tauJetPtCut = cms.double(15.),
    
    visMassCut = cms.double(20.)
)
