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
cfgTauLeadTrkPtBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtLooseIsolationCumulative')
cfgTauLeadTrkPtBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtLooseIsolationIndividual')

cfgTauEcalIsoBoostedLooseIsolation = copy.deepcopy(cfgTauEcalIsoBoosted)
cfgTauEcalIsoBoostedLooseIsolation.pluginName = "tauEcalIsoBoostedLooseIsolation"
cfgTauEcalIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalIsoLooseIsolationCumulative')
cfgTauEcalIsoBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuEcalIsoLooseIsolationIndividual')

cfgTauTrkIsoBoostedLooseIsolation = copy.deepcopy(cfgTauTrkIsoBoosted)
cfgTauTrkIsoBoostedLooseIsolation.pluginName = "tauTrkIsoBoostedLooseIsolation"
cfgTauTrkIsoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuTrkIsoLooseIsolationCumulative')
cfgTauTrkIsoBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuTrkIsoLooseIsolationIndividual')

cfgTauMuonVetoBoostedLooseIsolation = copy.deepcopy(cfgTauMuonVetoBoosted)
cfgTauMuonVetoBoostedLooseIsolation.pluginName = "tauMuonVetoBoostedLooseIsolation"
cfgTauMuonVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuMuonVetoLooseIsolationCumulative')
cfgTauMuonVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuMuonVetoLooseIsolationIndividual')

cfgTauElectronVetoBoostedLooseIsolation = copy.deepcopy(cfgTauElectronVetoBoosted)
cfgTauElectronVetoBoostedLooseIsolation.pluginName = "tauElectronVetoBoostedLooseIsolation"
cfgTauElectronVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuElectronVetoLooseIsolationCumulative')
cfgTauElectronVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuElectronVetoLooseIsolationIndividual')

cfgTauEcalCrackVetoBoostedLooseIsolation = copy.deepcopy(cfgTauEcalCrackVetoBoosted)
cfgTauEcalCrackVetoBoostedLooseIsolation.pluginName = "tauEcalCrackVetoBoostedLooseIsolation"
cfgTauEcalCrackVetoBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoLooseIsolationCumulative')
cfgTauEcalCrackVetoBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoLooseIsolationIndividual')

cfgTauProngCutBoostedLooseIsolation = copy.deepcopy(cfgTauProngCutBoosted)
cfgTauProngCutBoostedLooseIsolation.pluginName = "tauProngBoostedLooseIsolation"
cfgTauProngCutBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuProngLooseIsolationCumulative')
cfgTauProngCutBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuProngLooseIsolationIndividual')

cfgTauChargeCutBoostedLooseIsolation = copy.deepcopy(cfgTauChargeCutBoosted)
cfgTauChargeCutBoostedLooseIsolation.pluginName = "tauChargeBoostedLooseIsolation"
cfgTauChargeCutBoostedLooseIsolation.src_cumulative = cms.InputTag('selectedPatTausForWTauNuChargeLooseIsolationCumulative')
cfgTauChargeCutBoostedLooseIsolation.src_individual = cms.InputTag('selectedPatTausForWTauNuChargeLooseIsolationIndividual')

cfgRecoilJetBoostedLooseIsolation = copy.deepcopy(cfgRecoilJetBoosted)
cfgRecoilJetBoostedLooseIsolation.pluginName = "recoilJetBoostedLooseIsolation"
cfgRecoilJetBoostedLooseIsolation.src = cms.InputTag('selectedPatJetsEt15ForWTauNuLooseIsolationCumulative')

cfgCentralJetVetoBoostedLooseIsolation = copy.deepcopy(cfgCentralJetVetoBoosted)
cfgCentralJetVetoBoostedLooseIsolation.pluginName = "centralJetVetoBoostedLooseIsolation"
cfgCentralJetVetoBoostedLooseIsolation.src = cms.InputTag('selectedPatJetsEt15ForWTauNuLooseIsolationCumulative')

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
