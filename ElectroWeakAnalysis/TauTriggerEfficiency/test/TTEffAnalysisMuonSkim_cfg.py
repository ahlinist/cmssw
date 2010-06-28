import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("TTEffSKIM")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )
# This is also neede for printing debugs
process.MessageLogger.debugModules = cms.untracked.vstring("IdentifiedTaus","IdentifiedTauFilter")

#from ElectroWeakAnalysis.TauTriggerEfficiency.MinimumBias_BeamCommissioning09_SD_AllMinBias_Dec19thSkim_336p3_v1_RAW_RECO import *
#process.source = source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	"rfio:/castor/cern.ch/user/s/slehti/testData/Ztautau_Spring10-START3X_V26_S09-v1-RAW-RECO.root"
    )
)


process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")

process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.TTEffSkimFilter = cms.Path(
	process.TTEffSkimCounterAllEvents *
	process.MuonHLTs *
	process.TTEffSkimCounterSavedEvents
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim.root"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('TTEffSkimFilter')
    )
)
process.out_step = cms.EndPath(process.output)

