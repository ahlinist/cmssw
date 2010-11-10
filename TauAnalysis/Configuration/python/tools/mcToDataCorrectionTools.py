import FWCore.ParameterSet.Config as cms
import copy

import PhysicsTools.PatAlgos.tools.helpers as patutils

#--------------------------------------------------------------------------------
# utility functions to apply Z-recoil corrections to MEt
# NOTE: implementations specific to different analysis channels
#--------------------------------------------------------------------------------

def applyZrecoilCorrection_runZtoMuTau(process):

    process.load("TauAnalysis.RecoTools.recoZllRecoilCorrection_cfi")

    if hasattr(process, "produceMuTauPairs"):
        process.patPFMETsZllRecoilCorrected = cms.EDProducer("ZllRecoilCorrectionMuTauPair",
            process.recoZllRecoilCorrectionParameter,                                         
            src = cms.InputTag('allMuTauPairs')
        )
        process.patPFMETsZllRecoilCorrected.svFit = process.allMuTauPairs.svFit

        process.patPFMETsZllRecoilCorrectionSequence = cms.Sequence(
            process.allMuTauPairs
           * process.patPFMETsZllRecoilCorrected
        )

        process.produceMuTauPairs.replace(process.allMuTauPairs,
                                          process.patPFMETsZllRecoilCorrectionSequence)

    if hasattr(process, "produceMuTauPairsLooseMuonIsolation"):
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation = process.patPFMETsZllRecoilCorrected.clone(
            src = cms.InputTag('allMuTauPairsLooseMuonIsolation')
        )
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation.svFit = process.allMuTauPairsLooseMuonIsolation.svFit

        process.patPFMETsZllRecoilCorrectionSequenceLooseMuonIsolation = cms.Sequence(
            process.allMuTauPairsLooseMuonIsolation
           * process.patPFMETsZllRecoilCorrectedLooseMuonIsolation
        )

        process.produceMuTauPairsLooseMuonIsolation.replace(process.allMuTauPairsLooseMuonIsolation,
                                                            process.patPFMETsZllRecoilCorrectionSequenceLooseMuonIsolation)

    # iterate over all sequences attached to process object
    # and replace:
    #  o allMuTauPairs --> cms.InputTag('patPFMETsZllRecoilCorrected', 'diTauCandidates')
    #  o allMuTauPairsLooseMuonIsolation --> cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'diTauCandidates')
    for processAttrName in dir(process):
        processAttr = getattr(process, processAttrName)
        if isinstance(processAttr, cms.Sequence):
            print "--> Replacing InputTags in sequence:", processAttrName
            if processAttrName.find("LooseMuonIsolation") != -1:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('allMuTauPairsLooseMuonIsolation'),
                  cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'diTauCandidates'))
            else:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('allMuTauPairs'), 
                  cms.InputTag('patPFMETsZllRecoilCorrected', 'diTauCandidates'))

    # check if process object has GenericAnalyzer modules specific to ZtoMuTau channel attached to it.
    # If it has, replace in "regular" analysis sequence:
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrected', 'met')
    # and in analysis sequence with loose muon isolation applied (used for factorization purposes):
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'met')
    if hasattr(process, "analyzeZtoMuTauSequence"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoMuTauSequence, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeZtoMuTauSequence_factorizedWithMuonIsolation"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoMuTauSequence_factorizedWithMuonIsolation, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeZtoMuTauSequence_factorizedWithoutMuonIsolation"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoMuTauSequence_factorizedWithoutMuonIsolation, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'met'))

    # disable warnings in MET histogram managers
    # that num. MET objects != 1
    if hasattr(process, "caloMEtHistManager"):
        process.caloMEtHistManager.expectUniqueMEt = cms.bool(False)
    if hasattr(process, "pfMEtHistManager"):    
        process.pfMEtHistManager.expectUniqueMEt = cms.bool(False)

    # restore InputTag of ZllRecoilCorrection modules
    if hasattr(process, "patPFMETsZllRecoilCorrected"):
        process.patPFMETsZllRecoilCorrected.src = cms.InputTag('allMuTauPairs')
    if hasattr(process, "patPFMETsZllRecoilCorrectedLooseMuonIsolation"):
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation.src = cms.InputTag('allMuTauPairs')    

def applyZrecoilCorrection_runAHtoMuTau(process):

    applyZrecoilCorrection_runZtoMuTau(process)
    
    # check if process object has GenericAnalyzer modules specific to AHtoMuTau channel attached to it.
    # If it has, replace in "regular" analysis sequence:
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrected', 'met')
    # and in analysis sequence with loose muon isolation applied (used for factorization purposes):
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'met')
    if hasattr(process, "analyzeAHtoMuTauSequence"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeAHtoMuTauSequence, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeAHtoMuTauSequence_factorizedWithMuonIsolation"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeAHtoMuTauSequence_factorizedWithMuonIsolation, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeAHtoMuTauSequence_factorizedWithoutMuonIsolation"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeAHtoMuTauSequence_factorizedWithoutMuonIsolation, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'met'))

    # restore InputTag of ZllRecoilCorrection modules
    if hasattr(process, "patPFMETsZllRecoilCorrected"):
        process.patPFMETsZllRecoilCorrected.src = cms.InputTag('allMuTauPairs')
    if hasattr(process, "patPFMETsZllRecoilCorrectedLooseMuonIsolation"):
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation.src = cms.InputTag('allMuTauPairs')  
