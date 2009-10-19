import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import *

def addGenMetWithMu(process):
    process.load("TauAnalysis.GenSimTools.genMETWithMu_cff")
    process.producePrePat += process.produceGenMETwithMu


def addPFMet(process,redoGenMet=True,correct=False):
    process.load("PhysicsTools.PFCandProducer.pfType1MET_cff")
    process.layer1PFMETs = process.layer1METs.clone()
    process.layer1PFMETs.addMuonCorrections = False
    process.layer1PFMETs.addTrigMatch = False
    if correct:
        process.patAODExtraReco += process.pfCorMET
        process.layer1PFMETs.metSource = cms.InputTag('pfType1MET')
    else:
        process.patAODExtraReco += process.pfMET
        process.layer1PFMETs.metSource = cms.InputTag('pfMET')
    if redoGenMet:
        addGenMetWithMu(process)
        process.layer1PFMETs.genMETSource = cms.InputTag('genMETWithMu')
    process.allLayer1Objects.replace(process.layer1METs,
                                     process.layer1METs + process.layer1PFMETs)

def addTCMet(process,):
    process.layer1TCMETs = process.layer1METs.clone()
    process.layer1TCMETs.addMuonCorrections = False
    process.layer1TCMETs.addTrigMatch = False
    process.layer1TCMETs.metSource = cms.InputTag('tcMet')
    process.layer1TCMETs.genMETSource = cms.InputTag('genMETWithMu')
    process.allLayer1Objects.replace(process.layer1METs,
                                     process.layer1METs + process.layer1TCMETs)

def replaceMETforDiTaus(process,
                        oldMet = cms.InputTag('layer1METs'),
                        newMet = cms.InputTag('layer1PFMETs') ):
    massSearchReplaceParam(process.produceDiTauPairsAllKinds,
                           'srcMET', oldMet, newMet)

def replaceMETforTauNu(process,
                        oldMet = cms.InputTag('layer1METs'),
                        newMet = cms.InputTag('layer1PFMETs') ):
    massSearchReplaceParam(process.produceTauNuPairs,
                           'srcMET', oldMet, newMet)
def replaceMETforMet(process,
                     oldMet = cms.InputTag('Layer1METs'),
                     newMet = cms.InputTag('layer1PFMETs')):
    massSearchReplaceParam(process.selectLayer1METs,
                           'src',oldMet,newMet)


