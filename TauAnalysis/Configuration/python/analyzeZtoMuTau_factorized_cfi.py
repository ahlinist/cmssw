import FWCore.ParameterSet.Config as cms
import copy

# import config for event selection, event print-out and analysis sequence
# defined for the "regular" case without factorization of muon isolation
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

muTauHistManagers_factorizedWithoutMuonIsolation = copy.deepcopy(muTauHistManagers)

muTauHistManagers_factorizedWithMuonIsolation = cms.vstring( 'genPhaseSpaceEventInfoHistManager',
                                                             'muonHistManager',
                                                             'vertexHistManager',
                                                             'triggerHistManager' )

#--------------------------------------------------------------------------------
# define event selection criteria specific to factorization
#--------------------------------------------------------------------------------

# muon candidate selection with "loose" muon isolation criteria applied
muonTrkIsoCutLooseMuonIsolation = copy.deepcopy(muonTrkIsoCut)
muonTrkIsoCutLooseMuonIsolation.name = cms.string('muonTrkIsoCutLooseMuonIsolation')
muonTrkIsoCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulativeLooseMuonIsolation')
muonTrkIsoCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividualLooseMuonIsolation')

muonEcalIsoCutLooseMuonIsolation = copy.deepcopy(muonEcalIsoCut)
muonEcalIsoCutLooseMuonIsolation.name = cms.string('muonEcalIsoCutLooseMuonIsolation')
muonEcalIsoCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulativeLooseMuonIsolation')
muonEcalIsoCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividualLooseMuonIsolation')

muonAntiPionCutLooseMuonIsolation = copy.deepcopy(muonAntiPionCut)
muonAntiPionCutLooseMuonIsolation.name = cms.string('muonAntiPionCutLooseMuonIsolation')
muonAntiPionCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulativeLooseMuonIsolation')
muonAntiPionCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividualLooseMuonIsolation')

muonTrkIPcutLooseMuonIsolation = copy.deepcopy(muonTrkIPcut)
muonTrkIPcutLooseMuonIsolation.name = cms.string('muonTrkIPcutLooseMuonIsolation')
muonTrkIPcutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation')
muonTrkIPcutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividualLooseMuonIsolation')

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(diTauCandidateForMuTauAntiOverlapVeto)
diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.name = cms.string('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation')
diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation')

diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation = copy.deepcopy(diTauCandidateForMuTauAcoplanarityCut)
diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation.name = cms.string('diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation')
diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsAcoplanarityCumulativeLooseMuonIsolation')
diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsAcoplanarityIndividualLooseMuonIsolation')

diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation = copy.deepcopy(diTauCandidateForMuTauZeroChargeCut)
diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.name = cms.string('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation')
diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation')
diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation')

diTauCandidateForMuTauMt1METCutLooseMuonIsolation = copy.deepcopy(diTauCandidateForMuTauMt1METCut)
diTauCandidateForMuTauMt1METCutLooseMuonIsolation.name = cms.string('diTauCandidateForMuTauMt1METCutLooseMuonIsolation')
diTauCandidateForMuTauMt1METCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsMt1METCumulativeLooseMuonIsolation')
diTauCandidateForMuTauMt1METCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsMt1METIndividualLooseMuonIsolation')

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump_factorizedWithoutMuonIsolation = copy.deepcopy(muTauEventDump)
muTauEventDump_factorizedWithoutMuonIsolation.name = cms.string('muTauEventDump_factorizedWithoutMuonIsolation')
muTauEventDump_factorizedWithoutMuonIsolation.output = cms.string("std::cout")
muTauEventDump_factorizedWithoutMuonIsolation.triggerConditions = cms.vstring("")

muTauEventDump_factorizedWithMuonIsolation = copy.deepcopy(muTauEventDump)
muTauEventDump_factorizedWithMuonIsolation.name = cms.string('muTauEventDump_factorizedWithMuonIsolation')
muTauEventDump_factorizedWithMuonIsolation.output = cms.string("std::cout")
muTauEventDump_factorizedWithMuonIsolation.triggerConditions = cms.vstring("")

#--------------------------------------------------------------------------------
# define factorization specific analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence_factorizedWithoutMuonIsolation = cms.VPSet(
  # fill histograms for full event sample
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),

  # generator level phase-space selection
  # (NOTE: to be used in case of Monte Carlo samples
  #        overlapping in simulated phase-space only !!)
  cms.PSet(
    filter = cms.string('genPhaseSpaceCut'),
    title = cms.string('gen. Phase-Space'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),
    
  # trigger selection
  cms.PSet(
    filter = cms.string('Trigger'),
    title = cms.string('mu15 || isoMu11 Trigger'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),

  # primary event vertex selection
  cms.PSet(
    filter = cms.string('primaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexPosition'),
    title = cms.string('-50 < zVertex < +50 cm'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation
  ),
    
  # selection of muon candidate
  # produced in muonic tau decay
  cms.PSet(
    filter = cms.string('globalMuonCut'),
    title = cms.string('global Muon'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('muonEtaCut'),
    title = cms.string('-2.1 < eta(Muon) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonPtCut'),
    title = cms.string('Pt(Muon) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonHLTmatchCut'),
    title = cms.string('Muon Trigger match'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsHLTmatchCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIsoCutLooseMuonIsolation'),
    title = cms.string('Muon Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulativeLooseMuonIsolation')
  ),
  cms.PSet(
    filter = cms.string('muonEcalIsoCutLooseMuonIsolation'),
    title = cms.string('Muon ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulativeLooseMuonIsolation')
  ),
  cms.PSet(
    filter = cms.string('muonAntiPionCutLooseMuonIsolation'),
    title = cms.string('Muon pi-Veto'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulativeLooseMuonIsolation')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIPcutLooseMuonIsolation'),
    title = cms.string('Muon Track IP'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation')
  ),
    
  # selection of tau-jet candidate
  # produced in hadronic tau decay
  cms.PSet(
    filter = cms.string('tauAntiOverlapWithMuonsVeto'),
    title = cms.string('Tau not overlapping w. Muon'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto')
  ),
  cms.PSet(
    filter = cms.string('tauEtaCut'),
    title = cms.string('-2.1 < eta(Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauPtCut'),
    title = cms.string('Pt(Tau) > 20 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauPt20Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkCut'),
    title = cms.string('Tau lead. Track find.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkPtCut'),
    title = cms.string('Tau lead. Track Pt'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkPtCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauTrkIsoCut'),
    title = cms.string('Tau Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauEcalIsoCut'),
    title = cms.string('Tau ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauProngCut'),
    title = cms.string('Tau 1||3-Prong'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauProngCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauMuonVeto'),
    title = cms.string('Tau mu-Veto'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative')
  ),

  #selection of muon + tau-jet combinations
  cms.PSet(
    filter = cms.string('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation'),
    title = cms.string('dR(Muon-Tau) > 0.7'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation'),
    title = cms.string('dPhi(Muon-MET) < 2.4'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAcoplanarityCumulativeLooseMuonIsolation')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation'),
    title = cms.string('Charge(Muon+Tau) = 0'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation'),
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForMuTauMt1METCutLooseMuonIsolation'),
    title = cms.string('M_{T}(Muon-MET) < 60 GeV'),
    saveRunEventNumbers = cms.vstring('passed_cumulative')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithoutMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulativeLooseMuonIsolation',
                          'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METCumulativeLooseMuonIsolation')
  )
)

muTauAnalysisSequence_factorizedWithMuonIsolation = cms.VPSet(
  # fill histograms for full event sample
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),

  # generator level phase-space selection
  # (NOTE: to be used in case of Monte Carlo samples
  #        overlapping in simulated phase-space only !!)
  cms.PSet(
    filter = cms.string('genPhaseSpaceCut'),
    title = cms.string('gen. Phase-Space'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),
    
  # trigger selection
  cms.PSet(
    filter = cms.string('Trigger'),
    title = cms.string('mu15 || isoMu11 Trigger'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),

  # primary event vertex selection
  cms.PSet(
    filter = cms.string('primaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexPosition'),
    title = cms.string('-50 < zVertex < +50 cm'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation
  ),
    
  # selection of muon candidate
  # produced in muonic tau decay
  cms.PSet(
    filter = cms.string('globalMuonCut'),
    title = cms.string('global Muon'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('muonEtaCut'),
    title = cms.string('-2.1 < eta(Muon) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonPtCut'),
    title = cms.string('Pt(Muon) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonHLTmatchCut'),
    title = cms.string('Muon Trigger match'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsHLTmatchCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIsoCut'),
    title = cms.string('Muon Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonEcalIsoCut'),
    title = cms.string('Muon ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonAntiPionCut'),
    title = cms.string('Muon pi-Veto'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIPcut'),
    title = cms.string('Muon Track IP'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = muTauHistManagers_factorizedWithMuonIsolation,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative')
  )
)
