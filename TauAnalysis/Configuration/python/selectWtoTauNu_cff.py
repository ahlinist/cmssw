import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('PATTriggerEventSelector'),
    src = cms.InputTag('patTriggerEvent'),
    hltAcceptPaths = cms.vstring('HLT_SingleIsoTau20_Trk15_MET25_v3','HLT_SingleIsoTau20_Trk15_MET25_v4')
    )
cfgTrigger2 = cms.PSet(
    pluginName = cms.string('PseudoTrigger'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    minNumber = cms.uint32(1)        
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
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuPt20Individual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgHtRatioCut = cms.PSet(
    pluginName = cms.string('htRatio'),
    pluginType = cms.string('HtRatioMinEventSelector'),
    src = cms.InputTag('selectedHtRatioCumulative'),
    systematics = cms.vstring(htRatioSystematics.keys()),
    minNumber = cms.uint32(1)
    )

cfgPFMetPt = cms.PSet(
    pluginName = cms.string('PFmetPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatPFMETsPt30Cumulative'),
    systematics = cms.vstring(metSystematicsForWtoTauNu.keys()),
    minNumber = cms.uint32(1)
)

cfgTauTrkVertex = cms.PSet(
    pluginName = cms.string('tauTrkVertex'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuTrkMatchVertexCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuTrkMatchVertexIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
    )

cfgTauLeadTrk = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)


cfgTauIso = cms.PSet(
    pluginName = cms.string('tauIso'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuIsoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuMuonVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuElectronVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauEmFraction = cms.PSet(
    pluginName = cms.string('tauEmFraction'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEmFractionCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEmFractionIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('tauEcalCrackVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuProngCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuProngIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuChargeCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuChargeIndividual'), # 
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

# veto events containing additional electron with Et > 15 GeV                                       
cfgElectronVeto = cms.PSet(
    pluginName = cms.string('electronVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatElectronsPt15Cumulative'), 
    maxNumber = cms.uint32(0)                                                      
)

# veto events containing additional muons with Et > 15 GeC
cfgMuonVeto = cms.PSet(
    pluginName = cms.string('muonVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatMuonsPFRelIsoCumulative'),
    maxNumber = cms.uint32(0)
)

# MET topology cut
#cfgMetTopologyCut = cms.PSet(
#    pluginName = cms.string('metTopologyCut'),
#    pluginType = cms.string('MEtTopologyMinEventSelector'),
#    src = cms.InputTag('selectedMEtTopology05Cumulative'),
#    systematics = cms.vstring(metTopologySystematics.keys()),
#    minNumber = cms.uint32(1)
#    )


wToTauNuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ 
    cfgTrigger,
    cfgPrimaryEventVertex,
    cfgPrimaryEventVertexQuality,
    cfgPrimaryEventVertexPosition,
    cfgTauEtaCut,
    cfgTauPtCut,
    cfgTauTrkVertex,
    cfgTauLeadTrk,
    cfgTauLeadTrkPt,
    cfgTauMuonVeto,
    cfgMuonVeto,
    cfgTauElectronVeto,
    cfgTauEmFraction,
    cfgElectronVeto,
    cfgTauIso,
    cfgTauProngCut,
    cfgTauChargeCut,
    cfgTauEcalCrackVeto,
    cfgPFMetPt,
    cfgHtRatioCut,
    cfgTrigger2
#    cfgMetTopologyCut
    ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
    )

produceEventSelFlagsWtoTauNu = wToTauNuEventSelConfigurator.configure()

isRecWtoTauNu = cms.EDProducer("BoolEventSelFlagProducer",
                               pluginName = cms.string('isRecWtoTauNu'),
                               pluginType = cms.string('MultiBoolEventSelFlagSelector'),
                               flags = cms.VInputTag(
    cms.InputTag('Trigger'),
    cms.InputTag('primaryEventVertex'),
    cms.InputTag('primaryEventVertexQuality'),
    cms.InputTag('primaryEventVertexPosition'),
    cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
    cms.InputTag('tauEcalCrackVeto','cumulative'),
    cms.InputTag('muonVeto'),
    cms.InputTag('electronVeto'),
    cms.InputTag('PFmetPtCut'),
    cms.InputTag('htRatio'),
    cms.InputTag('PseudoTrigger')
#    cms.InputTag('metTopologyCut')
    )
                               )


selectWtoTauNuEvents = cms.Sequence(
    produceEventSelFlagsWtoTauNu
    * isRecWtoTauNu
    )
