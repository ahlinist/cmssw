import FWCore.ParameterSet.Config as cms
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

from TauAnalysis.RecoTools.patLeptonSelection_cff import *
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

#
#--------------------------------------------------------------------------------
#

def disableSysUncertainties_runZtoMuTau(process):
    print("<disableSysUncertainties_runZtoMuTau>:")
    
    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    process.produceGenObjects.remove(process.produceSysErrGenEventReweights)

    removeModules(process, "selectZtoMuTauEvents", moduleNamePattern, pyNameSpace)

    removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "sysUncertaintyBinnerForMuTau")
    process.analyzeZtoMuTauEvents.estimateSysUncertainties = cms.bool(False)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithMuonIsolation"):
        removeAnalyzer(process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation.analysisSequence, "sysUncertaintyBinnerForMuTau")
        process.analyzeZtoMuTauEvents_factorizedWithMuonIsolation.estimateSysUncertainties = cms.bool(False)
    if hasattr(process, "analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation"):
        removeAnalyzer(process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence, "sysUncertaintyBinnerForMuTau")
        process.analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.estimateSysUncertainties = cms.bool(False)    

def enableSysUncertainties_runZtoMuTau(process):
    print("<enableSysUncertainties_runZtoMuTau>:")
    
    pyNameSpace = None

    setattr(patMuonSelConfigurator, "systematics", muonSystematics)
    process.selectLayer1Muons = patMuonSelConfigurator.configure(process = process)

    setattr(patMuonSelConfiguratorLooseIsolation, "systematics", muonSystematics)
    process.selectLayer1MuonsLooseIsolation = patMuonSelConfiguratorLooseIsolation.configure(process = process)

    setattr(patTauSelConfigurator, "systematics", tauSystematics)
    process.selectLayer1Taus = patTauSelConfigurator.configure(process = process)

    setattr(muTauPairProdConfigurator, "systematics", {
        "sysMuonPtUp"           : { "srcLeg1" : "selectedLayer1MuonsTrkIPsysMuonPtUpCumulative" },
        "sysMuonPtDown"         : { "srcLeg1" : "selectedLayer1MuonsTrkIPsysMuonPtDownCumulative" },
        "sysTauJetEnUp"         : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetEnUpCumulative" },
        "sysTauJetEnDown"       : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetEnDownCumulative" }, 
        "sysTauJetThetaUp"      : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetThetaUpCumulative" }, 
        "sysTauJetThetaDown"    : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetThetaDownCumulative" }, 
        "sysTauJetPhiUp"        : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetPhiUpCumulative" }, 
        "sysTauJetPhiDown"      : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetPhiDownCumulative" }, 
        "sysTauLeadTrackPtUp"   : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauLeadTrackPtUpCumulative" }, 
        "sysTauLeadTrackPtDown" : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauLeadTrackPtDownCumulative" }
    })
    process.produceMuTauPairs = muTauPairProdConfigurator.configure(process = process)

    setattr(muTauPairProdConfiguratorLooseMuonIsolation, "systematics", {
        "sysMuonPtUp"           : { "srcLeg1" : "selectedLayer1MuonsTrkIPlooseMuonIsolationSysMuonPtUpCumulative" },
        "sysMuonPtDown"         : { "srcLeg1" : "selectedLayer1MuonsTrkIPlooseMuonIsolationSysMuonPtDownCumulative" },
        "sysTauJetEnUp"         : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetEnUpCumulative" },
        "sysTauJetEnDown"       : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetEnDownCumulative" }, 
        "sysTauJetThetaUp"      : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetThetaUpCumulative" }, 
        "sysTauJetThetaDown"    : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetThetaDownCumulative" }, 
        "sysTauJetPhiUp"        : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetPhiUpCumulative" }, 
        "sysTauJetPhiDown"      : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauJetPhiDownCumulative" }, 
        "sysTauLeadTrackPtUp"   : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauLeadTrackPtUpCumulative" }, 
        "sysTauLeadTrackPtDown" : { "srcLeg2" : "selectedLayer1TausMuonVetoSysTauLeadTrackPtDownCumulative" }
    })
    process.produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(process = process)

    setattr(patMuTauPairSelConfigurator, "systematics", muTauPairSystematics)
    process.selectMuTauPairs = patMuTauPairSelConfigurator.configure(process = process)
    
    setattr(patMuTauPairSelConfiguratorLooseMuonIsolation, "systematics", muTauPairSystematics)
    process.selectMuTauPairsLooseMuonIsolation = patMuTauPairSelConfiguratorLooseMuonIsolation.configure(process = process)




