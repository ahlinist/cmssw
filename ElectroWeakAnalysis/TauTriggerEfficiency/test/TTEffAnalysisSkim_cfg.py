import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("TTEffSKIM")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
    )
# This is also neede for printing debugs
process.MessageLogger.debugModules = cms.untracked.vstring("IdentifiedTaus","IdentifiedTauFilter")

process.load('CondCore.DBCommon.CondDBSetup_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR10_E_V4A::All'
process.GlobalTag.globaltag = 'MC_3XY_V25::All'
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

#from ElectroWeakAnalysis.TauTriggerEfficiency.MinimumBias_BeamCommissioning09_SD_AllMinBias_Dec19thSkim_336p3_v1_RAW_RECO import *
#process.source = source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FE8C8E24-7C50-DF11-8490-0030486790A6.root',
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FE8323A7-7C50-DF11-A7C6-00248C0BE016.root',
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FE3BD1F6-7B50-DF11-A603-0026189438E0.root',
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FCEC93A7-7C50-DF11-B786-0026189438E0.root',
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FCADDDAB-7C50-DF11-9B0D-003048679296.root',
        '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_357ReReco-v3/0188/FC6793CC-7D50-DF11-8323-003048679228.root'
    )
)


# This is for filtering on L1 technical trigger bit: MB and no beam halo
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')
# For MC
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

process.scrapping = cms.EDFilter("FilterOutScraping",
    	applyfilter = cms.untracked.bool(True),
    	debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
)

###
#process.load("RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingPionPtCut_cfi")
#process.thisPFRecoTauDiscriminationByLeadingPionPtCut = cms.EDFilter("PFRecoTauDiscriminationByLeadingPionPtCut",
#    PFTauProducer = cms.InputTag('shrinkingConePFTauProducer'),
#    MinPtLeadingPion = cms.double(3.0)
#)
#
#process.PFTausSelected = cms.EDFilter("PFTauSelector",
#    src = cms.InputTag("shrinkingConePFTauProducer"),
#    discriminators = cms.VPSet(
#       cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByTrackIsolationHighEfficiency"),selectionCut=cms.double(0.5)),
#       cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.double(0.5))
#    )
#)

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.pftauSkim_cff")
process.tauFilter = cms.Path(
	process.hltLevel1GTSeed *
	process.scrapping *
	process.looseShrikingConePFTaus *
	process.thePFTauDiscByLeadTrkFinding *
	process.thePFTauDiscByIsolation *
	process.PFTausSelected *
	process.PFTauSkimmed
)

from ElectroWeakAnalysis.TauTriggerEfficiency.eventContent_cfi import *
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim.root"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tauFilter')
    )
)
process.out_step = cms.EndPath(process.output)

