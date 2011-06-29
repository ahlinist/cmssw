import FWCore.ParameterSet.Config as cms
import copy

isData = False
doRECO = True

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
    process.GlobalTag.globaltag = 'GR_R_311_V4::All'
else:
    process.GlobalTag.globaltag = 'START41_V0::All'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
)

if(isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/tmp/slehti/muTau_HLT-AOD.root'
    )
  )
else:
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#	'/store/mc/Fall10/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/GEN-SIM-RECO/START38_V12-v1/0004/E6D9BDE2-86C8-DF11-9827-00215E221782.root'
	'file:/tmp/slehti/Fall10_DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola_GEN-SIM-RAW_START38_V12-v1_0000_8037FA24-3EC8-DF11-9725-00215E221B48.root'
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

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.ZtoMuTauFilter_cfi")
process.PFTauSkimmed = process.muTauFilterSequence

process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.TTEffSkimFilter = cms.Path(
	process.TTEffSkimCounterAllEvents *
        process.primaryVertexFilter * 
	process.scrapping *
	process.PFTau *
	process.PFTauSkimmed *
	process.TTEffSkimCounterSavedEvents
)

# Output definition
process.FEVTEventContent.outputCommands.append('drop *_*_*_TTEffSKIM')
process.FEVTEventContent.outputCommands.append('drop *_*_*_RECO')
process.FEVTEventContent.outputCommands.append('keep edmHepMCProduct_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoGenParticles_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFTaus_*_*_TTEffSKIM')
process.FEVTEventContent.outputCommands.append('keep recoPFTauDiscriminator_*_*_TTEffSKIM')
process.FEVTEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
process.FEVTEventContent.outputCommands.append('keep *_elecpreid_*_*')
process.FEVTEventContent.outputCommands.append('keep *_particleFlow_*_*')
process.FEVTEventContent.outputCommands.append('keep *_ak5PFJets_*_*')
process.FEVTEventContent.outputCommands.append('keep *_generalTracks_*_*')
process.FEVTEventContent.outputCommands.append('keep *_hltHbhereco_*_*')
process.FEVTEventContent.outputCommands.append('keep recoMuons_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFBlocks_*_*_*')
process.FEVTEventContent.outputCommands.append('keep L1GctJetCands_*_*_*')
process.FEVTEventContent.outputCommands.append('keep HcalNoiseSummary_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFMETs_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoCaloMETs_*_*_*')
process.FEVTEventContent.outputCommands.append('keep *_l1extraParticles_*_*')
process.FEVTEventContent.outputCommands.append('keep L1GlobalTriggerReadoutRecord_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoBeamSpot_*_*_*')
process.FEVTEventContent.outputCommands.append('keep edmMergeableCounter_*_*_*')
#process.FEVTEventContent.outputCommands.append('keep *')

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("TTEffSkim.root"),
    outputCommands = process.FEVTEventContent.outputCommands,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('TTEffSkimFilter')
    )
)

if(isData):
  process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
else:
  process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)
process.PFTau_step = cms.Path(process.PFTau)

process.schedule = cms.Schedule(process.PFTau_step,process.TTEffSkimFilter,process.endjob_step,process.out_step)
if(doRECO):
    process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.TTEffSkimFilter,process.endjob_step,process.out_step)
