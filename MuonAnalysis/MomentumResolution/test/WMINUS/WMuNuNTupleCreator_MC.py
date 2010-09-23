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
#process.wmnHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# For 36X MC:
process.wmnHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI36X")

# Source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
            #"file:/ciet3b/data4/DataMu2010/GoldenWmunus_ICHEP10.root"
            #"rfio:///castor/cern.ch/user/j/jalcaraz/Data2010/GoldenWmunus_ICHEP10.root"
            #"file:/ciet3b/data4/Summer10_10invpb_AODSIM/Wmunu/Wmunu_1.root"
      ),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*'
      )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

import os
file_directory = "/ciet3b/data4/Summer10_All_MinimalAOD/WmunuMINUS"
for file in os.listdir(file_directory):
         process.source.fileNames.append("file:" + file_directory + "/" + file)
print "files to be processed %d" % (len(process.source.fileNames))

process.wmnPrunedGenParticles = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("prunedGenParticles"),
    select = cms.vstring(
        "drop *"
      , "keep++ pdgId = 24"
      , "keep++ pdgId = -24"
    )
)


                                                                   
# Path
process.path = cms.Path(
  process.goldenWMNSelectionSequence
  * process.wmnPrunedGenParticles
)

process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))  
process.myEventContent.outputCommands.extend(                                      
      cms.untracked.vstring(                                                       
            'keep *_wmnCands_*_*'                                                  
            , 'keep *_wmnPrunedGenParticles_*_*'

      )
)

# Output
process.wmnOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      #, fileName = cms.untracked.string('ZMuMuCandidates.root')
      , fileName = cms.untracked.string('EdmWmnTreeMCMINUSShort.root')
)

# End path
process.outpath = cms.EndPath(process.wmnOutputModule)



