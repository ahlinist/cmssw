## V-gamma Z(->mumu)gamma subskim
## Based on ElectroWeakAnalysis/Skimming/test/EWK_ZMuMuSubskim.py
## and PhysicsTools/PatAlgos/python/patTemplate_cfg.py

import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.ZMuMuGammaSubskim_cfi import *

## Manipulate the PAT sequences here
from PhysicsTools.PatAlgos.tools.coreTools import *
# removeAllPATObjectsBut(process, ['Photons'])
removeSpecificPATObjects(process, ['Muons'])
removeCleaning(process)

## Define the PAT default path
process.patPath = cms.Path(process.patDefaultSequence)

## Define the V-gamma paths
process.MuMuGammasPath = process.dimuonsPath.copy()
process.MuMuGammasPath += process.patDefaultSequence
process.MuMuGammasPath += process.MuMuGammasSequence

process.MuMuGammasOneTrackPath = process.dimuonsOneTrackPath.copy()
process.MuMuGammasOneTrackPath += process.patDefaultSequence
process.MuMuGammasOneTrackPath += process.MuMuGammasOneTrackSequence

## Manipulate the output commands
## Add the zMuMuSubskim output commands (they may have been deleted by
## PAT tools).
## Add gen particles and extra photon related collections
from ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent import *
eventContent = vgExtraPhotonEventContent +\
  vgMuMuGammaEventContent +\
  vgExtraAnalyzerKitEventContent

process.out.outputCommands = (zMuMuSubskimOutputModule.outputCommands +
  process.out.outputCommands +
  cms.untracked.vstring('keep recoGenParticles_genParticles_*_*',
    *eventContent
  )
)

## Some additional things you may want to change
# process.GlobalTag.globaltag = 'START3X_V26::All'
process.options.wantSummary = False        ##  (to suppress the long output at the end of the job)

relvalPath = '/castor/cern.ch/cms/store/relval/CMSSW_3_5_7/RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012'
fileList = ['10B71379-4549-DF11-9D80-003048D15D22.root',
  '34FD3B1D-6949-DF11-9529-0018F3D09612.root',
  '4C8D7358-4449-DF11-86BB-003048678A6C.root',
  'B0EF52E8-4549-DF11-91D4-003048678AE4.root',
  'E0784FDE-4449-DF11-AA23-003048678C9A.root',
  'F6369161-4749-DF11-8D77-003048678B8E.root',
]
process.source.fileNames = ['rfio:' + relvalPath + '/' + file for file in fileList]

process.source.skipEvents = cms.untracked.uint32(0)
process.maxEvents.input = 100
process.out.fileName = 'testZMuMuGammaSubskim.root'

#process.maxEvents = cms.untracked.PSet(output = cms.untracked.int32(1000) )
process.countPatPhotons.minNumber = 1
process.MuMuGammasCountFilter.minNumber = 0
# process.MuMuGammasOneTrackCountFilter.minNumber = 1


process.vgMuons = cms.EDProducer("ZMuMuAdapter",
# process.vgMuons = cms.EDProducer("CandViewShallowCloneUncombiner",
  #src = cms.InputTag("MuMuGammas")
  src = cms.InputTag("dimuonsGlobal")
)

#process.vgMuonsPath = process.dimuonsPath.copy()
process.vgMuonsPath = process.MuMuGammasPath.copy()
process.vgMuonsPath += process.vgMuons
process.out.outputCommands += cms.untracked.vstring('keep *_vgMuons_*_*')

## only store events passing the vgMuonsPath
process.out.SelectEvents.SelectEvents = cms.vstring("vgMuonsPath")

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


