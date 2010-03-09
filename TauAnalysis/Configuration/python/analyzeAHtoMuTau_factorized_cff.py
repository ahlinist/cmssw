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

analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithMuonIsolation = analyzeAHtoMuTauEvents_centralJetVeto.clone(
    name = cms.string('ahMuTauAnalyzer_centralJetVeto_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequence_centralJetVeto_factorizedWithMuonIsolation
)
analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithMuonIsolation = analyzeAHtoMuTauEvents_centralJetBtag.clone(
    name = cms.string('ahMuTauAnalyzer_centralJetBtag_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequence_centralJetBtag_factorizedWithMuonIsolation
)
analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeAHtoMuTauEvents_factorizedWithMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithMuonIsolation
   * analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithMuonIsolation
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
  [ [ evtSelMuonTrkIso, evtSelMuonTrkIsoLooseIsolation ],
    [ evtSelMuonEcalIso, evtSelMuonEcalIsoLooseIsolation ],
    [ evtSelMuonAntiPion, evtSelMuonAntiPionLooseIsolation ],
    [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto, evtSelDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauZeroCharge, evtSelDiTauCandidateForAHtoMuTauZeroChargeLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauMt1MET, evtSelDiTauCandidateForAHtoMuTauMt1METlooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauPzetaDiff, evtSelDiTauCandidateForAHtoMuTauPzetaDiffLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauNonBackToBack, evtSelDiTauCandidateForAHtoMuTauNonBackToBackLooseMuonIsolation ],
    [ evtSelDiTauCandidateForAHtoMuTauValidCollinearApprox, evtSelDiTauCandidateForAHtoMuTauValidCollinearApproxLooseMuonIsolation ],
    [ evtSelCentralJetVeto, evtSelCentralJetVetoLooseMuonIsolation ],
    [ evtSelCentralJetEt20, evtSelCentralJetEt20LooseMuonIsolation ],
    [ evtSelCentralJetEt20bTag, evtSelCentralJetEt20bTagLooseMuonIsolation ] ]

analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEvents_centralJetVeto.clone(
    name = cms.string('ahMuTauAnalyzer_centralJetVeto_factorizedWithoutMuonIsolation')
)
analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithoutMuonIsolation, eventSelectionReplacements)
analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequence_centralJetVeto_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithoutMuonIsolation = analyzeAHtoMuTauEvents_centralJetBtag.clone(
    name = cms.string('ahMuTauAnalyzer_centralJetBtag_factorizedWithoutMuonIsolation')
)
analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithoutMuonIsolation, eventSelectionReplacements)
analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithoutMuonIsolation.analysisSequence = \
  muTauAnalysisSequence_centralJetBtag_factorizedWithoutMuonIsolation

analyzeAHtoMuTauEvents_factorizedWithoutMuonIsolation = cms.Sequence(
    analyzeAHtoMuTauEvents_centralJetVeto_factorizedWithoutMuonIsolation
   * analyzeAHtoMuTauEvents_centralJetBtag_factorizedWithoutMuonIsolation
)    
