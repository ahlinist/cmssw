import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analysisSequenceTools import switchHistManagers
from TauAnalysis.Configuration.analysisSequenceTools import replaceHistManagerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> elec + mu events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

elecMuHistManagers_factorizedWithoutElectronIsolation = copy.deepcopy(elecMuHistManagers)

elecMuHistManagers_factorizedWithElectronIsolation = cms.vstring(
    'genPhaseSpaceEventInfoHistManager',
    'electronHistManager',
    'muonHistManager',
    'vertexHistManager',
    'triggerHistManager'
)

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# electron candidate selection with "loose" electron isolation criteria applied
evtSelElectronTrkIsoLooseIsolation = copy.deepcopy(evtSelElectronTrkIso)
evtSelElectronTrkIsoLooseIsolation.src_cumulative = cms.InputTag('electronTrkIsoCutLooseIsolation', 'cumulative')
evtSelElectronTrkIsoLooseIsolation.src_individual = cms.InputTag('electronTrkIsoCutLooseIsolation', 'individual')

evtSelElectronEcalIsoLooseIsolation = copy.deepcopy(evtSelElectronEcalIso)
evtSelElectronEcalIsoLooseIsolation.src_cumulative = cms.InputTag('electronEcalIsoCutLooseIsolation', 'cumulative')
evtSelElectronEcalIsoLooseIsolation.src_individual = cms.InputTag('electronEcalIsoCutLooseIsolation', 'individual')

evtSelElectronTrkLooseIsolation = copy.deepcopy(evtSelElectronTrk)
evtSelElectronTrkLooseIsolation.src_cumulative = cms.InputTag('electronTrkCutLooseIsolation', 'cumulative')
evtSelElectronTrkLooseIsolation.src_individual = cms.InputTag('electronTrkCutLooseIsolation', 'individual')

evtSelElectronTrkIPlooseIsolation = copy.deepcopy(evtSelElectronTrkIP)
evtSelElectronTrkIPlooseIsolation.src_cumulative = cms.InputTag('electronTrkIPcutLooseIsolation', 'cumulative')
evtSelElectronTrkIPlooseIsolation.src_individual = cms.InputTag('electronTrkIPcutLooseIsolation', 'individual')

# selection of di-tau candidates composed of combination of muon with "loosely" isolated electron 
evtSelDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuAntiOverlapVeto)
evtSelDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuZeroCharge)
evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuAcoplanarity12LooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuAcoplanarity12)
evtSelDiTauCandidateForElecMuAcoplanarity12LooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuAcoplanarity12LooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuMt1MET)
evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuMt1METcutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuMt1METcutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuMt2MET)
evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuMt2METcutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuMt2METcutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuPzetaDiffLooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuPzetaDiff)
evtSelDiTauCandidateForElecMuPzetaDiffLooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuPzetaDiffLooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation', 'individual')

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump_factorizedWithoutElectronIsolation = copy.deepcopy(elecMuEventDump)
elecMuEventDump_factorizedWithoutElectronIsolation.name = cms.string('elecMuEventDump_factorizedWithoutElectronIsolation')
elecMuEventDump_factorizedWithoutElectronIsolation.output = cms.string("std::cout")
elecMuEventDump_factorizedWithoutElectronIsolation.triggerConditions = cms.vstring("")

elecMuEventDump_factorizedWithElectronIsolation = copy.deepcopy(elecMuEventDump)
elecMuEventDump_factorizedWithElectronIsolation.name = cms.string('elecMuEventDump_factorizedWithElectronIsolation')
elecMuEventDump_factorizedWithElectronIsolation.output = cms.string("std::cout")
elecMuEventDump_factorizedWithElectronIsolation.triggerConditions = cms.vstring("")

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecMuAnalysisSequence_factorizedWithoutElectronIsolation = copy.deepcopy(elecMuAnalysisSequence)
switchHistManagers(elecMuAnalysisSequence_factorizedWithoutElectronIsolation, elecMuHistManagers_factorizedWithoutElectronIsolation)
replaceHistManagerInputTags(elecMuAnalysisSequence_factorizedWithoutElectronIsolation,
    [ ["selectedLayer1ElectronsTrkIsoCumulative", "selectedLayer1ElectronsTrkIsoLooseIsolationCumulative"],
      ["selectedLayer1ElectronsEcalIsoCumulative", "selectedLayer1ElectronsEcalIsoLooseIsolationCumulative"],
      ["selectedLayer1ElectronsTrkCumulative", "selectedLayer1ElectronsTrkLooseIsolationCumulative"],
      ["selectedLayer1ElectronsTrkIPcumulative", "selectedLayer1ElectronsTrkIPlooseIsolationCumulative"],
      ["selectedElecMuPairsAntiOverlapVetoCumulative", "selectedElecMuPairsAntiOverlapVetoLooseElectronIsolationCumulative"],
      ["selectedElecMuPairsZeroChargeCumulative", "selectedElecMuPairsZeroChargeLooseElectronIsolationCumulative"],
      ["selectedElecMuPairsAcoplanarity12Cumulative", "selectedElecMuPairsAcoplanarity12LooseElectronIsolationCumulative"],
      ["selectedElecMuPairsMt1METcumulative", "selectedElecMuPairsMt1METlooseElectronIsolationCumulative"],
      ["selectedElecMuPairsMt2METcumulative", "selectedElecMuPairsMt2METlooseElectronIsolationCumulative"],
      ["selectedElecMuPairsPzetaDiffCumulative", "selectedElecMuPairsPzetaDiffLooseElectronIsolationCumulative"] ]
) 
elecMuAnalysisSequence_factorizedWithElectronIsolation = copy.deepcopy(elecMuAnalysisSequence)
switchHistManagers(elecMuAnalysisSequence_factorizedWithElectronIsolation, elecMuHistManagers_factorizedWithElectronIsolation)
