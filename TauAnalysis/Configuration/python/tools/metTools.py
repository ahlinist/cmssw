import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import *

def addPFMet(process,correct=False):
    process.load("PhysicsTools.PFCandProducer.pfType1MET_cff")
    process.patPFMETs = process.patMETs.clone()
    process.patPFMETs.addMuonCorrections = False

    process.makePatPFMETs = cms.Sequence(process.patPFMETs)
    if correct:
        process.makePatPFMETs.replace(process.patPFMETs,
                                         process.pfCorMET*process.patPFMETs)
        process.patPFMETs.metSource = cms.InputTag('pfType1MET')
    else:
        process.makePatPFMETs.replace(process.patPFMETs,
                                         process.pfMET*process.patPFMETs)
        process.patPFMETs.metSource = cms.InputTag('pfMET')
    process.patPFMETs.genMETSource = cms.InputTag('genMetTrue')
    process.makePatMETs += process.makePatPFMETs

def addTCMet(process):
    process.layer1TCMETs = process.patMETs.clone()
    process.layer1TCMETs.addMuonCorrections = False
    process.layer1TCMETs.metSource = cms.InputTag('tcMet')
    process.layer1TCMETs.genMETSource = cms.InputTag('genMETWithMu')
    process.patCandidates.replace(process.patMETs,
                                     process.patMETs + process.layer1TCMETs)

def replaceMETforDiTaus(process,
                        oldMet = cms.InputTag('patMETs'),
                        newMet = cms.InputTag('patPFMETs') ):
    massSearchReplaceParam(process.produceDiTauPairsAllKinds,
                           'srcMET', oldMet, newMet)

def replaceMETforTauNu(process,
                        oldMet = cms.InputTag('patMETs'),
                        newMet = cms.InputTag('patPFMETs') ):
    massSearchReplaceParam(process.produceTauNuPairs,
                           'srcMET', oldMet, newMet)
def replaceMETforMet(process,
                     oldMet = cms.InputTag('Layer1METs'),
                     newMet = cms.InputTag('patPFMETs')):
    massSearchReplaceParam(process.selectLayer1METs,
                           'src',oldMet,newMet)


