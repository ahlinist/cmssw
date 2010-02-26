import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# import config for selection of W --> tau nu events
# defined for the "regular" case without factorization of tau isolation
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectWtoTauNu_boosted_cff import *

#--------------------------------------------------------------------------------
# define event selection criteria for W --> tau nu channel
# specific to factorization
#--------------------------------------------------------------------------------

# tau candidate selection with "loose" tau isolation criteria applied
cfgTauLeadTrkPtBoostedLooseIsolation = copy.deepcopy(cfgTauLeadTrkPtBoosted)
cfgTauLeadTrkPtBoostedLooseIsolation.pluginName = "tauLeadTrkPtBoostedLooseIsolation"
cfgTauLeadTrkPtBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtLooseIsolationCumulative')
cfgTauLeadTrkPtBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtLooseIsolationIndividual')

cfgTauEcalIsoBoostedLooseIsolation = copy.deepcopy(cfgTauEcalIsoBoosted)
cfgTauEcalIsoBoostedLooseIsolation.pluginName = "tauEcalIsoBoostedLooseIsolation"
cfgTauEcalIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEcalIsoLooseIsolationCumulative')
cfgTauEcalIsoBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuEcalIsoLooseIsolationIndividual')

cfgTauTrkIsoBoostedLooseIsolation = copy.deepcopy(cfgTauTrkIsoBoosted)
cfgTauTrkIsoBoostedLooseIsolation.pluginName = "tauTrkIsoBoostedLooseIsolation"
cfgTauTrkIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoLooseIsolationCumulative')
cfgTauTrkIsoBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoLooseIsolationIndividual')

cfgTauMuonVetoBoostedLooseIsolation = copy.deepcopy(cfgTauMuonVetoBoosted)
cfgTauMuonVetoBoostedLooseIsolation.pluginName = "tauMuonVetoBoostedLooseIsolation"
cfgTauMuonVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoLooseIsolationCumulative')
cfgTauMuonVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoLooseIsolationIndividual')

cfgTauElectronVetoBoostedLooseIsolation = copy.deepcopy(cfgTauElectronVetoBoosted)
cfgTauElectronVetoBoostedLooseIsolation.pluginName = "tauElectronVetoBoostedLooseIsolation"
cfgTauElectronVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoLooseIsolationCumulative')
cfgTauElectronVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoLooseIsolationIndividual')

cfgTauEcalCrackVetoBoostedLooseIsolation = copy.deepcopy(cfgTauEcalCrackVetoBoosted)
cfgTauEcalCrackVetoBoostedLooseIsolation.pluginName = "tauEcalCrackVetoBoostedLooseIsolation"
cfgTauEcalCrackVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative')
cfgTauEcalCrackVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationIndividual')

cfgTauProngCutBoostedLooseIsolation = copy.deepcopy(cfgTauProngCutBoosted)
cfgTauProngCutBoostedLooseIsolation.pluginName = "tauProngBoostedLooseIsolation"
cfgTauProngCutBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuProngLooseIsolationCumulative')
cfgTauProngCutBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuProngLooseIsolationIndividual')

cfgTauChargeCutBoostedLooseIsolation = copy.deepcopy(cfgTauChargeCutBoosted)
cfgTauChargeCutBoostedLooseIsolation.pluginName = "tauChargeBoostedLooseIsolation"
cfgTauChargeCutBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuChargeLooseIsolationCumulative')
cfgTauChargeCutBoostedLooseIsolation.src_individual = cms.InputTag('selectedLayer1TausForWTauNuChargeLooseIsolationIndividual')

cfgRecoilJetBoostedLooseIsolation = copy.deepcopy(cfgRecoilJetBoosted)
cfgRecoilJetBoostedLooseIsolation.pluginName = "recoilJetBoostedLooseIsolation"
cfgRecoilJetBoostedLooseIsolation.src = cms.InputTag('selectedLayer1JetsEt15ForWTauNuLooseIsolationCumulative')

cfgCentralJetVetoBoostedLooseIsolation = copy.deepcopy(cfgCentralJetVetoBoosted)
cfgCentralJetVetoBoostedLooseIsolation.pluginName = "centralJetVetoBoostedLooseIsolation"
cfgCentralJetVetoBoostedLooseIsolation.src = cms.InputTag('selectedLayer1JetsEt15ForWTauNuLooseIsolationCumulative')

cfgPhiMetTauBoostedLooseIsolation = copy.deepcopy(cfgPhiMetTauBoosted)
cfgPhiMetTauBoostedLooseIsolation.pluginName = "phiMetTauBoostedLooseIsolation"
cfgPhiMetTauBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedTauNuPairsDPhiMetTauLooseIsolationCumulative')
cfgPhiMetTauBoostedLooseIsolation.src_individual = cms.InputTag('selectedTauNuPairsDPhiMetTauLooseIsolationIndividual')


wToTauNuBoostedEventSelConfiguratorLooseTauIsolation = eventSelFlagProdConfigurator(
    [ 
      cfgTauLeadTrkPtBoostedLooseIsolation,
      cfgTauEcalIsoBoostedLooseIsolation,
      cfgTauTrkIsoBoostedLooseIsolation,
      cfgTauProngCutBoostedLooseIsolation,
      cfgTauChargeCutBoostedLooseIsolation,
      cfgTauMuonVetoBoostedLooseIsolation,
      cfgTauElectronVetoBoostedLooseIsolation,
      cfgTauEcalCrackVetoBoostedLooseIsolation,
      cfgRecoilJetBoostedLooseIsolation,
      cfgCentralJetVetoBoostedLooseIsolation,
      cfgPhiMetTauBoostedLooseIsolation
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectWtoTauNuEventsBoostedLooseTauIsolation = wToTauNuBoostedEventSelConfiguratorLooseTauIsolation.configure()
