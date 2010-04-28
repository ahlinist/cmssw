import FWCore.ParameterSet.Config as cms

maxevents = 100
isMC = False
version = 2   # version 1 = default (loose), version 2 = (medium), version 3 = (tight)
# VERSION 2 is the currently recommended version, as of 22 April 2010.

process = cms.Process('USER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# GLOBAL TAGS: REPLACE WITH WHATEVER IS APPROPRIATE FOR YOUR WORK!

# MC
if (isMC):
    process.GlobalTag.globaltag ='START3X_V26A::All'

# DATA
else:
    process.GlobalTag.globaltag ='GR10_P_V4::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxevents)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F65A94F7-4141-DF11-9F4E-003048D47A80.root'
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('output_file.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

# Other statements
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# New SeverityLevelComputer that forces RecHits with UserDefinedBit0 set to be excluded from new rechit collection
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")
process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)

# HF RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
if version==1:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
elif version==2:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif version==3:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()

process.output.outputCommands.append("keep *_hfrecoReflagged_*_*")
#new rechit collection name is:  HFRecHitsSorted_hfrecoReflagged__USER

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"

# Path and EndPath definitions
process.reflagging_step = cms.Path(process.hfrecoReflagged)
process.rereco_step = cms.Path(process.caloTowersRec*(process.recoJets*process.recoJetIds+process.recoTrackJets)*process.recoJetAssociations*process.metreco) # re-reco jets and met
#process.rereco_step = cms.Path(process.towerMaker*process.ak5CaloJets*process.met) # a simpler use case
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.reflagging_step,process.rereco_step,process.out_step)
