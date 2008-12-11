#! /usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("validation")

# keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# data
process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) 
)
process.source = cms.Source ("PoolSource",
    fileNames = cms.untracked.vstring (
        # /RelValTTbar/CMSSW_2_2_1_IDEAL_V9_v1/GEN-SIM-RECO
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0003/0401DC5B-F6C4-DD11-B584-000423D8F63C.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/122816BF-E6C5-DD11-8A8C-001617C3B78C.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/18CF2B07-41C5-DD11-B2F9-000423D6A6F4.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/1A67C708-41C5-DD11-AFEE-001617C3B73A.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/1E7F9C03-41C5-DD11-B323-001617E30F58.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/2AAEC706-41C5-DD11-852A-0019DB29C5FC.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/30758A26-41C5-DD11-947D-000423D6CA6E.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/3A098402-41C5-DD11-ADB1-0016177CA778.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/4C976B02-41C5-DD11-B036-001617C3B654.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/50FDB507-41C5-DD11-A7C5-001617E30D38.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/5C456707-41C5-DD11-94A3-001617E30F4C.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/604B91FB-40C5-DD11-981F-000423DD2F34.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/6E089A04-41C5-DD11-B9A0-001617E30D4A.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/80C3010C-41C5-DD11-95F8-000423D9853C.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/A0C3EFFD-40C5-DD11-835D-000423D9863C.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/B6185DE8-0FC5-DD11-A033-000423D6CA6E.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/B88F6215-41C5-DD11-A0A3-000423D985E4.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/CE1AB8FB-40C5-DD11-AB80-000423D6CA42.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/D00D60FF-40C5-DD11-9CA6-001617E30D40.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/D602F604-41C5-DD11-A3DD-001617DBD288.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/DA380D04-41C5-DD11-986F-001617E30CE8.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/DA97D207-41C5-DD11-B615-001617C3B70E.root',
        '/store/relval/CMSSW_2_2_1/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v1/0004/F8C5FE02-41C5-DD11-81CD-001617C3B778.root'
    )
)

# default configuration with frontier conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi")

# validation plots
process.load("RecoBTag.Analysis.bTagAnalysis_cfi")
process.bTagAnalysis.fastMC = True
process.bTagAnalysis.jetMCSrc = 'IC5byValAlgo'
#process.bTagAnalysis.allHistograms = True

# run btag, then validation
process.btag  = cms.Path(process.softMuonByIP3dBJetTags + process.softMuonByPtBJetTags)
process.plots = cms.Path(process.myPartons + process.iterativeCone5Flavour + process.bTagAnalysis)

process.schedule = cms.Schedule(process.btag, process.plots)
