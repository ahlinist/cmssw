## V-gamma Z(->mumu)gamma subskim + Vg Ntupelizer
## Based on
##   ElectroWeakAnalysis/Skimming/test/EWK_ZMuMuSubskim.py,
##   PhysicsTools/PatAlgos/python/patTemplate_cfg.py and
##   ElectroWeakAnalysis/MultiBosons/test/Skimming/ZMuMuGammaSubskim.py

import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.ZMuMuGammaSubskimMC_cfi import *

## Manipulate the PAT sequences here
from PhysicsTools.PatAlgos.tools.coreTools import *
#removeAllPATObjectsBut(process, ['Photons'])
#removeCleaning(process)

## Define the PAT default path
process.patPath = cms.Path(process.patDefaultSequence)

## Load the Zmumu adapter module
process.load("ElectroWeakAnalysis.MultiBosons.ZMuMuAdapter_cfi")

## Load the VgAnalyzerKit module
process.load("ElectroWeakAnalysis.MultiBosons.VgAnalyzerKit_cfi")
process.VgAnalyzerKit.muonSrc   = "vgMuonsGlobal"

## Define the V-gamma path
process.vgDimuonsPath = process.dimuonsPath.copy()
process.vgDimuonsPath += process.patDefaultSequence
process.vgDimuonsPath *= process.MuMuGammasGlobalSequence
process.vgDimuonsPath *= process.vgMuonsGlobal
process.vgDimuonsPath *= process.VgAnalyzerKit

## Manipulate the output commands
## Add the zMuMuSubskim output commands (they may have been deleted by
## PAT tools).
## Add gen particles and extra photon related collections
#from ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent import *
#eventContent = vgExtraPhotonEventContent +\
  #vgMuMuGammaEventContent +\
  #vgExtraAnalyzerKitEventContent

#process.out.outputCommands = (zMuMuSubskimOutputModule.outputCommands +
  #process.out.outputCommands +
  #cms.untracked.vstring('keep recoGenParticles_genParticles_*_*',
    #*eventContent
  #)
#)

## Configure the output to store only the provenance in the edm pool file
process.out.outputCommands = ["drop *"]
process.out.SelectEvents.SelectEvents = ["vgDimuonsPath"]
process.out.fileName = "vgtree_ZMuMuAdapter_Provenance.root"

process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree_ZMuMuAdapter.root'))

## Some additional things you may want to change
# process.GlobalTag.globaltag = 'START3X_V26::All'
process.options.wantSummary = False        ##  (to suppress the long output at the end of the job)

## Use 357 Zmumu relval sample
relvalPath = '/store/relval/CMSSW_3_5_7/RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012'
fileList = ['10B71379-4549-DF11-9D80-003048D15D22.root', '34FD3B1D-6949-DF11-9529-0018F3D09612.root']
process.source.fileNames = [relvalPath + '/' + file for file in fileList]

process.source.skipEvents = cms.untracked.uint32(0)
#process.maxEvents.input = 20
process.maxEvents = cms.untracked.PSet(output = cms.untracked.int32(10) )

# process.countPatPhotons.minNumber = 1  # 1: rejects events w/o photons
# process.MuMuGammasCountFilter.minNumber = 0 # 1: rejects events w/o dimuons
# process.MuMuGammasGlobalCountFilter.minNumber = 1 # 1: rejects events w/o dimuons
# process.MuMuGammasOneTrackCountFilter.minNumber = 1

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


