import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# preselection of events considered in data-driven background estimation methods
#--------------------------------------------------------------------------------

genPhaseSpaceFilter = cms.EDFilter("EventSelPluginFilter",
    selector = cms.PSet(
        pluginName = cms.string('genPhaseSpaceCut'),
        pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
        src = cms.InputTag('genPhaseSpaceEventInfo'),
        cut = cms.string('')
    )
)

electronsBgEstPreselection = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationCumulative'),                                        
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

electronTrkCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("electronTrkCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('electronsBgEstPreselection'),
    minNumber = cms.uint32(1)
)

muonsBgEstPreselection = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('selectedLayer1MuonsEcalIsoLooseIsolationCumulative'),                                        
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

muonTrkCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("muonTrkCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('muonsBgEstPreselection'),
    minNumber = cms.uint32(1)
)

elecMuPairsBgEstPreselection = cms.EDProducer("PATElecMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsBgEstPreselection'),
    srcLeg2 = cms.InputTag('muonsBgEstPreselection'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

elecMuPairCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("elecMuPairCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('elecMuPairsBgEstPreselection'),
    minNumber = cms.uint32(1)
)                                                                             

produceBoolEventSelFlags = cms.Sequence(
    electronsBgEstPreselection + electronTrkCutBgEstPreselection
   +muonsBgEstPreselection + muonTrkCutBgEstPreselection
   +elecMuPairsBgEstPreselection + elecMuPairCutBgEstPreselection
)

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkCutBgEstPreselection'),
        cms.InputTag('muonTrkCutBgEstPreselection'),
        cms.InputTag('elecMuPairCutBgEstPreselection')
    )
)

# exclude loosely selected electrons and muons from jets collections
# (otherwise, non-isolated electrons/muons may get double-counted,
#  once in the electron/muon collection plus once in the jet collection)
from TauAnalysis.RecoTools.patJetSelection_cff import *
selectedLayer1JetsAntiOverlapWithLeptonsVeto.srcNotToBeFiltered = cms.VInputTag(
    "electronsBgEstPreselection",
    "muonsBgEstPreselection",
    "selectedLayer1TausProngCumulative"
)

