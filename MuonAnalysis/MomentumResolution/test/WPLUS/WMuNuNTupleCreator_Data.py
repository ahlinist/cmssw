import FWCore.ParameterSet.Config as cms

process = cms.Process("WMNNtupleCREATOR")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(10))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# WMN selection filter and W candidates
process.load("ElectroWeakAnalysis.WMuNu.wmnSelection_cfi")
# For data:
process.wmnHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# For 36X MC:
#process.wmnHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI36X")

# Source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
            "file:/ciet3b/data3/GoldenWmunus/GoldenWmunus_132440-140182.root",
            "file:/ciet3b/data3/GoldenWmunus/GoldenWmunus_140183-142557.root",
            "file:/ciet3b/data3/GoldenWmunus/GoldenWmunus_142558-143179.root",
            "file:/ciet3b/data3/GoldenWmunus/GoldenWmunus_143180-143336.root",
            "file:/ciet3b/data3/GoldenWmunus/GoldenWmunus_143337-144114.root"

            #"rfio:///castor/cern.ch/user/j/jalcaraz/Data2010/GoldenWmunus_ICHEP10.root"
            #"file:/ciet3b/data4/Summer10_10invpb_AODSIM/Wmunu/Wmunu_1.root"
      ),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*',
            'drop edmTriggerResults_TriggerResults__HighPtMuskim'
      )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

                                                                  
# Path
process.path = cms.Path(
  process.goldenWMNSelectionSequence
)

process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))  
process.myEventContent.outputCommands.extend(                                      
      cms.untracked.vstring(                                                       
            'keep *_wmnCands_*_*'                                                  

      )
)

# Output
process.wmnOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      #, fileName = cms.untracked.string('ZMuMuCandidates.root')
      , fileName = cms.untracked.string('EdmWmnTreeDataShort.root')
)

# End path
process.outpath = cms.EndPath(process.wmnOutputModule)



