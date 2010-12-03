import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.tools.analysisSequenceTools import replaceAnalyzerInputTags

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# muon candidate selection with "loose" muon isolation criteria applied
evtSelMuonPFRelIsoLooseIsolation = evtSelMuonPFRelIso.clone(
    src_cumulative = cms.InputTag('muonPFRelIsoCutLooseIsolation', 'cumulative'),
    src_individual = cms.InputTag('muonPFRelIsoCutLooseIsolation', 'individual')
)
evtSelMuonTrkIPlooseIsolation = evtSelMuonTrkIP.clone(
    src_cumulative = cms.InputTag('muonTrkIPcutLooseIsolation', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIPcutLooseIsolation', 'individual')
)    

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = evtSelDiTauCandidateForMuTauAntiOverlapVeto.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation = evtSelDiTauCandidateForMuTauMt1MET.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation = evtSelDiTauCandidateForMuTauPzetaDiff.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'individual')
)

evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation = evtSelDiTauCandidateForMuTauZeroCharge.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'individual')
)

evtSelDiTauCandidateForMuTauNonZeroChargeLooseMuonIsolation = evtSelDiTauCandidateForMuTauNonZeroCharge.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauNonZeroChargeCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauNonZeroChargeCutLooseMuonIsolation', 'individual')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump_factorizedWithoutMuonIsolation = muTauEventDump.clone(
    pluginName = cms.string('muTauEventDump_factorizedWithoutMuonIsolation'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)    

muTauEventDump_factorizedWithMuonIsolation = muTauEventDump.clone(
    pluginName = cms.string('muTauEventDump_factorizedWithMuonIsolation'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring()
)    

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

inputTagReplacements = [
    [ "selectedPatMuonsPFRelIsoCumulative", "selectedPatMuonsPFRelIsoLooseIsolationCumulative" ],
    [ "selectedPatMuonsTrkIPcumulative", "selectedPatMuonsTrkIPlooseIsolationCumulative" ],
    [ "selectedMuTauPairsAntiOverlapVetoCumulative", "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsMt1METcumulative", "selectedMuTauPairsMt1METlooseMuonIsolationCumulative" ],
    [ "selectedMuTauPairsPzetaDiffCumulative", "selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative" ],
    [ "muTauPairZmumuHypotheses", "muTauPairZmumuHypothesesLooseMuonIsolation" ],
    [ "muTauPairVisMassHypotheses", "muTauPairVisMassHypothesesLooseMuonIsolation" ]
]

muTauAnalysisSequenceOS_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequenceOS)
inputTagReplacementsOS = copy.deepcopy(inputTagReplacements)
inputTagReplacementsOS.append([ "selectedMuTauPairsZeroChargeCumulative", "selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative" ])
replaceAnalyzerInputTags(muTauAnalysisSequenceOS_factorizedWithoutMuonIsolation, inputTagReplacementsOS)

muTauAnalysisSequenceOS_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequenceOS)

muTauAnalysisSequenceSS_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequenceSS)
inputTagReplacementsSS = copy.deepcopy(inputTagReplacements)
inputTagReplacementsSS.append([ "selectedMuTauPairsNonZeroChargeCumulative", "selectedMuTauPairsNonZeroChargeLooseMuonIsolationCumulative" ])
replaceAnalyzerInputTags(muTauAnalysisSequenceSS_factorizedWithoutMuonIsolation, inputTagReplacementsSS)

muTauAnalysisSequenceSS_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequenceSS)

