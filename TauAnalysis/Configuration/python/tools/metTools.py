import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import *

def addPFMet(process, correct = False):    
    process.load("JetMETCorrections.Type1MET.MetType1Corrections_cff")    
    process.metJESCorAK5PFJet.jetPTthreshold = cms.double(10.0)
    process.metJESCorAK5PFJet.useTypeII = cms.bool(True)

    process.patPFMETs = process.patMETs.clone()
    process.patPFMETs.addMuonCorrections = False
    process.patPFMETs.genMETSource = cms.InputTag('genMetTrue')

    if correct:
        process.patPFMETs.metSource = cms.InputTag('metJESCorAK5PFJet')
        process.makePatPFMETs = cms.Sequence(process.metJESCorAK5PFJet * process.patPFMETs)
    else:
        process.patPFMETs.metSource = cms.InputTag('pfMet')
        process.makePatPFMETs = cms.Sequence(process.patPFMETs)

    process.makePatMETs += process.makePatPFMETs
    
    return process.makePatMETs

def addCorrectedPFMet(process, isMC, doApplyType0corr, doApplySysShiftCorr, runPeriod, doSmearJets):

    process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
    
    process.makeCorrectedPatMETs = cms.Sequence()

    if isMC:
        import PhysicsTools.PatAlgos.tools.helpers as configtools
        process.type0PFMEtCorrection.remove(process.type0PFMEtCorrectionPFCandToVertexAssociation)
        process.makeCorrectedPatMETs += process.type0PFMEtCorrectionPFCandToVertexAssociation
        configtools.cloneProcessingSnippet(process, process.producePatPFMETCorrections, "NoSmearing")
        process.selectedPatJetsForMETtype1p2CorrNoSmearing.src = cms.InputTag('patJetsNotOverlappingWithLeptonsForMEtUncertainty')
        process.selectedPatJetsForMETtype2CorrNoSmearing.src = process.selectedPatJetsForMETtype1p2CorrNoSmearing.src
    else:
        doSmearJets = False

    sysShiftCorrParameter = None
    if doApplySysShiftCorr:
        process.load("JetMETCorrections.Type1MET.pfMETsysShiftCorrections_cfi")
        if runPeriod == "2011RunA":
            if isMC:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runAvsNvtx_mc
            else:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runAvsNvtx_data
        elif runPeriod == "2011RunB" or runPeriod == "2012RunA":
            # CV: use corrections determined for 2011 run B data for 2012 run A data,
            #     until corrections specific to 2012 run A data have been determined
            if isMC:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runBvsNvtx_mc
            else:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runBvsNvtx_data
        else:
            raise ValueError("Invalid runPeriod = %s !!" % runPeriod)

    from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
    runMEtUncertainties(
        process,
        electronCollection = '',
        photonCollection = '',
        muonCollection = cms.InputTag('patMuons'),
        tauCollection = '',
        jetCollection = cms.InputTag('patJets'),
        doSmearJets = doSmearJets,
        doApplyType0corr = doApplyType0corr,
        sysShiftCorrParameter = sysShiftCorrParameter,
        doApplySysShiftCorr = doApplySysShiftCorr,
        addToPatDefaultSequence = False
    )

    if isMC:
        process.patPFMet.addGenMET = cms.bool(True)
        process.patPFMetNoSmearing.addGenMET = cms.bool(True)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L3Absolute")
        process.patPFJetMETtype1p2CorrNoSmearing.jetCorrLabel = cms.string("L3Absolute")
    
        process.makeCorrectedPatMETs += process.metUncertaintySequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrectionsNoSmearing
    else:
        process.patPFMet.addGenMET = cms.bool(False)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L2L3Residual")
    
        process.makeCorrectedPatMETs += process.patJetsNotOverlappingWithLeptonsForMEtUncertainty
        process.makeCorrectedPatMETs += process.pfMEtSysShiftCorrSequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrections

    return process.makeCorrectedPatMETs

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
                           'src', oldMet ,newMet)


