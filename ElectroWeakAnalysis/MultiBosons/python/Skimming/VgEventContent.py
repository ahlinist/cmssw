extraConversionEventContent = [
    'keep *_photonCore_*_*',
    'keep *_conversions_*_*',
    'keep *_trackerOnlyConversions_*_*',
    'keep *_ckfInOutTracksFromConversions_*_*',
    'keep *_ckfOutInTracksFromConversions_*_*',
    ]

vgExtraPhotonEventContent = [
    'keep *_photons_*_*',
    'keep *_photonCore_*_*',
    'keep *_conversions_*_*',
    'keep *_trackerOnlyConversions_*_*',
    'keep *_ckfInOutTracksFromConversions_*_*',
    'keep *_ckfOutInTracksFromConversions_*_*',
    'keep *_*5x5SuperClusters*_*_*',
    'keep *_multi5x5BasicClusters_*Endcap*_*',
    'keep *_hybridSuperClusters_*_*',
    'keep *_correctedHybridSuperClusters_*_*',
    ]

vgExtraAnalyzerKitEventContent = [
    'keep *_allTrackCandidates_*_*',
    'keep *_genParticles_*_*',
    'keep *_hltGtDigis_*_*',
    'keep *_TriggerResults_*_*',
    'keep *_gtDigis_*_*',
    'keep *_offlinePrimaryVertices_*_*',
    'keep *_towerMaker_*_*',
    'keep *_generalTracks_*_*',
    'keep *_tcMet_*_*',
    'keep *_pfMet_*_*',
    'keep *_reducedEcalRecHitsEB_*_*',
    'keep *_reducedEcalRecHitsEE_*_*',
    'keep *_globalMuons_*_*',
    'keep *_pixelTracks_*_*',
    'keep *_standAloneMuons_*_*',
    'keep *_tevMuons_*_*'
    ]

vgMuMuGammaEventContent = [
    'keep *_MuMuGammas_*_*',
    'keep *_MuMuGammasOneTrack_*_*',
    ]

vgCandsEventContent = [
    'keep *_electronPlusMETs_*_*',
    'keep *_muonPlusMETs_*_*',
    'keep *_dielectrons_*_*',
    'keep *_dimuons_*_*',
    'keep *_WENuGammaCands_*_*',
    'keep *_WMuNuGammaCands_*_*',
    'keep *_ZEEGammaCands_*_*',
    'keep *_ZMuMuGammaCands_*_*',
    'keep *_ZInvisibleGammaCands_*_*',
    ]

extraSkimEventContent = vgCandsEventContent[:] + \
    extraConversionEventContent[:] + [
        "keep *_gsfElectronCores_*_*",
        "keep *_offlinePrimaryVertices_*_*",
        "keep *_offlineBeamSpot_*_*",
        "keep *_TriggerResults_*_PAT",
        ## keep lightweight generator content
        "keep *_generator_*_*",
        "drop edmHepMCProduct_*_*_*"
        ]

