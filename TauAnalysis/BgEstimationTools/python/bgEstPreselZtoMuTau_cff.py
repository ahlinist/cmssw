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

tauProngCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("tauProngCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('selectedLayer1TausProngCumulative'),
    minNumber = cms.uint32(1)                                                
)

muTauPairsBgEstPreselection = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsBgEstPreselection'),
    srcLeg2 = cms.InputTag('selectedLayer1TausProngCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

muTauPairCutBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("muTauPairCutBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('muTauPairsBgEstPreselection'),
    minNumber = cms.uint32(1)
)                                                                             

produceBoolEventSelFlags = cms.Sequence(
    muonsBgEstPreselection + muonTrkCutBgEstPreselection
   +tauProngCutBgEstPreselection
   +muTauPairsBgEstPreselection + muTauPairCutBgEstPreselection
)

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkCutBgEstPreselection'),
        cms.InputTag('tauProngCutBgEstPreselection'),
        cms.InputTag('muTauPairCutBgEstPreselection')
    )
)

# exclude loosely selected muons and tau-jets from jets collections
# (otherwise, non-isolated muons may get double-counted,
#  once in the muon collection plus once in the jet collection, for example)
from TauAnalysis.RecoTools.patJetSelection_cff import *
selectedLayer1JetsAntiOverlapWithLeptonsVeto.srcNotToBeFiltered = cms.VInputTag(
    "selectedLayer1ElectronsTrkIPcumulative",
    "muonsBgEstPreselection",
    "selectedLayer1TausProngCumulative"
)
