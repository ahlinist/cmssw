import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYZE")

process.inputs = cms.PSet (
    fileNames = cms.vstring() + [
        # Your data goes here:
        #"file:VGammaPAT_DimuonSkim_testWinter10.root"
        #os.path.join("file:",
                     #"/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu",
                     #"VGammaSkim_LyonSyncTest_DimuonSkim_testWinter10.root"
                     #)
        os.path.join(
            "file:",
            "/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu",
            file
        ) for file in [
            "VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_1_of_4.root",
            "VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_2_of_4.root",
            "VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_3_of_4.root",
            "VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_4_of_4.root",
        ]
    ]
)

process.outputs = cms.PSet (
    outputName = cms.string('basicExample.root')
)

process.analysis = cms.PSet(
    verbosity = cms.int32(1),
    muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
    photonSrc = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    beamSpotSrc = cms.InputTag("offlineBeamSpot"),
    ipDefinition = cms.string("RECO_globalTrackDxy_BS"),
    ipCondition = cms.string("LTGT")
)


