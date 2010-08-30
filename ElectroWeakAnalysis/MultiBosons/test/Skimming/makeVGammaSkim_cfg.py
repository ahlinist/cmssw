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
import ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent as vgEventContent

from PhysicsTools.PatAlgos.patTemplate_cfg import * ## Default PAT process skeleton
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
from ElectroWeakAnalysis.MultiBosons.Skimming.options import *
from ElectroWeakAnalysis.MultiBosons.Skimming.jobOptions import *

## See link below for the definition of the selection
## https://twiki.cern.ch/twiki/bin/view/CMS/VGammaFirstPaper#Vgamma_Group_skims
skimVersion = 1

## Define default options specific to this configuration file
options.jobType = "testSpring10McCern"
# options.jobType = "testPromptRecoV4Cern"

## Parse (command-line) arguments - this overrides the options given above
options.parseArguments()
applyJobOptions(options)

## Input
process.source.fileNames = options.inputFiles
if options.isMaxEventsOutput:
  process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(options.maxEvents)
    )
else:
  process.maxEvents.input = options.maxEvents
process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

## Global tag
process.GlobalTag.globaltag = options.globalTag

## Remove MC matching if we run on data
## (No need to remove pfMET and tcMET explicitly if this is done first
if options.isRealData:
  removeMCMatching(process)

## Add non-default MET flavors
addPfMET(process , 'PF')
addTcMET(process , 'TC')

## Spring10 MC was produced with CMSS_3_5_6 - make sure we can run on it
if options.use35XInput:
  run36xOn35xInput(process, genJets = "ak5GenJets")

## Embed lepton tracks
process.patMuons.embedTrack = True
process.patElectrons.embedTrack = True

## Add photon cluster shape user data
# process.load(
#   "ElectroWeakAnalysis.MultiBosons.Skimming.patPhotonClusterShape_cff"
#   )


## Keep only global and tracker muons
process.selectedPatMuons.cut = "isGlobalMuon | isTrackerMuon"

## No overlap of phtons and electrons
process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = True

## PAT Trigger
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.muonTriggerMatchHLTMuons_cfi")
process.patTriggerMatcher += process.muonTriggerMatchHLTMuons
process.patTriggerMatcher.remove( process.patTriggerMatcherElectron )
process.patTriggerMatcher.remove( process.patTriggerMatcherMuon )
process.patTriggerMatcher.remove( process.patTriggerMatcherTau )
process.patTriggerEvent.patTriggerMatches = [ "muonTriggerMatchHLTMuons" ]
switchOnTrigger(process)
switchOnTriggerMatchEmbedding(process)

## Define VGamma Paths
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.VGammaSkimSequences_cff")
process.WENuGammaPath  = cms.Path(process.patDefaultSequence * process.WENuGammaSequence)
process.WMuNuGammaPath = cms.Path(process.patDefaultSequence * process.WMuNuGammaSequence)
process.ZEEGammaPath   = cms.Path(process.patDefaultSequence * process.ZEEGammaSequence)
process.ZMuMuGammaPath = cms.Path(process.patDefaultSequence * process.ZMuMuGammaSequence)
process.ZInvisibleGammaPath = cms.Path(process.patDefaultSequence * process.ZInvisibleGammaSequence)


## Add VGamma event content
# process.out.outputCommands += vgEventContent.vgExtraPhotonEventContent
# process.out.outputCommands += vgEventContent.vgExtraAnalyzerKitEventContent
process.out.outputCommands += vgEventContent.vgCandsEventContent
process.out.outputCommands += ["keep *_TriggerResults_*_PAT"]
process.out.SelectEvents.SelectEvents = ["WMuNuGammaPath"]
process.out.fileName = options.outputFile
process.options.wantSummary = options.wantSummary

## Add tab completion + history during inspection
if __name__ == "__main__": import user
