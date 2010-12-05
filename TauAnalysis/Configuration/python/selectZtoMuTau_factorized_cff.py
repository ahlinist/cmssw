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

cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation = cfgDiTauCandidateForMuTauZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsZeroChargeLooseMuonIsolationIndividual')
)

cfgDiTauCandidateForMuTauNonZeroChargeCutLooseMuonIsolation = cfgDiTauCandidateForMuTauNonZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForMuTauNonZeroChargeCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsNonZeroChargeLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsNonZeroChargeLooseMuonIsolationIndividual')
)

zToMuTauEventSelConfiguratorLooseMuonIsolationOS = eventSelFlagProdConfigurator(
    [ cfgMuonPFRelIsoCutLooseIsolation,
      cfgMuonTrkIPcutLooseIsolation,
      cfgDiTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation,
      cfgDiTauCandidateForMuTauMt1METcutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauPzetaDiffCutLooseMuonIsolation,
      cfgDiTauCandidateForMuTauZeroChargeCutLooseMuonIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsZtoMuTauLooseMuonIsolationOS = zToMuTauEventSelConfiguratorLooseMuonIsolationOS.configure()

zToMuTauEventSelConfiguratorLooseMuonIsolationSS = eventSelFlagProdConfigurator(
    [ cfgDiTauCandidateForMuTauNonZeroChargeCutLooseMuonIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsZtoMuTauLooseMuonIsolationSS = zToMuTauEventSelConfiguratorLooseMuonIsolationSS.configure()

produceEventSelFlagsZtoMuTauLooseMuonIsolation = cms.Sequence(
    produceEventSelFlagsZtoMuTauLooseMuonIsolationOS * produceEventSelFlagsZtoMuTauLooseMuonIsolationSS
)    

selectZtoMuTauEventsLooseMuonIsolation = cms.Sequence(produceEventSelFlagsZtoMuTauLooseMuonIsolation)
    
