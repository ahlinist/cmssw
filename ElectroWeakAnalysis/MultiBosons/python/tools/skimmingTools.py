## September 8, 2010, veverka@caltech.edu
import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.trigTools import *

def removeTriggerPathsForAllBut(matchHltPaths, notToRemove):
    """Take a dictionary matchHltPaths of the form
    value: key = collection: HLT path list
    and remove all the HLT path lists (set value = []) for all collections
    that are not listed in 'notToRemove' which is a list of strings
    """
    for key in matchHltPaths.keys():
        if not key in notToRemove:
            matchHltPaths[key] = []


def getTargetBaseName(name):
  'Return e.g. baseName = "electron" for name = "cleanPatElectrons".'
  ## Frist make all lower case and remove trailing "s".
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
    matchedCuts = cms.string('path('')'),
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
      ## L. Gray 27.4.2011 add in change to deal with versioned triggers      
      moduleLabel = baseName.lower() + "TriggerMatch" + path.replace("_", "").replace("v*","")
      setattr(process,
        moduleLabel,
        triggerMatchTemplate.clone(
          src = target,
          pathNames = [path],
          matchedCuts = cms.string('path("'+path+'")'),
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
  # for target in hltPaths.keys(): <-------------------------------------------
  process.patTriggerSequence = cms.Sequence(
    process.patTrigger *
    process.patTriggerMatcher *
    process.patTriggerEvent *
    process.patTriggerMatchEmbedder
  )
# def embedTriggerMatches(process, hltPaths): <--------------------------------

###############################################################################
def addPhotonReReco(process):
    """Include the photon re-reco sequence in the patDefaultSequence
    See https://hypernews.cern.ch/HyperNews/CMS/get/egamma/960.html
    and
    https://hypernews.cern.ch/HyperNews/CMS/get/egamma/958/1/1/1/1/1/1/1/1/1/1/1.html
    """
    process.photonReReco = cms.Sequence(process.ckfTracksFromConversions *
                                        process.conversionSequence *
                                        process.photonSequence *
                                        process.photonIDSequence)

    # Edit the pat sequence to do the rereco
    process.patDefaultSequence = cms.Sequence(process.photonReReco*
                                              process.patDefaultSequence)
# def addPhotonReReco(process): <-----------------------------------------------------


###############################################################################
def addRhoFromFastJet(process, after):
    """Add rho from FastJet for pile-up corrected isolation"""

    ## Load and configure the producer
    process.load('RecoJets.JetProducers.kt4PFJets_cfi')
    process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6,
                                                 doRhoFastjet = True )
    process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

    ## Append it to the processing sequence after the sequence `after'
    after *= process.kt6PFJets

    ## Add ot the the output
    process.out.outputCommands.append( "keep *_*_rho_*" )
    process.out.outputCommands.append( "keep *_*_sigma_*" )
# def addRhoFromFastJet(process, before): <------------------------------------

###############################################################################
def switchOnDAVertices(process, after):
    """Switch On Deterministic Annealing Vertices"""

    ## Load and configure the producer
    process.load('RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi')
    process.offlinePrimaryVertices = process.offlinePrimaryVerticesDA.clone()
    process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
    process.offlinePrimaryVertices.TkClusParameters.vertexSize = cms.double(0.01)
    after *= process.offlinePrimaryVertices
# def switchOnDAVertices(process, after): <------------------------------------

###############################################################################
def switchOnPileupReweighting(process, after):
    """Calculate the pile-up reweighting factor from data"""
    
    ## Load and configure the producer
    process.load('ElectroWeakAnalysis.MultiBosons.Skimming.PUWeightProducer_cff')    
    after *= process.pileupweight
# def switchOnPileupReweighting(process, after): <------------------------------------

###############################################################################
#must be run after you add in all the pat sequences!
def addIsoForPU(process):
    """Calculate isolation with proper criteria given that we have pileup."""

    #properly setup pat Electron Isolation
    process.eleIsoDepositTkPU = process.eleIsoDepositTk.clone()    
    # use per-vertex track isolation for the electrons, from HGG isolation
    from ElectroWeakAnalysis.MultiBosons.Skimming.eleTrackExtractorBlocks_cfi import EleIsoTrackExtractorBlock
    process.eleIsoDepositTkPU.ExtractorPSet = EleIsoTrackExtractorBlock
    #setup isolation producers
    process.eleIsoFromDepsTkPU03 = process.eleIsoFromDepsTk.clone()
    process.eleIsoFromDepsTkPU03.deposits = cms.VPSet(cms.PSet(src = cms.InputTag("eleIsoDepositTkPU"),
                                                               deltaR = cms.double(0.3),
                                                               weight = cms.string('1'),
                                                               vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', #jurrasic
                                                                                   'Threshold(0.7)'), #this matches what is done is standard iso
                                                               skipDefaultVeto = cms.bool(True),
                                                               mode = cms.string('sum'))
                                                      )    
    process.eleIsoFromDepsTkPU04 = process.eleIsoFromDepsTkPU03.clone()
    process.eleIsoFromDepsTkPU04.deposits  = cms.VPSet(cms.PSet(src = cms.InputTag("eleIsoDepositTkPU"),
                                                                deltaR = cms.double(0.4),
                                                                weight = cms.string('1'),
                                                                vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', #jurrasic
                                                                                    'Threshold(0.7)'),
                                                                skipDefaultVeto = cms.bool(True),
                                                                mode = cms.string('sum'))
                                                       )    
    #remove veto region from electron HCAL isolation
    process.eleIsoFromDepsHcalFromTowersPU = process.eleIsoFromDepsHcalFromTowers.clone()
    process.eleIsoFromDepsHcalFromTowersPU.deposits = cms.VPSet(cms.PSet(src = cms.InputTag("eleIsoDepositHcalFromTowers"),
                                                                         deltaR = cms.double(0.4),
                                                                         weight = cms.string('1'),
                                                                         vetos = cms.vstring(), #remove the cone veto for HCAL iso
                                                                         skipDefaultVeto = cms.bool(True),
                                                                         mode = cms.string('sum'))
                                                                )

    #configure pat isolation sequence for new modules
    process.eleTrackIsoPUDR03 = cms.EDProducer("ElectronMapDoubleToFloat",
                                               collectionSrc = cms.InputTag('gsfElectrons'),
                                               valMapSrc = cms.InputTag('eleIsoFromDepsTkPU03'))
    process.eleTrackIsoPUDR04 = process.eleTrackIsoPUDR03.clone()
    process.eleTrackIsoPUDR04.valMapSrc = cms.InputTag('eleIsoFromDepsTkPU04')
    process.eleHcalTowerIsoPUDR04 = process.eleTrackIsoPUDR03.clone()
    process.eleHcalTowerIsoPUDR04.valMapSrc = cms.InputTag('eleIsoFromDepsHcalFromTowersPU')
    #add to sequences
    process.patElectronIsolation.replace( process.eleIsoDepositTk,cms.Sequence(process.eleIsoDepositTk+
                                                                               process.eleIsoDepositTkPU) )
    process.patElectronIsolation.replace( process.eleIsoFromDepsTk,
                                          cms.Sequence(process.eleIsoFromDepsTk+
                                                       process.eleIsoFromDepsTkPU03 +
                                                       process.eleTrackIsoPUDR03 +
                                                       process.eleIsoFromDepsTkPU04 +
                                                       process.eleTrackIsoPUDR04) )
    process.patElectronIsolation.replace( process.eleIsoFromDepsHcalFromTowers,
                                          cms.Sequence(process.eleIsoFromDepsHcalFromTowers +
                                                       process.eleIsoFromDepsHcalFromTowersPU +
                                                       process.eleHcalTowerIsoPUDR04) )
    #add new isolations as user floats to the corresponding pat object
    process.patElectrons.userData.userFloats.src.append( cms.InputTag('eleTrackIsoPUDR03') )
    process.patElectrons.userData.userFloats.src.append( cms.InputTag('eleTrackIsoPUDR04') )
    process.patElectrons.userData.userFloats.src.append( cms.InputTag('eleHcalTowerIsoPUDR04') )
    process.makePatElectrons.replace(process.patElectrons,cms.Sequence(process.patElectronIsolation+process.patElectrons))
    
    #change default patPhoton Isolation
    process.gamIsoDepositTkPU = process.gamIsoDepositTk.clone()    
    # use per-vertex track isolation for the electrons, from HGG isolation
    from ElectroWeakAnalysis.MultiBosons.Skimming.gamTrackExtractorBlocks_cfi import GamIsoTrackExtractorBlock
    process.gamIsoDepositTkPU.ExtractorPSet = GamIsoTrackExtractorBlock
    #setup isolation producers
    process.gamIsoFromDepsTkPU03 = process.gamIsoFromDepsTk.clone()
    process.gamIsoFromDepsTkPU03.deposits = cms.VPSet(cms.PSet(src = cms.InputTag("gamIsoDepositTkPU"),
                                                               deltaR = cms.double(0.3),
                                                               weight = cms.string('1'),
                                                               vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', #jurrasic
                                                                                   'Threshold(1.0)'),
                                                               skipDefaultVeto = cms.bool(True),
                                                               mode = cms.string('sum'))
                                                                 )
    process.gamIsoFromDepsTkPU04 = process.gamIsoFromDepsTkPU03.clone()    
    process.gamIsoFromDepsTkPU04.deposits = cms.VPSet(cms.PSet(src = cms.InputTag("gamIsoDepositTkPU"),
                                                               deltaR = cms.double(0.4),
                                                               weight = cms.string('1'),
                                                               vetos = cms.vstring('RectangularEtaPhiVeto(-0.015,0.015,-0.5,0.5)', #jurrasic
                                                                                   'Threshold(1.0)'),
                                                               skipDefaultVeto = cms.bool(True),
                                                               mode = cms.string('sum'))
                                                      )
    #remove veto region from photon HCAL isolation
    process.gamIsoFromDepsHcalFromTowersPU = process.gamIsoFromDepsHcalFromTowers.clone()
    process.gamIsoFromDepsHcalFromTowersPU.deposits = cms.VPSet(cms.PSet(src = cms.InputTag("gamIsoDepositHcalFromTowers"),
                                                                         deltaR = cms.double(0.4),
                                                                         weight = cms.string('1'),
                                                                         vetos = cms.vstring(), #remove the cone veto for HCAL iso
                                                                         skipDefaultVeto = cms.bool(True),
                                                                         mode = cms.string('sum'))
                                                                )

    #configure pat isolation sequence for new modules
    process.gamTrackIsoPUDR03 = cms.EDProducer("PhotonMapDoubleToFloat",
                                               collectionSrc = cms.InputTag('photons'),
                                               valMapSrc = cms.InputTag('gamIsoFromDepsTkPU03'))
    process.gamTrackIsoPUDR04 = process.gamTrackIsoPUDR03.clone()
    process.gamTrackIsoPUDR04.valMapSrc = cms.InputTag('gamIsoFromDepsTkPU04')
    process.gamHcalTowerIsoPUDR04 = process.gamTrackIsoPUDR03.clone()
    process.gamHcalTowerIsoPUDR04.valMapSrc = cms.InputTag('gamIsoFromDepsHcalFromTowersPU')
    #add to sequences
    process.patPhotonIsolation.replace( process.gamIsoDepositTk,cms.Sequence(process.gamIsoDepositTk+
                                                                             process.gamIsoDepositTkPU) )
    process.patPhotonIsolation.replace( process.gamIsoFromDepsTk,
                                        cms.Sequence(process.gamIsoFromDepsTk+
                                                     process.gamIsoFromDepsTkPU03 +
                                                     process.gamTrackIsoPUDR03 +
                                                     process.gamIsoFromDepsTkPU04 +
                                                     process.gamTrackIsoPUDR04) )
    process.patPhotonIsolation.replace( process.gamIsoFromDepsHcalFromTowers,
                                        cms.Sequence(process.gamIsoFromDepsHcalFromTowers +
                                                     process.gamIsoFromDepsHcalFromTowersPU +
                                                     process.gamHcalTowerIsoPUDR04) )

    #add new isolations as user floats to the corresponding pat object    
    process.patPhotons.userData.userFloats.src.append( cms.InputTag('gamTrackIsoPUDR03') )
    process.patPhotons.userData.userFloats.src.append( cms.InputTag('gamTrackIsoPUDR04') )
    process.patPhotons.userData.userFloats.src.append( cms.InputTag('gamHcalTowerIsoPUDR04') )
    process.makePatPhotons.replace(process.patPhotons,cms.Sequence(process.patPhotonIsolation+process.patPhotons))
# def switchToIsoForPU(process, after): <------------------------------------

