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

tausBgEstPreselection = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('selectedLayer1TausProngCumulative'),    
    srcNotToBeFiltered = cms.InputTag('electronsBgEstPreselection'),                                        
    dRmin = cms.double(0.7),                       
    filter = cms.bool(False)
)

tauProngCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("tauProngCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('tausBgEstPreselection'),
    minNumber = cms.uint32(1)                                                
)

elecTauPairsBgEstPreselection = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('electronsBgEstPreselection'),
    srcLeg2 = cms.InputTag('tausBgEstPreselection'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

elecTauPairCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("elecTauPairCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('elecTauPairsBgEstPreselection'),
    minNumber = cms.uint32(1)
)                                                                             

produceBoolEventSelFlags = cms.Sequence(
    electronsBgEstPreselection + electronTrkCutBgEstPreselection
   +tauProngCutBgEstPreselection
   +elecTauPairsBgEstPreselection + elecTauPairCutBgEstPreselection
)

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        #cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkCutBgEstPreselection'),
        cms.InputTag('tauProngCutBgEstPreselection'),
        cms.InputTag('elecTauPairCutBgEstPreselection')
    )
)

# exclude loosely selected electrons and tau-jets from jets collections
# (otherwise, non-isolated electrons may get double-counted,
#  once in the electron collection plus once in the jet collection, for example)
from TauAnalysis.RecoTools.patJetSelection_cff import *
selectedLayer1JetsAntiOverlapWithLeptonsVeto.srcNotToBeFiltered = cms.VInputTag(
    "electronsBgEstPreselection",
    "selectedLayer1MuonsTrkIPcumulative",
    "tausBgEstPreselection"
)


