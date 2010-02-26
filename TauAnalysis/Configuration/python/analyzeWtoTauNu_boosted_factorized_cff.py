import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of W --> tau-jet + nu events with and without tau isolation criteria applied;
# import config of "regular" W --> tau-jet + nu analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeWtoTauNu_boosted_factorized_cfi import *
from TauAnalysis.Configuration.analyzeWtoTauNu_boosted_cff import *
from TauAnalysis.Configuration.tools.factorizationTools import replaceEventSelections

#--------------------------------------------------------------------------------
# define W --> tau-jet + nu analysis module
# for the path with "regular" tau isolation criteria applied
#--------------------------------------------------------------------------------

analyzeWtoTauNuEventsBoosted_factorizedWithTauIsolation = copy.deepcopy(analyzeWtoTauNuEventsBoosted)
analyzeWtoTauNuEventsBoosted_factorizedWithTauIsolation.name = cms.string('wTauNuBoostedAnalyzer_factorizedWithTauIsolation')
analyzeWtoTauNuEventsBoosted_factorizedWithTauIsolation.eventDumps[0] = wTauNuBoostedEventDump_factorizedWithTauIsolation
analyzeWtoTauNuEventsBoosted_factorizedWithTauIsolation.analysisSequence = wTauNuBoostedAnalysisSequence_factorizedWithTauIsolation

#--------------------------------------------------------------------------------
# define W --> tau-jet + nu analysis module for the path with "loose" tau isolation criteria applied
#
# NOTE: modifications to analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation
#       modify the original analyzeWtoTauNuEventsBoosted sequence
#
#      --> analyzeWtoTauNuEventsBoosted_factorizedWithTauIsolation needs to be defined
#          before analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation !!
#--------------------------------------------------------------------------------

analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation = copy.copy(analyzeWtoTauNuEventsBoosted)
analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation.name = cms.string('wTauNuBoostedAnalyzer_factorizedWithoutTauIsolation')
replaceEventSelections(analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation, 
                       [ 
        [ evtSelTauLeadTrkPtBoosted, evtSelTauLeadTrkPtBoostedLooseIsolation],
        [ evtSelTauIsoBoosted, evtSelTauEcalIsoBoostedLooseIsolation],
        [ evtSelTauTaNCBoosted, evtSelTauTrkIsoBoostedLooseIsolation ],
        [ evtSelTauProngBoosted, evtSelTauProngBoostedLooseIsolation ],
        [ evtSelTauChargeBoosted, evtSelTauChargeBoostedLooseIsolation ],
        [ evtSelTauMuonVetoBoosted, evtSelTauMuonVetoBoostedLooseIsolation ],
        [ evtSelTauElectronVetoBoosted, evtSelTauElectronVetoBoostedLooseIsolation ],
        [ evtSelTauEcalCrackVetoBoosted, evtSelTauEcalCrackVetoBoostedLooseIsolation ],
        [ evtSelRecoilJetBoosted, evtSelRecoilJetBoostedLooseIsolation ],
        [ evtSelCentralJetVetoBoosted, evtSelCentralJetVetoBoostedLooseIsolation],
        [ evtSelPhiMetTauBoosted, evtSelPhiMetTauBoostedLooseIsolation]
        ]
                       )                       
analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation.eventDumps[0] = wTauNuBoostedEventDump_factorizedWithoutTauIsolation
analyzeWtoTauNuEventsBoosted_factorizedWithoutTauIsolation.analysisSequence = wTauNuBoostedAnalysisSequence_factorizedWithoutTauIsolation
