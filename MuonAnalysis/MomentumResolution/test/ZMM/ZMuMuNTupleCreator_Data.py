import FWCore.ParameterSet.Config as cms

process = cms.Process("ZMMNTupleCREATOR")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(100))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ZMM selection filter and Z candidates
process.load("MuonAnalysis.MomentumResolution.goldenZmmSelection_cfi")

# Source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
            #"rfio:/castor/cern.ch/user/j/jalcaraz/Data2010/GoldenZmumus_132440-144114.root"
            #"file:/ciet3b/data3/GoldenZmumus/RECOGoldenZmumus_VBTF_132440-144114.root"
            "file:/ciet3b/data4/MUSKIM2010B/MUAODRED_SKIM_144115-146729_1.root"
),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*',
            'drop edmTriggerResults_TriggerResults__*',
            'keep edmTriggerResults_TriggerResults__HLT',
            'keep edmTriggerResults_TriggerResults__REDIGI',
            'keep edmTriggerResults_TriggerResults__REDIGI36X'
      )
)

## Files in a directory on disk
#file_directory = "/ciet3b/data4/MUSKIM2010/"
#process.source.fileNames = cms.untracked.vstring()
#import os
#for file in os.listdir(file_directory):
#         process.source.fileNames.append("file:" + file_directory + "/" + file)

# Events processed
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


# Path
process.path = cms.Path(
      process.goldenZMMSelectionSequence
)

# Write a customized output
process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))
process.myEventContent.outputCommands.extend(
      cms.untracked.vstring(
            'keep *_zmmCands_*_*'
      )
)

# Output
process.zmmOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      , SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('path')
      )
      , fileName = cms.untracked.string('EdmZmmTreeDataShort.root')
)

# End path
process.outpath = cms.EndPath(process.zmmOutputModule)
