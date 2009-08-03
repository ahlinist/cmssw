import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of Z --> mu + tau-jet events with and without muon isolation criteria applied;
# import config of "regular" Z --> mu + tau-jet analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *
from TauAnalysis.Configuration.analyzeZtoMuTau_cff import *
from TauAnalysis.Configuration.factorizationTools import replaceEventSelections

#--------------------------------------------------------------------------------
# define Z --> mu + tau-jet analysis module
# for the path with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoMuTauEvents_factorizedWithMuonIsolation = copy.deepcopy(analyzeZtoMuTauEvents)
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.name = cms.string('zMuTauAnalyzer_factorizedWithMuonIsolation')
analyzeZtoMuTauEvents_factorizedWithMuonIsolation.histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    tauHistManager,
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
replaceEventSelections(analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation, 
    [ [ evtSelMuonTrkIso, evtSelMuonTrkIsoLooseIsolation ],
      [ evtSelMuonEcalIso, evtSelMuonEcalIsoLooseIsolation ],
      [ evtSelMuonAntiPion, evtSelMuonAntiPionLooseIsolation ],
      [ evtSelMuonTrkIP, evtSelMuonTrkIPlooseIsolation ],
      [ evtSelDiTauCandidateForMuTauAntiOverlapVeto, evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauZeroCharge, evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauAcoplanarity12, evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauMt1MET, evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation ],
      [ evtSelDiTauCandidateForMuTauPzetaDiff, evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation ] ]
)                       
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.eventDumps[0] = muTauEventDump_factorizedWithoutMuonIsolation
analyzeZtoMuTauEvents_factorizedWithoutMuonIsolation.analysisSequence = muTauAnalysisSequence_factorizedWithoutMuonIsolation

