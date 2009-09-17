import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

# vertex selection
cfgPrimaryEventVertex = cms.PSet(
    pluginName = cms.string('primaryEventVertex'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('primaryEventVertexQuality'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('primaryEventVertexPosition'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    minNumber = cms.uint32(1)
)

# tau selection
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuPt20Individual'),
    minNumber = cms.uint32(1)
)

cfgMetPt = cms.PSet(
    pluginName = cms.string('metPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1METsPt15'),
    #src_cumulative = cms.InputTag('selectedLayer1METsForWTauNuPt15Cumulative'),
    #src_individual = cms.InputTag('selectedLayer1METsForWTauNuPt15Individual'),
    minNumber = cms.uint32(1)
)

cfgTauLeadTrk = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauTrkIso = cms.PSet(
    pluginName = cms.string('tauTrkIso'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('tauEcalCrackVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuProngIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuChargeCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuChargeIndividual'),
    minNumber = cms.uint32(1)
    )

# veto events containing additional central jets with Et > 10 GeV                                       
cfgCentralJetVeto = cms.PSet(
    pluginName = cms.string('centralJetVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    maxNumber = cms.uint32(0)                                                      
    )

# Recoil jet energy cut
cfgRecoilEnergyFromCaloTowersCut = cms.PSet(
    pluginName = cms.string('recoilEnergyFromCaloTowersCut'),
    pluginType = cms.string('PATTauRecoilEnergyFromCaloTowersMinEventSelector'),
    src = cms.InputTag('tauRecoilEnergyFromCaloTowersPt5'),
    minNumber = cms.uint32(1)
    )
#cfgRecoilEnergyFromJetsCut = cms.PSet(
#    pluginName = cms.string('recoilEnergyFromJetsCut'),
#    pluginType = cms.string('PATTauRecoilEnergyFromJetsMinEventSelector'),
#    src = cms.InputTag('tauRecoilEnergyFromJetsPt1'),
#    minNumber = cms.uint32(1)
#    )


#cut on met topology
#cfgCentralJetVeto = cms.PSet(
#    pluginName = cms.string('centralJetVeto'),
#    pluginType = cms.string('PATCandViewMaxEventSelector'),
#    src = cms.InputTag('selectedLayer1TausForWTauNuMetTopology'),
#    maxNumber = cms.uint32(0)                                                      
#    )


wToTauNuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ 
	cfgPrimaryEventVertex,
	cfgPrimaryEventVertexQuality,
	cfgPrimaryEventVertexPosition,

	cfgTauEtaCut,
	cfgTauPtCut,
	cfgMetPt,
	cfgTauLeadTrk,
	cfgTauLeadTrkPt,
	cfgTauTrkIso,
	cfgTauMuonVeto,
	cfgTauElectronVeto,
        cfgTauEcalCrackVeto,
	cfgTauProngCut,
        cfgTauChargeCut,
        cfgCentralJetVeto,
	cfgRecoilEnergyFromCaloTowersCut
#	cfgRecoilEnergyFromJetsCut,
	],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectWtoTauNuEvents = wToTauNuEventSelConfigurator.configure()
