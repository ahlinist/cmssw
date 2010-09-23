import FWCore.ParameterSet.Config as cms

process = cms.Process("ZMMNTupleCREATOR")

# Printouts
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(10))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ZMM selection filter and Z candidates
process.load("ElectroWeakAnalysis.ZMuMu.zmmSelection_cfi")
# For data:
process.zmmHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
# For 36X MC:
#process.zmmHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","REDIGI")

# Source
process.source = cms.Source("PoolSource", 
      fileNames = cms.untracked.vstring(
#            "rfio:///castor/cern.ch/user/d/degrutto/2010/ZCands/RECO/EWK_HighPtDiMuonSkim_SD_Mu_140_116_126_23ZCands.root"
    "file:/ciet3b/data3/GoldenZmumus/AODGoldenZmumus_132440-144114.root"
),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*'
      )
)

"""import os
file_directory = "/ciet3b/data4/Spring10_All_MinimalAOD/Zmumu_M20-powheg"
for file in os.listdir(file_directory):
         process.source.fileNames.append("file:" + file_directory + "/" + file)
"""

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# Prune the generator collection
process.zmmPrunedGenParticles = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("prunedGenParticles"),
    select = cms.vstring(
        "drop *"
      , "keep++ pdgId = 23"
      , "keep++ pdgId = 24"
      , "keep++ pdgId = -24"
    )
)

# Output histograms
#process.TFileService = cms.Service("TFileService", 
#                                   fileName = cms.string("ZMuMuNtuple.root"),
#                                   closeFileFast = cms.untracked.bool(True)
#)
                                     
# Path
process.path = cms.Path(
  process.goldenZMMSelectionSequence
#  * process.zmmPrunedGenParticles                            -----------------------------------------
 # *process.goodZToMuMuEdmNtuple                                                                      |
  )#                                                                                                  |
    #                                                                                                 |
# Write a customized output                                                                           |
process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))  #                  |--------> Uncomment these 2 lines when MC
process.myEventContent.outputCommands.extend(                                      #                  |
      cms.untracked.vstring(                                                       #                  |
            'keep *_zmmCands_*_*'                                                  #                  |
#            , 'keep *_zmmPrunedGenParticles_*_*'             -----------------------------------------
            #'keep *_goodZToMuMuEdmNtuple_*_*'               
      )
)

# Output
process.zmmOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      #, fileName = cms.untracked.string('ZMuMuCandidates.root')
      , fileName = cms.untracked.string('EdmZmmTreeDataShort.root')
)

# End path
process.outpath = cms.EndPath(process.zmmOutputModule)


# The rest is commented out
# You may want to uncomment it for saving the selected 
# vector boson candidates and dughters in standard CMSSW format
"""
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
      , fileName = cms.untracked.string('ZMuMuCandidates.root')
)

# End path
process.outpath = cms.EndPath(process.zmmOutputModule)
"""
