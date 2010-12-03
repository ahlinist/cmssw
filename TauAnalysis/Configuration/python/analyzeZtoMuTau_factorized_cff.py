import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of Z --> mu + tau-jet events with and without muon isolation criteria applied;
# import config of "regular" Z --> mu + tau-jet analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *
from TauAnalysis.Configuration.analyzeZtoMuTau_cff import *
from TauAnalysis.Configuration.tools.factorizationTools import replaceEventSelections

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation = analyzeZtoMuTauEventsOS.clone(
    name = cms.string('zMuTauAnalyzerOS_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceOS_factorizedWithMuonIsolation
)
if len(analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation = analyzeZtoMuTauEventsOS.clone(
    name = cms.string('zMuTauAnalyzerSS_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceSS_factorizedWithMuonIsolation
)
if len(analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeZtoMuTauSequence_factorizedWithMuonIsolation = cms.Sequence(
    analyzeZtoMuTauEventsOS_factorizedWithMuonIsolation * analyzeZtoMuTauEventsSS_factorizedWithMuonIsolation
)

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

eventSelectionReplacements = [
    [ evtSelMuonPFRelIso, evtSelMuonPFRelIsoLooseIsolation ],
    [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
    [ evtSelDiTauCandidateForMuTauAntiOverlapVeto, evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation ],
    [ evtSelDiTauCandidateForMuTauMt1MET, evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation ],
    [ evtSelDiTauCandidateForMuTauPzetaDiff, evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation ]
]

analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation = analyzeZtoMuTauEventsOS.clone(
    name = cms.string('zMuTauAnalyzerOS_factorizedWithoutMuonIsolation')
)
if len(analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
eventSelectionReplacementsOS = copy.deepcopy(eventSelectionReplacements)
eventSelectionReplacementsOS.append([ evtSelDiTauCandidateForMuTauZeroCharge,
                                      evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation ])
replaceEventSelections(analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation, eventSelectionReplacementsOS)
analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequenceOS_factorizedWithoutMuonIsolation

analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation = analyzeZtoMuTauEventsSS.clone(
    name = cms.string('zMuTauAnalyzerSS_factorizedWithoutMuonIsolation')
)
if len(analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
eventSelectionReplacementsSS = copy.deepcopy(eventSelectionReplacements)
eventSelectionReplacementsSS.append([ evtSelDiTauCandidateForMuTauNonZeroCharge,
                                      evtSelDiTauCandidateForMuTauNonZeroChargeLooseMuonIsolation ])
replaceEventSelections(analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation, eventSelectionReplacementsSS)
analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequenceSS_factorizedWithoutMuonIsolation

analyzeZtoMuTauSequence_factorizedWithoutMuonIsolation = cms.Sequence(
    analyzeZtoMuTauEventsOS_factorizedWithoutMuonIsolation * analyzeZtoMuTauEventsSS_factorizedWithoutMuonIsolation
)


