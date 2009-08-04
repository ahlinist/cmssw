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
cfgMuonTrkIsoCutLooseIsolation = copy.deepcopy(cfgMuonTrkIsoCut)
cfgMuonTrkIsoCutLooseIsolation.pluginName = "muonTrkIsoCutLooseIsolation"
cfgMuonTrkIsoCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoLooseIsolationCumulative')
cfgMuonTrkIsoCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoLooseIsolationIndividual')

cfgMuonEcalIsoCutLooseIsolation = copy.deepcopy(cfgMuonEcalIsoCut)
cfgMuonEcalIsoCutLooseIsolation.pluginName = "muonEcalIsoCutLooseIsolation"
cfgMuonEcalIsoCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative')
cfgMuonEcalIsoCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationIndividual')

cfgMuonAntiPionCutLooseIsolation = copy.deepcopy(cfgMuonAntiPionCut)
cfgMuonAntiPionCutLooseIsolation.pluginName = "muonAntiPionCutLooseIsolation"
cfgMuonAntiPionCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoLooseIsolationCumulative')
cfgMuonAntiPionCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1MuonsPionVetoLooseIsolationIndividual')

cfgMuonTrkIPcutLooseIsolation = copy.deepcopy(cfgMuonTrkIPcut)
cfgMuonTrkIPcutLooseIsolation.pluginName = "muonTrkIPcutLooseIsolation"
cfgMuonTrkIPcutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPlooseIsolationCumulative')
cfgMuonTrkIPcutLooseIsolation.src_individual = cms.InputTag('selectedLayer1MuonsTrkIPlooseIsolationIndividual')

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauAntiOverlapVeto)
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.pluginName = "diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation"
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauZeroChargeCut)
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation"
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauAcoplanarity12Cut)
cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation"
cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsAcoplanarity12LooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsAcoplanarity12LooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauMt1METcut)
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauMt1METcutLooseMuonIsolation"
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationIndividual')

cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation = copy.deepcopy(cfgDiTauCandidateForMuTauPzetaDiffCut)
cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation.pluginName = "diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation"
cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation.src_cumulative = cms.InputTag('selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative')
cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation.src_individual = cms.InputTag('selectedMuTauPairsPzetaDiffLooseMuonIsolationIndividual')

zToMuTauEventSelConfiguratorLooseMuonIsolation = eventSelFlagProdConfigurator(
    [ cfgMuonTrkIsoCutLooseIsolation,
      cfgMuonEcalIsoCutLooseIsolation,
      cfgMuonAntiPionCutLooseIsolation,
      cfgMuonTrkIPcutLooseIsolation,
      cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation,
      cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoMuTauEventsLooseMuonIsolation = zToMuTauEventSelConfiguratorLooseMuonIsolation.configure()
