import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# utility functions to apply Z-recoil corrections to MEt
# NOTE: implementations specific to different analysis channels
#--------------------------------------------------------------------------------

import copy

import PhysicsTools.PatAlgos.tools.helpers as patutils

#--------------------------------------------------------------------------------
# Z --> muon + tau-jet, A/H --> muon + tau-jet channels
#--------------------------------------------------------------------------------

def restoreZllRecoilCorrectionInputTags_ZtoMuTau(process):
    if hasattr(process, "patPFMETsZllRecoilCorrected"):
        process.patPFMETsZllRecoilCorrected.src = cms.InputTag('allMuTauPairs')
    if hasattr(process, "allMuTauPairsPFMETsZllRecoilCorrected"):
        process.allMuTauPairsPFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
          cms.InputTag('allMuTauPairs')
    if hasattr(process, "patPFMETsZllRecoilCorrectedLooseMuonIsolation"):
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation.src = cms.InputTag('allMuTauPairsLooseMuonIsolation')
    if hasattr(process, "allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation"):
        process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation.srcReRecoDiTauObjects = \
          cms.InputTag('allMuTauPairsLooseMuonIsolation')

def applyZrecoilCorrection_runZtoMuTau(process):

    #print("<applyZrecoilCorrection_runZtoMuTau>:")
    #print(" --> applying Z-recoil correction to MET !!")

    process.load("TauAnalysis.RecoTools.recoZllRecoilCorrection_cfi")

    if hasattr(process, "produceMuTauPairs"):
        process.patPFMETsZllRecoilCorrected = cms.EDProducer("ZllRecoilCorrectionMuTauPair",
            process.recoZllRecoilCorrectionParameter,                                         
            src = cms.InputTag('allMuTauPairs')
        )

        process.allMuTauPairsPFMETsZllRecoilCorrected = process.allMuTauPairs.clone()
        process.allMuTauPairsPFMETsZllRecoilCorrected.srcMET = \
           cms.InputTag('patPFMETsZllRecoilCorrected', 'met')
        process.allMuTauPairsPFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
           cms.InputTag('allMuTauPairs')
        process.allMuTauPairsPFMETsZllRecoilCorrected.srcReRecoDiTauToMEtAssociations = \
           cms.InputTag('patPFMETsZllRecoilCorrected', 'diTauToMEtAssociations')

        process.patPFMETsZllRecoilCorrectionSequence = cms.Sequence(
            process.allMuTauPairs
           * process.patPFMETsZllRecoilCorrected
           * process.allMuTauPairsPFMETsZllRecoilCorrected
        )

        process.produceMuTauPairs.replace(process.allMuTauPairs,
                                          process.patPFMETsZllRecoilCorrectionSequence)

    if hasattr(process, "produceMuTauPairsLooseMuonIsolation"):
        process.patPFMETsZllRecoilCorrectedLooseMuonIsolation = process.patPFMETsZllRecoilCorrected.clone(
            src = cms.InputTag('allMuTauPairsLooseMuonIsolation')
        )

        process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation = process.allMuTauPairsLooseMuonIsolation.clone()
        process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation.srcMET = \
           cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'met')
        process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation.srcReRecoDiTauObjects = \
           cms.InputTag('allMuTauPairsLooseMuonIsolation')
        process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation.srcReRecoDiTauToMEtAssociations = \
           cms.InputTag('patPFMETsZllRecoilCorrectedLooseMuonIsolation', 'diTauToMEtAssociations')

        process.patPFMETsZllRecoilCorrectionSequenceLooseMuonIsolation = cms.Sequence(
            process.allMuTauPairsLooseMuonIsolation
           * process.patPFMETsZllRecoilCorrectedLooseMuonIsolation
           * process.allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation
        )

        process.produceMuTauPairsLooseMuonIsolation.replace(process.allMuTauPairsLooseMuonIsolation,
                                                            process.patPFMETsZllRecoilCorrectionSequenceLooseMuonIsolation)

    # iterate over all sequences attached to process object
    # and replace:
    #  o allMuTauPairs --> allMuTauPairsPFMETsZllRecoilCorrected
    #  o allMuTauPairsLooseMuonIsolation --> allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation
    for processAttrName in dir(process):
        processAttr = getattr(process, processAttrName)
        if isinstance(processAttr, cms.Sequence):
            print "--> Replacing InputTags in sequence:", processAttrName
            if processAttrName.find("LooseMuonIsolation") != -1:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('allMuTauPairsLooseMuonIsolation'),
                  cms.InputTag('allMuTauPairsPFMETsZllRecoilCorrectedLooseMuonIsolation'))
            else:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('allMuTauPairs'), 
                  cms.InputTag('allMuTauPairsPFMETsZllRecoilCorrected'))

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
    restoreZllRecoilCorrectionInputTags_ZtoMuTau(process)

def applyZrecoilCorrection_runAHtoMuTau(process):

    #print("<applyZrecoilCorrection_runAHtoMuTau>:")
    #print(" --> applying Z/A/H-recoil correction to MET !!")

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
    restoreZllRecoilCorrectionInputTags_ZtoMuTau(process)

#--------------------------------------------------------------------------------
# Z --> tau-jet + tau-jet channel
#--------------------------------------------------------------------------------

def restoreZllRecoilCorrectionInputTags_ZtoDiTau(process):
    if hasattr(process, "patPFMETsZllRecoilCorrected"):
        process.patPFMETsZllRecoilCorrected.src = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative')
    if hasattr(process, "selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected"):
        process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
          cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative')
    if hasattr(process, "patPFMETsZllRecoilCorrectedLoose2ndTau"):
        process.patPFMETsZllRecoilCorrectedLoose2ndTau.src = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative')
    if hasattr(process, "selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected"):
        process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
          cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative')

def applyZrecoilCorrection_runZtoDiTau(process):

    #print("<applyZrecoilCorrection_runZtoDiTau>:")
    #print(" --> applying Z-recoil correction to MET !!")

    process.load("TauAnalysis.RecoTools.recoZllRecoilCorrection_cfi")

    if hasattr(process, "produceDiTauPairs"):
        process.patPFMETsZllRecoilCorrected = cms.EDProducer("ZllRecoilCorrectionDiTauPair",
            process.recoZllRecoilCorrectionParameter,                                         
            src = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative')
        )

        process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected = \
           process.selectedDiTauPairs2ndTauElectronVetoCumulative.clone()
        process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected.srcMET = \
           cms.InputTag('patPFMETsZllRecoilCorrected', 'met')
        process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
           cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative')
        process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauToMEtAssociations = \
           cms.InputTag('patPFMETsZllRecoilCorrected', 'diTauToMEtAssociations')

        process.patPFMETsZllRecoilCorrectionSequence = cms.Sequence(
            process.selectedDiTauPairs2ndTauElectronVetoCumulative
           * process.patPFMETsZllRecoilCorrected
           * process.selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected
        )

        process.produceDiTauPairs.replace(process.selectedDiTauPairs2ndTauElectronVetoCumulative,
                                          process.patPFMETsZllRecoilCorrectionSequence)

        process.patPFMETsZllRecoilCorrectedLoose2ndTau = process.patPFMETsZllRecoilCorrected.clone(
            src = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative')
        )

        process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected = \
           process.selectedDiTauPairs2ndTauElectronVetoLooseCumulative.clone()
        process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected.srcMET = \
           cms.InputTag('patPFMETsZllRecoilCorrectedLoose2ndTau', 'met')
        process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauObjects = \
           cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative')
        process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected.srcReRecoDiTauToMEtAssociations = \
           cms.InputTag('patPFMETsZllRecoilCorrectedLoose2ndTau', 'diTauToMEtAssociations')

        process.patPFMETsZllRecoilCorrectionSequenceLoose2ndTau = cms.Sequence(
            process.selectedDiTauPairs2ndTauElectronVetoLooseCumulative
           * process.patPFMETsZllRecoilCorrectedLoose2ndTau
           * process.selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected
        )

        process.produceDiTauPairs.replace(process.selectedDiTauPairs2ndTauElectronVetoLooseCumulative,
                                          process.patPFMETsZllRecoilCorrectionSequenceLoose2ndTau)

    # iterate over all sequences attached to process object
    # and replace:
    #  o selectedDiTauPairs2ndTauElectronVetoCumulative
    #   --> selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected
    #  o selectedDiTauPairs2ndTauElectronVetoLooseCumulative
    #   --> selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected
    for processAttrName in dir(process):
        processAttr = getattr(process, processAttrName)
        if isinstance(processAttr, cms.Sequence):
            print "--> Replacing InputTags in sequence:", processAttrName
            if processAttrName.find("2ndTauElectronVetoLoose") != -1:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulative'),
                  cms.InputTag('selectedDiTauPairs2ndTauElectronVetoLooseCumulativePFMETsZllRecoilCorrected'))
            elif processAttrName.find("2ndTauElectronVeto") != -1:
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative'),
                  cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulativePFMETsZllRecoilCorrected'))

    # check if process object has GenericAnalyzer modules specific to ZtoMuTau channel attached to it.
    # If it has, replace in "regular" analysis sequence:
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrected', 'met')
    # and in analysis sequence with loose muon isolation applied (used for factorization purposes):
    #  o patPFMETs --> cms.InputTag('patPFMETsZllRecoilCorrectedLoose2ndTau', 'met')
    if hasattr(process, "analyzeZtoDiTauSequence"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoDiTauSequence, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeZtoDiTauSequence_factorizedTight2ndTau"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoDiTauSequence_factorizedTight2ndTau, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrected', 'met'))
    if hasattr(process, "analyzeZtoDiTauSequence_factorizedLoose2ndTau"):
        patutils.massSearchReplaceAnyInputTag(process.analyzeZtoDiTauSequence_factorizedLoose2ndTau, cms.InputTag('patPFMETs'),
          cms.InputTag('patPFMETsZllRecoilCorrectedLoose2ndTau', 'met'))

    # disable warnings in MET histogram managers
    # that num. MET objects != 1
    if hasattr(process, "caloMEtHistManager"):
        process.caloMEtHistManager.expectUniqueMEt = cms.bool(False)
    if hasattr(process, "pfMEtHistManager"):    
        process.pfMEtHistManager.expectUniqueMEt = cms.bool(False)

    # restore InputTag of ZllRecoilCorrection modules
    restoreZllRecoilCorrectionInputTags_ZtoDiTau(process)
