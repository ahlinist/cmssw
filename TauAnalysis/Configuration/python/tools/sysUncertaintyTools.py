import FWCore.ParameterSet.Config as cms
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

from TauAnalysis.RecoTools.patLeptonSelection_cff import *
from TauAnalysis.RecoTools.patLeptonSystematics_cff import *
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
from TauAnalysis.GenSimTools.sysErrGenEventReweights_cfi import *

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
                                
                flagModuleLabel_systematic = composeModuleName(flag.getModuleLabel(), expSysUncertainty)
                
                if hasattr(process, flagModuleLabel_systematic):
                    flag_systematic = copy.deepcopy(flag)
                    flag_systematic.setModuleLabel(flagModuleLabel_systematic)
                    flags_systematic.append(flag_systematic)
                else:
                    flags_systematic.append(flag)

            setattr(module_systematic, "flags", cms.VInputTag(flags_systematic))

            moduleName_systematic = composeModuleName(moduleName, expSysUncertainty)
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

def disableSysUncertainties_runZtoMuTau(process):
    #print("<disableSysUncertainties_runZtoMuTau>:")
    
    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)
    process.producePatTupleZtoMuTauSpecific.remove(process.prodSmearedMuons)
    process.producePatTupleZtoMuTauSpecific.remove(process.prodSmearedTaus)

    if hasattr(process, "selectZtoMuTauEvents"):
        removeModules(process, "selectZtoMuTauEvents", moduleNamePattern, pyNameSpace)
    if hasattr(process, "selectZtoMuTauEventsLooseMuonIsolation"):
        removeModules(process, "selectZtoMuTauEventsLooseMuonIsolation", moduleNamePattern, pyNameSpace)

    if hasattr(process, "analyzeZtoMuTauEvents"):
        removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "sysUncertaintyBinnerForMuTau")
        process.analyzeZtoMuTauEvents.estimateSysUncertainties = cms.bool(False)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithMuonIsolation"):
        removeAnalyzer(process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation.analysisSequence, "sysUncertaintyBinnerForMuTau")
        process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation.estimateSysUncertainties = cms.bool(False)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation"):
        removeAnalyzer(process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence, "sysUncertaintyBinnerForMuTau")
        process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.estimateSysUncertainties = cms.bool(False)    

def enableSysUncertainties_runZtoMuTau(process):
    #print("<enableSysUncertainties_runZtoMuTau>:")
    
    pyNameSpace = None

    setattr(patMuonSelConfigurator, "systematics", muonSystematics)
    process.selectLayer1Muons = patMuonSelConfigurator.configure(process = process)

    setattr(patMuonSelConfiguratorLooseIsolation, "systematics", muonSystematics)
    process.selectLayer1MuonsLooseIsolation = patMuonSelConfiguratorLooseIsolation.configure(process = process)

    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectLayer1Taus = patTauSelConfigurator.configure(process = process)

    setattr(patTauSelConfiguratorForMuTau, "systematics", tauSystematics)
    process.selectLayer1TausForMuTau = patTauSelConfiguratorForMuTau.configure(process = process)

    setattr(muTauPairProdConfigurator, "systematics", {
        "sysMuonPtUp"           : { "srcLeg1" : "selectedLayer1MuonsTrkIPsysMuonPtUpCumulative" },
        "sysMuonPtDown"         : { "srcLeg1" : "selectedLayer1MuonsTrkIPsysMuonPtDownCumulative" },
        "sysTauJetEnUp"         : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetEnUpCumulative" },
        "sysTauJetEnDown"       : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetEnDownCumulative" }, 
        "sysTauJetThetaUp"      : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetThetaUpCumulative" }, 
        "sysTauJetThetaDown"    : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetThetaDownCumulative" }, 
        "sysTauJetPhiUp"        : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetPhiUpCumulative" }, 
        "sysTauJetPhiDown"      : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetPhiDownCumulative" }
    })
    process.produceMuTauPairs = muTauPairProdConfigurator.configure(process = process)

    setattr(muTauPairProdConfiguratorLooseMuonIsolation, "systematics", {
        "sysMuonPtUp"           : { "srcLeg1" : "selectedLayer1MuonsTrkIPlooseIsolationSysMuonPtUpCumulative" },
        "sysMuonPtDown"         : { "srcLeg1" : "selectedLayer1MuonsTrkIPlooseIsolationSysMuonPtDownCumulative" },
        "sysTauJetEnUp"         : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetEnUpCumulative" },
        "sysTauJetEnDown"       : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetEnDownCumulative" }, 
        "sysTauJetThetaUp"      : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetThetaUpCumulative" }, 
        "sysTauJetThetaDown"    : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetThetaDownCumulative" }, 
        "sysTauJetPhiUp"        : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetPhiUpCumulative" }, 
        "sysTauJetPhiDown"      : { "srcLeg2" : "selectedLayer1TausForMuTauMuonVetoSysTauJetPhiDownCumulative" }
    })
    process.produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(process = process)

    setattr(patMuTauPairSelConfigurator, "systematics", muTauPairSystematics)
    process.selectMuTauPairs = patMuTauPairSelConfigurator.configure(process = process)
    
    setattr(patMuTauPairSelConfiguratorLooseMuonIsolation, "systematics", muTauPairSystematics)
    process.selectMuTauPairsLooseMuonIsolation = patMuTauPairSelConfiguratorLooseMuonIsolation.configure(process = process)

    if hasattr(process, "isRecZtoMuTau"):        
        expSysUncertainties = getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics ]
        )
        addBoolEventSelFlagProducer(process, "isRecZtoMuTau", expSysUncertainties, "selectZtoMuTauEvents")
    
    if hasattr(process, "analyzeZtoMuTauEvents"):
        process.analyzeZtoMuTauEvents.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithMuonIsolation"):
        process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation.estimateSysUncertainties = cms.bool(True)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation"):
        process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.estimateSysUncertainties = cms.bool(True)    

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
# specific to Z --> tau-jet + tau-jet channel
#--------------------------------------------------------------------------------

def disableSysUncertainties_runZtoDiTau(process):
    #print("<disableSysUncertainties_runZtoDiTau>:")
    
    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectZtoDiTauEvents", moduleNamePattern, pyNameSpace)

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

def disableSysUncertainties_runAHtoMuTau(process):
    #print("<disableSysUncertainties_runAHtoMuTau>:")
    
    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectAHtoMuTauEvents", moduleNamePattern, pyNameSpace)
    if hasattr(process, "selectAHtoMuTauEventsLooseMuonIsolation"):
        removeModules(process, "selectAHtoMuTauEventsLooseMuonIsolation", moduleNamePattern, pyNameSpace)





