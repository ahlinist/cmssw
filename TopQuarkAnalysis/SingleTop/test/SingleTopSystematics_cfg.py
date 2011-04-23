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


#'file:/tmp/oiorio/TSampleEleWJets_Old_PU_16_1_lOM.root',
#'file:/tmp/oiorio/TSampleEleWJets_Old_PU_102_0_TPi.root',
#'file:/tmp/oiorio/TSampleEleWJets_Old_PU_68_0_DGv.root',

'file:/tmp/oiorio/TChannelEle.root'
#'file:TSampleMuQCDMu_PF2PAT.root'
#'file:/tmp/oiorio/TSampleEleTChannel_10_1_T2Y.root'
#'file:/tmp/oiorio/TSampleEleWJets_89_1_oMj.root'


),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
#eventsToProcess = cms.untracked.VEventRange('1:19517967-1:19517969'),
)


#from WJets import *
#from WJets_Old_PU import *

#process.source.fileNames = WJets_Old_PU_EleT
#process.source.fileNames = WJetsEleT

                           

process.SystematicsAnalyzer = cms.EDAnalyzer('SingleTopSystematicsDumper',                              
systematics = cms.untracked.vstring("BTagUp","BTagDown","MisTagUp","MisTagDown","JESUp","JESDown","UnclusteredMETUp","UnclusteredMETDown"),
channelInfo = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
#    originalEvents = cms.untracked.double(14800000),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(14.5),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    loosePtCut = cms.untracked.double(30.0),#Default 30.0 GeV
    ),


#Part of the kin quantities:
leptonsPt = cms.InputTag("nTupleElectrons","topElectronsPt"),  
leptonsPz = cms.InputTag("nTupleElectrons","topElectronsPz"),  
leptonsPhi = cms.InputTag("nTupleElectrons","topElectronsPhi"),  
leptonsEta = cms.InputTag("nTupleElectrons","topElectronsEta"),  
leptonsCharge = cms.InputTag("nTupleElectrons","topElectronsCharge"),  

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

process.TFileService = cms.Service("TFileService", fileName = cms.string("plotsTChannelMu.root"))

from DataMu_B import *
from DataEle_B import *

from SingleTopAnalyzers_cfi import *

process.load("SingleTopAnalyzers_cfi")
#process.source.fileNames = cms.untracked.vstring('file:/tmp/oiorio/TTBarOldEle.root')
process.source.fileNames = DataMuT
process.TFileService.fileName = "plotsDataMuVanilla.root"




process.PathSys = cms.Path(
#    process.SystematicsAnalyzer
#    process.PlotsTTBarOldEle
    process.PlotsDataMu
    )
