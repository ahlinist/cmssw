import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of Z --> mu + tau-jet events with and without muon isolation criteria applied;
# import config of "regular" Z --> mu + tau-jet analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *
from TauAnalysis.Configuration.analyzeZtoMuTau_cff import *

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoMuTauEvents_factorizedWithMuonIsolation = copy.deepcopy(analyzeZtoMuTauEvents)
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.name = cms.string('zMuTauAnalyzer_factorizedWithMuonIsolation')
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    vertexHistManager,
    triggerHistManager
)
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithMuonIsolation

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "loose" muon isolation criteria applied
#
# NOTE: modifications to analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation
#       modify the original analyzeZtoMuTauEvents sequence
#
#      --> analyzeZtoMuTauEvents_factorizedWithMuonIsolation needs to be defined
#          before analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation !!
#
#--------------------------------------------------------------------------------

analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation = copy.copy(analyzeZtoMuTauEvents)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.name = cms.string('zMuTauAnalyzer_factorizedWithoutMuonIsolation')
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonTrkIso)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonTrkIsoLooseIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonEcalIso)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonEcalIsoLooseIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonAntiPion)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonAntiPionLooseIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonTrkIP)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonTrkIPlooseIsolation)      
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauAntiOverlapVeto)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauZeroCharge)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauMt1MET)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithoutMuonIsolation

