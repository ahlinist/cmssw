## V-gamma Z(->mumu)gamma subskim
## Based on ElectroWeakAnalysis/Skimming/test/EWK_ZMuMuSubskim.py
## and PhysicsTools/PatAlgos/python/patTemplate_cfg.py

import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.ZMuMuGammaSubskim_cfi import *

## Manipulate the PAT sequences here
from PhysicsTools.PatAlgos.tools.coreTools import *
removeAllPATObjectsBut(process, ['Photons'])
removeCleaning(process)

## Define the PAT default path
process.patPath = cms.Path(process.patDefaultSequence)

## Define the V-gamma paths
process.MuMuGammasPath = process.dimuonsPath.copy()
process.MuMuGammasPath += process.patDefaultSequence
process.MuMuGammasPath *= process.MuMuGammasSequence

process.MuMuGammasOneTrackPath = process.dimuonsOneTrackPath.copy()
process.MuMuGammasOneTrackPath += process.patDefaultSequence
process.MuMuGammasOneTrackPath *= process.MuMuGammasOneTrackSequence

## Manipulate the output commands
## Add the zMuMuSubskim output commands (they may have been deleted by
## PAT tools).
## Add gen particles and extra photon related collections
from ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent import *
process.out.outputCommands = (zMuMuSubskimOutputModule.outputCommands +
  process.out.outputCommands +
  cms.untracked.vstring('keep recoGenParticles_genParticles_*_*',
    *(vgExtraPhotonEventContent + vgMuMuGammaEventContent)
  )
)

## Some additional things you may want to change
# process.GlobalTag.globaltag = 'START3X_V26::All'
# relvalPath = '/castor/cern.ch/cms/store/relval/CMSSW_3_5_7/RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012'
# process.source.fileNames = ['rfio:' + relvalPath + '/10B71379-4549-DF11-9D80-003048D15D22.root']

# process.maxEvents.input = 10
process.maxEvents = cms.untracked.PSet(output = cms.untracked.int32(10) )
process.countPatPhotons.minNumber = 1

process.out.fileName = 'testZMuMuGammaSubskim.root'

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


