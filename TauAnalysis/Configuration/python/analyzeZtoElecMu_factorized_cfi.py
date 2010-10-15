import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.tools.analysisSequenceTools import replaceAnalyzerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> elec + mu events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# electron candidate selection with "loose" electron isolation criteria applied
evtSelElectronIsoLooseIsolation = copy.deepcopy(evtSelElectronIso)
evtSelElectronIsoLooseIsolation.src_cumulative = cms.InputTag('electronIsoCutLooseIsolation', 'cumulative')
evtSelElectronIsoLooseIsolation.src_individual = cms.InputTag('electronIsoCutLooseIsolation', 'individual')

# selection of di-tau candidates composed of combination of muon with "loosely" isolated electron 
evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuZeroCharge)
evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuZeroChargeLooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuMt1MET)
evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuMt1METcutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuMt1METlooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuMt1METcutLooseElectronIsolation', 'individual')

evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation = copy.deepcopy(evtSelDiTauCandidateForElecMuMt2MET)
evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation.src_cumulative = cms.InputTag('diTauCandidateForElecMuMt2METcutLooseElectronIsolation', 'cumulative')
evtSelDiTauCandidateForElecMuMt2METlooseElectronIsolation.src_individual = cms.InputTag('diTauCandidateForElecMuMt2METcutLooseElectronIsolation', 'individual')

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump_factorizedWithoutElectronIsolation = copy.deepcopy(elecMuEventDump)
elecMuEventDump_factorizedWithoutElectronIsolation.name = cms.string('elecMuEventDump_factorizedWithoutElectronIsolation')
elecMuEventDump_factorizedWithoutElectronIsolation.output = cms.string("std::cout")
elecMuEventDump_factorizedWithoutElectronIsolation.triggerConditions = cms.vstring()

elecMuEventDump_factorizedWithElectronIsolation = copy.deepcopy(elecMuEventDump)
elecMuEventDump_factorizedWithElectronIsolation.name = cms.string('elecMuEventDump_factorizedWithElectronIsolation')
elecMuEventDump_factorizedWithElectronIsolation.output = cms.string("std::cout")
elecMuEventDump_factorizedWithElectronIsolation.triggerConditions = cms.vstring()

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecMuAnalysisSequence_factorizedWithoutElectronIsolation = copy.deepcopy(elecMuAnalysisSequence)
replaceAnalyzerInputTags(elecMuAnalysisSequence_factorizedWithoutElectronIsolation,
    [ ["selectedPatElectronsForElecMuIsoCumulative", "selectedPatElectronsForElecMuIsoLooseIsolationCumulative"],
      ["selectedElecMuPairsZeroChargeCumulative", "selectedElecMuPairsZeroChargeLooseElectronIsolationCumulative"],
      ["selectedElecMuPairsMt1METcumulative", "selectedElecMuPairsMt1METlooseElectronIsolationCumulative"],
      ["selectedElecMuPairsMt2METcumulative", "selectedElecMuPairsMt2METlooseElectronIsolationCumulative"],
      ["elecMuPairZmumuHypotheses", "elecMuPairZmumuHypothesesLooseElectronIsolation"],
      ["elecMuPairVisMassHypotheses", "elecMuPairVisMassHypothesesLooseElectronIsolation"] ]
) 

elecMuAnalysisSequence_factorizedWithElectronIsolation = copy.deepcopy(elecMuAnalysisSequence)
