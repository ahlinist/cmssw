import FWCore.ParameterSet.Config as cms
import copy

#from TauAnalysis.Configuration.analysisSequenceTools import switchHistManagers
#from TauAnalysis.Configuration.analysisSequenceTools import replaceHistManagerInputTags
from TauAnalysis.Configuration.tools.analysisSequenceTools import replaceAnalyzerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of W --> tau + nu events
# defined for the "regular" case without factorization of tau isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeWtoTauNu_boosted_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# tau candidate selection with "loose" tau isolation criteria applied
evtSelTauLeadTrkPtBoostedLooseIsolation = copy.deepcopy(evtSelTauLeadTrkPtBoosted)
evtSelTauLeadTrkPtBoostedLooseIsolation.src_cumulative = cms.InputTag('tauLeadTrkPtBoostedLooseIsolation', 'cumulative')
evtSelTauLeadTrkPtBoostedLooseIsolation.src_individual = cms.InputTag('tauLeadTrkPtBoostedLooseIsolation', 'individual')

evtSelTauEcalIsoBoostedLooseIsolation = copy.deepcopy(evtSelTauIsoBoosted)
evtSelTauEcalIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('tauEcalIsoBoostedLooseIsolation', 'cumulative')
evtSelTauEcalIsoBoostedLooseIsolation.src_individual = cms.InputTag('tauEcalIsoBoostedLooseIsolation', 'individual')

evtSelTauTrkIsoBoostedLooseIsolation = copy.deepcopy(evtSelTauTaNCBoosted)
evtSelTauTrkIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('tauTrkIsoBoostedLooseIsolation', 'cumulative')
evtSelTauTrkIsoBoostedLooseIsolation.src_individual = cms.InputTag('tauTrkIsoBoostedLooseIsolation', 'individual')

evtSelTauProngBoostedLooseIsolation = copy.deepcopy(evtSelTauProngBoosted)
evtSelTauProngBoostedLooseIsolation.src_cumulative = cms.InputTag('tauProngBoostedLooseIsolation', 'cumulative')
evtSelTauProngBoostedLooseIsolation.src_individual = cms.InputTag('tauProngBoostedLooseIsolation', 'individual')

evtSelTauChargeBoostedLooseIsolation = copy.deepcopy(evtSelTauChargeBoosted)
evtSelTauChargeBoostedLooseIsolation.src_cumulative = cms.InputTag('tauChargeBoostedLooseIsolation', 'cumulative')
evtSelTauChargeBoostedLooseIsolation.src_individual = cms.InputTag('tauChargeBoostedLooseIsolation', 'individual')

evtSelTauMuonVetoBoostedLooseIsolation = copy.deepcopy(evtSelTauMuonVetoBoosted)
evtSelTauMuonVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('tauMuonVetoBoostedLooseIsolation', 'cumulative')
evtSelTauMuonVetoBoostedLooseIsolation.src_individual = cms.InputTag('tauMuonVetoBoostedLooseIsolation', 'individual')

evtSelTauElectronVetoBoostedLooseIsolation = copy.deepcopy(evtSelTauElectronVetoBoosted)
evtSelTauElectronVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('tauElectronVetoBoostedLooseIsolation', 'cumulative')
evtSelTauElectronVetoBoostedLooseIsolation.src_individual = cms.InputTag('tauElectronVetoBoostedLooseIsolation', 'individual')

evtSelTauEcalCrackVetoBoostedLooseIsolation = copy.deepcopy(evtSelTauEcalCrackVetoBoosted)
evtSelTauEcalCrackVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('tauEcalCrackVetoBoostedLooseIsolation', 'cumulative')
evtSelTauEcalCrackVetoBoostedLooseIsolation.src_individual = cms.InputTag('tauEcalCrackVetoBoostedLooseIsolation', 'individual')

evtSelRecoilJetBoostedLooseIsolation = copy.deepcopy(evtSelRecoilJetBoosted)
evtSelRecoilJetBoostedLooseIsolation.src = cms.InputTag('recoilJetBoostedLooseIsolation')

evtSelCentralJetVetoBoostedLooseIsolation = copy.deepcopy(evtSelCentralJetVetoBoosted)
evtSelCentralJetVetoBoostedLooseIsolation.src = cms.InputTag('centralJetVetoBoostedLooseIsolation')

evtSelPhiMetTauBoostedLooseIsolation = copy.deepcopy(evtSelPhiMetTauBoosted)
evtSelPhiMetTauBoostedLooseIsolation.src_cumulative = cms.InputTag('phiMetTauBoostedLooseIsolation', 'cumulative')
evtSelPhiMetTauBoostedLooseIsolation.src_individual = cms.InputTag('phiMetTauBoostedLooseIsolation', 'individual')

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

wTauNuBoostedEventDump_factorizedWithoutTauIsolation = copy.deepcopy(wTauNuBoostedEventDump)
wTauNuBoostedEventDump_factorizedWithoutTauIsolation.name = cms.string('wTauNuBoostedEventDump_factorizedWithoutTauIsolation')
#wTauNuEventDump_factorizedWithoutTauIsolation.output = cms.string("std::cout")
#wTauNuEventDump_factorizedWithoutTauIsolation.triggerConditions = cms.vstring("")

wTauNuBoostedEventDump_factorizedWithTauIsolation = copy.deepcopy(wTauNuBoostedEventDump)
wTauNuBoostedEventDump_factorizedWithTauIsolation.name = cms.string('wTauNuBoostedEventDump_factorizedWithTauIsolation')
#wTauNuEventDump_factorizedWithTauIsolation.output = cms.string("std::cout")
#wTauNuEventDump_factorizedWithTauIsolation.triggerConditions = cms.vstring("")

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

wTauNuBoostedAnalysisSequence_factorizedWithoutTauIsolation = copy.deepcopy(wTauNuBoostedAnalysisSequence)
replaceAnalyzerInputTags(wTauNuBoostedAnalysisSequence_factorizedWithoutTauIsolation,
                            [
        ["selectedLayer1TausForWTauNuLeadTrkPtCumulative", "selectedLayer1TausForWTauNuLeadTrkPtLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuEcalIsoCumulative", "selectedLayer1TausForWTauNuEcalIsoLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuTrkIsoCumulative", "selectedLayer1TausForWTauNuTrkIsoLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuProngCumulative", "selectedLayer1TausForWTauNuProngLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuChargeCumulative", "selectedLayer1TausForWTauNuChargeLooseIsolationCumulative"],      
        ["selectedLayer1TausForWTauNuMuonVetoCumulative", "selectedLayer1TausForWTauNuMuonVetoLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuElectronVetoCumulative", "selectedLayer1TausForWTauNuElectronVetoLooseIsolationCumulative"],
        ["selectedLayer1TausForWTauNuEcalCrackVetoCumulative", "selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative"],
        ["selectedLayer1JetsEt15ForWTauNuCumulative", "selectedLayer1JetsEt15ForWTauNuLooseIsolationCumulative"],
        ["selectedTauNuPairsDPhiMetTau","selectedTauNuPairsDPhiMetTauLooseIsolation"],
        ["tauRecoilEnergyFromCaloTowers","tauRecoilEnergyFromCaloTowersLooseIsolation"],
        ["tauRecoilEnergyFromForwardCaloTowers","tauRecoilEnergyFromForwardCaloTowersLooseIsolation"],
        ["tauRecoilEnergyFromJets ","tauRecoilEnergyFromJetsLooseIsolation"],
        ["allTauNuPairs","allTauNuPairsLooseIsolation"]
        ]
) 

wTauNuBoostedAnalysisSequence_factorizedWithTauIsolation = copy.deepcopy(wTauNuBoostedAnalysisSequence)
