import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# import config for selection of Z --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> mu + tau-jet channel
# specific to factorization
#--------------------------------------------------------------------------------

# muon candidate selection with "loose" muon isolation criteria applied
cfgMuonTrkIsoCutLooseMuonIsolation = copy.deepcopy(cfgMuonTrkIsoCut)
cfgMuonTrkIsoCutLooseMuonIsolation.pluginName = "muonTrkIsoCutLooseMuonIsolation"
cfgMuonTrkIsoCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoLooseIsolationCumulative')
cfgMuonTrkIsoCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoLooseIsolationIndividual')

cfgMuonEcalIsoCutLooseMuonIsolation = copy.deepcopy(cfgMuonEcalIsoCut)
cfgMuonEcalIsoCutLooseMuonIsolation.pluginName = "muonEcalIsoCutLooseMuonIsolation"
cfgMuonEcalIsoCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative')
cfgMuonEcalIsoCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationIndividual')

cfgMuonAntiPionCutLooseMuonIsolation = copy.deepcopy(cfgMuonAntiPionCut)
cfgMuonAntiPionCutLooseMuonIsolation.pluginName = "muonAntiPionCutLooseMuonIsolation"
cfgMuonAntiPionCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoLooseIsolationCumulative')
cfgMuonAntiPionCutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsPionVetoLooseIsolationIndividual')

cfgMuonTrkIPcutLooseMuonIsolation = copy.deepcopy(cfgMuonTrkIPcut)
cfgMuonTrkIPcutLooseMuonIsolation.pluginName = "muonTrkIPcutLooseMuonIsolation"
cfgMuonTrkIPcutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPlooseIsolationCumulative')
cfgMuonTrkIPcutLooseMuonIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIPlooseIsolationIndividual')

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauAntiOverlapVeto)
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.pluginName = "diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation"
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauZeroChargeCut)
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation"
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauMt1METcut)
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauMt1METcutLooseMuonIsolation"
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationIndividual')

zToMuTauEventSelConfiguratorLooseMuonIsolation = eventSelFlagProdConfigurator(
    [ cfgMuonTrkIsoCutLooseMuonIsolation,
      cfgMuonEcalIsoCutLooseMuonIsolation,
      cfgMuonAntiPionCutLooseMuonIsolation,
      cfgMuonTrkIPcutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation,
      cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoMuTauEventsLooseMuonIsolation = zToMuTauEventSelConfiguratorLooseMuonIsolation.configure()
