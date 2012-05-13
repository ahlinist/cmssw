import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.coreTools import restrictInputToAOD
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag 

def switchToAOD(process, triggerHistManager = None, eventDumpPlugin = None):

    # call "standard" PAT function to restrict all InputTags to AOD event content
    restrictInputToAOD(process, [ "All", ])

    # switch collection of ECAL recHits used as input for IsoDeposit computation
    # from list of all ECAL recHits in the event to "reduced" collections
    # limited to cones of size dR = 0.6 around electron candidates
    if hasattr(process, "p"):
        massSearchReplaceAnyInputTag(process.p, cms.InputTag("ecalRecHit", "EcalRecHitsEB"), cms.InputTag("reducedEcalRecHitsEB"))
        massSearchReplaceAnyInputTag(process.p, cms.InputTag("ecalRecHit", "EcalRecHitsEE"), cms.InputTag("reducedEcalRecHitsEE")) 

    # disable PAT trigger matching
    # (not yet implemented for photons and jets)
    if hasattr(process, "patTriggerSequence"):
        process.patDefaultSequence.remove(process.patTriggerSequence)
    process.patElectrons.embedHighLevelSelection = cms.bool(False)
    #process.patPhotons.embedHighLevelSelection = cms.bool(False)
    process.patMuons.embedHighLevelSelection = cms.bool(False)
    process.patTaus.embedHighLevelSelection = cms.bool(False)
    #process.patJets.embedHighLevelSelection = cms.bool(False)
    process.patMETs.embedHighLevelSelection = cms.bool(False)
    if hasattr(process, "patPFMETs"):
        process.patPFMETs.embedHighLevelSelection = cms.bool(False)

    if triggerHistManager is not None:
        triggerHistManager.hltResultsSource = cms.InputTag('')
        triggerHistManager.l1Bits = cms.vstring()
        triggerHistManager.hltPaths = cms.vstring()
       
    if eventDumpPlugin is not None:
        eventDumpPlugin.l1GtReadoutRecordSource = cms.InputTag('')
        eventDumpPlugin.l1GtObjectMapRecordSource = cms.InputTag('')
        eventDumpPlugin.hltResultsSource = cms.InputTag('')
    
   
