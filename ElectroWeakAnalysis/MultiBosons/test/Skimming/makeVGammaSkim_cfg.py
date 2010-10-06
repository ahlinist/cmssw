###############################################################################
## This is the VGamma PAT configuration + Skim for Spring10 MC full sim samples
###############################################################################

## TO-DO List
## * Do we want to use VBTF WLNu candidates instead of ours leptonPlusMETs?
##   They have the acop method.

import copy
import FWCore.ParameterSet.Config as cms
import ElectroWeakAnalysis.MultiBosons.Skimming.VgEventContent as vgEventContent

from PhysicsTools.PatAlgos.patTemplate_cfg import * ## Default PAT process skeleton
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
from ElectroWeakAnalysis.MultiBosons.Skimming.egammaUserDataProducts_cff import *
from ElectroWeakAnalysis.MultiBosons.Skimming.jobOptions import *
from ElectroWeakAnalysis.MultiBosons.Skimming.options import options as defaultOptions
from ElectroWeakAnalysis.MultiBosons.tools.skimmingTools import embedTriggerMatches

## See link below for the definition of the selection
## https://twiki.cern.ch/twiki/bin/view/CMS/VGammaFirstPaper#Vgamma_Group_skims
skimVersion = 3  # Do we need this?
basePath = "ElectroWeakAnalysis.MultiBosons.Skimming." # shorthand

options = copy.deepcopy(defaultOptions)

## Define default options specific to this configuration file
options.jobType = "testSummer10"
#options.jobType = "testMC"
#options.jobType = "testRealData"

## Parse (command-line) arguments - this overrides the options given above
# options.parseArguments()
applyJobOptions(options)

## Input
process.source.fileNames = options.inputFiles
if options.isMaxEventsOutput:
  process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(options.maxEvents)
    )
else:
  process.maxEvents.input = options.maxEvents

## Global tag
process.GlobalTag.globaltag = options.globalTag

## HLT trigger
process.load(basePath + "hltFilter_cfi")
process.hltFilter.HLTPaths = options.hltPaths
process.hltFilter.TriggerResultsTag = \
  "TriggerResults::" + options.hltProcessName

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
process.selectedPatMuons.cut = cms.string("isGlobalMuon | isTrackerMuon")

## Reject soft jets to reduce event content
process.selectedPatJets.cut = cms.string("pt > 30")

## Overlap of photons and electrons
##+ Set to True to remove overlapping photons from the event, False to embed
##+ embed the overlap information
process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = False

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

process.load("RecoEcal.EgammaClusterProducers.preshowerClusterShape_cfi")
process.load("EgammaAnalysis.PhotonIDProducers.piZeroDiscriminators_cfi")

#process.piZeroDiscriminators.preshClusterShapeProducer = "multi5x5PreshowerClusterShape"
#process.piZeroDiscriminators.preshClusterShapeCollectionX = "multi5x5PreshowerXClustersShape"
#process.piZeroDiscriminators.preshClusterShapeCollectionY = "multi5x5PreshowerYClustersShape"

# process.eca = cms.EDAnalyzer("EventContentAnalyzer")
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.pi0Discriminator_cfi")
process.patDefaultSequence.replace(process.patPhotons,
  process.preshowerClusterShape *
  process.piZeroDiscriminators  *
  process.pi0Discriminator      *
  process.patPhotons
  )
process.patPhotons.userData.userFloats.src.append(
  cms.InputTag("pi0Discriminator") #, "piZeroDiscriminatorsPhotonPi0DiscriminatorAssociationMap")
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

## Add electron official electron ID from the Egamma / VBTF
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
process.patDefaultSequence.replace(process.patElectrons,
  process.simpleEleIdSequence * process.patElectrons
  )
process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
  simpleEleId95relIso = cms.InputTag("simpleEleId95relIso"),
  simpleEleId90relIso = cms.InputTag("simpleEleId90relIso"),
  simpleEleId85relIso = cms.InputTag("simpleEleId85relIso"),
  simpleEleId80relIso = cms.InputTag("simpleEleId80relIso"),
  simpleEleId70relIso = cms.InputTag("simpleEleId70relIso"),
  simpleEleId60relIso = cms.InputTag("simpleEleId60relIso"),
  simpleEleId95cIso   = cms.InputTag("simpleEleId95cIso"),
  simpleEleId90cIso   = cms.InputTag("simpleEleId90cIso"),
  simpleEleId85cIso   = cms.InputTag("simpleEleId85cIso"),
  simpleEleId80cIso   = cms.InputTag("simpleEleId80cIso"),
  simpleEleId70cIso   = cms.InputTag("simpleEleId70cIso"),
  simpleEleId60cIso   = cms.InputTag("simpleEleId60cIso"),
  )

## PAT Trigger
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
switchOnTrigger(process)
process.patTrigger.processName = options.hltProcessName
process.patTriggerEvent.processName = options.hltProcessName
matchHltPaths = {
  "cleanPatElectrons": options.electronTriggerMatchPaths,
  "cleanPatMuons"    : options.muonTriggerMatchPaths,
  "cleanPatTaus"     : options.tauTriggerMatchPaths,
  "cleanPatPhotons"  : options.photonTriggerMatchPaths,
  "cleanPatJets"     : options.jetTriggerMatchPaths,
  "patMETs"          : options.metTriggerMatchPaths,
  "patMETsPF"        : options.metTriggerMatchPaths,
  "patMETsTC"        : options.metTriggerMatchPaths,
  }
embedTriggerMatches(process, matchHltPaths)
## Drop matched target collections from the event content to only keep the trigger matched versions
for collection in matchHltPaths.keys():
  vgEventContent.extraSkimEventContent.append("drop *_%s_*_*" % collection)

## Define Paths
process.skimFilterSequence = cms.Sequence(process.hltFilter)

if options.skimType == "MuonPhoton":
  process.load(basePath + "muonPhotonSkimFilterSequence_cff")
  process.skimFilterSequence += process.muonPhotonSkimFilterSequence
elif options.skimType == "ElectronPhoton":
  process.load(basePath + "electronPhotonSkimFilterSequence_cff")
  process.skimFilterSequence += process.electronPhotonSkimFilterSequence

process.load(basePath + "VGammaSkimSequences_cff")

if options.isRealData:
  ## Add cleaning of collision data (no scraping events etc.)
  ##+ https://twiki.cern.ch/twiki/bin/viewauth/CMS/Collisions2010Recipes
  process.load(basePath + "goodCollisionDataSequence_cff")
  ## Remove the hltPhysicsDeclared - it kills some good events, reference?
  process.goodCollisionDataSequence.remove("hltPhysicsDeclared")
  ## Run the hltPhysicsDeclared filter in a separate path to
  ##+ store its result in the triggerEvent product.
  process.hltPhysicsDeclaredPath = cms.Path(process.hltPhysicsDeclared)
  process.defaultSequence = cms.Sequence(
    process.goodCollisionDataSequence +
    process.skimFilterSequence +
    process.patDefaultSequence
  )
else:
  process.load(basePath + "prunedGenParticles_cfi")
  process.defaultSequence = cms.Sequence(
    process.skimFilterSequence +
    process.prunedGenParticles *
    process.patDefaultSequence
  )
  ## Add parton shower related filters (prevent ISR/FSR double-counting)
  process.isLeadingPhotonPythiaPartonShowerIsr = cms.Path(
    process.defaultSequence *
    process.pythiaPartonShowerIsrSequence
  )
  process.isLeadingPhotonPythiaPartonShowerFsr = cms.Path(
    process.defaultSequence *
    process.pythiaPartonShowerFsrSequence
  )
  process.hasPhotonCandidateNotPythiaPartonShower = cms.Path(
    process.defaultSequence *
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

## Output configuration (add event content, select events, output file name)
process.out.outputCommands += vgEventContent.extraSkimEventContent

if not options.isRealData:
  process.out.outputCommands += ["keep *_prunedGenParticles_*_PAT"]

if options.skimType == "MuonPhoton":
  process.out.SelectEvents.SelectEvents = ["WMuNuGammaPath"]
elif options.skimType == "ElectronPhoton":
  process.out.SelectEvents.SelectEvents = ["WENuGammaPath"]
else:
  raise RuntimeError, "Illegal skimType option: %s" % options.skimType
# if options.skimType == ... <------------------------------------------------

process.out.fileName = options.outputFile

## Logging
process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery
if not options.isRealData:
  ## Suppress many warnings about missing HLT prescale tables
  process.MessageLogger.categories += ["hltPrescaleTable"]
  process.MessageLogger.cerr.hltPrescaleTable = cms.untracked.PSet(
    limit = cms.untracked.int32(5)
    )


process.options.wantSummary = options.wantSummary

## Relax the pt for the tests on data
if options.jobType == "testRealData":
  process.WMuNuGammaPath.remove(process.hltFilter)
  process.muonPlusMETFilter.cut = cms.string("daughter('lepton').pt > 3")
  process.WMuNuGammaPath.remove(process.WENuGammaFilter)

if options.jobType == "testSummer10":
#   process.WENuGammaPath.remove(process.hltFilter)
#   process.WENuGammaPath.remove(process.electronPlusMETFilter)
#   process.WENuGammaPath.remove(process.WENuGammaFilter)
    pass

## Add tab completion + history during inspection
if __name__ == "__main__": import user
