import FWCore.ParameterSet.Config as cms
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer
from TauAnalysis.Configuration.tools.factorizationTools import replaceSysAnalyzerModules

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
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsEt20Cumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsEt20SysJetEnUpCumulative')
            },
            "sysJetEnDown" : {
                "smearedParticles.srcOriginal" : cms.InputTag('selectedPatJetsEt20Cumulative'),
                "smearedParticles.srcSmeared"  : cms.InputTag('selectedPatJetsEt20SysJetEnDownCumulative')
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
    



