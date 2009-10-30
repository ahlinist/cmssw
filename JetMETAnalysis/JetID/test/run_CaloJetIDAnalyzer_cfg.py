################################################################################
#
# run_CaloJetIDAnalyzer_cfg.py
# ----------------------------
#
# 1) tracks in CRAFT not useful for tag&probe - disabled throughout
# 2) all jets should be booked
#
################################################################################

import FWCore.ParameterSet.Config as cms


#!
#! PROCESS
#!
process = cms.Process("JETID")


#!
#! INPUT
#!
qcd80to120Files = cms.untracked.vstring(
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0003/D6D98D7E-9CC1-DE11-BD55-000423D99F1E.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/B2B5E8F0-8BC0-DE11-8BCE-000423D987FC.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/A6A29A2B-91C0-DE11-B24E-003048D2C0F0.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/921C96C3-8DC0-DE11-A39A-000423D94908.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/6C4A8F11-8AC0-DE11-8F0E-0030486733D8.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/500CA566-92C0-DE11-9A62-001617C3B77C.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_31X_V9-v3/0002/44601D3B-8FC0-DE11-B8DC-0030487C6062.root'
    )

qcdFiles = cms.untracked.vstring(
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/FC5633F4-CAC0-DE11-9B8C-0030487C6090.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/AC1E71D1-C9C0-DE11-AD08-0030487C6090.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/AAEB831F-E4C0-DE11-85CC-0030487C6090.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/80B9B076-C6C0-DE11-970A-000423D98B6C.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/7A6512C0-9CC1-DE11-9228-0030487A18F2.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/1CE3F169-C7C0-DE11-914D-0030487C6090.root',
    '/store/relval/CMSSW_3_3_1/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_31X_V9-v3/0003/16D5C17F-C8C0-DE11-9BEE-0030487A1990.root'
    )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.source = cms.Source( 
    "PoolSource" , fileNames = qcd80to120Files
    )


#!
#! SERVICES
#!
process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    cout         = cms.untracked.PSet( threshold = cms.untracked.string('WARNING') )
    )
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string('JetID.root'),
)
#process.Timing = cms.Service("Timing")
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")


#!
#! MODULES
#!

# KINEMATIC JET PRESELECTION
process.ak5CaloJetsEtaPt = cms.EDProducer(
    "CaloJetSelector",
    src    = cms.InputTag('ak5CaloJets'),
    cut    = cms.string('pt>0.0 && abs(eta)<9.9')
    )

# JET-TO-TRACKS ASSOCIATION
process.ak5CaloJetsJTA = cms.EDProducer(
    "JetTracksAssociatorAtVertex",
    jets     = cms.InputTag('ak5CaloJetsEtaPt'),
    tracks   = cms.InputTag('generalTracks'),
    coneSize = cms.double(0.5)
    )

# TRACK-JET RECONSTRUCTION
process.load("RecoJets.JetProducers.TracksForJets_cff")
process.load("RecoJets.JetProducers.ak5TrackJets_cfi")

# JEC
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorAK5Calo")

# HCAL noise removal, courtesy of John Paul
process.load("RecoMET.METProducers.hcalnoiseinfoproducer_cfi")
process.hcalnoise.refillRefVectors = cms.bool(True)
process.hcalnoise.requirePedestals = cms.bool(False)
process.hcalnoise.hcalNoiseRBXCollName = "hcalnoise"

# JETID
process.load('JetMETAnalysis.JetID.CaloJetIDAnalyzer_cfi')
process.ak5JetID = process.jetid.clone(
    bookAll              = True,
    useTracking          = True,
    requireRandomTrigger = False,
    srcCaloJets          = 'ak5CaloJetsEtaPt',
    srcCaloJetTrkAssoc   = 'ak5CaloJetsJTA',
    srcTracks            = 'generalTracks',
    srcTrackJets         = 'ak5TrackJets',
    jecTag               = 'L2L3JetCorrectorAK5Calo'
    )

#!
#! PATHS
#!

process.ak5Jets = cms.Sequence(
    process.ak5CaloJetsEtaPt*
    process.ak5CaloJetsJTA*
    process.tracksForJets*
    process.ak5TrackJets

    )

process.analysis = cms.Path(
    process.ak5Jets*
    process.hcalnoise*
    process.ak5JetID
    )

