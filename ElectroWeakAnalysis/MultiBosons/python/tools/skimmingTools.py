## September 8, 2010, veverka@caltech.edu
import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.trigTools import *

def getTargetBaseName(name):
  ## Return e.g. baseName = "electron" for name = "cleanPatElectrons".
  ##+ Frist make all lower case and remove trailing "s".
  baseName = name.lower()
  ## Remove various PAT prefixes.
  baseName = baseName.replace("selected", "")
  baseName = baseName.replace("clean", "")
  baseName = baseName.replace("pat", "")
  baseName = baseName.replace("pf", "")
  baseName = baseName.replace("tc", "")
  baseName = baseName.rstrip("s")
  if baseName == "met": 
    baseName = "MET"
  else:
    baseName = baseName.title()
  return baseName


def embedTriggerMatches(process, hltPaths):
  """
  Defines default PATTriggerMatchers for all combinations of target collections
  (like cleanPatMuons) and hlt paths (like HLT_Mu9 and HLT_Mu11) defined by the hltPaths
  dictionary (like {"cleanPatMuons" : ["HLT_Mu9", "HLT_Mu11"]}) and tweaks the pat
  trigger settings and sequence to embed these matches.
  """

  # Template trigger match producer
  triggerMatchTemplate = cms.EDProducer( "PATTriggerMatcherDRLessByR",
    src     = cms.InputTag( "dummySource" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( "*" ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( "*" ),
    pathNames      = cms.vstring( "dummyPathName" ),
    collectionTags = cms.vstring( "*" ),
    #maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.3 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True )
    )

  ## Initialize relevant workflow items
  process.patTriggerEvent.patTriggerMatches = cms.VInputTag()
  if hasattr(process, "patTriggerMatcher"):
    delattr(process, "patTriggerMatcher")
  if hasattr(process, "patTriggerMatchEmbedder"):
    delattr(process, "patTriggerMatchEmbedder")

  for target in hltPaths.keys():
    ## Get the target base name, e.g. "muon" for target = "cleanPatMuons"
    baseName = getTargetBaseName(target)
    ## Get the target trigger match label, e.g. "cleanPatMuonsTriggerMatch"
    triggerMatchLabel = target + "TriggerMatch"
    ## Get the target trigger match module name, e.g. "PATTriggerMatchMuonEmbedder"
    triggerMatchModule = "PATTriggerMatch" + baseName + "Embedder"
    ## Attach the target trigger match module to the process, e.g.
    ##+ process.cleanPatMuonsTriggerMatch = cms.EDProducer("PATTriggerMatchMuonEmbedder",
    ##+   matches = cms.VInputTag(),
    ##+   src = cms.InputTag("cleanPatMuons")
    ##+   )
    setattr(process,
      triggerMatchLabel,
      cms.EDProducer(triggerMatchModule,
        matches = cms.VInputTag(),
        src = cms.InputTag(target),
        )
      )
    targetTriggerMatch = getattr(process, triggerMatchLabel)
    for path in hltPaths[target]:
      ## Get the module name, e.g. "cleanPatMuonsTriggerMatchHLTMu9"
      ##+ for target = "cleanPatMuons" and path = "HLT_Mu9"
      moduleLabel = baseName.lower() + "TriggerMatch" + path.replace("_", "")
      setattr(process,
        moduleLabel,
        triggerMatchTemplate.clone(
          src = target,
          pathNames = [path]
          )
        )
      module = getattr(process, moduleLabel)
      process.patTriggerEvent.patTriggerMatches.append(moduleLabel)
      targetTriggerMatch.matches.append(moduleLabel)
      if not hasattr(process, "patTriggerMatcher"):
        process.patTriggerMatcher = cms.Sequence(module)
      else:
        process.patTriggerMatcher += module
    # for path in hltPaths[target]: <-------------------------------------------------
    if not hasattr(process, "patTriggerMatchEmbedder"):
      process.patTriggerMatchEmbedder = cms.Sequence(targetTriggerMatch)
    else:
      process.patTriggerMatchEmbedder += targetTriggerMatch
  # for target in hltPaths.keys(): <--------------------------------------------------
  process.patTriggerSequence = cms.Sequence(
    process.patTrigger *
    process.patTriggerMatcher *
    process.patTriggerEvent *
    process.patTriggerMatchEmbedder
  )
# def embedTriggerMatches(process, hltPaths): <---------------------------------------

def addPhotonReReco(process):
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.MagneticField_38T_cff')
    process.load('Configuration.StandardSequences.Geometry_cff')
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
    
    process.photonReReco = cms.Sequence(process.conversionSequence*
                                        process.trackerOnlyConversionSequence*
                                        process.photonSequence*
                                        process.photonIDSequence)
    #edit the pat sequence to do the rereco
    process.patDefaultSequence = cms.Sequence(process.photonReReco*process.patDefaultSequence)
# def addPhotonReReco(process): <-----------------------------------------------------
