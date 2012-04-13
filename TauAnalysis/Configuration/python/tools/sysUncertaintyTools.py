import FWCore.ParameterSet.Config as cms
import copy
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.RecoTools.patLeptonSelection_cff import *
from TauAnalysis.RecoTools.patJetSelection_cff import patJetSelConfigurator
from TauAnalysis.RecoTools.patJetSelectionForAHtoMuTau_cff import \
     patJetSelConfiguratorForAHtoMuTau, patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation
from TauAnalysis.RecoTools.patJetSelectionForAHtoElecTau_cff import \
     patJetSelConfiguratorForAHtoElecTauJetTag, patJetSelConfiguratorForAHtoElecTauBtag
from TauAnalysis.RecoTools.patJetSelectionForWTauNu_cff import patJetSelConfiguratorForWTauNu, patJetSelConfiguratorForWTauNu2
from TauAnalysis.RecoTools.patPFMetSelection_cff import patPFMETSelConfigurator

from TauAnalysis.CandidateTools.muTauPairProduction_cff import \
     muTauPairProdConfigurator, muTauPairProdConfiguratorLooseMuonIsolation
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import elecTauPairProdConfigurator
from TauAnalysis.CandidateTools.tauNuPairProduction_cff import tauNuPairProdConfigurator
from TauAnalysis.CandidateTools.tauNuPairSelection_cfi import patTauNuPairSelConfigurator
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import \
     patElecTauPairSelConfiguratorOS, patElecTauPairSelConfiguratorLooseElectronIsolationOS, \
     patElecTauPairSelConfiguratorSS, patElecTauPairSelConfiguratorLooseElectronIsolationSS, \
     patMuTauPairSelConfiguratorOS, patMuTauPairSelConfiguratorLooseMuonIsolationOS, \
     patMuTauPairSelConfiguratorSS, patMuTauPairSelConfiguratorLooseMuonIsolationSS
from TauAnalysis.CandidateTools.muTauPairSelectionForAHtoMuTau_cff import \
     patMuTauPairSelConfiguratorForAHtoMuTauOS, patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS, \
     patMuTauPairSelConfiguratorForAHtoMuTauSS, patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationSS
from TauAnalysis.CandidateTools.elecTauPairSelectionForAHtoElecTau_cff import \
     patElecTauPairSelConfiguratorForAHtoElecTauOS, patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationOS, \
     patElecTauPairSelConfiguratorForAHtoElecTauSS, patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationSS
from TauAnalysis.CandidateTools.vbfEventProductionForAHtoElecTau_cff import \
        vbfEventProdConfiguratorForAHtoElecTau
from TauAnalysis.CandidateTools.vbfEventSelectionForAHtoElecTau_cff import \
        vbfEventHypothesesSelConfiguratorForAHtoElecTau
from TauAnalysis.CandidateTools.htRatio_cfi import htRatioProdConfigurator, htRatioSelConfigurator
from TauAnalysis.CandidateTools.metTopology_cfi import * 

from TauAnalysis.Configuration.selectZtoMuTau_cff import \
     zToMuTauEventSelConfiguratorOS, zToMuTauEventSelConfiguratorSS
from TauAnalysis.Configuration.selectZtoMuTau_factorized_cff import \
     zToMuTauEventSelConfiguratorLooseMuonIsolationOS, zToMuTauEventSelConfiguratorLooseMuonIsolationSS
from TauAnalysis.Configuration.selectAHtoMuTau_cff import \
     ahToMuTauEventSelConfiguratorOS, ahToMuTauEventSelConfiguratorSS
from TauAnalysis.Configuration.selectAHtoMuTau_factorized_cff import \
     ahToMuTauEventSelConfiguratorLooseMuonIsolationOS, ahToMuTauEventSelConfiguratorLooseMuonIsolationSS
from TauAnalysis.Configuration.selectZtoElecTau_cff import \
     zToElecTauEventSelConfiguratorOS, zToElecTauEventSelConfiguratorSS
from TauAnalysis.Configuration.selectAHtoElecTau_cff import \
     ahToElecTauEventSelConfiguratorOS, ahToElecTauEventSelConfiguratorSS
from TauAnalysis.Configuration.selectWtoTauNu_cff import wToTauNuEventSelConfigurator

from TauAnalysis.CandidateTools.tools.objProdConfigurator import objProdConfigurator
from TauAnalysis.CandidateTools.tools.objSelConfigurator import objSelConfigurator
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer
from TauAnalysis.Configuration.tools.factorizationTools import replaceSysAnalyzerModules
from TauAnalysis.GenSimTools.sysErrGenEventReweights_cfi import *
from TauAnalysis.RecoTools.tools.configureZllRecoilCorrection import configureZllRecoilCorrection

#--------------------------------------------------------------------------------
# generic utility functions for factorization
# usable for all channels
#--------------------------------------------------------------------------------

def removeModules(process, sequenceName, moduleNamePattern, pyNameSpace):
    # auxiliary function to remove from sequence
    # all modules matching pattern of module names passed as function argument

    moduleNamePattern_regexp = re.compile(moduleNamePattern)

    if hasattr(process, sequenceName):
        sequence = getattr(process, sequenceName)

        modules = listModules(sequence)

        for module in modules:
            if hasattr(module, "label"):
                moduleName = module.label()

                if moduleNamePattern_regexp.match(moduleName):
                    sequence.remove(module)

def addBoolEventSelFlagProducer(process, moduleName, expSysUncertainties, sequenceName):

    if hasattr(process, moduleName) and hasattr(process, sequenceName):

        module = getattr(process, moduleName)
        sequence = getattr(process, sequenceName)

        for expSysUncertainty in expSysUncertainties:
            module_systematic = copy.deepcopy(module)

            flags = getattr(module, "flags")
            flags_systematic = []

            for flag in flags:

                flagModuleLabel_systematic = composeModuleName([ flag.getModuleLabel(), expSysUncertainty ])

                if hasattr(process, flagModuleLabel_systematic):
                    flag_systematic = copy.deepcopy(flag)
                    flag_systematic.setModuleLabel(flagModuleLabel_systematic)
                    flags_systematic.append(flag_systematic)
                else:
                    flags_systematic.append(flag)

            setattr(module_systematic, "flags", cms.VInputTag(flags_systematic))

            moduleName_systematic = composeModuleName([ moduleName, expSysUncertainty ])
            setattr(process, moduleName_systematic, module_systematic)

            module_systematic = getattr(process, moduleName_systematic)

            sequence._seq = sequence._seq * module_systematic

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# in PAT-tuple production
#--------------------------------------------------------------------------------

def disableSysUncertainties_patTupleProduction(process):
    #print("<disableSysUncertainties_patTupleProduction>:")

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to Z --> muon + tau-jet channel
#--------------------------------------------------------------------------------

def enableSysUncertainties_runZtoMuTau(process):
    print("<enableSysUncertainties_runZtoMuTau>:")
    print("--> **enabling** estimation of systematic uncertainties...")

    pyNameSpace = None

    process.produceGenObjects += process.produceSysErrGenEventReweights

    process.load("TauAnalysis.RecoTools.patLeptonSystematics_cff")
    process.producePatTupleZtoMuTauSpecific.replace(process.selectPatMuons, process.prodSmearedMuons + process.selectPatMuons)
    process.producePatTupleZtoMuTauSpecific.replace(process.selectPatTaus, process.prodSmearedTaus + process.selectPatTaus)

    process.load("TauAnalysis.RecoTools.patJetSystematics_cff")
    process.producePatTupleZtoMuTauSpecific.replace(process.selectPatJets, process.prodSmearedJets + process.selectPatJets)

    process.produceEventSelFlagsZtoMuTauOS = \
      zToMuTauEventSelConfiguratorOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoMuTauLooseMuonIsolationOS = \
      zToMuTauEventSelConfiguratorLooseMuonIsolationOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoMuTauSS = \
      zToMuTauEventSelConfiguratorSS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoMuTauLooseMuonIsolationSS = \
      zToMuTauEventSelConfiguratorLooseMuonIsolationSS.configure(process = process, estimateSysUncertainties = True)

    setattr(patMuonSelConfigurator, "systematics", muonSystematics)
    process.selectPatMuons = patMuonSelConfigurator.configure(process = process)

    setattr(patMuonSelConfiguratorLooseIsolation, "systematics", muonSystematics)
    process.selectPatMuonsLooseIsolation = patMuonSelConfiguratorLooseIsolation.configure(process = process)

    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectPatTaus = patTauSelConfigurator.configure(process = process)

    setattr(patTauSelConfiguratorForMuTau, "systematics", tauSystematics)
    process.selectPatTausForMuTau = patTauSelConfiguratorForMuTau.configure(process = process)

    setattr(patJetSelConfigurator, "systematics", jetSystematics)
    process.selectPatJets = patJetSelConfigurator.configure(process = process)

    # produce collections of pat::(PF)Jets
    #  o of Pt > 10 GeV for MET Type I correction uncertainties
    #  o of Pt < 10 GeV for MET Type II ("unclustered energy") correction uncertainties
    #
    # NOTE: the "unclustered energy" is smeared by an additional resolution uncertainty of 10% * sum(Et) of jets with Pt < 10 GeV
    #
    process.selectedPatJetsForMEtTypeIcorr = cms.EDFilter("PATJetSelector",
        src = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),                                    
        cut = cms.string('pt > 10.'), 
        filter = cms.bool(False)
    )

    process.selectedPatJetsForMEtTypeIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
    )
    process.selectedPatJetsForMEtTypeIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
    )

    process.selectedPatJetsForMEtTypeIIcorr = process.selectedPatJetsForMEtTypeIcorr.clone(
        cut = cms.string('pt < 10.')
    )

    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
    )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp.jecUncertaintyValue = cms.double(0.10)
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
    )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown.jecUncertaintyValue = cms.double(0.10)

    process.producePatJetsForMEtCorrUncertainty = cms.Sequence(
        process.selectedPatJetsForMEtTypeIcorr
       * process.selectedPatJetsForMEtTypeIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIcorrSysJetEnDown
       * process.selectedPatJetsForMEtTypeIIcorr
       * process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown
    )
    process.produceMuTauPairsAll += process.producePatJetsForMEtCorrUncertainty
    
    smearMEtUnclustedEnergyResolution = cms.double(0.10)

    process.smearedMET = cms.EDProducer("SmearedMETProducer",
        src = cms.InputTag('patPFMETs'),
        smearedParticles = cms.PSet()
    )
    smearedMETconfigurator = objProdConfigurator(
        process.smearedMET,
        pyModuleName = __name__,
        systematics = {
            "sysMuonPtUp" : {
                "smearedParticles.muons.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPcumulative'),
                "smearedParticles.muons.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtUpCumulative')
            },
            "sysMuonPtDown" : {
                "smearedParticles.muons.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPcumulative'),
                "smearedParticles.muons.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtDownCumulative')
            },
            "sysTauJetEnUp" : {
                "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
                "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnUpCumulative')
            },
            "sysTauJetEnDown" : {
                "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
                "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnDownCumulative')
            },
            "sysJetEnUp" : {
                "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnUp'),
                "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnUp'),
                "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
            },
            "sysJetEnDown" : {
                "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnDown'),
                "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnDown'),
                "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
            }
        }
    )
    process.prodSmearedMET = smearedMETconfigurator.configure(process = process)
    process.produceMuTauPairsAll += process.prodSmearedMET 

    setattr(muTauPairProdConfigurator, "systematics", {
        "sysMuonPtUp" : {
            "srcLeg1" : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysMuonPtUp')
        },
        "sysMuonPtDown" : {
            "srcLeg1" : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysMuonPtDown')
        },
        "sysTauJetEnUp" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
        },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnDown')
        },
        "sysJetEnUp" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnUp')
        },
        "sysJetEnDown" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnDown')
        }
    })
    process.produceMuTauPairs = muTauPairProdConfigurator.configure(process = process)
    process.produceMuTauPairsAll.replace(process.produceMuTauPairs, process.allMuTauPairs)
    process.produceMuTauPairsAll += process.produceMuTauPairs

    # apply Z-recoil corrections to shifted/smeared diTau objects
    if hasattr(process, "allMuTauPairsZllRecoilCorrected"):
        for sysName, src in muTauPairSystematics.items():

            if sysName.find("ZllRecoilCorrection") != -1:
                continue;

            configZllRecoilCorrection = configureZllRecoilCorrection(process, src.getModuleLabel(), "ZllRecoilCorrectionMuTauPair")
            process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
            muTauPairSystematics[sysName] = configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrectedName']

    # add uncertainties on Z-recoil correction
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairs",
                                                             "ZllRecoilCorrectionMuTauPair", +1., "SysUp")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairs",
                                                             "ZllRecoilCorrectionMuTauPair", -1., "SysDown")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']

    process.smearedMETlooseMuonIsolation = process.smearedMET.clone()
    smearedMETconfiguratorLooseMuonIsolation = objProdConfigurator(
        process.smearedMETlooseMuonIsolation,
        pyModuleName = __name__,
        systematics = {
            "sysMuonPtUp" : {
                "smearedParticles.muons.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
                "smearedParticles.muons.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtUpCumulative')
            },
            "sysMuonPtDown" : {
                "smearedParticles.muons.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
                "smearedParticles.muons.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtDownCumulative')
            },
            "sysTauJetEnUp" : {
                "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
                "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnUpCumulative')
            },
            "sysTauJetEnDown" : {
                "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
                "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnDownCumulative')
            },
            "sysJetEnUp" : {
                "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnUp'),
                "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnUp'),
                "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
            },
            "sysJetEnDown" : {
                "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnDown'),
                "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnDown'),
                "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
            }
        }
    )
    process.prodSmearedMETlooseMuonIsolation = smearedMETconfiguratorLooseMuonIsolation.configure(process = process)
    process.produceMuTauPairsAll += process.prodSmearedMETlooseMuonIsolation
 
    setattr(muTauPairProdConfiguratorLooseMuonIsolation, "systematics", {
        "sysMuonPtUp" : {
            "srcLeg1" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysMuonPtUp')
        },
        "sysMuonPtDown" : {
            "srcLeg1" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysMuonPtDown')
        },
        "sysTauJetEnUp" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
        },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauElectronVetoSysTauJetEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnDown')
        },
        "sysJetEnUp" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnUp')
        },
        "sysJetEnDown" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnDown')
        }
    })
    process.produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(process = process)
    process.produceMuTauPairsAll.replace(process.produceMuTauPairsLooseMuonIsolation, process.allMuTauPairsLooseMuonIsolation)
    process.produceMuTauPairsAll += process.produceMuTauPairsLooseMuonIsolation

    # apply Z-recoil corrections to shifted/smeared diTau objects
    if hasattr(process, "allMuTauPairsLooseMuonIsolationZllRecoilCorrected"):
        for sysName, src in muTauPairSystematicsLooseMuonIsolation.items():

            if sysName.find("ZllRecoilCorrection") != -1:
                continue;

            configZllRecoilCorrection = configureZllRecoilCorrection(process, src.getModuleLabel(), "ZllRecoilCorrectionMuTauPair")
            process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
            muTauPairSystematicsLooseMuonIsolation[sysName] = configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrectedName']

    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairsLooseMuonIsolation",
                                                             "ZllRecoilCorrectionMuTauPair", +1., "SysUp")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairsLooseMuonIsolation",
                                                             "ZllRecoilCorrectionMuTauPair", -1., "SysDown")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']

    setattr(patMuTauPairSelConfiguratorOS, "systematics", muTauPairSystematics)
    if hasattr(process, "allMuTauPairsZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorOS, "src", "allMuTauPairsZllRecoilCorrected")
    process.selectMuTauPairsOS = patMuTauPairSelConfiguratorOS.configure(process = process)
    if hasattr(process, "allMuTauPairsZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorSS, "src", "allMuTauPairsZllRecoilCorrected")
    setattr(patMuTauPairSelConfiguratorSS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsSS = patMuTauPairSelConfiguratorSS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorLooseMuonIsolationOS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    if hasattr(process, "allMuTauPairsLooseMuonIsolationZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorLooseMuonIsolationOS, "src", "allMuTauPairsLooseMuonIsolationZllRecoilCorrected")
    process.selectMuTauPairsLooseMuonIsolationOS = patMuTauPairSelConfiguratorLooseMuonIsolationOS.configure(process = process)
    setattr(patMuTauPairSelConfiguratorLooseMuonIsolationSS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    if hasattr(process, "allMuTauPairsLooseMuonIsolationZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorLooseMuonIsolationSS, "src", "allMuTauPairsLooseMuonIsolationZllRecoilCorrected")
    process.selectMuTauPairsLooseMuonIsolationSS = patMuTauPairSelConfiguratorLooseMuonIsolationSS.configure(process = process)

    if hasattr(process, "isRecZtoMuTau"):
        expSysUncertainties = getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
              muTauPairSystematics,
              jetSystematics ]
        )
        addBoolEventSelFlagProducer(process, "isRecZtoMuTau", expSysUncertainties, "selectZtoMuTauEvents")

    if hasattr(process, "analyzeZtoMuTauEventsOS"):
        process.analyzeZtoMuTauEventsOS.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation"):
        process.analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation"):
        process.analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation.estimateSysUncertainties = cms.bool(True)

    if hasattr(process, "analyzeZtoMuTauEventsSS"):
        process.analyzeZtoMuTauEventsSS.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation"):
        process.analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation"):
        process.analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation.estimateSysUncertainties = cms.bool(True)

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to Z --> electron + tau-jet channel
#--------------------------------------------------------------------------------

def disableSysUncertainties_runZtoElecTau(process):
    #print("<disableSysUncertainties_runZtoElecTau>:")

    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectZtoElecTauEvents", moduleNamePattern, pyNameSpace)
    if hasattr(process, "selectZtoElecTauEventsLooseElectronIsolation"):
        removeModules(process, "selectZtoElecTauEventsLooseElectronIsolation", moduleNamePattern, pyNameSpace)

def enableSysUncertainties_runZtoElecTau(process):
    print("<enableSysUncertainties_runZtoElecTau>:")
    print("--> **enabling** estimation of systematic uncertainties...")

    pyNameSpace = None

    process.produceGenObjects += process.produceSysErrGenEventReweights

    process.load("TauAnalysis.RecoTools.patLeptonSystematics_cff")
    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatElectrons, process.prodSmearedElectrons + process.selectPatElectrons)
    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatTaus, process.prodSmearedTaus + process.selectPatTaus)

    process.load("TauAnalysis.RecoTools.patJetSystematics_cff")
    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatJets, process.prodSmearedJets + process.selectPatJets)

    # save trigger accept paths
    hltPaths = process.Trigger.selectors[0].hltAcceptPaths

    process.produceEventSelFlagsZtoElecTauOSwithSys = \
            zToElecTauEventSelConfiguratorOS.configure(process = process, estimateSysUncertainties = True)
    #process.produceEventSelFlagsZtoElecTauLooseElectronIsolationOSwithSys = \
    #        zToElecTauEventSelConfiguratorLooseElectronIsolationOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoElecTauSSwithSys = \
            zToElecTauEventSelConfiguratorSS.configure(process = process, estimateSysUncertainties = True)
    #process.produceEventSelFlagsZtoElecTauLooseElectronIsolationSSwithSys = \
    #        zToElecTauEventSelConfiguratorLooseElectronIsolationSS.configure(process = process, estimateSysUncertainties = True)
    
    # correct hlt accept paths
    process.Trigger.selectors[0].hltAcceptPaths = hltPaths

    process.produceEventSelFlagsZtoElecTau = \
            cms.Sequence( process.produceEventSelFlagsZtoElecTauOSwithSys + process.produceEventSelFlagsZtoElecTauSSwithSys )
    #process.produceEventSelFlagsZtoElecTauLooseElectronIsolation = \
    #        cms.Sequence( process.produceEventSelFlagsZtoElecTauLooseElectronIsolationOSwithSys + process.produceEventSelFlagsZtoElecTauLooseElectronIsolationSSwithSys )

    # for sequences to be reconfigured, replace with dummy sequence in parent sequences
    process.tmp = cms.Sequence()
    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatElectronsForElecTau, process.tmp)
    setattr(patElectronSelConfiguratorForElecTau, "systematics", electronSystematics)
    process.selectPatElectronsForElecTauWithSys = patElectronSelConfiguratorForElecTau.configure(process = process)
    process.producePatTupleZtoElecTauSpecific.replace(process.tmp, process.selectPatElectronsForElecTauWithSys)

    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatTaus,process.tmp)
    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectPatTausWithSys = patTauSelConfigurator.configure(process = process)
    process.producePatTupleZtoElecTauSpecific.replace(process.tmp,process.selectPatTausWithSys)

    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatTausForElecTau,process.tmp)
    setattr(patTauSelConfiguratorForElecTau, "systematics", tauSystematics)
    process.selectPatTausForElecTauWithSys = patTauSelConfiguratorForElecTau.configure(process = process)
    process.producePatTupleZtoElecTauSpecific.replace(process.tmp,process.selectPatTausForElecTauWithSys)

    process.producePatTupleZtoElecTauSpecific.replace(process.selectPatJets,process.tmp)
    setattr(patJetSelConfigurator, "systematics", jetSystematics)
    process.selectPatJetsWithSys = patJetSelConfigurator.configure(process = process)
    process.producePatTupleZtoElecTauSpecific.replace(process.tmp,process.selectPatJetsWithSys)

    # produce collections of pat::(PF)Jets
    #  o of Pt > 10 GeV for MET Type I correction uncertainties
    #  o of Pt < 10 GeV for MET Type II ("unclustered energy") correction uncertainties
    #
    # NOTE: the "unclustered energy" is smeared by an additional resolution uncertainty of 10% * sum(Et) of jets with Pt < 10 GeV
    #
    process.selectedPatJetsForMEtTypeIcorr = cms.EDFilter("PATJetSelector",
            src = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
            cut = cms.string('pt > 10.'),
            filter = cms.bool(False)
            )

    process.selectedPatJetsForMEtTypeIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
            )
    process.selectedPatJetsForMEtTypeIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
            )

    process.selectedPatJetsForMEtTypeIIcorr = process.selectedPatJetsForMEtTypeIcorr.clone(
            cut = cms.string('pt < 10.')
            )

    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
            )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp.jecUncertaintyValue = cms.double(0.10)
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
            )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown.jecUncertaintyValue = cms.double(0.10)

    process.producePatJetsForMEtCorrUncertainty = cms.Sequence(
            process.selectedPatJetsForMEtTypeIcorr
            * process.selectedPatJetsForMEtTypeIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIcorrSysJetEnDown
            * process.selectedPatJetsForMEtTypeIIcorr
            * process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown
            )
    process.produceElecTauPairsAll += process.producePatJetsForMEtCorrUncertainty

    smearMEtUnclustedEnergyResolution = cms.double(0.10)


    process.smearedMET = cms.EDProducer("SmearedMETProducer",
            src = cms.InputTag('patPFMETs'),
            smearedParticles = cms.PSet()
            )
    smearedMETconfigurator = objProdConfigurator(
            process.smearedMET,
            pyModuleName = __name__,
            systematics = {
                "sysElectronEnUp" : {        
                    "smearedParticles.electrons.srcOriginal" : cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
                    "smearedParticles.electrons.srcSmeared"  : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnUpCumulative')
                    },
                "sysElectronEnDown" : {
                    "smearedParticles.electrons.srcOriginal" : cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
                    "smearedParticles.electrons.srcSmeared"  : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnDownCumulative')
                    },
                "sysTauJetEnUp" : {
                    "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                    "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnUpCumulative')
                    },
                "sysTauJetEnDown" : {
                    "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                    "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnDownCumulative')
                    }, 
                "sysJetEnUp" : {
                    "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                    "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnUp'),
                    "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                    "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnUp'),
                    "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
                    },
                "sysJetEnDown" : {
                    "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                    "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnDown'),
                    "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                    "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnDown'),
                    "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
                    }
                }
            )
    process.prodSmearedMET = smearedMETconfigurator.configure(process = process)
    process.produceElecTauPairsAll += process.prodSmearedMET

    setattr(elecTauPairProdConfigurator, "systematics", {
        "sysElectronEnUp" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronEnUp')
            },
        "sysElectronEnDown" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronEnDown')           
            },
        "sysTauJetEnUp" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
            },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnDown')
            }, 
        "sysJetEnUp" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnUp')
            },
        "sysJetEnDown" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnDown')
            }
        })
    process.produceElecTauPairs = elecTauPairProdConfigurator.configure(process = process)
    process.produceElecTauPairsAll.replace(process.produceElecTauPairs, process.allElecTauPairs)
    process.produceElecTauPairsAll += process.produceElecTauPairs

    # apply Z-recoil corrections to shifted/smeared diTau objects
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        for sysName, src in elecTauPairSystematics.items():

            if sysName.find("ZllRecoilCorrection") != -1:
                continue;
            
            # get SV fit configuration parameter set
            nSVfit = getattr(process.allElecTauPairsZllRecoilCorrected, "nSVfit")

            configZllRecoilCorrection = configureZllRecoilCorrection(process, src.getModuleLabel(), "ZllRecoilCorrectionElecTauPair")
            #print 'adding sequence ' + str(configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']) + ' to produceElecTauPairsAll'
            process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
            elecTauPairSystematics[sysName] = configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrectedName']
            # add SV fit configuration parameter set to new recoil-corrected 
            module = getattr(process,elecTauPairSystematics[sysName])
            setattr(module,"nSVfit",nSVfit)
            # update SV fit configuration to include appropriately shifted input electron or tau collection
            module.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg1.src = module.srcLeg1
            module.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg2.src = module.srcLeg2

    # add uncertainties on Z-recoil correction
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allElecTauPairs",
            "ZllRecoilCorrectionElecTauPair", +1., "SysUp")
    process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allElecTauPairs",
            "ZllRecoilCorrectionElecTauPair", -1., "SysDown")
    process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']


    setattr(patElecTauPairSelConfiguratorOS, "systematics", elecTauPairSystematics)
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        setattr(patElecTauPairSelConfiguratorOS, "src", "allElecTauPairsZllRecoilCorrected")
    process.selectElecTauPairsOSwithSys = patElecTauPairSelConfiguratorOS.configure(process = process)
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        setattr(patElecTauPairSelConfiguratorSS, "src", "allElecTauPairsZllRecoilCorrected")
    setattr(patElecTauPairSelConfiguratorSS, "systematics", elecTauPairSystematics)
    process.selectElecTauPairsSSwithSys = patElecTauPairSelConfiguratorSS.configure(process = process)

    process.selectElecTauPairs = cms.Sequence(process.selectElecTauPairsOSwithSys + process.selectElecTauPairsSSwithSys)

    if hasattr(process, "isRecZtoElecTau"):        
        expSysUncertainties = getSysUncertaintyNames(
                [ electronSystematics,
                    tauSystematics,
                    elecTauPairSystematics,
                    jetSystematics ]
                )
        addBoolEventSelFlagProducer(process, "isRecZtoElecTau", expSysUncertainties, "selectZtoElecTauEvents")

    if hasattr(process, "analyzeZtoElecTauEventsOS"):
        process.analyzeZtoElecTauEventsOS.estimateSysUncertainties = cms.bool(True)

    if hasattr(process, "analyzeZtoElecTauEventsSS"):
        process.analyzeZtoElecTauEventsSS.estimateSysUncertainties = cms.bool(True)

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to Z --> electron + muon channel
#--------------------------------------------------------------------------------

def disableSysUncertainties_runZtoElecMu(process):
    #print("<disableSysUncertainties_runZtoElecMu>:")

    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectZtoElecMuEvents", moduleNamePattern, pyNameSpace)
    if hasattr(process, "selectZtoElecMuEventsLooseElectronIsolation"):
        removeModules(process, "selectZtoElecMuEventsLooseElectronIsolation", moduleNamePattern, pyNameSpace)

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to W --> tau-jet + nu channel
#--------------------------------------------------------------------------------

def disableSysUncertainties_runWtoTauNu(process):
    #print("<disableSysUncertainties_runWtoTauNu>:")

    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectWtoTauNuEvents", moduleNamePattern, pyNameSpace)
    if hasattr(process, "selectWtoTauNuEventsLooseIsolation"):
        removeModules(process, "selectWtoTauNuEventsLooseIsolation", moduleNamePattern, pyNameSpace)

#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to MSSM Higgs --> muon + tau-jet channel
#--------------------------------------------------------------------------------

def enableSysUncertainties_runAHtoMuTau(process):
    print("<enableSysUncertainties_runAHtoMuTau>:")
    print("--> **enabling** estimation of systematic uncertainties...")

    enableSysUncertainties_runZtoMuTau(process)

    process.produceEventSelFlagsAHtoMuTauOS = \
      ahToMuTauEventSelConfiguratorOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsAHtoMuTauLooseMuonIsolationOS = \
      ahToMuTauEventSelConfiguratorLooseMuonIsolationOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsAHtoMuTauSS = \
      ahToMuTauEventSelConfiguratorSS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsAHtoMuTauLooseMuonIsolationSS = \
      ahToMuTauEventSelConfiguratorLooseMuonIsolationSS.configure(process = process, estimateSysUncertainties = True)
  
    setattr(patMuTauPairSelConfiguratorForAHtoMuTauOS, "systematics", muTauPairSystematics)
    if hasattr(process, "allMuTauPairsZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorForAHtoMuTauOS, "src", "allMuTauPairsZllRecoilCorrected")
    process.selectMuTauPairsForAHtoMuTauOS = patMuTauPairSelConfiguratorForAHtoMuTauOS.configure(process = process)
    setattr(patMuTauPairSelConfiguratorForAHtoMuTauSS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsForAHtoMuTauSS = patMuTauPairSelConfiguratorForAHtoMuTauSS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    if hasattr(process, "allMuTauPairsLooseMuonIsolationZllRecoilCorrected"):
        setattr(patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS, "src", "allMuTauPairsLooseMuonIsolationZllRecoilCorrected")
    process.selectMuTauPairsForAHtoMuTauLooseMuonIsolationOS = \
      patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS.configure(process = process)
    setattr(patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationSS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    process.selectMuTauPairsForAHtoMuTauLooseMuonIsolationSS = \
      patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationSS.configure(process = process)

    setattr(patJetSelConfiguratorForAHtoMuTau, "systematics", jetSystematics)
    process.selectPatJetsForAHtoMuTau = patJetSelConfiguratorForAHtoMuTau.configure(process = process)
    
    setattr(patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation, "systematics", jetSystematics)
    process.selectPatJetsForAHtoMuTauLooseMuonIsolation = \
      patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(process = process)

    #setattr(patTagJetSelConfiguratorForVBF, "systematics", jetSystematics)
    #process.selectPatTagJetsForVBF = patTagJetSelConfiguratorForVBF.configure(process = process)

    #setattr(patCentralJetSelConfiguratorForVBF, "systematics", jetSystematics)
    #process.selectPatCentralJetsForVBF = patCentralJetSelConfiguratorForVBF.configure(process = process)

    expSysUncertainties = getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
          muTauPairSystematics,
          jetSystematics ]
    )
    if hasattr(process, "isRecAHtoMuTauCentralJetVeto"):
        addBoolEventSelFlagProducer(process, "isRecAHtoMuTauCentralJetVeto", expSysUncertainties, "selectAHtoMuTauEvents")
    if hasattr(process, "isRecAHtoMuTauCentralJetBtag"):
        addBoolEventSelFlagProducer(process, "isRecAHtoMuTauCentralJetBtag", expSysUncertainties, "selectAHtoMuTauEvents")

    for sign in ['OS', 'SS']:
        for btag in ['_woBtag', '_wBtag']:
            for suffix in ['', '_factorizedWithMuonIsolation', '_factorizedWithoutMuonIsolation']:
                name = 'analyzeAHtoMuTauEvents' + sign + btag + suffix
                if hasattr(process, name):
                    getattr(process, name).estimateSysUncertainties = cms.bool(True)

    # update InputTags of systematic uncertainty histogram managers
    if hasattr(process, "sysUncertaintyHistManagerForMuTau"):
        sysUncertaintyHistManagerMuTauPairSystematics = {
            "sysMuonPtUp"                : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysMuonPtUpCumulative'),
            "sysMuonPtDown"              : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysMuonPtDownCumulative'),
            "sysTauJetEnUp"              : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysTauJetEnUpCumulative'),
            "sysTauJetEnDown"            : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysTauJetEnDownCumulative'),
            "sysJetEnUp"                 : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysJetEnUpCumulative'),
            "sysJetEnDown"               : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysJetEnDownCumulative'),
			"sysZllRecoilCorrectionUp"   : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysZllRecoilCorrectionUpCumulative'),
			"sysZllRecoilCorrectionDown" : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeSysZllRecoilCorrectionDownCumulative')
        }
        process.sysUncertaintyHistManagerForMuTau_updated = \
          copy.deepcopy(process.sysUncertaintyHistManagerForMuTau)
        process.sysUncertaintyHistManagerForMuTau_updated.histManagers[0].config.diTauCandidateSource = \
          cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative')
        process.sysUncertaintyHistManagerForMuTau_updated.histManagers[0].systematics.diTauCandidateSource = \
          getSysUncertaintyParameterSets([ sysUncertaintyHistManagerMuTauPairSystematics ])
        process.sysUncertaintyHistManagerForMuTau_updated.histManagers[1].config.diTauCandidateSource = \
          cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative')
        process.sysUncertaintyHistManagerForMuTau_updated.histManagers[1].systematics.diTauCandidateSource = \
          process.sysUncertaintyHistManagerForMuTau_updated.histManagers[0].systematics.diTauCandidateSource

        for genAnalyzerModuleName in [ "analyzeAHtoMuTauEventsOS_woBtag", 
                                       "analyzeAHtoMuTauEventsOS_wBtag",
                                       "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation", 
                                       "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation",
                                       "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation",
                                       "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation" ]:
            if hasattr(process, genAnalyzerModuleName):
                print("--> replacing %s in %s" % ("sysUncertaintyHistManagerForMuTau", genAnalyzerModuleName))
                genAnalyzerMode = getattr(process, genAnalyzerModuleName) 
                replaceSysAnalyzerModules(genAnalyzerMode,
                  [ [ process.sysUncertaintyHistManagerForMuTau, process.sysUncertaintyHistManagerForMuTau_updated ] ]
                )   

    if hasattr(process, "sysUncertaintyHistManagerForMuTauLooseMuonIsolation"):
        sysUncertaintyHistManagerMuTauPairSystematicsLooseMuonIsolation = {
            "sysMuonPtUp"                : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysMuonPtUpCumulative'),
            "sysMuonPtDown"              : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysMuonPtDownCumulative'),
            "sysTauJetEnUp"              : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysTauJetEnUpCumulative'),
            "sysTauJetEnDown"            : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysTauJetEnDownCumulative'),
            "sysJetEnUp"                 : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysJetEnUpCumulative'),
            "sysJetEnDown"               : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysJetEnDownCumulative'),
            "sysZllRecoilCorrectionUp"   : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysZllRecoilCorrectionUpCumulative'),
            "sysZllRecoilCorrectionDown" : cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationSysZllRecoilCorrectionDownCumulative')
        }
        process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated = \
          copy.deepcopy(process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation)
        process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated.histManagers[0].config.diTauCandidateSource = \
          cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationCumulative')
        process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated.histManagers[0].systematics.diTauCandidateSource = \
          getSysUncertaintyParameterSets([ sysUncertaintyHistManagerMuTauPairSystematicsLooseMuonIsolation ])
        process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated.histManagers[1].config.diTauCandidateSource = \
          cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationCumulative')
        process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated.histManagers[1].systematics.diTauCandidateSource = \
          process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated.histManagers[0].systematics.diTauCandidateSource   

        for genAnalyzerModuleName in [ "analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation", 
                                       "analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation",
                                       "analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation",
                                       "analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation" ]:
            if hasattr(process, genAnalyzerModuleName):
                print("--> replacing %s in %s" % ("sysUncertaintyHistManagerForMuTauLooseMuonIsolation", genAnalyzerModuleName))
                genAnalyzerMode = getattr(process, genAnalyzerModuleName) 
                replaceSysAnalyzerModules(genAnalyzerMode,
                  [ [ process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation, process.sysUncertaintyHistManagerForMuTauLooseMuonIsolation_updated ] ]
                )   
    
#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to SM/MSSM Higgs --> electron + tau-jet channel
#--------------------------------------------------------------------------------

def enableSysUncertainties_runAHtoElecTau(process):
    print("<enableSysUncertainties_runAHtoElecTau>:")
    print("--> **enabling** estimation of systematic uncertainties...")

    pyNameSpace = None

    process.produceGenObjects += process.produceSysErrGenEventReweights

    # add smeared lepton production
    process.load("TauAnalysis.RecoTools.patLeptonSystematics_cff")
    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatElectrons, process.prodSmearedElectrons + process.selectPatElectrons)
    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatTaus, process.prodSmearedTaus + process.selectPatTaus)

    process.load("TauAnalysis.RecoTools.patJetSystematics_cff")
    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatJets, process.prodSmearedJets + process.selectPatJets)

    # Save trigger accept paths. If paths have been configured in the recoSampleDefinitions file,
    # those changes will be lost when 'process.Trigger' is regenerated by 'eventSelFlagProdConfigurator' objects
    hltPaths = process.Trigger.selectors[0].hltAcceptPaths
    
    # reconfigure event selection (boolean) collection production with systematics
    #   -- first SM categories 
    process.produceEventSelFlagsZtoElecTauOSwithSys = \
            zToElecTauEventSelConfiguratorOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoElecTauSSwithSys = \
            zToElecTauEventSelConfiguratorSS.configure(process = process, estimateSysUncertainties = True)
    #  -- next MSSM categories
    process.produceEventSelFlagsAHtoElecTauOSwithSys = \
            ahToElecTauEventSelConfiguratorOS.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsAHtoElecTauSSwithSys = \
            ahToElecTauEventSelConfiguratorSS.configure(process = process, estimateSysUncertainties = True)
    
    # correct hlt accept paths
    process.Trigger.selectors[0].hltAcceptPaths = hltPaths

    process.produceEventSelFlagsZtoElecTau = \
            cms.Sequence( process.produceEventSelFlagsZtoElecTauOSwithSys + process.produceEventSelFlagsZtoElecTauSSwithSys )
    process.produceEventSelFlagsAHtoElecTau = \
            cms.Sequence( process.produceEventSelFlagsAHtoElecTauOSwithSys + process.produceEventSelFlagsAHtoElecTauSSwithSys )

    # for sequences to be reconfigured, replace with dummy sequence in parent sequences
    process.tmp = cms.Sequence()
    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatElectronsForElecTau, process.tmp)
    setattr(patElectronSelConfiguratorForElecTau, "systematics", electronSystematics)
    process.selectPatElectronsForElecTauWithSys = patElectronSelConfiguratorForElecTau.configure(process = process)
    process.producePatTupleAHtoElecTauSpecific.replace(process.tmp, process.selectPatElectronsForElecTauWithSys)

    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatTaus,process.tmp)
    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectPatTausWithSys = patTauSelConfigurator.configure(process = process)
    process.producePatTupleAHtoElecTauSpecific.replace(process.tmp,process.selectPatTausWithSys)

    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatTausForElecTau,process.tmp)
    setattr(patTauSelConfiguratorForElecTau, "systematics", tauSystematics)
    process.selectPatTausForElecTauWithSys = patTauSelConfiguratorForElecTau.configure(process = process)
    process.producePatTupleAHtoElecTauSpecific.replace(process.tmp,process.selectPatTausForElecTauWithSys)

    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatJets,process.tmp)
    setattr(patJetSelConfigurator, "systematics", jetSystematics)
    process.selectPatJetsWithSys = patJetSelConfigurator.configure(process = process)
    process.producePatTupleAHtoElecTauSpecific.replace(process.tmp,process.selectPatJetsWithSys)

    # add systematics to jet selection for SM and MSSM analyses
    process.tmp = cms.Sequence()
    process.producePatTupleAHtoElecTauSpecific.replace(process.selectPatJetsForAHtoElecTau, process.tmp)

    setattr(patJetSelConfiguratorForAHtoElecTauJetTag, "systematics", jetSystematics)
    process.selectPatJetsForAHtoElecTauJetTagWithSys = patJetSelConfiguratorForAHtoElecTauJetTag.configure(process = process)

    setattr(patJetSelConfiguratorForAHtoElecTauBtag, "systematics", jetSystematics)
    process.selectPatJetsForAHtoElecTauBtagWithSys = patJetSelConfiguratorForAHtoElecTauBtag.configure(process = process)

    process.producePatTupleAHtoElecTauSpecific.replace(process.tmp, 
            process.selectPatJetsForAHtoElecTauBtagWithSys + process.selectPatJetsForAHtoElecTauJetTagWithSys)

    # produce collections of pat::(PF)Jets
    #  o of Pt > 10 GeV for MET Type I correction uncertainties
    #  o of Pt < 10 GeV for MET Type II ("unclustered energy") correction uncertainties
    #
    # NOTE: the "unclustered energy" is smeared by an additional resolution uncertainty of 10% * sum(Et) of jets with Pt < 10 GeV
    #
    process.selectedPatJetsForMEtTypeIcorr = cms.EDFilter("PATJetSelector",
            src = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
            cut = cms.string('pt > 10.'),
            filter = cms.bool(False)
            )

    process.selectedPatJetsForMEtTypeIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
            )
    process.selectedPatJetsForMEtTypeIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIcorr')
            )

    process.selectedPatJetsForMEtTypeIIcorr = process.selectedPatJetsForMEtTypeIcorr.clone(
            cut = cms.string('pt < 10.')
            )

    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp = process.patJetsJECshiftUp.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
            )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp.jecUncertaintyValue = cms.double(0.10)
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown = process.patJetsJECshiftDown.clone(
            src = cms.InputTag('selectedPatJetsForMEtTypeIIcorr')
            )
    process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown.jecUncertaintyValue = cms.double(0.10)

    process.producePatJetsForMEtCorrUncertainty = cms.Sequence(
            process.selectedPatJetsForMEtTypeIcorr
            * process.selectedPatJetsForMEtTypeIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIcorrSysJetEnDown
            * process.selectedPatJetsForMEtTypeIIcorr
            * process.selectedPatJetsForMEtTypeIIcorrSysJetEnUp * process.selectedPatJetsForMEtTypeIIcorrSysJetEnDown
            )
    process.produceElecTauPairsAll += process.producePatJetsForMEtCorrUncertainty

    smearMEtUnclustedEnergyResolution = cms.double(0.10)


    process.smearedMET = cms.EDProducer("SmearedMETProducer",
            src = cms.InputTag('patPFMETs'),
            smearedParticles = cms.PSet()
            )
    smearedMETconfigurator = objProdConfigurator(
            process.smearedMET,
            pyModuleName = __name__,
            systematics = {
                "sysElectronEnUp" : {        
                    "smearedParticles.electrons.srcOriginal" : cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
                    "smearedParticles.electrons.srcSmeared"  : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnUpCumulative')
                    },
                "sysElectronEnDown" : {
                    "smearedParticles.electrons.srcOriginal" : cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
                    "smearedParticles.electrons.srcSmeared"  : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnDownCumulative')
                    },
                "sysTauJetEnUp" : {
                    "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                    "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnUpCumulative')
                    },
                "sysTauJetEnDown" : {
                    "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                    "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnDownCumulative')
                    }, 
                "sysJetEnUp" : {
                    "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                    "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnUp'),
                    "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                    "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnUp'),
                    "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
                    },
                "sysJetEnDown" : {
                    "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIcorr'),
                    "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIcorrSysJetEnDown'),
                    "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeIIcorr'),
                    "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIIcorrSysJetEnDown'),
                    "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
                    }
                }
            )
    process.prodSmearedMET = smearedMETconfigurator.configure(process = process)
    process.produceElecTauPairsAll += process.prodSmearedMET

    setattr(elecTauPairProdConfigurator, "systematics", {
        "sysElectronEnUp" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronEnUp')
            },
        "sysElectronEnDown" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsForElecTauTrkIPsysElectronEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronEnDown')           
            },
        "sysTauJetEnUp" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
            },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnDown')
            }, 
        "sysJetEnUp" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnUp')
            },
        "sysJetEnDown" : {
            "srcMET"  : cms.InputTag('smearedMETsysJetEnDown')
            }
        })
    process.produceElecTauPairs = elecTauPairProdConfigurator.configure(process = process)
    process.produceElecTauPairsAll.replace(process.produceElecTauPairs, process.allElecTauPairs)
    process.produceElecTauPairsAll += process.produceElecTauPairs

    # add systematics to VBF object production
    setattr(vbfEventProdConfiguratorForAHtoElecTau, "systematics", {
        "sysElectronEnUp" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysElectronEnUpCumulative')
            },
        "sysElectronEnDown" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysElectronEnDownCumulative')
            },
        "sysTauJetEnUp" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysTauJetEnUpCumulative')
            },
        "sysTauJetEnDown" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysTauJetEnDownCumulative')
            }, 
        "sysJetEnUp" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysJetEnUpCumulative'),
            "srcTagJets" : cms.InputTag('selectedPatJetsForAHtoElecTauJetTagSysJetEnUpCumulative'),
            "srcCentralJets" : cms.InputTag('selectedPatJetsForAHtoElecTauJetTagSysJetEnUpCumulative')
            },
        "sysJetEnDown" : {
            "srcDiTaus" : cms.InputTag('selectedElecTauPairsMt1METsysJetEnDownCumulative'),
            "srcTagJets" : cms.InputTag('selectedPatJetsForAHtoElecTauJetTagSysJetEnDownCumulative'),
            "srcCentralJets" : cms.InputTag('selectedPatJetsForAHtoElecTauJetTagSysJetEnDownCumulative')
            }
    })
    process.produceVBFEventHypothesesForAHtoElecTau = \
            vbfEventProdConfiguratorForAHtoElecTau.configure(process = process)

    # apply Z-recoil corrections to shifted/smeared diTau objects
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        for sysName, src in elecTauPairSystematics.items():

            if sysName.find("ZllRecoilCorrection") != -1:
                continue;
            
            # get SV fit configuration parameter set
            nSVfit = getattr(process.allElecTauPairsZllRecoilCorrected, "nSVfit")

            configZllRecoilCorrection = configureZllRecoilCorrection(process, src.getModuleLabel(), "ZllRecoilCorrectionElecTauPair")
            #print 'adding sequence ' + str(configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']) + ' to produceElecTauPairsAll'
            process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
            elecTauPairSystematics[sysName] = configZllRecoilCorrection['diTauProducerModuleZllRecoilCorrectedName']
            # add SV fit configuration parameter set to new recoil-corrected pairs
            module = getattr(process,elecTauPairSystematics[sysName])
            setattr(module,"nSVfit",nSVfit)
            # update SV fit configuration to include appropriately shifted input electron or tau collection
            module.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg1.src = module.srcLeg1
            module.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg2.src = module.srcLeg2

        # add uncertainties on Z-recoil correction
        configZllRecoilCorrection = configureZllRecoilCorrection(process, "allElecTauPairs",
            "ZllRecoilCorrectionElecTauPair", +1., "SysUp")
        process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
        configZllRecoilCorrection = configureZllRecoilCorrection(process, "allElecTauPairs",
            "ZllRecoilCorrectionElecTauPair", -1., "SysDown")
        process.produceElecTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']

    # add uncertainties to e+tau pair selection
    setattr(patElecTauPairSelConfiguratorOS, "systematics", elecTauPairSystematics)
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        setattr(patElecTauPairSelConfiguratorOS, "src", "allElecTauPairsZllRecoilCorrected")
    process.selectElecTauPairsOSwithSys = patElecTauPairSelConfiguratorOS.configure(process = process)
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        setattr(patElecTauPairSelConfiguratorSS, "src", "allElecTauPairsZllRecoilCorrected")
    setattr(patElecTauPairSelConfiguratorSS, "systematics", elecTauPairSystematics)
    process.selectElecTauPairsSSwithSys = patElecTauPairSelConfiguratorSS.configure(process = process)

    process.selectElecTauPairs = cms.Sequence(process.selectElecTauPairsOSwithSys + process.selectElecTauPairsSSwithSys)

    setattr(patElecTauPairSelConfiguratorForAHtoElecTauOS, "systematics", elecTauPairSystematics)
    if hasattr(process, "allElecTauPairsZllRecoilCorrected"):
        setattr(patElecTauPairSelConfiguratorForAHtoElecTauOS, "src", "allElecTauPairsZllRecoilCorrected")
    process.selectElecTauPairsForAHtoElecTauOSwithSys = patElecTauPairSelConfiguratorForAHtoElecTauOS.configure(process = process)
    setattr(patElecTauPairSelConfiguratorForAHtoElecTauSS, "systematics", elecTauPairSystematics)
    process.selectElecTauPairsForAHtoElecTauSSwithSys = patElecTauPairSelConfiguratorForAHtoElecTauSS.configure(process = process)

    process.selectElecTauPairsForAHtoElecTau = cms.Sequence(process.selectElecTauPairsForAHtoElecTauOSwithSys * process.selectElecTauPairsForAHtoElecTauSSwithSys)

    # add uncertainties to VBF object selection
    setattr(vbfEventHypothesesSelConfiguratorForAHtoElecTau, "systematics", elecTauPairVBFEventSystematics)
    process.selectVBFEventHypothesesForAHtoElecTau = vbfEventHypothesesSelConfiguratorForAHtoElecTau.configure(process = process)

    expSysUncertainties = getSysUncertaintyNames(
            [ electronSystematics,
                tauSystematics,
                elecTauPairSystematics,
                jetSystematics ]
            )
    if hasattr(process, "isRecZtoElecTau"):        
        addBoolEventSelFlagProducer(process, "isRecZtoElecTau", expSysUncertainties, "selectZtoElecTauEvents")
    if hasattr(process, "isRecAHtoElecTau"):
        addBoolEventSelFlagProducer(process, "isRecAHtoElecTau", expSysUncertainties, "selectAHtoElecTauEvents")
    if hasattr(process, "isRecAHtoElecTauBtag"):
        addBoolEventSelFlagProducer(process, "isRecAHtoElecTauBtag", expSysUncertainties, "selectAHtoElecTauEvents")
    if hasattr(process, "isRecAHtoElecTauBtagVeto"):
        addBoolEventSelFlagProducer(process, "isRecAHtoElecTauBtagVeto", expSysUncertainties, "selectAHtoElecTauEvents")
    if hasattr(process, "isRecAHtoElecTauVBFtag"):
        addBoolEventSelFlagProducer(process, "isRecAHtoElecTauVBFtag", expSysUncertainties, "selectAHtoElecTauEvents")
    #if hasattr(process, "isRecAHtoElecTauZeroJets"):
    #    addBoolEventSelFlagProducer(process, "isRecAHtoElecTauZeroJets", expSysUncertainties, "selectAHtoElecTauEvents")

    # turn on systematics in all GenericAnalyzer modules
    # and update InputTags of systematic uncertainty histogram managers
    sysUncertaintyHistManagerElecTauPairSystematics_MSSM = {
                "sysElectronEnUp"                : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysElectronEnUpCumulative'),
                "sysElectronEnDown"              : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysElectronEnDownCumulative'),
                "sysTauJetEnUp"              : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysTauJetEnUpCumulative'),
                "sysTauJetEnDown"            : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysTauJetEnDownCumulative'),
                "sysJetEnUp"                 : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysJetEnUpCumulative'),
                "sysJetEnDown"               : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysJetEnDownCumulative')
                #"sysZllRecoilCorrectionUp"   : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysZllRecoilCorrectionUpCumulative'),
                #"sysZllRecoilCorrectionDown" : cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeSysZllRecoilCorrectionDownCumulative')
    }
    sysUncertaintyHistManagerElecTauPairSystematics_SM = {
                "sysElectronEnUp"                : cms.InputTag('selectedElecTauPairsZeroChargeSysElectronEnUpCumulative'),
                "sysElectronEnDown"              : cms.InputTag('selectedElecTauPairsZeroChargeSysElectronEnDownCumulative'),
                "sysTauJetEnUp"              : cms.InputTag('selectedElecTauPairsZeroChargeSysTauJetEnUpCumulative'),
                "sysTauJetEnDown"            : cms.InputTag('selectedElecTauPairsZeroChargeSysTauJetEnDownCumulative'),
                "sysJetEnUp"                 : cms.InputTag('selectedElecTauPairsZeroChargeSysJetEnUpCumulative'),
                "sysJetEnDown"               : cms.InputTag('selectedElecTauPairsZeroChargeSysJetEnDownCumulative')
                #"sysZllRecoilCorrectionUp"   : cms.InputTag('selectedElecTauPairsZeroChargeSysZllRecoilCorrectionUpCumulative'),
                #"sysZllRecoilCorrectionDown" : cms.InputTag('selectedElecTauPairsZeroChargeSysZllRecoilCorrectionDownCumulative')
    }

    for sign in ['OS', 'SS']:
        for cat in ['_woBtag', '_wBtag', '_VBF']:
            genAnalyzerModuleName = 'analyzeAHtoElecTauEvents' + sign + cat
            if hasattr(process, genAnalyzerModuleName):
                print 'Turning on systematics for analyzer %s' % genAnalyzerModuleName
                getattr(process, genAnalyzerModuleName).estimateSysUncertainties = cms.bool(True)
                # update sysUncertaintyHistManagerForElecTau InputTags
                if hasattr(process, "sysUncertaintyHistManagerForElecTau"):
                    
                    # default categories are MSSM
                    sysUncertaintyHistManagerElecTauPairSystematics = sysUncertaintyHistManagerElecTauPairSystematics_MSSM
                    diTauCandidateSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeCumulative')
                    # switch if doing SM categories
                    if cat is '_VBF':
                        sysUncertaintyHistManagerElecTauPairSystematics = sysUncertaintyHistManagerElecTauPairSystematics_SM
                        diTauCandidateSource = cms.InputTag('selectedElecTauPairsZeroChargeCumulative')
                    #
                    process.sysUncertaintyHistManagerForElecTau_updated = \
                        copy.deepcopy(process.sysUncertaintyHistManagerForElecTau)
                    process.sysUncertaintyHistManagerForElecTau_updated.histManagers[0].config.diTauCandidateSource = \
                            diTauCandidateSource
                    process.sysUncertaintyHistManagerForElecTau_updated.histManagers[0].systematics.diTauCandidateSource = \
                        getSysUncertaintyParameterSets([ sysUncertaintyHistManagerElecTauPairSystematics ])
                    process.sysUncertaintyHistManagerForElecTau_updated.histManagers[1].config.diTauCandidateSource = \
                            diTauCandidateSource
                    process.sysUncertaintyHistManagerForElecTau_updated.histManagers[1].systematics.diTauCandidateSource = \
                        process.sysUncertaintyHistManagerForElecTau_updated.histManagers[0].systematics.diTauCandidateSource

                    print("--> replacing %s in %s" % ("sysUncertaintyHistManagerForElecTau", genAnalyzerModuleName))
                    genAnalyzerMode = getattr(process, genAnalyzerModuleName)
                    replaceSysAnalyzerModules(genAnalyzerMode,
                        [ [ process.sysUncertaintyHistManagerForElecTau, process.sysUncertaintyHistManagerForElecTau_updated ] ]
                        )
    
    process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleAHtoElecTauSpecific)


#--------------------------------------------------------------------------------
# functions to enable/disable estimation of systematic uncertainties
# specific to W-->tau nu channel
#--------------------------------------------------------------------------------

def enableSysUncertainties_runWtoTauNu(process):    
    print("<enableSysUncertainties_runAHtoElecTau>:")
    print("--> **enabling** estimation of systematic uncertainties...")

    pyNameSpace = None

#    process.produceGenObjects += process.produceSysErrGenEventReweights

    process.produceEventSelFlagsWtoTauNu = wToTauNuEventSelConfigurator.configure(process = process, estimateSysUncertainties = True)

    process.load("TauAnalysis.RecoTools.patLeptonSystematics_cff")
    process.producePatTupleWtoTauNuSpecific.replace(process.selectPatTaus, process.prodSmearedTaus + process.selectPatTaus)

    process.load("TauAnalysis.RecoTools.patJetSystematics_cff")
    process.producePatTupleWtoTauNuSpecific.replace(process.selectPatJets, process.prodSmearedJets + process.selectPatJets)

    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectPatTaus = patTauSelConfigurator.configure(process = process)

    setattr(patTauSelConfiguratorForWTauNu, "systematics", tauSystematics)
    process.selectPatTausForWTauNu = patTauSelConfiguratorForWTauNu.configure(process = process)

    setattr(patJetSelConfigurator, "systematics", jetSystematics)
    process.selectPatJets = patJetSelConfigurator.configure(process = process)

    setattr(patJetSelConfiguratorForWTauNu, "systematics", jetSystematics)
    process.selectPatJetsForWTauNu = patJetSelConfiguratorForWTauNu.configure(process = process)

    setattr(patJetSelConfiguratorForWTauNu2, "systematics", jetSystematics)
    process.selectPatJetsForWTauNu2 = patJetSelConfiguratorForWTauNu2.configure(process = process)

    process.selectedPatJetsForMEtTypeI = cms.EDFilter("PATJetSelector",
                                                      src = cms.InputTag('selectedPatJetsAntiOverlapWithTausVetoForWTauNuCumulative'),
                                                      cut = cms.string('pt > 10.'),
                                                      filter = cms.bool(False)
                                                      )
    process.selectedPatJetsForMEtTypeISysJetEnUp = process.patJetsJECshiftUp.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeI')
        )
    process.selectedPatJetsForMEtTypeISysJetEnDown = process.patJetsJECshiftDown.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeI')
        )

    process.selectedPatJetsForMEtTypeII = process.selectedPatJetsForMEtTypeI.clone(
        cut = cms.string('pt < 10.')
        )
    process.selectedPatJetsForMEtTypeIISysJetEnUp = process.patJetsJECshiftUp.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeII')
        )
    process.selectedPatJetsForMEtTypeIISysJetEnDown = process.patJetsJECshiftDown.clone(
        src = cms.InputTag('selectedPatJetsForMEtTypeII')
        )

    process.selectedPatJetsForMEtTypeIISysJetEnUp.jecUncertaintyValue = cms.double(0.10)
    process.selectedPatJetsForMEtTypeIISysJetEnDown.jecUncertaintyValue = cms.double(0.10)        

    process.producePatJetsForMEt = cms.Sequence(
            process.selectedPatJetsForMEtTypeI
                   * process.selectedPatJetsForMEtTypeISysJetEnUp * process.selectedPatJetsForMEtTypeISysJetEnDown
                   * process.selectedPatJetsForMEtTypeII
                   * process.selectedPatJetsForMEtTypeIISysJetEnUp * process.selectedPatJetsForMEtTypeIISysJetEnDown
                )
    
    process.producePatTupleWtoTauNuSpecific.replace(process.selectedPatJetsAntiOverlapWithTausVetoForWTauNuCumulative, process.selectedPatJetsAntiOverlapWithTausVetoForWTauNuCumulative + process.producePatJetsForMEt)

    smearMEtUnclustedEnergyResolution = cms.double(0.10)
        
    process.smearedMET = cms.EDProducer("SmearedMETProducer",
                                        src = cms.InputTag('patPFMETs'),
                                        smearedParticles = cms.PSet()
                                        )    

    smearedMETconfigurator = objProdConfigurator(
        process.smearedMET,
        pyModuleName = __name__,
        systematics = {
        "sysTauJetEnUp" : {
        "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'), 
        "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnUpCumulative')
        },
        "sysTauJetEnDown" : {
        "smearedParticles.taus.srcOriginal" : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
        "smearedParticles.taus.srcSmeared"  : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnDownCumulative')
        },
        "sysJetEnUp" : {
        "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeI'),
        "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeISysJetEnUp'),
        "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeII'),
        "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIISysJetEnUp'),
        "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
        },
        "sysJetEnDown" : {
        "smearedParticles.jetsTypeI.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeI'),
        "smearedParticles.jetsTypeI.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeISysJetEnDown'),
        "smearedParticles.jetsTypeII.srcOriginal" : cms.InputTag('selectedPatJetsForMEtTypeII'),
        "smearedParticles.jetsTypeII.srcSmeared"  : cms.InputTag('selectedPatJetsForMEtTypeIISysJetEnDown'),
        "smearedParticles.jetsTypeII.smearByResolutionUncertainty" : smearMEtUnclustedEnergyResolution
        }
        }
        )

    process.prodSmearedMET = smearedMETconfigurator.configure(process = process)

    setattr(patPFMETSelConfigurator, "systematics", metSystematicsForWtoTauNu)
    process.selectPatPFMETs = patPFMETSelConfigurator.configure(process = process)
 
    process.producePatTupleWtoTauNuSpecific.replace(process.selectPatPFMETs, process.prodSmearedMET + process.selectPatPFMETs)

    setattr(tauNuPairProdConfigurator, "systematics",{
        "sysTauJetEnUp" : {
        "srcVisDecayProducts"           : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnUpCumulative'),
        "srcMET"                        : cms.InputTag('smearedMETsysTauJetEnUp')
        },
        "sysTauJetEnDown" : {
        "sysTauJetEnDown"               : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnDownCumulative'),
        "srcMET"                        : cms.InputTag('smearedMETsysTauJetEnDown')
        },
        "sysJetEnUp" : {
        "srcMET" : cms.InputTag('smearedMETsysJetEnUp')
        },
        "sysJetEnDown" : {
        "srcMET" : cms.InputTag('smearedMETsysJetEnDown')
        }
        })

    process.produceTauNuPairs = tauNuPairProdConfigurator.configure(process = process)

    setattr(patTauNuPairSelConfigurator, "systematics", tauNuPairSystematics)
    process.selectTauNuPairs = patTauNuPairSelConfigurator.configure(process = process)
    process.producePatTupleWtoTauNuSpecific.replace(process.selectedTauNuPairsMt40Cumulative, process.selectTauNuPairs)

### define smeared HT ratio 
    setattr(htRatioProdConfigurator, "systematics", {
        "sysTauJetEnUp" : {
        "srcTau"         : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnUpCumulative')
        },
        "sysTauJetEnDown" : {
        "srcTau"         : cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnDownCumulative')
        },
        "sysJetEnUp" : {
        "srcJet" : cms.InputTag('selectedPatJetsEt15ForWTauNuSysJetEnUpCumulative')    
        },
        "sysJetEnDown" : {
        "srcJet" : cms.InputTag('selectedPatJetsEt15ForWTauNuSysJetEnDownCumulative')
        }
        })    
    process.produceHtRatio = htRatioProdConfigurator.configure(process = process)

    setattr(htRatioSelConfigurator, "systematics", htRatioSystematics)
    process.selectHtRatio = htRatioSelConfigurator.configure(process = process)
#    process.producePatTupleWtoTauNuSpecific.replace(process.selectHtRatio, process.selectHtRatio)    

## Smeared MET-topology
    
#    metTopologies.srcEnergyDeposits = cms.VInputTag(
#        'selectedPatJetsForMEtTypeI',
#        'selectedPatJetsForMEtTypeII',
#        'selectedPatTausForWTauNuEcalCrackVetoCumulative'
#        )
    setattr(metTopologyProdConfigurator, "systematics",{
        "sysTauJetEnUp" : {
        "srcEnergyDeposits" : cms.VInputTag('selectedPatJetsForMEtTypeI',
                                            'selectedPatJetsForMEtTypeII',
                                            'selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnUpCumulative'
                                            )
        },
        "sysTauJetEnDown" : {
        "srcEnergyDeposits" : cms.VInputTag('selectedPatJetsForMEtTypeI',
                                            'selectedPatJetsForMEtTypeII',
                                            'selectedPatTausForWTauNuEcalCrackVetoSysTauJetEnDownCumulative'
                                            )
        },
        "sysJetEnUp" : {
        "srcEnergyDeposits" : cms.VInputTag('selectedPatJetsForMEtTypeISysJetEnUp',
                                            'selectedPatJetsForMEtTypeIISysJetEnUp',
                                            'selectedPatTausForWTauNuEcalCrackVetoCumulative'
                                            )
        },
        "sysJetEnDown" : {
        "srcEnergyDeposits" : cms.VInputTag('selectedPatJetsForMEtTypeISysJetEnDown',
                                            'selectedPatJetsForMEtTypeIISysJetEnDown',
                                            'selectedPatTausForWTauNuEcalCrackVetoCumulative'
                                            )
        }
        })

    process.produceMEtTopology = metTopologyProdConfigurator.configure(process = process)

    setattr(metTopologySelConfigurator,"systematics",metTopologySystematics)
    process.selectMEtTopology = metTopologySelConfigurator.configure(process = process)
    
    if hasattr(process, "isRecWtoTauNu") :
        expSysUncertainties = getSysUncertaintyNames(
            [ tauSystematics,
              jetSystematics,
              metSystematicsForWtoTauNu,
              htRatioSystematics,
              tauNuPairSystematics,
              metTopologySystematics
              ]
            )
    addBoolEventSelFlagProducer(process,"isRecWtoTauNu", expSysUncertainties, "selectWtoTauNuEvents")

    if hasattr(process,"analyzeWtoTauNuEvents"):
        process.analyzeWtoTauNuEvents.estimateSysUncertainties = cms.bool(True)
