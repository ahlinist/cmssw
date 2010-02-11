import FWCore.ParameterSet.Config as cms

ewkElecMuValHistManager = cms.PSet(
    pluginName = cms.string("ewkElecMuValHistManager"),
    pluginType = cms.string("EwkElecMuValHistManager"),
    
    # name of DQM (sub)directory in which histograms for Z --> tau+ tau- --> electron + muon channel get stored
    dqmDirectory = cms.string("ElecMuChannel"),
    
    # names of input collections
    triggerResultsSource = cms.InputTag("TriggerResults::HLT"),
    vertexSource = cms.InputTag("offlinePrimaryVertices"),
    beamSpotSource = cms.InputTag("offlineBeamSpot"),
    electronSource = cms.InputTag("gsfElectrons"),
    muonSource = cms.InputTag("muons"),
    caloMEtSource = cms.InputTag("met"),
    pfMEtSource = cms.InputTag("pfMet"),
    
    # high-level trigger paths
    # (at least one of the paths specified in the list is required to be passed)
    hltPaths = cms.vstring(
        #"HLT_Mu3",
        #"HLT_Ele10_LW_L1R"
    ),

    # event selection criteria
    electronEtaCut = cms.double(2.5),
    electronPtCut = cms.double(15.),
    electronTrackIsoCut = cms.double(2.),
    electronEcalIsoCut = cms.double(3.),
    electronIsoMode = cms.string("absoluteIso"),

    muonEtaCut = cms.double(2.5),
    muonPtCut = cms.double(15.),
    muonTrackIsoCut = cms.double(2.),
    muonEcalIsoCut = cms.double(3.),
    muonIsoMode = cms.string("absoluteIso"),
    
    visMassCut = cms.double(20.)
)
