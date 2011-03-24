import FWCore.ParameterSet.Config as cms

process = cms.Process("SingleTopSystematics")


process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    FailPath = cms.untracked.vstring('ProductNotFound','Type Mismatch')
    )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff") ### real data
process.GlobalTag.globaltag = cms.string("START39_V9::All")



#Load B-Tag
#MC measurements from 36X
process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDBMC36X")
process.load ("RecoBTag.PerformanceDB.BTagPerformanceDBMC36X")
##Data measurements from Fall10
process.load ("RecoBTag.PerformanceDB.BTagPerformanceDB1011")
process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDB1011")

#Data measurements from Fall10
#process.load ("RecoBTag.PerformanceDB.BTagPerformanceDB100426")
#process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDB100426")


#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (

'file:TSampleMuQCDMu.root'

),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("plotting.root"))

process.SystematicsAnalyzer = cms.EDAnalyzer('SingleTopSystematicsDumper',                              
systematics = cms.untracked.vstring("BTagUp","BTagDown","MisTagUp","MisTagDown","JESUp","JESDown","UnclusteredMETUp","UnclusteredMETDown"),
channelInfo = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets"),
    originalEvents = cms.untracked.double(15000000),
    finalLumi = cms.untracked.double(36.1),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    ),


#Part of the kin quantities:
leptonsPt = cms.InputTag("nTupleMuons","topMuonsPt"),  
leptonsPz = cms.InputTag("nTupleMuons","topMuonsPz"),  
leptonsPhi = cms.InputTag("nTupleMuons","topMuonsPhi"),  
leptonsEta = cms.InputTag("nTupleMuons","topMuonsEta"),  
leptonsCharge = cms.InputTag("nTupleMuons","topMuonsCharge"),  

jetsEta = cms.InputTag("nTupleTopJetsPF","topJetsPFEta"),  
jetsPx = cms.InputTag("nTupleTopJetsPF","topJetsPFPx"),  
jetsPy = cms.InputTag("nTupleTopJetsPF","topJetsPFPy"),  
jetsPz = cms.InputTag("nTupleTopJetsPF","topJetsPFPz"),  
jetsPt = cms.InputTag("nTupleTopJetsPF","topJetsPFPt"),  

jetsEnergy = cms.InputTag("nTupleTopJetsPF","topJetsPFE"),  
jetsBTagAlgo = cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighPur"),  
jetsAntiBTagAlgo =  cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighEff"),  
jetsFlavour = cms.InputTag("nTupleTopJetsPF","topJetsPFFlavour"),   

jetsCorrTotal = cms.InputTag("nTupleTopJetsPF","topJetsPFJetCorrTotal"),   

METPhi = cms.InputTag("nTuplePatMETsPF","patMETsPFPhi"),
METPt = cms.InputTag("nTuplePatMETsPF","patMETsPFPt"),

UnclusteredMETPx = cms.InputTag("UnclusteredMETPF","UnclusteredMETPx"),
UnclusteredMETPy = cms.InputTag("UnclusteredMETPF","UnclusteredMETPy"),


#patJets = cms.InputTag("topJetsPF"),

)

process.PathSys = cms.Path(
    process.SystematicsAnalyzer
    )
