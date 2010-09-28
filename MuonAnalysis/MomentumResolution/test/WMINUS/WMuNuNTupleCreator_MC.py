import FWCore.ParameterSet.Config as cms

process = cms.Process("WMNNtupleCREATOR")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(100))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# WMN selection filter and W candidates
process.load("MuonAnalysis.MomentumResolution.goldenWmnSelection_cfi")

# Source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
            #"rfio:/castor/cern.ch/..."
            #"file:/MyMCOnDisk/..."
      ),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*'
      )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

## Files in a directory on disk
file_directory = "/ciet3b/data4/Summer10_10invpb_AODSIM/WmunuMINUS"
#file_directory = "/ciet3b/data4/Summer10_All_MinimalAOD/WmunuMINUS"
process.source.fileNames = cms.untracked.vstring()
import os
for file in os.listdir(file_directory):
         process.source.fileNames.append("file:" + file_directory + "/" + file)
print "files to be processed %d" % (len(process.source.fileNames))

process.wmnPrunedGenParticles = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticles"),
    #src = cms.InputTag("prunedGenParticles"),
    select = cms.vstring(
        "drop *"
      , "keep++ pdgId = 24"
      , "keep++ pdgId = -24"
    )
)


                                                                   
# Path
process.path = cms.Path(
      process.wmnPrunedGenParticles *
      process.goldenWMNSelectionSequence
)

process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))  
process.myEventContent.outputCommands.extend(                                      
      cms.untracked.vstring(                                                       
            'keep *_wmnPrunedGenParticles_*_*', 
            'keep *_wmnCands_*_*'                                                  

      )
)

# Output
process.wmnOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      , SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('path')
      )
      , fileName = cms.untracked.string('EdmWmnTreeMCMINUSShort.root')
)

# End path
process.outpath = cms.EndPath(process.wmnOutputModule)
