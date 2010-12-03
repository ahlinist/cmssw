import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of MSSM Higgs A/H --> mu + tau-jet events with and without muon isolation criteria applied;
# import config of "regular" A/H --> mu + tau-jet analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeAHtoMuTau_factorized_cfi import *
from TauAnalysis.Configuration.analyzeAHtoMuTau_cff import *
from TauAnalysis.Configuration.tools.factorizationTools import replaceEventSelections

#--------------------------------------------------------------------------------
# define A/H --> mu + tau-jet analysis modules
# for the paths with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEventsOS_woBtag.clone(
    name = cms.string('ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceOS_woBtag_factorizedWithMuonIsolation
)

analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEventsOS_wBtag.clone(
    name = cms.string('ahMuTauAnalyzerOS_wBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceOS_wBtag_factorizedWithMuonIsolation
)

analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEventsOS_woBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_woBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceSS_woBtag_factorizedWithMuonIsolation
)

analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEventsOS_wBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_wBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequenceSS_wBtag_factorizedWithMuonIsolation
)

# Add in the event dumps, if they exist
if len(analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
if len(analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
if len(analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
if len(analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeAHtoMuTauEvents_factorizedWithMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEventsOS_woBtag_factorizedWithMuonIsolation * analyzeAHtoMuTauEventsOS_wBtag_factorizedWithMuonIsolation
   * analyzeAHtoMuTauEventsSS_woBtag_factorizedWithMuonIsolation * analyzeAHtoMuTauEventsSS_wBtag_factorizedWithMuonIsolation
)

#--------------------------------------------------------------------------------
# define A/H --> mu + tau-jet analysis modules
# for the path with "loose" muon isolation criteria applied
#
# NOTE: modifications to analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation
#       modify the original analyzeAHtoMuTauEvents sequence
#
#      --> analyzeAHtoMuTauEvents_factorizedWithMuonIsolation needs to be defined
#          before analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation !!
#
#--------------------------------------------------------------------------------

eventSelectionReplacements = \
  [ [ evtSelMuonPFRelIso, evtSelMuonPFRelIsoLooseIsolation ],
    [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto, evtSelDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauMt1MET, evtSelDiTauCandidateForAHtoMuTauMt1METlooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauPzetaDiff, evtSelDiTauCandidateForAHtoMuTauPzetaDiffLooseMuonIsolation ],
    [ evtSelNonCentralJetEt20bTag, evtSelNonCentralJetEt20bTagLooseMuonIsolation ],
    [ evtSelCentralJetEt20, evtSelCentralJetEt20LooseMuonIsolation ],
    [ evtSelCentralJetEt20bTag, evtSelCentralJetEt20bTagLooseMuonIsolation ] ]

eventSelectionReplacementsOS = copy.deepcopy(eventSelectionReplacements)
eventSelectionReplacementsOS.append([ evtSelDiTauCandidateForAHtoMuTauZeroCharge,
                                      evtSelDiTauCandidateForAHtoMuTauZeroChargeLooseMuonIsolation ])

analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEventsOS_woBtag.clone(
    name = cms.string('ahMuTauAnalyzerOS_woBtag_factorizedWithoutMuonIsolation')
)
if analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation, eventSelectionReplacementsOS)
analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequenceOS_woBtag_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEventsOS_wBtag.clone(
    name = cms.string('ahMuTauAnalyzerOS_wBtag_factorizedWithoutMuonIsolation')
)
if analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation, eventSelectionReplacementsOS)
analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequenceOS_wBtag_factorizedWithoutMuonIsolation

eventSelectionReplacementsSS = copy.deepcopy(eventSelectionReplacements)
eventSelectionReplacementsSS.append([ evtSelDiTauCandidateForAHtoMuTauNonZeroCharge,
                                      evtSelDiTauCandidateForAHtoMuTauNonZeroChargeLooseMuonIsolation ])

analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEventsSS_woBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_woBtag_factorizedWithoutMuonIsolation')
)
if analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation, eventSelectionReplacementsSS)
analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequenceSS_woBtag_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEventsSS_wBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_wBtag_factorizedWithoutMuonIsolation')
)
if analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation, eventSelectionReplacementsSS)
analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequenceSS_wBtag_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEventsOS_woBtag_factorizedWithoutMuonIsolation * analyzeAHtoMuTauEventsOS_wBtag_factorizedWithoutMuonIsolation
   * analyzeAHtoMuTauEventsSS_woBtag_factorizedWithoutMuonIsolation * analyzeAHtoMuTauEventsSS_wBtag_factorizedWithoutMuonIsolation
)
