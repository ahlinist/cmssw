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
    
    process.load("RecoMET.METProducers.mvaPFMET_cff")
    if isMC:
        process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")
    else:
        process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
    process.pfMEtMVA.srcCorrJets = cms.InputTag('calibratedAK5PFJetsForPFMEtMVA')
    process.pfMEtMVA.srcLeptons = cms.VInputTag('goodMuons')
    process.pfMEtMVA.verbosity = cms.int32(0)
    process.patPFMetMVA = process.patMETs.clone(
        metSource = cms.InputTag('pfMEtMVA'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.patMEtMVAsequence = cms.Sequence(process.pfMEtMVAsequence + process.patPFMetMVA)

    process.load("JetMETCorrections.Type1MET.noPileUpPFMET_cff")
    if isMC:
        process.calibratedAK5PFJetsForNoPileUpMEt.correctors = cms.vstring("ak5PFL1FastL2L3")
    else:
        process.calibratedAK5PFJetsForNoPileUpMEt.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
        
    process.noPileUpPFMEt.srcLeptons = cms.VInputTag('patMuons')
    process.noPileUpPFMEt.verbosity = cms.int32(0)
    process.patPFMetNoPileUp = process.patMETs.clone(
        metSource = cms.InputTag('noPileUpPFMEt'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.patMEtNoPileUpSequence = cms.Sequence(process.noPileUpPFMEtSequence + process.patPFMetNoPileUp)
    
    process.makeCorrectedPatMETs = cms.Sequence()

    if isMC:
        import PhysicsTools.PatAlgos.tools.helpers as configtools
        process.type0PFMEtCorrection.remove(process.type0PFMEtCorrectionPFCandToVertexAssociation)
        process.makeCorrectedPatMETs += process.type0PFMEtCorrectionPFCandToVertexAssociation
        configtools.cloneProcessingSnippet(process, process.producePatPFMETCorrections, "NoSmearing")
        process.selectedPatJetsForMETtype1p2CorrNoSmearing.src = cms.InputTag('patJetsNotOverlappingWithLeptonsForMEtUncertainty')
        process.selectedPatJetsForMETtype2CorrNoSmearing.src = process.selectedPatJetsForMETtype1p2CorrNoSmearing.src
        configtools.cloneProcessingSnippet(process, process.patMEtMVAsequence, "NoSmearing")
        process.patMEtNoPileUpSequence.remove(process.type0PFMEtCorrection)
        configtools.cloneProcessingSnippet(process, process.patMEtNoPileUpSequence, "NoSmearing")
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
        elif runPeriod == "2011RunB":
            if isMC:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runBvsNvtx_mc
            else:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2011runBvsNvtx_data
        elif runPeriod == "2012RunAplusB":
            if isMC:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc
            else:
                sysShiftCorrParameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_data
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
        makeType1corrPFMEt = True,
        makeType1p2corrPFMEt = True,
        makePFMEtByMVA = True,
        makeNoPileUpPFMEt = True,
        doApplyType0corr = doApplyType0corr,
        sysShiftCorrParameter = sysShiftCorrParameter,
        doApplySysShiftCorr = doApplySysShiftCorr,
        addToPatDefaultSequence = False
    )

    if isMC:
        process.patPFMet.addGenMET = cms.bool(True)
        process.patPFMetMVA.addGenMET = cms.bool(True)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L3Absolute")
        process.patPFJetMETtype1p2CorrNoSmearing.jetCorrLabel = cms.string("L3Absolute")
    
        process.makeCorrectedPatMETs += process.metUncertaintySequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrectionsNoSmearing
        process.makeCorrectedPatMETs += process.patMEtMVAsequenceNoSmearing
        process.makeCorrectedPatMETs += process.patMEtNoPileUpSequenceNoSmearing
    else:
        process.patPFMet.addGenMET = cms.bool(False)
        process.patPFMetMVA.addGenMET = cms.bool(False)
        process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L2L3Residual")
    
        process.makeCorrectedPatMETs += process.patJetsNotOverlappingWithLeptonsForMEtUncertainty
        if hasattr(process, "pfMEtSysShiftCorrSequence"):
            process.makeCorrectedPatMETs += process.pfMEtSysShiftCorrSequence
        process.makeCorrectedPatMETs += process.producePatPFMETCorrections
        process.makeCorrectedPatMETs += process.patMEtMVAsequence
        process.makeCorrectedPatMETs += process.patMEtNoPileUpSequence

    ##process.noPileUpPFMEtData.verbosity = cms.int32(1)
    ##process.noPileUpPFMEt.verbosity = cms.int32(1)
    ##if hasattr(process, "noPileUpPFMEtDataNoSmearing") and hasattr(process, "noPileUpPFMEtNoSmearing"):
    ##    process.noPileUpPFMEtDataNoSmearing.verbosity = cms.int32(1)
    ##    process.noPileUpPFMEtNoSmearing.verbosity = cms.int32(1)
        
    process.load("JetMETCorrections.Type1MET.mvaPFMET_cff")
    process.pfMEtInputForMVAMEt = cms.EDProducer("PFMEtFromPATMEtProducer",
        src = cms.InputTag('patType1CorrectedPFMet'),
        srcCov = cms.InputTag('pfMEtSignCovMatrix')
    )
    process.makeCorrectedPatMETs += process.pfMEtInputForMVAMEt
    process.mvaPFMEt.srcMEt = cms.InputTag('pfMEtInputForMVAMEt')
    ##process.mvaPFMEt.srcMEtCov = cms.InputTag('pfMEtSignCovMatrix')
    process.mvaPFMEt.srcMEtCov = cms.InputTag('') # NOTE: take MET covariance matrix from reco::PFMET object
    process.mvaPFMEt.srcLeptons = cms.VInputTag('patMuons')    
    if isMC:
        process.calibratedAK5PFJetsForMVAMEt.correctors = cms.vstring('ak5PFL1FastL2L3')
        import RecoMET.METProducers.METSigParams_cfi as jetResolutions
        process.smearedAK5PFJetsForTauAnalysisMEtTools = cms.EDProducer("SmearedPFJetProducer",
            src = cms.InputTag('calibratedAK5PFJetsForMVAMEt'),
            dRmaxGenJetMatch = cms.string('TMath::Min(0.5, 0.1 + 0.3*TMath::Exp(-0.05*(genJetPt - 10.)))'),
            sigmaMaxGenJetMatch = cms.double(5.),                                                                  
            inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
            lutName = cms.string('pfJetResolutionMCtoDataCorrLUT'),
            jetResolutions = jetResolutions.METSignificance_params,
            skipRawJetPtThreshold = cms.double(10.), # GeV
            skipCorrJetPtThreshold = cms.double(1.e-2),
            srcGenJets = cms.InputTag('ak5GenJetsNoNu'),
            ##verbosity = cms.int32(1)
        )
        process.ak5PFJetSequenceForMVAMEt += process.smearedAK5PFJetsForTauAnalysisMEtTools
        process.puJetIdDataForMVAMEt.jets = cms.InputTag('smearedAK5PFJetsForTauAnalysisMEtTools')
        process.puJetIdForMVAMEt.jets = cms.InputTag('smearedAK5PFJetsForTauAnalysisMEtTools')
        process.mvaPFMEtData.srcJets = cms.InputTag('smearedAK5PFJetsForTauAnalysisMEtTools')
        process.mvaPFMEt.sysShiftCorrections = process.sysShiftCorrections_2012runAplusBvsNvtx_mc
    else:
        process.calibratedAK5PFJetsForMVAMEt.correctors = cms.vstring('ak5PFL1FastL2L3Residual')
        process.mvaPFMEt.sysShiftCorrections = process.sysShiftCorrections_2012runAplusBvsNvtx_data
    process.makeCorrectedPatMETs += process.mvaPFMEtSequence
    process.patPFMetMVA2 = process.patMETs.clone(
        metSource = cms.InputTag('mvaPFMEt'),
        addMuonCorrections = cms.bool(False),
        genMETSource = cms.InputTag('genMetTrue')
    )
    process.makeCorrectedPatMETs += process.patPFMetMVA2

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


