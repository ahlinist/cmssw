import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# import config for selection of Z --> elec + mu events
# defined for the "regular" case without factorization of muon isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoElecMu_cff import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> elec + mu channel
# specific to factorization
#--------------------------------------------------------------------------------

# electron candidate selection with "loose" electron isolation criteria applied
cfgElectronTrkIsoCutLooseIsolation = copy.deepcopy(cfgElectronTrkIsoCut)
cfgElectronTrkIsoCutLooseIsolation.pluginName = "electronTrkIsoCutLooseIsolation"
cfgElectronTrkIsoCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIsoLooseIsolationCumulative')
cfgElectronTrkIsoCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1ElectronsTrkIsoLooseIsolationIndividual')

cfgElectronEcalIsoCutLooseIsolation = copy.deepcopy(cfgElectronEcalIsoCut)
cfgElectronEcalIsoCutLooseIsolation.pluginName = "electronEcalIsoCutLooseIsolation"
cfgElectronEcalIsoCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationCumulative')
cfgElectronEcalIsoCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationIndividual')

cfgElectronTrkCutLooseIsolation = copy.deepcopy(cfgElectronTrkCut)
cfgElectronTrkCutLooseIsolation.pluginName = "electronTrkCutLooseIsolation"
cfgElectronTrkCutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkLooseIsolationCumulative')
cfgElectronTrkCutLooseIsolation.src_individual = cms.InputTag('selectedLayer1ElectronsTrkLooseIsolationIndividual')

cfgElectronTrkIPcutLooseIsolation = copy.deepcopy(cfgElectronTrkIPcut)
cfgElectronTrkIPcutLooseIsolation.pluginName = "electronTrkIPcutLooseIsolation"
cfgElectronTrkIPcutLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPlooseIsolationCumulative')
cfgElectronTrkIPcutLooseIsolation.src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPlooseIsolationIndividual')

# selection of di-tau candidates composed of combination of muon with "loosely" isolated electron
cfgDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuAntiOverlapVeto)
cfgDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation.pluginName = "diTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation"
cfgDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsAntiOverlapVetoLooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsAntiOverlapVetoLooseElectronIsolationIndividual')

cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuZeroChargeCut)
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation"
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeLooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsZeroChargeLooseElectronIsolationIndividual')

cfgDiTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuAcoplanarity12Cut)
cfgDiTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation"
cfgDiTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsAcoplanarity12LooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsAcoplanarity12LooseElectronIsolationIndividual')

cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuMt1METcut)
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuMt1METcutLooseElectronIsolation"
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsMt1METlooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsMt1METlooseElectronIsolationIndividual')
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuMt2METcut)
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuMt2METcutLooseElectronIsolation"
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsMt2METlooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsMt2METlooseElectronIsolationIndividual')

cfgDiTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuPzetaDiffCut)
cfgDiTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation"
cfgDiTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsPzetaDiffLooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsPzetaDiffLooseElectronIsolationIndividual')

zToElecMuEventSelConfiguratorLooseElectronIsolation = eventSelFlagProdConfigurator(
    [ cfgElectronTrkIsoCutLooseIsolation,
      cfgElectronEcalIsoCutLooseIsolation,
      cfgElectronTrkCutLooseIsolation,
      cfgElectronTrkIPcutLooseIsolation,
      cfgDiTauCandidateForElecMuAntiOverlapVetoLooseElectronIsolation,
      cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuAcoplanarity12CutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuPzetaDiffCutLooseElectronIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoElecMuEventsLooseElectronIsolation = zToElecMuEventSelConfiguratorLooseElectronIsolation.configure()
