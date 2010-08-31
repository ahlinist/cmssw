###############################################################################
## This is the VGamma PAT configuration + Skim for Spring10 MC full sim samples
###############################################################################

## TO-DO List
## * Prune gen particles (done)
## * Check event size and reduce it (partially)
## * Do we want to use VBTF WLNu candidates instead of ours leptonPlusMETs?
##   They have the acop method.
## * Add VBTF electron ID working points
## * Embed photon showershape variables as user floats (done)

import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent as vgEventContent

from PhysicsTools.PatAlgos.patTemplate_cfg import * ## Default PAT process skeleton
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
from ElectroWeakAnalysis.MultiBosons.Skimming.egammaUserDataProducts_cff import *
from ElectroWeakAnalysis.MultiBosons.Skimming.jobOptions import *
from ElectroWeakAnalysis.MultiBosons.Skimming.options import *

## See link below for the definition of the selection
## https://twiki.cern.ch/twiki/bin/view/CMS/VGammaFirstPaper#Vgamma_Group_skims
skimVersion = 3  # Do we need this?
basePath = "ElectroWeakAnalysis.MultiBosons.Skimming." # shorthand

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

## Remove MC matching and apply cleaning if we run on data
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

## Keep only global and tracker muons
process.selectedPatMuons.cut = "isGlobalMuon | isTrackerMuon"

## Reject soft jets to reduce event content
process.selectedPatJets.cut = "pt > 30"

## No overlap of photons and electrons
process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = True

## Add photon user data
process.load(basePath + "photonUserData_cfi")
process.patDefaultSequence.replace(process.patPhotons,
  process.photonUserData * process.patPhotons
  )
process.patPhotons.userData.userFloats.src = egammaUserDataFloats(
  moduleName = "photonUserData"
  )
process.patPhotons.userData.userInts.src = egammaUserDataInts(
  moduleName = "photonUserData"
  )

## Add electron user data
process.load(basePath + "electronUserData_cfi")
process.patDefaultSequence.replace(process.patElectrons,
  process.electronUserData * process.patElectrons
  )
process.patElectrons.userData.userFloats.src = egammaUserDataFloats(
  moduleName = "electronUserData"
  )
process.patElectrons.userData.userInts.src = egammaUserDataInts(
  moduleName = "electronUserData"
  )

## PAT Trigger
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.load(basePath + "muonTriggerMatchHLTMuons_cfi")
switchOnTrigger(process)
switchOnTriggerMatchEmbedding(process)

process.patTriggerEvent.patTriggerMatches = [ "muonTriggerMatchHLTMuons" ]
process.cleanPatMuonsTriggerMatch.matches = [ "muonTriggerMatchHLTMuons" ]
process.patTriggerMatcher = cms.Sequence(process.muonTriggerMatchHLTMuons)
process.patTriggerMatchEmbedder = cms.Sequence(
  process.cleanPatMuonsTriggerMatch
  )

## Define Paths
process.load(basePath + "VGammaSkimSequences_cff")

if options.isRealData:
  ## Add cleaning for no scraping events etc.
  process.load(basePath + "goodCollisionDataSequence_cff")
  process.defaultSequence = cms.Sequence(
    process.goodCollisionDataSequence +
    process.patDefaultSequence
  )
else:
  process.load(basePath + "prunedGenParticles_cfi")
  process.defaultSequence = cms.Sequence(
    process.prunedGenParticles *
    process.patDefaultSequence
  )
  ## Add parton shower related filters (prevent ISR/FSR double-counting)
  process.isLeadingPhotonPythiaPartonShowerIsr = cms.Path(
    process.patDefaultSequence *
    process.pythiaPartonShowerIsrSequence
  )
  process.isLeadingPhotonPythiaPartonShowerFsr = cms.Path(
    process.patDefaultSequence *
    process.pythiaPartonShowerFsrSequence
  )
  process.hasPhotonCandidateNotPythiaPartonShower = cms.Path(
    process.patDefaultSequence *
    process.pythiaPartonShowerPhotonVeto
  )
# if options.isRealData <-----------------------------------------------------

process.WENuGammaPath  = cms.Path(
  process.defaultSequence * process.WENuGammaSequence
  )
process.WMuNuGammaPath = cms.Path(
  process.defaultSequence * process.WMuNuGammaSequence
  )
process.ZEEGammaPath   = cms.Path(
  process.defaultSequence * process.ZEEGammaSequence
  )
process.ZMuMuGammaPath = cms.Path(
  process.defaultSequence * process.ZMuMuGammaSequence
  )
process.ZInvisibleGammaPath = cms.Path(
  process.defaultSequence * process.ZInvisibleGammaSequence
  )

## HLT trigger
process.hltFilter.HLTPaths = options.hltPaths

## Add VGamma event content
process.out.outputCommands += vgEventContent.vgCandsEventContent
process.out.outputCommands += vgEventContent.extraConversionEventContent
process.out.outputCommands += ["keep *_TriggerResults_*_PAT"]
process.out.outputCommands += ["drop *_cleanPatMuons_*_PAT"]
if not options.isRealData:
  process.out.outputCommands += ["keep *_prunedGenParticles_*_PAT"]
process.out.SelectEvents.SelectEvents = ["WMuNuGammaPath"]
process.out.fileName = options.outputFile
process.options.wantSummary = options.wantSummary

## Add tab completion + history during inspection
if __name__ == "__main__": import user
