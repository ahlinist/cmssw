import FWCore.ParameterSet.Config as cms

ewkElecTauValHistManager = cms.PSet(
    pluginName = cms.string("ewkElecTauValHistManager"),
    pluginType = cms.string("EwkElecTauValHistManager"),
    
    # name of DQM (sub)directory in which histograms for Z --> tau+ tau- --> electron + tau-jet channel get stored
    dqmDirectory = cms.string("ElecTauChannel"),
    
    # names of input collections
    triggerResultsSource = cms.InputTag("TriggerResults::HLT"),
    vertexSource = cms.InputTag("offlinePrimaryVertices"),
    beamSpotSource = cms.InputTag("offlineBeamSpot"),
    electronSource = cms.InputTag("gsfElectrons"),
    tauJetSource = cms.InputTag("shrinkingConePFTauProducer"),
    caloMEtSource = cms.InputTag("met"),
    pfMEtSource = cms.InputTag("pfMet"),
    
    tauDiscrByLeadTrackFinding = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackFinding"),
    tauDiscrByLeadTrackPtCut = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackPtCut"),
    tauDiscrByTrackIso = cms.InputTag("shrinkingConePFTauDiscriminationByTrackIsolation"),
    tauDiscrByEcalIso = cms.InputTag("shrinkingConePFTauDiscriminationByECALIsolation"),
    tauDiscrAgainstElectrons = cms.InputTag("shrinkingConePFTauDiscriminationAgainstElectron"),
    tauDiscrAgainstMuons = cms.InputTag("shrinkingConePFTauDiscriminationAgainstMuon"),
    
    # high-level trigger paths
    # (at least one of the paths specified in the list is required to be passed)
    hltPaths = cms.vstring(
        #"HLT_Ele10_LW_L1R"
    ),
    
    # event selection criteria
    electronEtaCut = cms.double(2.5),
    electronPtCut = cms.double(15.),
    electronTrackIsoCut = cms.double(2.),
    electronEcalIsoCut = cms.double(3.),
    electronIsoMode = cms.string("absoluteIso"),
    
    tauJetEtaCut = cms.double(2.5),
    tauJetPtCut = cms.double(15.),
    
    visMassCut = cms.double(20.)
)
