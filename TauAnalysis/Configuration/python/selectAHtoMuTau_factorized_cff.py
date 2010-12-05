import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.selectZtoMuTau_factorized_cff import *

#--------------------------------------------------------------------------------
# import config for selection of MSSM Higgs A/H --> mu + tau-jet events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectAHtoMuTau_cff import *

#--------------------------------------------------------------------------------
# define event selection criteria for A/H --> mu + tau-jet channel
# specific to factorization
#--------------------------------------------------------------------------------

# selection of di-tau candidates composed of combination of tau-jet with "loosely" isolated muon 
cfgDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation = cfgDiTauCandidateForAHtoMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolationIndividual')
)
cfgDiTauCandidateForAHtoMuTauMt1METcutLooseMuonIsolation = cfgDiTauCandidateForAHtoMuTauMt1METcut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauMt1METcutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolationIndividual')
)    
cfgDiTauCandidateForAHtoMuTauPzetaDiffCutLooseMuonIsolation = cfgDiTauCandidateForAHtoMuTauPzetaDiffCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauPzetaDiffCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationIndividual')
)

cfgDiTauCandidateForAHtoMuTauZeroChargeCutLooseMuonIsolation = cfgDiTauCandidateForAHtoMuTauZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauZeroChargeCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolationIndividual')
)

cfgDiTauCandidateForAHtoMuTauNonZeroChargeCutLooseMuonIsolation = cfgDiTauCandidateForAHtoMuTauNonZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauNonZeroChargeCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauNonZeroChargeLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauNonZeroChargeLooseMuonIsolationIndividual')
)

# central jet veto/b-jet candidate selection
# for not not overlapping with loosely "isolated" muons
cfgCentralJetEt20bTagVetoLooseMuonIsolation = cfgCentralJetEt20bTagVeto.clone(
    pluginName = cms.string('centralJetEt20bTagVetoLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagLooseMuonIsolationIndividual')
)
cfgCentralJetEt20CutLooseMuonIsolation = cfgCentralJetEt20Cut.clone(
    pluginName = cms.string('centralJetEt20CutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolationIndividual')
)
cfgCentralJetEt20bTagCutLooseMuonIsolation = cfgCentralJetEt20bTagCut.clone(
    pluginName = cms.string('centralJetEt20bTagCutLooseMuonIsolation'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagLooseMuonIsolationCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagLooseMuonIsolationIndividual')
)

ahToMuTauEventSelConfiguratorLooseMuonIsolationOS = eventSelFlagProdConfigurator(
    [ cfgMuonPFRelIsoCutLooseIsolation,
      cfgMuonTrkIPcutLooseIsolation,
      cfgDiTauCandidateForAHtoMuTauAntiOverlapVetoLooseMuonIsolation,
      cfgDiTauCandidateForAHtoMuTauMt1METcutLooseMuonIsolation,
      cfgDiTauCandidateForAHtoMuTauPzetaDiffCutLooseMuonIsolation,
      cfgDiTauCandidateForAHtoMuTauZeroChargeCutLooseMuonIsolation,
      cfgCentralJetEt20bTagVetoLooseMuonIsolation,
      cfgCentralJetEt20CutLooseMuonIsolation,
      cfgCentralJetEt20bTagCutLooseMuonIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoMuTauLooseMuonIsolationOS = ahToMuTauEventSelConfiguratorLooseMuonIsolationOS.configure()

ahToMuTauEventSelConfiguratorLooseMuonIsolationSS = eventSelFlagProdConfigurator(
    [ cfgDiTauCandidateForAHtoMuTauNonZeroChargeCutLooseMuonIsolation ],
        boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoMuTauLooseMuonIsolationSS = ahToMuTauEventSelConfiguratorLooseMuonIsolationSS.configure()

produceEventSelFlagsAHtoMuTauLooseMuonIsolation = cms.Sequence(
    produceEventSelFlagsAHtoMuTauLooseMuonIsolationOS * produceEventSelFlagsAHtoMuTauLooseMuonIsolationSS
)

selectAHtoMuTauEventsLooseMuonIsolation = cms.Sequence(produceEventSelFlagsAHtoMuTauLooseMuonIsolation)
    
