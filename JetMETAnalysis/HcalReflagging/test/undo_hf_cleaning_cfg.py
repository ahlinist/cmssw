# CAUTION: This cfg file should only be used in CMSSW_3_X_Y >= 3_5_7. Prior to this release
#          HF cleaning was not applied. Hence there is no cleaning to be undone

import FWCore.ParameterSet.Config as cms

maxevents = 100
isMC = False
version = 2   # version 1 = default (loose), version 2 = (medium), version 3 = (tight)

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

# New SeverityLevelComputer that can be used to undo the default HF cleaning
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFDigiTime")

print "These are the severity levels for the various rechit flags:"
print "(Severity > 10 causes rechit to be ignored by CaloTower maker)"
for i in process.hcalRecAlgos.SeverityLevels:
    print i



# Path and EndPath definitions
process.rereco_step = cms.Path(process.caloTowersRec*(process.recoJets*process.recoJetIds+process.recoTrackJets)*process.recoJetAssociations*process.metreco) # re-reco jets and met
#process.rereco_step = cms.Path(process.towerMaker*process.ak5CaloJets*process.met) # a simpler use case
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.rereco_step,process.out_step)
