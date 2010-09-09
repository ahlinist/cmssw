## September 8, 2010, veverka@caltech.edu
import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.trigTools import *

def embedTriggerMatches(process, hltPaths):
  """
  Defines default PATTriggerMatchers for all combinations of target collections
  (like cleanPatMuons) and hlt paths (like HLT_Mu9 and HLT_Mu11) defined by the hltPaths
  dictionary (like {"cleanPatMuons" : ["HLT_Mu9", "HLT_Mu11"]}) and tweaks the pat
  trigger settings and sequence to embed these matches.
  """

  # Template trigger match producer
  triggerMatchTemplate = cms.EDFilter( "PATTriggerMatcherDRDPtLessByR",
    src     = cms.InputTag( "dummySource" ),
    matched = cms.InputTag( "patTrigger" ),
    andOr          = cms.bool( False ),
    filterIdsEnum  = cms.vstring( "*" ),
    filterIds      = cms.vint32( 0 ),
    filterLabels   = cms.vstring( "*" ),
    pathNames      = cms.vstring( "dummyPathName" ),
    collectionTags = cms.vstring( "*" ),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.5 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True )
    )

  switchOnTriggerMatchEmbedding(process)

  process.patTriggerEvent.patTriggerMatches = cms.VInputTag()
  process.patTriggerMatcher                 = cms.Sequence()
  process.patTriggerMatchEmbedder           = cms.Sequence()

  for target in hltPaths.keys():
    ## Get the target trigger match name, e.g. cleanPatMuonsTriggerMatch for target = cleanPatMuons.
    ##+ This should be defined by the tool switchOnTriggerMatchEmbedding
    targetTriggerMatchName = target + "TriggerMatch"
    targetTriggerMatch = getattr(process, targetTriggerMatchName)
    if not targetTriggerMatch:
      raise RuntimeError, "%s not defined." % targetTriggerMatchName
    targetTriggerMatch.matches = cms.VInputTag()
    for path in hltPaths[target]:
      ## Get the module name, e.g. "cleanPatMuonsTriggerMatch_HLT_Mu9"
      ##+ for target = "cleanPatMuons" and path = "HLT_Mu9"
      moduleName = target + "TriggerMatch_" + path
      setattr(process,
        moduleName,
        triggerMatchTemplate.clone(
          src = target,
          pathNames = [path]
          )
        )
      module = getattr(process, moduleName)
      if not module:
        raise RuntimeError, "Failed to create `%s'." % moduleName
      process.patTriggerEvent.patTriggerMatches.append(moduleName)
      targetTriggerMatch.matches.append(moduleName)
      process.patTriggerMatcher += module
    # for path in hltPaths[target]: <-------------------------------------------------
    process.patTriggerMatchEmbedder += targetTriggerMatch
  # for target in hltPaths.keys(): <--------------------------------------------------
# def embedTriggerMatches(process, hltPaths): <---------------------------------------
