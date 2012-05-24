import FWCore.ParameterSet.Config as cms
import copy

isData = True
doRECO = True
doMETleg = False

process = cms.Process("TTEffSKIM")
process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load("FWCore/MessageService/MessageLogger_cfi")
#process.MessageLogger.destinations = cms.untracked.vstring("cout")
#process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    )
# This is also neede for printing debugs
#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
#process.MessageLogger.debugModules = cms.untracked.vstring("IdentifiedTaus","IdentifiedTauFilter")

process.load('Configuration/StandardSequences/GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
if isData:
  process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
  process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
else:
  process.load('Configuration.StandardSequences.RawToDigi_cff')
  process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

if (isData):
    process.GlobalTag.globaltag = 'GR_R_52_V8::All'
else:
    process.GlobalTag.globaltag = 'START44_V13::All'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
)

if(isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
          '/store/data/Run2012A/Tau/RAW/v1/000/193/621/FCD858AA-9298-E111-8D12-001D09F251FE.root'
    )
  )
else:
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/tmp/slehti/Fall11_DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola_GEN-RAW_PU_S6_START42_V14B-v1_0000_86738BE8-EBF0-E011-8E03-003048C693E6.root'
#          "file:/tmp/mkortela/FCA4CF79-A5A6-E011-AA99-E0CB4E55366A.root"
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

# Produce PAT
import ElectroWeakAnalysis.TauTriggerEfficiency.Pat as pat
process.patSequence = pat.addPat(process, isData, False)

# Remove printout
process.makePatTaus.remove(process.tauGenJets)
process.makePatTaus.remove(process.tauGenJetsSelectorAllHadrons)
process.makePatTaus.remove(process.tauGenJetMatch)
process.makePatTaus.remove(process.tauGenJetMatchHpsPFTau)
process.patDefaultSequence.remove(process.tauGenJets)
process.patDefaultSequence.remove(process.tauGenJetsSelectorAllHadrons)
process.patDefaultSequence.remove(process.tauGenJetMatch)
process.patDefaultSequence.remove(process.tauGenJetMatchHpsPFTau)
del process.tauGenJets
del process.tauGenJetsSelectorAllHadrons
del process.tauGenJetMatch
del process.tauGenJetMatchHpsPFTau
process.patTaus.addGenJetMatch = False
process.patTaus.embedGenJetMatch = False
process.patTausHpsPFTau.addGenJetMatch = False
process.patTausHpsPFTau.embedGenJetMatch = False


import ElectroWeakAnalysis.TauTriggerEfficiency.ZtoMuTauFilter_cfi as zmutau
if doMETleg:
    process.PFTauSkimmed = zmutau.addTauSelection(process)
else:
    process.PFTauSkimmed = zmutau.addMuTauSelection(process)


process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.TTEffSkimFilter = cms.Path(
	process.TTEffSkimCounterAllEvents *
        process.primaryVertexFilter * 
	process.scrapping *
        process.patSequence *
	process.PFTauSkimmed *
	process.TTEffSkimCounterSavedEvents
)

# Produce the rho for L1FastJet
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.kt6PFJets.doRhoFastjet = True
#process.ak5PFJets.doAreaFastjet = True
#process.ak5PFJetSequence = cms.Sequence(process.kt6PFJets*process.ak5PFJets)

# Output definition
process.FEVTEventContent.outputCommands.extend([
        'drop *_*_*_TTEffSKIM',
        'drop *_*_*_RECO',
        'keep edmHepMCProduct_*_*_*',
        'keep recoGenParticles_*_*_*',
        'keep recoPFTaus_*_*_TTEffSKIM',
        'keep recoPFTauDiscriminator_*_*_TTEffSKIM',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_elecpreid_*_*',
        'keep *_particleFlow_*_*',
        'keep *_ak5PFJets_*_*',
        'keep recoTracks_*_*_*',
        'keep recoTrackExtras_*_*_*',
        'keep recoGsfTracks_*_*_*',
        'keep *_hltHbhereco_*_*',
        'keep recoMuons_*_*_*',
        'keep recoMuonMETCorrectionDataedmValueMap_*_*_*',
        'keep recoPFBlocks_*_*_*',
        'keep L1GctJetCands_*_*_*',
        'keep HcalNoiseSummary_*_*_*',
        'keep recoPFMETs_*_*_*',
        'keep recoCaloMETs_*_*_*',
        'keep *_l1extraParticles_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        'keep recoBeamSpot_*_*_*',
        'keep edmMergeableCounter_*_*_*',
        'keep *_patTaus*_*_*',
        'keep *_patMuons_*_*',
        'keep *_selectedPatTaus*_*_*',
        'keep *_selectedPatMuons_*_*',
        'keep *_selectedPatJets*_*_*',
        'keep *_addPileupInfo_*_*',
])
#process.FEVTEventContent.outputCommands.append('keep *')

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("TTEffSkim.root"),
    outputCommands = process.FEVTEventContent.outputCommands,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('TTEffSkimFilter')
    )
)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)
process.PFTau_step = cms.Path(process.PFTau)

if(doRECO):
    process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.TTEffSkimFilter,process.endjob_step,process.out_step)
#    process.recoPFJets.replace(process.ak5PFJets, process.ak5PFJetSequence)
else:
    process.schedule = cms.Schedule(process.PFTau_step,process.TTEffSkimFilter,process.endjob_step,process.out_step)
#    process.TTEffSkimFilter += process.ak5PFJetSequence
