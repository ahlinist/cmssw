###############################################################################
## This is the VGamma PAT configuration + Skim for Spring10 MC full sim samples
###############################################################################

## TO-DO List
## * Prune gen particles
## * Check event size and reduce it
## * Do we want to use VBTF WLNu candidates instead of ours leptonPlusMETs?
##   They have the acop method.
## * Add VBTF electron ID working points
## * Embed photon showershape variables as user floats


import FWCore.ParameterSet.Config as cms

## Import default PAT process skeleton
from PhysicsTools.PatAlgos.patTemplate_cfg import *

## Spring10 MC was produced with CMSS_3_5_6 - make sure we can run on it
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run36xOn35xInput(process, genJets = "ak5GenJets")

## Add non-default MET flavors
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process , 'PF')
addTcMET(process , 'TC')

## For consistency with the VgAnalyzerKit
process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = False

## Define VGamma Paths
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.VGammaSkimSequences_cff")
process.WENuGammaPath  = cms.Path(process.patDefaultSequence * process.WENuGammaSequence)
process.WMuNuGammaPath = cms.Path(process.patDefaultSequence * process.WMuNuGammaSequence)
process.ZEEGammaPath   = cms.Path(process.patDefaultSequence * process.ZEEGammaSequence)
process.ZMuMuGammaPath = cms.Path(process.patDefaultSequence * process.ZMuMuGammaSequence)
process.ZInvisibleGammaPath = cms.Path(process.patDefaultSequence * process.ZInvisibleGammaSequence)

## PAT Trigger
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger(process)
switchOnTriggerMatchEmbedding(process)

from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process)
removeMCMatching(process,names=['METs'],postfix="TC")
removeMCMatching(process,names=['METs'],postfix="PF")

## This is the global tag for the Spring10 MC
process.GlobalTag.globaltag = cms.string('GR_R_36X_V12B::All')

## Input
#fileNamePrefix = "rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_5_7/RelValZMM/GEN-SIM-RECO/START3X_V26-v1/0012/"
fileNamePrefix = "/store/data/Run2010A/Mu/RECO/Jun14thReReco_v1/0006/"
#fileList = """
#10B71379-4549-DF11-9D80-003048D15D22.root
#34FD3B1D-6949-DF11-9529-0018F3D09612.root
#4C8D7358-4449-DF11-86BB-003048678A6C.root
#B0EF52E8-4549-DF11-91D4-003048678AE4.root
#E0784FDE-4449-DF11-AA23-003048678C9A.root
#F6369161-4749-DF11-8D77-003048678B8E.root
#""".split()
fileList = """
B0222EAB-B07B-DF11-AE0A-001E0B5FE542.root
""".split()
process.source.fileNames = [fileNamePrefix + file for file in fileList]

process.maxEvents.input = -1
# process.maxEvents = cms.untracked.PSet(output = cms.untracked.int32(100))
# process.MessageLogger.cerr.FwkReport.reportEvery = 1000


## Add VGamma event content and customize file name
import ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent as vgEventContent
# process.out.outputCommands += vgEventContent.vgExtraPhotonEventContent
# process.out.outputCommands += vgEventContent.vgExtraAnalyzerKitEventContent
process.out.outputCommands += vgEventContent.vgCandsEventContent
process.out.outputCommands += ["keep *_TriggerResults_*_PAT"]
process.out.SelectEvents.SelectEvents = ["*Path"]

process.out.fileName = "VGammaSkim_v2.root"

process.patMuons.embedTrack = True
process.patElectrons.embedTrack = True

process.options.wantSummary = True

## Add tab completion + history during inspection
if __name__ == "__main__": import user
