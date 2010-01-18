import FWCore.ParameterSet.Config as cms
import re

from PhysicsTools.PatAlgos.tools.helpers import listModules

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

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
    moduleNamePattern = "\w+Sys\w+(Up|Down)"
    pyNameSpace = None

    removeModules(process, "selectLayer1Muons", moduleNamePattern, pyNameSpace)
    removeModules(process, "selectLayer1MuonsLooseIsolation", moduleNamePattern, pyNameSpace)
    removeModules(process, "selectLayer1Taus", moduleNamePattern, pyNameSpace)
    
    removeModules(process, "produceMuTauPairs", moduleNamePattern, pyNameSpace)
    removeModules(process, "produceMuTauPairsLooseMuonIsolation", moduleNamePattern, pyNameSpace)

    removeModules(process, "selectMuTauPairs", moduleNamePattern, pyNameSpace)
    removeModules(process, "selectMuTauPairsLooseMuonIsolation", moduleNamePattern, pyNameSpace)

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

