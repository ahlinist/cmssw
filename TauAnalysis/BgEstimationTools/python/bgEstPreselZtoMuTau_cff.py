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

tausBgEstPreselLooseTrkIso = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('selectedLayer1TausLeadTrkPtCumulative'),    
    cut = cms.string('tauID("trackIsolation") > 0.5 | chargedParticleIso < 8.'),
    filter = cms.bool(False)                                   
)

tausBgEstPreselLooseEcalIso = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('tausBgEstPreselLooseTrkIso'),    
    cut = cms.string('tauID("ecalIsolation") > 0.5 | gammaParticleIso < 8.'),
    filter = cms.bool(False)                                   
)

tausBgEstPreselection = cms.EDFilter("PATTauAntiOverlapSelector",
    src = cms.InputTag('tausBgEstPreselLooseEcalIso'),    
    srcNotToBeFiltered = cms.VInputTag('muonsBgEstPreselection'),                                        
    dRmin = cms.double(0.7),                       
    filter = cms.bool(False)
)

tauAntiOverlapWithMuonsVetoBgEstPreselection = cms.EDFilter("BoolEventSelFlagProducer",
    pluginName = cms.string("tauAntiOverlapWithMuonsVetoBgEstPreselection"),
    pluginType = cms.string("PATCandViewMinEventSelector"),
    src = cms.InputTag('tausBgEstPreselection'),
    minNumber = cms.uint32(1)                                                
)

muTauPairsBgEstPreselection = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsBgEstPreselection'),
    srcLeg2 = cms.InputTag('tausBgEstPreselection'),
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
   + tausBgEstPreselLooseTrkIso + tausBgEstPreselLooseEcalIso
   + tausBgEstPreselection + tauAntiOverlapWithMuonsVetoBgEstPreselection
   + muTauPairsBgEstPreselection + muTauPairCutBgEstPreselection
)

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkCutBgEstPreselection'),
        cms.InputTag('tauAntiOverlapWithMuonsVetoBgEstPreselection'),
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
    "tausBgEstPreselection"
)
