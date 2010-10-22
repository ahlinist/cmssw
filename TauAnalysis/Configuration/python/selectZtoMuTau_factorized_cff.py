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
cfgMuonPFRelIsoCutLooseIsolation = cfgMuonPFRelIsoCut.clone(
    pluginName = cms.string('muonPFRelIsoCutLooseIsolation'),
    src_cumulative = cms.InputTag('selectedPatMuonsPFRelIsoLooseIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPFRelIsoLooseIsolationIndividual')
)    

cfgMuonAntiPionCutLooseIsolation = cfgMuonAntiPionCut.clone(
    pluginName = cms.string('muonAntiPionCutLooseIsolation'),
    src_cumulative = cms.InputTag('selectedPatMuonsPionVetoLooseIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPionVetoLooseIsolationIndividual')
)    

cfgMuonTrkIPcutLooseIsolation = cfgMuonTrkIPcut.clone(
    pluginName = cms.string('muonTrkIPcutLooseIsolation'),
    src_cumulative = cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsTrkIPlooseIsolationIndividual')
)    

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation = cfgDiTauCandidateForMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoLooseMuonIsolationIndividual')
)

cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation = cfgDiTauCandidateForMuTauZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationIndividual')
)

cfgDiTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation = cfgDiTauCandidateForMuTauAcoplanarity12Cut.clone(
    pluginName = cms.string('diTauCandidateForMuTauAcoplanarity12CutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAcoplanarity12LooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAcoplanarity12LooseMuonIsolationIndividual')
)    

cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation = cfgDiTauCandidateForMuTauMt1METcut.clone(
    pluginName = cms.string('diTauCandidateForMuTauMt1METcutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsMt1METlooseMuonIsolationIndividual')
)    

cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation = cfgDiTauCandidateForMuTauPzetaDiffCut.clone(
    pluginName = cms.string('diTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsPzetaDiffLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsPzetaDiffLooseMuonIsolationIndividual')
)    

zToMuTauEventSelConfiguratorLooseMuonIsolation = eventSelFlagProdConfigurator(
    [ cfgMuonPFRelIsoCutLooseIsolation,
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
