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
##	"file:reco.root"
##	"rfio:/castor/cern.ch/user/s/slehti/reco.root"
##	"rfio:/castor/cern.ch/cms/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_336p3_v1/0008/FC895826-E8F0-DE11-8E36-001D0967DA76.root"
#    )
#)


from ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff import *
from ElectroWeakAnalysis.TauTriggerEfficiency.OfflineTauIDFilter_cff import *

process.IncludedHLTs = IncludedHLTs
process.IdentifiedTauFilter = IdentifiedTauFilter


process.load("RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingPionPtCut_cfi")
process.thisPFRecoTauDiscriminationByLeadingPionPtCut = cms.EDFilter("PFRecoTauDiscriminationByLeadingPionPtCut",
#    PFTauProducer = cms.InputTag('pfRecoTauProducerHighEfficiency'),
    PFTauProducer = cms.InputTag('shrinkingConePFTauProducer'),
    MinPtLeadingPion = cms.double(3.0)
)

process.PFTausSelected = cms.EDFilter("PFTauSelector",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
#	cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByTrackIsolationHighEfficiency"),selectionCut=cms.double(0.5))
#	cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByLeadingPionPtCutHighEfficiency"),selectionCut=cms.double(-0.5))
#       cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.double(0.5))
#	cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.double(-0.5))
    )
)# Now filtered the tau collection is made




process.CounterAllEvents = cms.EDAnalyzer("EventCounter",
	fileName = cms.untracked.string("skim.root"),
	name = cms.string("All events")
)
process.CounterHTLEvents = cms.EDAnalyzer("EventCounter",
	fileName = cms.untracked.string("skim.root"),
	name = cms.string("Passed HLT")
)
process.CounterOfflineEvents = cms.EDAnalyzer("EventCounter",
	fileName = cms.untracked.string("skim.root"),
	name = cms.string("Offline tau")
)
process.CounterSavedEvents = cms.EDAnalyzer("EventCounter",
	fileName = cms.untracked.string("skim.root"),
	name = cms.string("Saved events")
)

process.tauFilter = cms.Path(
	process.CounterAllEvents *
#	process.IncludedHLTs *
	process.CounterHTLEvents *
#        process.thisPFRecoTauDiscriminationByLeadingPionPtCut*
	process.PFTausSelected *
	process.CounterOfflineEvents *
        process.IdentifiedTauFilter *
	process.CounterSavedEvents
)

cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"),
cms.InputTag("ecalRecHit","EcalRecHitsEE"))

process.TauMCProducer = cms.EDProducer("HLTTauMCProducer",
GenParticles  = cms.untracked.InputTag("genParticles"),
       ptMinTau      = cms.untracked.double(3),
       ptMinMuon     = cms.untracked.double(3),
       ptMinElectron = cms.untracked.double(3),
       BosonID       = cms.untracked.vint32(23),
       EtaMax         = cms.untracked.double(2.5)
)

##process.pmc  = cms.Path(process.TauMCProducer)

from ElectroWeakAnalysis.TauTriggerEfficiency.eventContent_cfi import *
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim.root"),
####    outputCommands = OutputCommands,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tauFilter')
    )
)
process.out_step = cms.EndPath(process.output)

