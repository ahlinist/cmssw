import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# utility functions to add Z-recoil corrections to MEt
# to different analysis channels
#--------------------------------------------------------------------------------

import PhysicsTools.PatAlgos.tools.helpers as patutils

from TauAnalysis.RecoTools.tools.configureZllRecoilCorrection import configureZllRecoilCorrection

def _applyZllRecoilCorrection(process, diTauProductionSequenceName, diTauProducerModuleName, ZllRecoilCorrectionType,
                              genericAnalyzerSequenceNames = []):

    #print("<applyZllRecoilCorrection>:")

    configZllRecoilCorrection = \
      configureZllRecoilCorrection(process, diTauProducerModuleName, ZllRecoilCorrectionType)
    diTauProductionSequence = getattr(process, diTauProductionSequenceName)
    diTauProductionSequence += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
    print "INFO: Disabling NSVfit in %s for di-taus without corrected MET." % \
            diTauProducerModuleName
    diTauProducer = getattr(process, diTauProducerModuleName)
    diTauProducer.nSVfit = cms.PSet()

    if hasattr(process, diTauProductionSequenceName):

        # iterate over all sequences attached to process object
        # and replace InputTags:
        #  o diTauProducerModuleName --> diTauProducerModuleZllRecoilCorrectedName
        #  o patPFMETs --> cms.InputTag(patPFMETsZllRecoilCorrectionModuleName, 'met')
        for processAttrName in dir(process):
            processAttr = getattr(process, processAttrName)
            if isinstance(processAttr, cms.Sequence):
                #print "--> Replacing InputTags in sequence:", processAttrName
                patutils.massSearchReplaceAnyInputTag(processAttr, cms.InputTag(diTauProducerModuleName),
                  cms.InputTag(configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrectedName']))

        # replace InputTags:
        #  o patPFMETs --> cms.InputTag(patPFMETsZllRecoilCorrectionModuleName, 'met')
        # in GenericAnalyzer sequences
        for genericAnalyzerSequenceName in genericAnalyzerSequenceNames:
            if hasattr(process, genericAnalyzerSequenceName):
                genericAnalyzerSequence = getattr(process, genericAnalyzerSequenceName)
                patutils.massSearchReplaceAnyInputTag(genericAnalyzerSequence, cms.InputTag('patPFMETs'),
                  cms.InputTag(configZllRecoilCorrection['patPFMETsZllRecoilCorrectionModuleName'], 'met'))

        # restore InputTags of ZllRecoilCorrection modules
        configZllRecoilCorrection['patPFMETsZllRecoilCorrectionModule'].src = cms.InputTag(diTauProducerModuleName)
        configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrected'].srcReRecoDiTauObjects = \
           cms.InputTag(diTauProducerModuleName)

        # disable warnings in MET histogram managers
        # that num. MET objects != 1
	class updateMEtHistManagerVisitor():
	    def enter(self, genericAnalyzerModule):
	        if hasattr(genericAnalyzerModule, "analyzers"):
	    	    analyzers = getattr(genericAnalyzerModule, "analyzers")
	            for analyzer in analyzers:
	                if hasattr(analyzer, "pluginType"):
                            analyzerPluginType = getattr(analyzer, "pluginType").value()
	                    if analyzerPluginType == "CaloMEtHistManager" or analyzerPluginType == "PFMEtHistManager":
	                        setattr(analyzer, "expectUniqueMEt", cms.bool(False))
	    def leave(self, genericAnalyzerModule):
     	        pass
	for genericAnalyzerSequenceName in genericAnalyzerSequenceNames:
            if hasattr(process, genericAnalyzerSequenceName):
                genericAnalyzerSequence = getattr(process, genericAnalyzerSequenceName)
	        updateMEtHistManagers = updateMEtHistManagerVisitor()
	        genericAnalyzerSequence.visit(updateMEtHistManagers)
        if hasattr(process, "caloMEtHistManager"):
            process.caloMEtHistManager.expectUniqueMEt = cms.bool(False)
        if hasattr(process, "pfMEtHistManager"):
            process.pfMEtHistManager.expectUniqueMEt = cms.bool(False)

def _addEventWeight(process, genAnalyzerModuleNames, srcEventWeight, applyAfterFilterName = "*"):
    for genAnalyzerModuleName in genAnalyzerModuleNames:
        print "Trying to apply %s event weight to analyzer %s" % (
            srcEventWeight, genAnalyzerModuleName),
        if hasattr(process, genAnalyzerModuleName):
            print " -- added it!"
            genAnalyzerModule = getattr(process, genAnalyzerModuleName)
            pset = cms.PSet(
                src = cms.InputTag(srcEventWeight),
                applyAfterFilter = cms.string(applyAfterFilterName)
            )
            if hasattr(genAnalyzerModule, "eventWeights"):
                getattr(genAnalyzerModule, "eventWeights").append(pset)
            else:
                setattr(genAnalyzerModule, "eventWeights", cms.VPSet(pset))
        else:
            print " -- couldn't find it in the process, SKIPPING!"

#--------------------------------------------------------------------------------
# Z --> muon + tau-jet, A/H --> muon + tau-jet channels
#--------------------------------------------------------------------------------

def applyZrecoilCorrection_runZtoMuTau(process):

    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsAll", 'allMuTauPairs',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analyzeZtoMuTauSequence" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsAll", 'allMuTauPairsLooseMuonIsolation',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analyzeZtoMuTauSequence_factorizedWithMuonIsolation",
                                "analyzeZtoMuTauSequence_factorizedWithoutMuonIsolation" ])

def applyZrecoilCorrection_runZtoMuTau_bgEstTemplate(process):

    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsAll", 'allMuTauPairs',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analyzeZtoMuTauSequence" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsBgEstZmumuJetMisIdEnriched", 'muTauPairsBgEstZmumuJetMisIdEnriched',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analysisSequenceBgEstZmumuJetMisIdEnriched" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsBgEstZmumuMuonMisIdEnriched", 'muTauPairsBgEstZmumuMuonMisIdEnriched',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analysisSequenceBgEstZmumuMuonMisIdEnriched" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsBgEstQCDenriched", 'muTauPairsBgEstQCDenriched',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analysisSequenceBgEstQCDenriched" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsBgEstWplusJetsEnriched", 'muTauPairsBgEstWplusJetsEnriched',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analysisSequenceBgEstWplusJetsEnriched" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsBgEstTTplusJetsEnriched", 'muTauPairsBgEstTTplusJetsEnriched',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analysisSequenceBgEstTTplusJetsEnriched" ])

def applyZrecoilCorrection_runAHtoMuTau(process):

    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsAll", 'allMuTauPairs',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analyzeAHtoMuTauSequence" ])
    _applyZllRecoilCorrection(process,
                              "produceMuTauPairsAll", 'allMuTauPairsLooseMuonIsolation',
                              "ZllRecoilCorrectionMuTauPair",
                              [ "analyzeAHtoMuTauSequence_factorizedWithMuonIsolation",
                                "analyzeAHtoMuTauSequence_factorizedWithoutMuonIsolation" ])

def _addEventWeightZtoMuTau(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, [
            "analyzeZtoMuTauEventsOS",
            "analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation",
            "analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation",
            "analyzeZtoMuTauEventsSS",
            "analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation",
            "analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation",
        ],
        srcEventWeight, applyAfterFilterName)

def applyMuonTriggerEfficiencyCorrection_runZtoMuTau(process):

    process.load("TauAnalysis.RecoTools.muonTriggerEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleZtoMuTauSpecific"):
        process.producePatTupleZtoMuTauSpecific._seq = process.producePatTupleZtoMuTauSpecific._seq \
          * process.muonTriggerEfficiencyCorrection

    _addEventWeightZtoMuTau(process, "muonTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyMuonIsolationEfficiencyCorrection_runZtoMuTau(process):

    process.load("TauAnalysis.RecoTools.muonIsolationEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleZtoMuTauSpecific"):
        process.producePatTupleZtoMuTauSpecific._seq = process.producePatTupleZtoMuTauSpecific._seq \
          * process.muonIsolationEfficiencyCorrection

    _addEventWeightZtoMuTau(process, "muonIsolationEfficiencyCorrection", applyAfterFilterName = "evtSelMuonPFRelIso")

def applyVertexMultiplicityReweighting_runZtoMuTau(process):

    process.load("TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi")
    if hasattr(process, "producePatTupleZtoMuTauSpecific"):
        process.producePatTupleZtoMuTauSpecific._seq = process.producePatTupleZtoMuTauSpecific._seq \
          * process.selectedPrimaryVerticesTrackPtSumGt10 * process.vertexMultiplicityReweight

    _addEventWeightZtoMuTau(process, "vertexMultiplicityReweight")

def applyRhoNeutralReweighting_runZtoMuTau(process):

    process.load("TauAnalysis.RecoTools.vertexMultiplicityVsRhoPFNeutralReweight_cfi")
    if hasattr(process, "produceRecObjects"):
        process.produceRecObjects._seq = process.produceRecObjects._seq \
          * process.produceVertexMultiplicityVsRhoPFNeutralReweights

    _addEventWeightZtoMuTau(process, "vertexMultiplicityVsRhoPFNeutralReweight")

def _addEventWeightZtoMuTau_bgEstTemplate(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(process,
                    [ "analyzeEventsBgEstQCDenriched",
                      "analyzeEventsBgEstTTplusJetsEnriched",
                      "analyzeEventsBgEstWplusJetsEnriched",
                      "analyzeEventsBgEstZmumuJetMisIdEnriched",
                      "analyzeEventsBgEstZmumuMuonMisIdEnriched" ],
                    srcEventWeight, applyAfterFilterName)

def applyMuonTriggerEfficiencyCorrection_runZtoMuTau_bgEstTemplate(process):

    applyMuonTriggerEfficiencyCorrection_runZtoMuTau(process)

    _addEventWeightZtoMuTau_bgEstTemplate(process, "muonTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyMuonIsolationEfficiencyCorrection_runZtoMuTau_bgEstTemplate(process):

    applyMuonIsolationEfficiencyCorrection_runZtoMuTau(process)

    _addEventWeight(process,
	            [ "analyzeEventsBgEstQCDenriched" ],
	            "muonIsolationEfficiencyCorrection", applyAfterFilterName = "muonPFRelIsoCutBgEstQCDenriched")
    _addEventWeight(process,
                    [ "analyzeEventsBgEstTTplusJetsEnriched" ],
	            "muonIsolationEfficiencyCorrection", applyAfterFilterName = "muonPFRelIsoCutBgEstTTplusJetsEnriched")
    _addEventWeight(process,
                    [ "analyzeEventsBgEstWplusJetsEnriched" ],
	            "muonIsolationEfficiencyCorrection", applyAfterFilterName = "muonPFRelIsoCutBgEstWplusJetsEnriched")
    _addEventWeight(process,
	            [ "analyzeEventsBgEstZmumuJetMisIdEnriched",
                      "analyzeEventsBgEstZmumuMuonMisIdEnriched" ],
	            "muonIsolationEfficiencyCorrection", applyAfterFilterName = "evtSelMuonPFRelIso")

def applyVertexMultiplicityReweighting_runZtoMuTau_bgEstTemplate(process):

    applyVertexMultiplicityReweighting_runZtoMuTau(process)

    _addEventWeightZtoMuTau_bgEstTemplate(process, "vertexMultiplicityReweight")

def _addEventWeighAHtoMuTau(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, [
            "analyzeAHtoMuTauEventsOS_woBtag",
            "analyzeAHtoMuTauEventsOS_wBtag",
            "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsSS_woBtag",
            "analyzeAHtoMuTauEventsSS_wBtag",
            "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation",
            "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation",
            "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation",
        ],
        srcEventWeight, applyAfterFilterName)

def applyMuonTriggerEfficiencyCorrection_runAHtoMuTau(process):

    applyMuonTriggerEfficiencyCorrection_runZtoMuTau(process)

    _addEventWeighAHtoMuTau(process, "muonTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyMuonIsolationEfficiencyCorrection_runAHtoMuTau(process):

    applyMuonIsolationEfficiencyCorrection_runZtoMuTau(process)

    _addEventWeighAHtoMuTau(process, "muonIsolationEfficiencyCorrection", applyAfterFilterName = "evtSelMuonPFRelIso")

def applyVertexMultiplicityReweighting_runAHtoMuTau(process):

    applyVertexMultiplicityReweighting_runZtoMuTau(process)

    _addEventWeighAHtoMuTau(process, "vertexMultiplicityReweight")

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


#--------------------------------------------------------------------------------
# Z --> electron + tau-jet, A/H --> electron + tau-jet channels
#--------------------------------------------------------------------------------


def applyZrecoilCorrection_runZtoElecTau(process):

    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsAll", 'allElecTauPairs',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analyzeZtoElecTauSequence" ])

    # disable warnings in MET histogram managers
    # that num. MET objects != 1
    if hasattr(process, "caloMEtHistManagerTemplateFit"):
        process.caloMEtHistManagerTemplateFit.expectUniqueMEt = cms.bool(False)
    if hasattr(process, "pfMEtHistManagerTemplateFit"):
        process.pfMEtHistManagerTemplateFit.expectUniqueMEt = cms.bool(False)

def applyZrecoilCorrection_runZtoElecTau_bgEstTemplate(process):

    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsAll", 'allElecTauPairs',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analyzeZtoElecTauSequence" ])
    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsBgEstZtautauEnriched", 'elecTauPairsBgEstZtautauEnriched',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analysisSequenceBgEstZtautauEnriched" ])    
##     _applyZllRecoilCorrection(process,
##                               "produceElecTauPairsBgEstZeeJetMisIdEnriched", 'elecTauPairsBgEstZeeJetMisIdEnriched',
##                               "ZllRecoilCorrectionElecTauPair",
##                               [ "analysisSequenceBgEstZeeJetMisIdEnriched" ])
    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsBgEstZeeElectronMisIdEnriched", 'elecTauPairsBgEstZeeElectronMisIdEnriched',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analysisSequenceBgEstZeeElectronMisIdEnriched" ])
    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsBgEstQCDenriched", 'elecTauPairsBgEstQCDenriched',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analysisSequenceBgEstQCDenriched" ])
    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsBgEstWplusJetsEnriched", 'elecTauPairsBgEstWplusJetsEnriched',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analysisSequenceBgEstWplusJetsEnriched" ])
##     _applyZllRecoilCorrection(process,
##                               "produceElecTauPairsBgEstPhotonPlusJetsEnriched", 'elecTauPairsBgEstPhotonPlusJetsEnriched',
##                               "ZllRecoilCorrectionElecTauPair",
##                               [ "analysisSequenceBgEstPhotonPlusJetsEnriched" ])



    # disable warnings in MET histogram managers
    # that num. MET objects != 1
    if hasattr(process, "caloMEtHistManagerTemplateFit"):
        process.caloMEtHistManagerTemplateFit.expectUniqueMEt = cms.bool(False)
    if hasattr(process, "pfMEtHistManagerTemplateFit"):
        process.pfMEtHistManagerTemplateFit.expectUniqueMEt = cms.bool(False)

def applyZrecoilCorrection_runAHtoElecTau(process):

    _applyZllRecoilCorrection(process,
                              "produceElecTauPairsAll", 'allElecTauPairs',
                              "ZllRecoilCorrectionElecTauPair",
                              [ "analyzeAHtoElecTauSequence" ])

def _addEventWeightZtoElecTau(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, [
            "analyzeZtoElecTauEventsOS",
            "analyzeZtoElecTauEventsSS"
        ],
        srcEventWeight, applyAfterFilterName)

def _addEventWeighAHtoElecTau(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, [
            "analyzeAHtoElecTauEventsOS_woBtag",
            "analyzeAHtoElecTauEventsSS_woBtag",
            "analyzeAHtoElecTauEventsOS_wBtag",
            "analyzeAHtoElecTauEventsSS_wBtag",
            "analyzeAHtoElecTauEventsOS_VBF",
            "analyzeAHtoElecTauEventsSS_VBF",
            "analyzeAHtoElecTauEventsOS_ZeroOneJets",
            "analyzeAHtoElecTauEventsSS_ZeroOneJets",
            "analyzeAHtoElecTauEventsOS_Boosted",
            "analyzeAHtoElecTauEventsSS_Boosted"
        ],
        srcEventWeight, applyAfterFilterName)

def _addEventWeightZtoElecTau_bgEstTemplate(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(process,
                    [ "analyzeEventsBgEstZtautauEnriched",
                      "analyzeEventsBgEstQCDenriched",
#                      "analyzeEventsBgEstPhotonPlusJetsEnriched",
                      "analyzeEventsBgEstWplusJetsEnriched",
#                      "analyzeEventsBgEstZeeJetMisIdEnriched",
                      "analyzeEventsBgEstZeeElectronMisIdEnriched" ],
                    srcEventWeight, applyAfterFilterName)

def applyElectronTriggerEfficiencyCorrection_runZtoElecTau(process):

    process.load("TauAnalysis.RecoTools.electronTriggerEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleZtoElecTauSpecific"):
        process.producePatTupleZtoElecTauSpecific._seq = process.producePatTupleZtoElecTauSpecific._seq \
          * process.electronTriggerEfficiencyCorrection

    _addEventWeightZtoElecTau(process, "electronTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyElectronTriggerEfficiencyCorrection_runZtoElecTau_bgEstTemplate(process):

    applyElectronTriggerEfficiencyCorrection_runZtoElecTau(process)

    _addEventWeightZtoElecTau_bgEstTemplate(process, "electronTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyElectronTriggerEfficiencyCorrection_runAHtoElecTau(process):

    process.load("TauAnalysis.RecoTools.electronTriggerEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleAHtoElecTauSpecific"):
        process.producePatTupleAHtoElecTauSpecific._seq = process.producePatTupleAHtoElecTauSpecific._seq \
          * process.electronTriggerEfficiencyCorrection

    _addEventWeighAHtoElecTau(process, "electronTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")

def applyTauMetTriggerEfficiencyCorrection_runZtoElecTau(process):
    process.load("TauAnalysis.RecoTools.tauTriggerEfficiencyCorrectionForElecTau_cfi")
    if hasattr(process, "producePatTupleZtoElecTauSpecific"):
        process.producePatTupleZtoElecTauSpecific._seq = process.producePatTupleZtoElecTauSpecific._seq \
          * process.tauTriggerEfficiencyCorrectionForElecTau

    _addEventWeightZtoElecTau(process, "tauTriggerEfficiencyCorrectionForElecTau", applyAfterFilterName = "evtSelTrigger")

def applyTauMetTriggerEfficiencyCorrection_runZtoElecTau_bgEstTemplate(process):

    applyTauMETTriggerEfficiencyCorrection_runZtoElecTau(process)

    _addEventWeightZtoElecTau_bgEstTemplate(process, "tauTriggerEfficiencyCorrectionForElecTau", applyAfterFilterName = "evtSelTrigger")

def applyTauMetTriggerEfficiencyCorrection_runAHtoElecTau(process):

    process.load("TauAnalysis.RecoTools.tauTriggerEfficiencyCorrectionForElecTau_cfi")
    if hasattr(process, "producePatTupleAHtoElecTauSpecific"):
        process.producePatTupleAHtoElecTauSpecific._seq = process.producePatTupleAHtoElecTauSpecific._seq \
          * process.tauTriggerEfficiencyCorrectionForElecTau

    _addEventWeighAHtoElecTau(process, "tauTriggerEfficiencyCorrectionForElecTau", applyAfterFilterName = "evtSelTrigger")

def applyElectronIsolationEfficiencyCorrection_runZtoElecTau(process):

    process.load("TauAnalysis.RecoTools.electronIsolationEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleZtoElecTauSpecific"):
        process.producePatTupleZtoElecTauSpecific._seq = process.producePatTupleZtoElecTauSpecific._seq \
          * process.electronIsolationEfficiencyCorrection

    _addEventWeightZtoElecTau(process, "electronIsolationEfficiencyCorrection", applyAfterFilterName = "evtSelElectronIso")

def applyElectronIsolationEfficiencyCorrection_runZtoElecTau_bgEstTemplate(process):

    applyElectronIsolationEfficiencyCorrection_runZtoElecTau(process)

    _addEventWeight(process,
                    [ "analyzeEventsBgEstZtautauEnriched" ],
	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstZtautauEnriched")
    _addEventWeight(process,
	            [ "analyzeEventsBgEstQCDenriched" ],
	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstQCDenriched")
##     _addEventWeight(process,
##                     [ "analyzeEventsBgEstPhotonPlusJetsEnriched" ],
## 	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstPhotonPlusJetsEnriched")
    _addEventWeight(process,
                    [ "analyzeEventsBgEstWplusJetsEnriched" ],
	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstWplusJetsEnriched")
##     _addEventWeight(process,
## 	            [ "analyzeEventsBgEstZeeJetMisIdEnriched" ],
## 	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstZeeJetMisIdEnriched")
    _addEventWeight(process,
	            [ "analyzeEventsBgEstZeeElectronMisIdEnriched" ],
	            "electronIsolationEfficiencyCorrection", applyAfterFilterName = "electronIsoCutBgEstZeeElectronMisIdEnriched")

def applyElectronIsolationEfficiencyCorrection_runAHtoElecTau(process):

    process.load("TauAnalysis.RecoTools.electronIsolationEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleAHtoElecTauSpecific"):
        process.producePatTupleAHtoElecTauSpecific._seq = process.producePatTupleAHtoElecTauSpecific._seq \
          * process.electronIsolationEfficiencyCorrection

    _addEventWeighAHtoElecTau(process, "electronIsolationEfficiencyCorrection", applyAfterFilterName = "evtSelElectronIso")

def applyVertexMultiplicityReweighting_runZtoElecTau(process):
    process.load("TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi")
    if hasattr(process, "producePatTupleZtoElecTauSpecific"): 
        process.producePatTupleZtoElecTauSpecific._seq = process.producePatTupleZtoElecTauSpecific._seq \
          * process.selectedPrimaryVerticesTrackPtSumGt10 * process.vertexMultiplicityReweight
                
    _addEventWeightZtoElecTau(process, "vertexMultiplicityReweight")

def applyVertexMultiplicityReweighting_runZtoElecTau_bgEstTemplate(process):

    applyVertexMultiplicityReweighting_runZtoElecTau(process)

    _addEventWeightZtoElecTau_bgEstTemplate(process, "vertexMultiplicityReweight")

def applyRhoNeutralReweighting_runZtoElecTau(process):
    process.load("TauAnalysis.RecoTools.vertexMultiplicityVsRhoPFNeutralReweight_cfi")
    if hasattr(process, "produceRecObjects"):
        process.produceRecObjects._seq = process.produceRecObjects._seq \
          * process.produceVertexMultiplicityVsRhoPFNeutralReweights

    
    _addEventWeightZtoElecTau(process, "vertexMultiplicityVsRhoPFNeutralReweight")

def applyVertexMultiplicityReweighting_runAHtoElecTau(process):
    process.load("TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi")
    if hasattr(process, "producePatTupleAHtoElecTauSpecific"): 
        process.producePatTupleAHtoElecTauSpecific._seq = process.producePatTupleAHtoElecTauSpecific._seq \
          * process.selectedPrimaryVerticesTrackPtSumGt10 * process.vertexMultiplicityReweight
                
    _addEventWeightZtoElecTau(process, "vertexMultiplicityReweight")

def applyRhoNeutralReweighting_runAHtoElecTau(process):
    process.load("TauAnalysis.RecoTools.vertexMultiplicityVsRhoPFNeutralReweight_cfi")
    if hasattr(process, "produceRecObjects"):
        process.produceRecObjects._seq = process.produceRecObjects._seq \
          * process.produceVertexMultiplicityVsRhoPFNeutralReweights

    _addEventWeighAHtoElecTau(process, "vertexMultiplicityVsRhoPFNeutralReweight")

#--------------------------------------------------------------------------------
# W --> tau nu channel
#--------------------------------------------------------------------------------

def _addEventWeightWtoTauNu(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, ["analyzeWtoTauNuEvents"],
        srcEventWeight, applyAfterFilterName)
    
def _addEventWeightWtoTauNu_bgEstTemplate(process, srcEventWeight, applyAfterFilterName = "*"):

    _addEventWeight(
        process, [ "analyzeEventsBgEstQCDEnriched" ],
        srcEventWeight, applyAfterFilterName)        

def applyVertexMultiplicityReweighting_runWtoTauNu(process):

    process.load("TauAnalysis.RecoTools.vertexMultiplicityReweight_cfi")
    if hasattr(process, "producePatTupleWtoTauNuSpecific"):
        process.producePatTupleWtoTauNuSpecific._seq = process.producePatTupleWtoTauNuSpecific._seq * process.selectedPrimaryVerticesTrackPtSumGt10 * process.vertexMultiplicityReweight
        
    _addEventWeightWtoTauNu(process, "vertexMultiplicityReweight")
                        

def applyVertexMultiplicityReweighting_runWtoTauNu_bgEstTemplate(process):

    applyVertexMultiplicityReweighting_runWtoTauNu(process)
    
    _addEventWeightWtoTauNu_bgEstTemplate(process, "vertexMultiplicityReweight")

def applyTauMetTriggerEfficiencyCorrection_runWtoTauNu(process):
    process.load("TauAnalysis.RecoTools.tauMetTriggerEfficiencyCorrection_cfi")
    if hasattr(process, "producePatTupleWtoTauNuSpecific"):
        process.producePatTupleWtoTauNuSpecific._seq = process.producePatTupleWtoTauNuSpecific._seq * process.tauMetTriggerEfficiencyCorrection

    _addEventWeightWtoTauNu(process,"tauMetTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")
    
def applyElectronTriggerEfficiencyCorrection_runWtoTauNu(process):
    process.load("TauAnalysis.RecoTools.electronTriggerEfficiencyCorrection_cfi")
    if hasattr(process,"producePatTupleWtoTauNuSpecific"):
        process.producePatTupleWtoTauNuSpecific._seq = process.producePatTupleWtoTauNuSpecific._seq * process.electronTriggerEfficiencyCorrection

    _addEventWeightWtoTauNu(process,"electronTriggerEfficiencyCorrection", applyAfterFilterName = "evtSelTrigger")
