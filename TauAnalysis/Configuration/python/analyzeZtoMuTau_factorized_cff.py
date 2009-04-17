import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of Z --> mu + tau-jet events with and without muon isolation criteria applied;
# import config of "regular" Z --> mu + tau-jet analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *
from TauAnalysis.Configuration.analyzeZtoMuTau_cff import *
#from TauAnalysis.Configuration.factorizationTools import replaceEventSelection

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "loose" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation = copy.copy(analyzeZtoMuTauEvents)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.name = cms.string('zMuTauAnalyzer_factorizedWithoutMuonIsolation')
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonTrkIso)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonTrkIsoLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonEcalIso)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonEcalIsoLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonAntiPion)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonAntiPionLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelMuonTrkIP)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelMuonTrkIPlooseMuonIsolation)      
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauAntiOverlapVeto)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauZeroCharge)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.remove(evtSelDiTauCandidateForMuTauMt1MET)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection.append(evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelMuonTrkIso, evtSelMuonTrkIsoLooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelMuonEcalIso, evtSelMuonEcalIsoLooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelMuonAntiPion, evtSelMuonAntiPionLooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelMuonTrkIP, evtSelMuonTrkIPlooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelDiTauCandidateForMuTauAntiOverlapVeto, evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelDiTauCandidateForMuTauZeroCharge, evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation)
#replaceEventSelection(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventSelection,
#                      evtSelDiTauCandidateForMuTauMt1MET, evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation)
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithoutMuonIsolation

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoMuTauEvents_factorizedWithMuonIsolation = copy.deepcopy(analyzeZtoMuTauEvents)
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    vertexHistManager,
    triggerHistManager
)
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithMuonIsolation

