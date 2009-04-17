import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.factorizationTools import switchHistManagers

#--------------------------------------------------------------------------------
# import config for event print-out and analysis sequence of Z --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

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
evtSelMuonTrkIsoLooseIsolation = copy.deepcopy(evtSelMuonTrkIso)
evtSelMuonTrkIsoLooseIsolation.src_cumulative = cms.InputTag('muonTrkIsoCutLooseIsolation', 'cumulative')
evtSelMuonTrkIsoLooseIsolation.src_individual = cms.InputTag('muonTrkIsoCutLooseIsolation', 'individual')

evtSelMuonEcalIsoLooseIsolation = copy.deepcopy(evtSelMuonEcalIso)
evtSelMuonEcalIsoLooseIsolation.src_cumulative = cms.InputTag('muonEcalIsoCutLooseIsolation', 'cumulative')
evtSelMuonEcalIsoLooseIsolation.src_individual = cms.InputTag('muonEcalIsoCutLooseIsolation', 'individual')

evtSelMuonAntiPionLooseIsolation = copy.deepcopy(evtSelMuonAntiPion)
evtSelMuonAntiPionLooseIsolation.src_cumulative = cms.InputTag('muonAntiPionCutLooseIsolation', 'cumulative')
evtSelMuonAntiPionLooseIsolation.src_individual = cms.InputTag('muonAntiPionCutLooseIsolation', 'individual')

evtSelMuonTrkIPlooseIsolation = copy.deepcopy(evtSelMuonTrkIP)
evtSelMuonTrkIPlooseIsolation.src_cumulative = cms.InputTag('muonTrkIPcutLooseIsolation', 'cumulative')
evtSelMuonTrkIPlooseIsolation.src_individual = cms.InputTag('muonTrkIPcutLooseIsolation', 'individual')

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauAntiOverlapVeto)
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauZeroCharge)
evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauZeroChargeLooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation', 'individual')

evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation = copy.deepcopy(evtSelDiTauCandidateForMuTauMt1MET)
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation.src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'cumulative')
evtSelDiTauCandidateForMuTauMt1METlooseMuonIsolation.src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcutLooseMuonIsolation', 'individual')

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

muTauAnalysisSequence_factorizedWithoutMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
switchHistManagers(muTauAnalysisSequence_factorizedWithoutMuonIsolation, muTauHistManagers_factorizedWithoutMuonIsolation)
ref = muTauAnalysisSequence_factorizedWithoutMuonIsolation
ref[18].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoLooseIsolationCumulative')
ref[20].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoLooseIsolationCumulative')
ref[22].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoLooseIsolationCumulative')
ref[24].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative')
ref[26].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative')
ref[28].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEta21Cumulative')
ref[30].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauPt20Cumulative')
ref[32].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkCumulative')
ref[34].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkPtCumulative')
ref[36].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauTrkIsoCumulative')
ref[38].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEcalIsoCumulative')
ref[40].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauProngCumulative')
ref[42].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative')
ref[44].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative')
ref[46].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative')
ref[48].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METlooseMuonIsolationCumulative')
#ref[50].replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPlooseIsolationCumulative',
#                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
#                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METlooseMuonIsolationCumulative')
del ref

muTauAnalysisSequence_factorizedWithMuonIsolation = copy.deepcopy(muTauAnalysisSequence)
switchHistManagers(muTauAnalysisSequence_factorizedWithMuonIsolation, muTauHistManagers_factorizedWithMuonIsolation)
