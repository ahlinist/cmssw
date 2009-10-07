import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("SKIM")

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
process.MessageLogger.debugModules = cms.untracked.vstring("IdentifiedTaus",

"IdentifiedTauFilter")

#from ElectroWeakAnalysis.TauTriggerEfficiency.RelValQCD_Pt_80_120_GEN_SIM_DIGI_RAW_HLTDEBUG_RECO_IDEAL_V9_v2 import *
#from ElectroWeakAnalysis.TauTriggerEfficiency.QCD_Mike_HLTExtra import *
#process.source = source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	"file:reco.root"
##      "file:/tmp/chinhan/hltRECO.root"
##	"rfio:/castor/cern.ch/user/s/slehti/reco.root"
    )
)


from ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff import *
#from ElectroWeakAnalysis.TauTriggerEfficiency.OfflineTauIDProducer_cff import *
from ElectroWeakAnalysis.TauTriggerEfficiency.OfflineTauIDFilter_cff import *

#process.IdentifiedTaus = IdentifiedTaus
process.IncludedHLTs = IncludedHLTs
process.IdentifiedTauFilter = IdentifiedTauFilter





process.thisPFRecoTauDiscriminationByLeadingPionPtCut = cms.EDFilter("PFRecoTauDiscriminationByLeadingPionPtCut",
#    PFTauProducer = cms.InputTag('pfRecoTauProducerHighEfficiency'),
    PFTauProducer = cms.InputTag('shrinkingConePFTauProducer'),
    MinPtLeadingPion = cms.double(3.0)
)

process.PFTausSelected = cms.EDFilter("PFTauSelector",
#    src = cms.InputTag("pfRecoTauProducerHighEfficiency"),
    src = cms.InputTag("shrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
	#cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByTrackIsolationHighEfficiency"),selectionCut=cms.double(0.5))
	#cms.PSet( discriminator=cms.InputTag("pfRecoTauDiscriminationByLeadingPionPtCutHighEfficiency"),selectionCut=cms.double(0.5))
	cms.PSet( discriminator=cms.InputTag("thisPFRecoTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.double(0.5))
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
	process.IncludedHLTs *
	process.CounterHTLEvents *
        process.thisPFRecoTauDiscriminationByLeadingPionPtCut*
	process.PFTausSelected *
	process.CounterOfflineEvents *
#	process.IdentifiedTaus *
        process.IdentifiedTauFilter *
	process.CounterSavedEvents
)

## L1CaloSim
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
#process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
#process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")
#process.load("RecoTauTag.L1CaloSim.l1calosim_cfi")
#process.l1CaloSim.AlgorithmSource = "RecHits"
#process.l1CaloSim.EmInputs =
cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"),
cms.InputTag("ecalRecHit","EcalRecHitsEE"))
#process.l1CaloSim.DoBitInfo = cms.bool(True)
#process.l1CaloSim.EMActiveLevelIso = cms.double(2.0)
#process.l1CaloSim.HadActiveLevelIso = cms.double(2.0)
#process.l1CaloSim.IsolationEt = cms.double(4.0)
#
#process.p  = cms.Path(process.l1CaloSim)

process.TauMCProducer = cms.EDProducer("HLTTauMCProducer",
GenParticles  = cms.untracked.InputTag("genParticles"),
       ptMinTau      = cms.untracked.double(3),
       ptMinMuon     = cms.untracked.double(3),
       ptMinElectron = cms.untracked.double(3),
       BosonID       = cms.untracked.vint32(23),
       EtaMax         = cms.untracked.double(2.5)
)

process.pmc  = cms.Path(process.TauMCProducer)

from ElectroWeakAnalysis.TauTriggerEfficiency.eventContent_cfi import *
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim.root"),
####    outputCommands = OutputCommands,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tauFilter')
    )
)
process.out_step = cms.EndPath(process.output)

