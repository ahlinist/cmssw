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
cfgElectronIsoCutLooseIsolation = copy.deepcopy(cfgElectronIsoCut)
cfgElectronIsoCutLooseIsolation.pluginName = "electronIsoCutLooseIsolation"
cfgElectronIsoCutLooseIsolation.src_cumulative = cms.InputTag('selectedPatElectronsIsoLooseIsolationCumulative')
cfgElectronIsoCutLooseIsolation.src_individual = cms.InputTag('selectedPatElectronsIsoLooseIsolationIndividual')

# selection of di-tau candidates composed of combination of muon with "loosely" isolated electron
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuZeroChargeCut)
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuZeroChargeCutLooseElectronIsolation"
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeLooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsZeroChargeLooseElectronIsolationIndividual')

cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuMt1METCut)
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuMt1METcutLooseElectronIsolation"
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsMt1METlooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsMt1METlooseElectronIsolationIndividual')
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation = copy.deepcopy(cfgDiTauCandidateForElecMuMt2METCut)
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.pluginName = "diTauCandidateForElecMuMt2METcutLooseElectronIsolation"
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.src_cumulative = cms.InputTag('selectedElecMuPairsMt2METlooseElectronIsolationCumulative')
cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation.src_individual = cms.InputTag('selectedElecMuPairsMt2METlooseElectronIsolationIndividual')

zToElecMuEventSelConfiguratorLooseElectronIsolation = eventSelFlagProdConfigurator(
    [ cfgElectronIsoCutLooseIsolation,
      cfgDiTauCandidateForElecMuZeroChargeCutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuMt1METcutLooseElectronIsolation,
      cfgDiTauCandidateForElecMuMt2METcutLooseElectronIsolation ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoElecMuEventsLooseElectronIsolation = zToElecMuEventSelConfiguratorLooseElectronIsolation.configure()
