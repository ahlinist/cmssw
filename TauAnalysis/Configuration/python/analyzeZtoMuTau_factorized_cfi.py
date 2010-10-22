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
evtSelMuonAntiPionLooseIsolation = evtSelMuonAntiPion.clone(
    src_cumulative = cms.InputTag('muonAntiPionCutLooseIsolation', 'cumulative'),
    src_individual = cms.InputTag('muonAntiPionCutLooseIsolation', 'individual')
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
evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation = evtSelDiTauCandidateForMuTauZeroCharge.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForMuTauAcoplanarity12LooseMuonIsolation = evtSelDiTauCandidateForMuTauAcoplanarity12.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation = evtSelDiTauCandidateForMuTauMt1MET.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'individual')
)    
evtSelDiTauCandidateForMuTauPzetaDiffLooseMuonIsolation = evtSelDiTauCandidateForMuTauPzetaDiff.clone(
    src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation', 'individual')
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

muTauAnalysisSequence_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
replaceAnalyzerInputTags(muTauAnalysisSequence_factorizedWithoutMuonIsolation,
    [ [ "selectedPatMuonsPFRelIsoCumulative", "selectedPatMuonsPFRelIsoLooseIsolationCumulative" ],
      [ "selectedPatMuonsPionVetoCumulative", "selectedPatMuonsPionVetoLooseIsolationCumulative" ],
      [ "selectedPatMuonsTrkIPcumulative", "selectedPatMuonsTrkIPlooseIsolationCumulative" ],
      [ "selectedMuTauPairsAntiOverlapVetoCumulative", "selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative" ],
      [ "selectedMuTauPairsZeroChargeCumulative", "selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative" ],
      [ "selectedMuTauPairsAcoplanarity12Cumulative", "selectedMuTauPairsAcoplanarity12LooseMuonIsolationCumulative" ],
      [ "selectedMuTauPairsMt1METcumulative", "selectedMuTauPairsMt1METlooseMuonIsolationCumulative" ],
      [ "selectedMuTauPairsPzetaDiffCumulative", "selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative" ],
      [ "muTauPairZmumuHypotheses", "muTauPairZmumuHypothesesLooseMuonIsolation" ],
      [ "muTauPairVisMassHypotheses", "muTauPairVisMassHypothesesLooseMuonIsolation" ] ]
) 

muTauAnalysisSequence_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
