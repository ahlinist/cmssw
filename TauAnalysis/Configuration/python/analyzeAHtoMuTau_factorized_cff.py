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

analyzeAHtoMuTauEvents_woBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEvents_woBtag.clone(
    name = cms.string('ahMuTauAnalyzer_woBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequence_woBtag_factorizedWithMuonIsolation
)


analyzeAHtoMuTauEvents_wBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEvents_wBtag.clone(
    name = cms.string('ahMuTauAnalyzer_wBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequence_wBtag_factorizedWithMuonIsolation
)

# Add in the event dumps, if they exist
if len(analyzeAHtoMuTauEvents_woBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEvents_woBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation
if len(analyzeAHtoMuTauEvents_wBtag_factorizedWithMuonIsolation.eventDumps):
    analyzeAHtoMuTauEvents_wBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

# split analysis into b-Tag and no-b-Tag channels
analyzeAHtoMuTauEvents_factorizedWithMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEvents_woBtag_factorizedWithMuonIsolation
   * analyzeAHtoMuTauEvents_wBtag_factorizedWithMuonIsolation
)
# disable b-Tag channel for now
#analyzeAHtoMuTauEvents_factorizedWithMuonIsolation = cms.Sequence(analyzeAHtoMuTauEvents_woBtag_factorizedWithMuonIsolation)

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
    [ evtSelMuonAntiPion, evtSelMuonAntiPionLooseIsolation ],
    [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto, evtSelDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauZeroCharge, evtSelDiTauCandidateForAHtoMuTauZeroChargeLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauMt1MET, evtSelDiTauCandidateForAHtoMuTauMt1METlooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauPzetaDiff, evtSelDiTauCandidateForAHtoMuTauPzetaDiffLooseMuonIsolation ],
    [ evtSelNonCentralJetEt20bTag, evtSelNonCentralJetEt20bTagLooseMuonIsolation ],
    [ evtSelCentralJetEt20, evtSelCentralJetEt20LooseMuonIsolation ],
    [ evtSelCentralJetEt20bTag, evtSelCentralJetEt20bTagLooseMuonIsolation ] ]

analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEvents_woBtag.clone(
    name = cms.string('ahMuTauAnalyzer_woBtag_factorizedWithoutMuonIsolation')
)

replaceEventSelections(analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation, eventSelectionReplacements)
analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequence_woBtag_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEvents_wBtag.clone(
    name = cms.string('ahMuTauAnalyzer_wBtag_factorizedWithoutMuonIsolation')
)
if analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
if analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation.eventDumps:
    analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation

replaceEventSelections(analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation, eventSelectionReplacements)
analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequence_wBtag_factorizedWithoutMuonIsolation

# split analysis into b-Tag and no-b-Tag channels
analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation
   * analyzeAHtoMuTauEvents_wBtag_factorizedWithoutMuonIsolation
)
# disable b-Tag channel for now
#analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation = cms.Sequence(analyzeAHtoMuTauEvents_woBtag_factorizedWithoutMuonIsolation)
