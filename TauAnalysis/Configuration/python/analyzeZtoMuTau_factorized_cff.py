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

analyzeZtoMuTauEvents_factorizedWithMuonIsolation = analyzeZtoMuTauEvents.clone(
    name = cms.string('zMuTauAnalyzer_factorizedWithMuonIsolation'),
    analysisSequence = muTauAnalysisSequence_factorizedWithMuonIsolation
)
if len(analyzeZtoMuTauEvents_factorizedWithMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEvents_factorizedWithMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithMuonIsolation

analyzeZtoMuTauSequence_factorizedWithMuonIsolation = cms.Sequence(analyzeZtoMuTauEvents_factorizedWithMuonIsolation)

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

analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation = analyzeZtoMuTauEvents.clone(
    name = cms.string('zMuTauAnalyzer_factorizedWithoutMuonIsolation')
)
if len(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventDumps) > 0:
    analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
replaceEventSelections(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation, 
    [ [ evtSelMuonPFRelIso, evtSelMuonPFRelIsoLooseIsolation ],
      [ evtSelMuonAntiPion, evtSelMuonAntiPionLooseIsolation ],
      [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
      [ evtSelDiTauCandidateForMuTauAntiOverlapVeto, evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauZeroCharge, evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauAcoplanarity12, evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauMt1MET, evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauPzetaDiff, evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation ] ]
)                       
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithoutMuonIsolation

analyzeZtoMuTauSequence_factorizedWithoutMuonIsolation = cms.Sequence(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation)


