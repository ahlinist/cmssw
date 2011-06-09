import FWCore.ParameterSet.Config as cms
import copy

isData = False

process = cms.Process("TTEffSKIM")
process.load('Configuration.EventContent.EventContent_cff')

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
    )
# This is also neede for printing debugs
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.debugModules = cms.untracked.vstring("IdentifiedTaus","IdentifiedTauFilter")

process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
if (isData):
    process.GlobalTag.globaltag = 'GR_R_38X_V15::All'
else:
    process.GlobalTag.globaltag = 'START41_V0::All'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	'/store/mc/Fall10/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/GEN-SIM-RECO/START38_V12-v1/0004/E6D9BDE2-86C8-DF11-9827-00215E221782.root'
    )
)

#good vertex + scraping 
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),	
    maxd0 = cms.double(2)	
)
process.scrapping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

process.PFTausSelected = cms.EDFilter("PFTauSelector",
    #src = cms.InputTag("shrinkingConePFTauProducer"),
    src = cms.InputTag("hpsPFTauProducer"),
    discriminators = cms.VPSet(
	#cms.PSet( discriminator=cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
	#	  selectionCut=cms.double(0.5)
	#),
	cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
		  selectionCut=cms.double(0.5)
	),
	cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByVLooseIsolation"),
		  selectionCut=cms.double(0.5)
	),
	cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"),
		  selectionCut=cms.double(0.5)
	),
	cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),
		  selectionCut=cms.double(0.5)
	),
    ),
    cut = cms.string('pt > 15. && abs(eta) < 2.5') 
)

process.PFTauSkimmed = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('PFTausSelected'),
  minNumber = cms.uint32(1)
)

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")
if(not isData):
  process.MuonHLTs.hltResults    = cms.InputTag('TriggerResults::REDIGI311X')

process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.TTEffSkimFilter = cms.Path(
	process.TTEffSkimCounterAllEvents *
        process.primaryVertexFilter * 
	process.scrapping *
	#process.MuonHLTs *
	#process.ElecHLTs *
	process.PFTau *
	process.PFTausSelected *
	process.PFTauSkimmed *
	process.TTEffSkimCounterSavedEvents
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("TTEffSkim.root"),
    outputCommands = process.FEVTEventContent.outputCommands,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('TTEffSkimFilter')
    )
)
process.out_step = cms.EndPath(process.output)

