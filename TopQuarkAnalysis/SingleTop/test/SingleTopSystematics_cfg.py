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
##Measurements from Fall10
process.load ("RecoBTag.PerformanceDB.BTagPerformanceDB1011")
process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDB1011")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (

'file:/tmp/oiorio/edmntuple_tchannel_big.root',

),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
#eventsToProcess = cms.untracked.VEventRange('1:19517967-1:19517969'),
)



                           

process.SystematicsAnalyzer = cms.EDAnalyzer('SingleTopSystematicsDumper',                              
systematics = cms.untracked.vstring("BTagUp","BTagDown","MisTagUp","MisTagDown","JESUp","JESDown","UnclusteredMETUp","UnclusteredMETDown"),
channelInfo = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(14.5),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    loosePtCut = cms.untracked.double(30.0),#Default 30.0 GeV
    ),


#Part of the kin quantities:
leptonsEta = cms.InputTag("nTupleElectrons","tightElectronsEta"),  
leptonsPt = cms.InputTag("nTupleElectrons","tightElectronsPt"),  
leptonsPhi = cms.InputTag("nTupleElectrons","tightElectronsPhi"),  
leptonsEnergy = cms.InputTag("nTupleElectrons","tightElectronsE"),  
leptonsCharge = cms.InputTag("nTupleElectrons","tightElectronsCharge"),  
leptonsRelIso = cms.InputTag("nTupleElectrons","tightElectronsRelIso"),  
leptonsID = cms.InputTag("nTupleElectrons","tightElectronsSimpleEleId95cIso"),  

leptonsFlavour = cms.untracked.string("electron"),


looseMuonsRelIso = cms.InputTag("looseElectrons","looseElectronsRelIso"),  
looseElectronsRelIso = cms.InputTag("looseMuons","looseMuonsRelIso"),  


jetsPt = cms.InputTag("nTupleTopJetsPF","topJetsPFPt"),  
jetsPhi = cms.InputTag("nTupleTopJetsPF","topJetsPFPhi"),  
jetsEta = cms.InputTag("nTupleTopJetsPF","topJetsPFEta"),  
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

#Input

#from TChannel import *
#process.source.fileNames = TChannel_ntuple

process.source.fileNames = cms.untracked.vstring("file:TChannelMerged.root")

#Output

process.TFileService = cms.Service("TFileService", fileName = cms.string("TChannel.root"))

process.load("SingleTopAnalyzers_cfi")
process.load("SingleTopRootPlizer_cfi")
from SingleTopPSets_cfi import *

process.TreesEle.channelInfo = TChannelEle
process.TreesMu.channelInfo = TChannelMu
process.PlotsEle.channelInfo = TChannelEle
process.PlotsMu.channelInfo = TChannelMu
#process.TreesMu.systematics = cms.untracked.vstring();

process.PathSys = cms.Path(
#    process.PlotsMu +
#    process.PlotsEle +
    process.TreesMu +
    process.TreesEle
   )
