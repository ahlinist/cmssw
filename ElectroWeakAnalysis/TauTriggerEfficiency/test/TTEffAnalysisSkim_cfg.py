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

from ElectroWeakAnalysis.TauTriggerEfficiency.MinimumBias_BeamCommissioning09_SD_AllMinBias_Dec19thSkim_336p3_v1_RAW_RECO import *
process.source = source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
##	"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_336p3_v1/0008/FC895826-E8F0-DE11-8E36-001D0967DA76.root"
#    )
#)

# The proper luminosity sections
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(
    '123596:2-123596:9999',   # OK
    '123615:70-123615:9999',  # OK
    '123732:62-123732:9999',  # 62 -9999 (Pixel off in 56-61)
    '123815:8-123815:9999',   # 8 - 9999 ( why not 7 ?)
    '123818:2-123818:42',     # OK
    '123908:2-123908:12',     # 2 - 12 (why not 13 ?)
    '124008:1-124008:1',      # OK
    '124009:1-124009:68',     # OK
    '124020:12-124020:94',    # OK
    '124022:66-124022:179',   # OK
    '124023:38-124023:9999',  # OK
    '124024:2-124024:83',     # OK
    '124025:5-124025:13',     # 5 - 13 (why not 3 & 4 ?)
    '124027:24-124027:9999',  # OK
    '124030:2-124030:9999',   # 2 - 9999 ( why not 1 ?)
    '124120:1-124120:9999',   # OK
    '124275:3-124275:30'
)

# This is for filtering on L1 technical trigger bit: MB and no beam halo
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')

process.scrapping = cms.EDFilter("FilterOutScraping",
    	applyfilter = cms.untracked.bool(True),
    	debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
)

process.PFTausSelected = cms.EDFilter("PFTauSelector",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
#       cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByTrackIsolationHighEfficiency"),selectionCut$
#       cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByLeadingPionPtCutHighEfficiency"),selectionC$
#       cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.dou$
#       cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.dou$
    )
)

process.tauFilter = cms.Path(
	process.hltLevel1GTSeed *
	process.scrapping *
	process.PFTausSelected
)

from ElectroWeakAnalysis.TauTriggerEfficiency.eventContent_cfi import *
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim.root"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tauFilter')
    )
)
process.out_step = cms.EndPath(process.output)

