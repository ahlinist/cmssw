import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# import the two configs for event selection, event print-out and analysis sequence
# of Z --> elec + mu events with and without muon isolation criteria applied;
# import config of "regular" Z --> elec + mu analysis module
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecMu_factorized_cfi import *
from TauAnalysis.Configuration.analyzeZtoElecMu_cff import *
from TauAnalysis.Configuration.factorizationTools import replaceEventSelections

#--------------------------------------------------------------------------------
# define Z --> elec + mu analysis module
# for the path with "regular" muon isolation criteria applied
#--------------------------------------------------------------------------------

analyzeZtoElecMuEvents_factorizedWithElectronIsolation = copy.deepcopy(analyzeZtoElecMuEvents)
analyzeZtoElecMuEvents_factorizedWithElectronIsolation.name = cms.string('zElecMuAnalyzer_factorizedWithElectronIsolation')
analyzeZtoElecMuEvents_factorizedWithElectronIsolation.histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    electronHistManager,
    muonHistManager,
    vertexHistManager,
    triggerHistManager
)
analyzeZtoElecMuEvents_factorizedWithElectronIsolation.eventDumps[0] = elecMuEventDump_factorizedWithElectronIsolation
analyzeZtoElecMuEvents_factorizedWithElectronIsolation.analysisSequence = elecMuAnalysisSequence_factorizedWithElectronIsolation

#--------------------------------------------------------------------------------
# define Z --> mu + electron analysis module
# for the path with "loose" muon isolation criteria applied
#
# NOTE: modifications to analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation
#       modify the original analyzeZtoElecMuEvents sequence
#
#      --> analyzeZtoElecMuEvents_factorizedWithElectronIsolation needs to be defined
#          before analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation !!
#
#--------------------------------------------------------------------------------

analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation = copy.copy(analyzeZtoElecMuEvents)
analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation.name = cms.string('zElecMuAnalyzer_factorizedWithoutElectronIsolation')
replaceEventSelections(analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation,
    [ [ evtSelElectronTrkIso, evtSelElectronTrkIsoLooseIsolation ],
      [ evtSelElectronEcalIso, evtSelElectronEcalIsoLooseIsolation ],
      [ evtSelElectronTrk, evtSelElectronTrkLooseIsolation ],
      [ evtSelElectronTrkIP, evtSelElectronTrkIPlooseIsolation ],
      [ evtSelDiTauCandidateForElecMuAntiOverlapVeto, evtSelDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation ],
      [ evtSelDiTauCandidateForElecMuZeroCharge, evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation ],
      [ evtSelDiTauCandidateForElecMuAcoplanarity12, evtSelDiTauCandidateForElecMuAcoplanarity12LooseElectronIsolation ],
      [ evtSelDiTauCandidateForElecMuMt1MET, evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation ],
      [ evtSelDiTauCandidateForElecMuMt2MET, evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation ],
      [ evtSelDiTauCandidateForElecMuPzetaDiff, evtSelDiTauCandidateForElecMuPzetaDiffLooseElectronIsolation] ]
)                       
analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation.eventDumps[0] = elecMuEventDump_factorizedWithoutElectronIsolation
analyzeZtoElecMuEvents_factorizedWithoutElectronIsolation.analysisSequence = elecMuAnalysisSequence_factorizedWithoutElectronIsolation

