import FWCore.ParameterSet.Config as cms
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

from TauAnalysis.RecoTools.patLeptonSelection_cff import *
from TauAnalysis.RecoTools.patLeptonSystematics_cff import *
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
from TauAnalysis.CandidateTools.muTauPairSelectionForAHtoMuTau_cff import *
from TauAnalysis.RecoTools.patJetSelection_cff import *
from TauAnalysis.RecoTools.patJetSelectionForAHtoMuTau_cff import *
from TauAnalysis.RecoTools.tools.configureZllRecoilCorrection import configureZllRecoilCorrection
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
from TauAnalysis.GenSimTools.sysErrGenEventReweights_cfi import *

from TauAnalysis.Configuration.selectZtoMuTau_cff import *
from TauAnalysis.Configuration.selectZtoMuTau_factorized_cff import *
from TauAnalysis.Configuration.selectZtoElecTau_cff import *
from TauAnalysis.Configuration.selectZtoElecTau_factorized_cff import *
from TauAnalysis.Configuration.selectZtoDiTau_cff import *
from TauAnalysis.Configuration.selectZtoDiTau_factorized_cff import *
from TauAnalysis.Configuration.selectAHtoMuTau_cff import *
from TauAnalysis.Configuration.selectAHtoMuTau_factorized_cff import *
from TauAnalysis.Configuration.selectAHtoElecTau_cff import *
from TauAnalysis.Configuration.selectAHtoElecTau_factorized_cff import *

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

    process.smearedMET = cms.EDProducer("SmearedMETProducer",
        src = cms.InputTag('patPFMETs'),
        smearedParticles = cms.PSet()
    )
    smearedMETconfigurator = objProdConfigurator(
        process.smearedMET,
        pyModuleName = __name__,
        systematics = {
            "sysMuonPtUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPcumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtUpCumulative')
            },
            "sysMuonPtDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPcumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPsysMuonPtDownCumulative')
            },
            "sysTauJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnUpCumulative')
            },
            "sysTauJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnDownCumulative')
            },
            "sysJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnUpCumulative')
            },
            "sysJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnDownCumulative')
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
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
        },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnDownCumulative'),
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
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtUpCumulative')
            },
            "sysMuonPtDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatMuonsTrkIPlooseIsolationSysMuonPtDownCumulative')
            },
            "sysTauJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnUpCumulative')
            },
            "sysTauJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnDownCumulative')
            },
            "sysJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnUpCumulative')
            },
            "sysJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnDownCumulative')
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
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysTauJetEnUp')
        },
        "sysTauJetEnDown" : {
            "srcLeg2" : cms.InputTag('selectedPatTausForMuTauMuonVetoSysTauJetEnDownCumulative'),
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

    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairsLooseMuonIsolation",
                                                             "ZllRecoilCorrectionMuTauPair", +1., "SysUp")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']
    configZllRecoilCorrection = configureZllRecoilCorrection(process, "allMuTauPairsLooseMuonIsolation",
                                                             "ZllRecoilCorrectionMuTauPair", -1., "SysDown")
    process.produceMuTauPairsAll += configZllRecoilCorrection['patPFMETsZllRecoilCorrectionSequence']

    setattr(patMuTauPairSelConfiguratorOS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsOS = patMuTauPairSelConfiguratorOS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorSS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsSS = patMuTauPairSelConfiguratorSS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorLooseMuonIsolationOS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    process.selectMuTauPairsLooseMuonIsolationOS = patMuTauPairSelConfiguratorLooseMuonIsolationOS.configure(process = process)
    setattr(patMuTauPairSelConfiguratorLooseMuonIsolationSS, "systematics", muTauPairSystematicsLooseMuonIsolation)
    process.selectMuTauPairsLooseMuonIsolationSS = patMuTauPairSelConfiguratorLooseMuonIsolationSS.configure(process = process)

    if hasattr(process, "isRecZtoMuTau"):
        expSysUncertainties = getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
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

    process.produceGenObjects._seq = process.produceGenObjects._seq * process.produceSysErrGenEventReweights

    process.producePatTupleZtoElecTauSpecific._seq = process.prodSmearedElectrons * process.producePatTupleZtoElecTauSpecific._seq
    process.producePatTupleZtoElecTauSpecific._seq = process.prodSmearedTaus * process.producePatTupleZtoElecTauSpecific._seq
    process.producePatTupleZtoElecTauSpecific._seq = process.prodSmearedJets * process.producePatTupleZtoElecTauSpecific._seq

    process.produceEventSelFlagsZtoElecTau = \
      zToElecTauEventSelConfigurator.configure(process = process, estimateSysUncertainties = True)
    process.produceEventSelFlagsZtoElecTauLooseElectronIsolation = \
      zToElecTauEventSelConfiguratorLooseElectronIsolation.configure(process = process, estimateSysUncertainties = True)

    setattr(patElectronSelConfigurator, "systematics", electronSystematics)
    process.selectPatElectronsForElecTau = patElectronSelConfigurator.configure(process = process)

    setattr(patElectronSelConfiguratorLooseIsolation, "systematics", electronSystematics)
    process.selectPatElectronsForElecTauLooseIsolation = patElectronSelConfiguratorLooseIsolation.configure(process = process)

    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectPatTaus = patTauSelConfigurator.configure(process = process)

    setattr(patTauSelConfiguratorForElecTau, "systematics", tauSystematics)
    process.selectPatTausForElecTau = patTauSelConfiguratorForElecTau.configure(process = process)

    setattr(patJetSelConfigurator, "systematics", jetSystematics)
    process.selectPatJets = patJetSelConfigurator.configure(process = process)

    process.smearedMET = cms.EDProducer("SmearedMETProducer",
        src = cms.InputTag('patPFMETs'),
        smearedParticles = cms.PSet()
    )
    smearedMETconfigurator = objProdConfigurator(
        process.smearedMET,
        pyModuleName = __name__,
        systematics = {
            "sysElectronPtUp" : {        
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatElectronsTrkIPcumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatElectronsTrkIPsysElectronPtUpCumulative')
            },
            "sysElectronPtDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatElectronsTrkIPcumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatElectronsTrkIPsysElectronPtDownCumulative')
            },
            "sysTauJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnUpCumulative')
            },
            "sysTauJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatTausForElecTauElectronVetoSysTauJetEnDownCumulative')
            }, 
            "sysJetEnUp" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnUpCumulative')
            },
            "sysJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoSysJetEnDownCumulative')
            }
        }
    )
    process.prodSmearedMET = smearedMETconfigurator.configure(process = process)

    setattr(elecTauPairProdConfigurator, "systematics", {
        "sysElectronPtUp" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsTrkIPsysElectronPtUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronPtUp')
        },
        "sysElectronPtDown" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsTrkIPsysElectronPtDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronPtDown')           
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
    process.produceElecTauPairs._seq = process.prodSmearedMET * process.produceElecTauPairs._seq

    setattr(elecTauPairProdConfiguratorLooseElectronIsolation, "systematics", {
        "sysElectronPtUp" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsTrkIPlooseIsolationSysElectronPtUpCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronPtUp')
        },
        "sysElectronPtDown" : {
            "srcLeg1" : cms.InputTag('selectedPatElectronsTrkIPlooseIsolationSysElectronPtDownCumulative'),
            "srcMET"  : cms.InputTag('smearedMETsysElectronPtDown')           
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
    process.produceElecTauPairsLooseElectronIsolation = elecTauPairProdConfiguratorLooseElectronIsolation.configure(process = process)

    setattr(patElecTauPairSelConfigurator, "systematics", elecTauPairSystematics)
    process.selectElecTauPairs = patElecTauPairSelConfigurator.configure(process = process)
    
    setattr(patElecTauPairSelConfiguratorLooseElectronIsolation, "systematics", elecTauPairSystematics)
    process.selectElecTauPairsLooseElectronIsolation = patElecTauPairSelConfiguratorLooseElectronIsolation.configure(process = process)

    if hasattr(process, "isRecZtoElecTau"):        
        expSysUncertainties = getSysUncertaintyNames(
            [ electronSystematics,
              tauSystematics,
              jetSystematics ]
        )
        addBoolEventSelFlagProducer(process, "isRecZtoElecTau", expSysUncertainties, "selectZtoElecTauEvents")

    if hasattr(process, "analyzeZtoElecTauEvents"):
        process.analyzeZtoElecTauEvents.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoElecTauEvents_factorizedWithElectronIsolation"):
        process.analyzeZtoElecTauEvents_factorizedWithElectronIsolation.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoElecTauEvents_factorizedWithoutElectronIsolation"):
        process.analyzeZtoElecTauEvents_factorizedWithoutElectronIsolation.estimateSysUncertainties = cms.bool(True)    

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

    setattr(patMuTauPairSelConfiguratorForAHtoMuTauOS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsForAHtoMuTauOS = patMuTauPairSelConfiguratorForAHtoMuTauOS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorForAHtoMuTauSS, "systematics", muTauPairSystematics)
    process.selectMuTauPairsForAHtoMuTauSS = patMuTauPairSelConfiguratorForAHtoMuTauSS.configure(process = process)

    setattr(patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS, "systematics", muTauPairSystematicsLooseMuonIsolation)
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

    expSysUncertainties = getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
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







