import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('PATTriggerEventSelector'),
    src = cms.InputTag('patTriggerEvent'),
    hltAcceptPaths = cms.vstring('HLT_SingleIsoTau20_Trk15_MET20')
    )
# vertex selection
cfgPrimaryEventVertex = cms.PSet(
    pluginName = cms.string('primaryEventVertex'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
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
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuPt20Individual'),
    minNumber = cms.uint32(1)
)

cfgMetPt = cms.PSet(
    pluginName = cms.string('metPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatMETsPt15'),
    minNumber = cms.uint32(1)
)

cfgPFMetPt = cms.PSet(
    pluginName = cms.string('PFmetPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatPFMETsPt30'),
    minNumber = cms.uint32(1)
)

cfgTauLeadTrk = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauEcalIso = cms.PSet(
    pluginName = cms.string('tauEcalIso'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauTrkIso = cms.PSet(
    pluginName = cms.string('tauTrkIso'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuElectronVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('tauEcalCrackVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuProngCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuProngIndividual'),
    minNumber = cms.uint32(1)
)

cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuChargeCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuChargeIndividual'),
    minNumber = cms.uint32(1)
)

# veto events containing additional central jets with Et > 10 GeV                                       
cfgCentralJetVeto = cms.PSet(
    pluginName = cms.string('centralJetVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'), 
    maxNumber = cms.uint32(100)                                                      
)

# Recoil jet energy cut
cfgRecoilEnergyFromCaloTowersCut = cms.PSet(
    pluginName = cms.string('recoilEnergyFromCaloTowersCut'),
    pluginType = cms.string('PATTauRecoilEnergyFromCaloTowersMinEventSelector'),
    src = cms.InputTag('tauRecoilEnergyFromCaloTowersPt5'),
    minNumber = cms.uint32(0)
)

# MET topology cut
cfgMetTopologyCut = cms.PSet(
    pluginName = cms.string('metTopologyCut'),
    pluginType = cms.string('MEtTopologyMinEventSelector'),
    src = cms.InputTag('selectedMEtTopology025'),
    minNumber = cms.uint32(1)
    )


wToTauNuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ 
    cfgTrigger,
    cfgPrimaryEventVertex,
    cfgPrimaryEventVertexQuality,
    cfgPrimaryEventVertexPosition,
    cfgTauEtaCut,
    cfgTauPtCut,
    cfgPFMetPt,
    cfgMetPt,
    cfgTauLeadTrk,
    cfgTauLeadTrkPt,
    cfgTauEcalIso,
    cfgTauTrkIso,
    cfgTauProngCut,
    cfgTauChargeCut,
    cfgTauMuonVeto,
    cfgTauElectronVeto,
    cfgTauEcalCrackVeto,
    cfgCentralJetVeto,
    cfgRecoilEnergyFromCaloTowersCut,
    cfgMetTopologyCut
    ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
    )

selectWtoTauNuEvents = wToTauNuEventSelConfigurator.configure()
