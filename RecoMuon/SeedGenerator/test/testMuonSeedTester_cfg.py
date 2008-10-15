import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonSeedTesterTest")

# Messages

process.load("Configuration.StandardSequences.Reconstruction_cff")
 
process.load("RecoMuon.Configuration.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.load("RecoMuon.SeedGenerator.selectorSET_cff")
#process.load("RecoMuon.MuonSeedGenerator.standAloneMuonSeeds_cff")

#process.load("RecoMuon.TrackingTools.MuonServiceProxy_cff")

#from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
#from Geometry.CommonDetUnit.globalTrackingGeometry_cfi import *

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    'file:muplus_e_60_300_probev2__24.root'
    'file:/uscms/home/ibloch/SAMu5_2_1_4/CMSSW_2_1_4/src/RecoMuon/SeedGenerator//test/800D44DB-2A6C-DD11-9BD6-000423D99EEE.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('TESTSeedTester.root')
                               )

#process.MuonSeedTesterLabel = cms.EDProducer("MuonSeedTester",
#  //  MuonServiceProxy
#)

process.STAMuonAnalyzer = cms.EDFilter("STAMuonAnalyzer",
    DataType = cms.untracked.string('SimData'),
    StandAloneTrackCollectionLabel = cms.untracked.string('standAloneMuons'),
    MuonSeedCollectionLabel = cms.untracked.string('MuonSeedTester'),
    rootFileName = cms.untracked.string('STAMuonAnalyzer.root'),
)


process.p = cms.Path(process.MuonSeedTester*process.standAloneMuons*process.STAMuonAnalyzer)
#process.p = cms.Path(process.MuonSeedTester)
#process.this_is_the_end = cms.EndPath(process.out)
process.GlobalTag.globaltag = 'IDEAL_V6::All'
