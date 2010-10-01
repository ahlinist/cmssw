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
            #"rfio:/castor/cern.ch/..."
            #"file:/MyMCOnDisk/..."
),
      inputCommands = cms.untracked.vstring(
            'keep *',
            'drop *_MEtoEDMConverter_*_*',
            'drop *_lumiProducer_*_*'
      )
)

## Files in a directory on disk
#file_directory = "/ciet3b/data4/Summer10_10invpb_AODSIM/ZmumuPOWHEG"
file_directory = "/ciet3b/data4/Summer10_All_MinimalAOD/ZmumuPOWHEG"
process.source.fileNames = cms.untracked.vstring()
import os
for file in os.listdir(file_directory):
         process.source.fileNames.append("file:" + file_directory + "/" + file)

# Events processed
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# Prune the generator collection
process.zmmPrunedGenParticles = cms.EDProducer(
    "GenParticlePruner",
    #src = cms.InputTag("genParticles"),
    src = cms.InputTag("prunedGenParticles"),
    select = cms.vstring(
        "drop *"
      , "keep++ pdgId = 23"
    )
)

process.goodZToMuMuEdmNtuple = cms.EDProducer(
    "CandViewNtpProducer", 
    src = cms.InputTag("zmmCands"),
    lazyParser = cms.untracked.bool(True),
    variables = cms.VPSet(
    cms.PSet(
    tag = cms.untracked.string("mu1STApt"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STApx"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.px")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STApy"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.py")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STApz"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.pz")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STAp"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.p")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STAphi"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.phi")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu1STAeta"),
    quantity = cms.untracked.string("daughter(0).masterClone.outerTrack.eta")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STApt"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STApx"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.px")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STApy"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.py")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STApz"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.pz")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STAp"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.p")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STAphi"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.phi")
    ),
    cms.PSet(
    tag = cms.untracked.string("mu2STAeta"),
    quantity = cms.untracked.string("daughter(1).masterClone.outerTrack.eta")
    )

    )  
 )

# Path
process.path = cms.Path(
      process.zmmPrunedGenParticles *
      process.goldenZMMSelectionSequence *
      process.goodZToMuMuEdmNtuple
)

# Write a customized output
process.myEventContent = cms.PSet(outputCommands=cms.untracked.vstring('drop *'))
process.myEventContent.outputCommands.extend(
      cms.untracked.vstring(
            'keep *_zmmCands_*_*'
          , 'keep *_zmmPrunedGenParticles_*_*',
            'keep *_goodZToMuMuEdmNtuple_*_*'
      )
)

# Output
process.zmmOutputModule = cms.OutputModule("PoolOutputModule"
      , process.myEventContent
      , dropMetaDataForDroppedData = cms.untracked.bool(True)
      , SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('path')
      )
      , fileName = cms.untracked.string('EdmZmmTreeMC_STA.root')
)

# End path
process.outpath = cms.EndPath(process.zmmOutputModule)
