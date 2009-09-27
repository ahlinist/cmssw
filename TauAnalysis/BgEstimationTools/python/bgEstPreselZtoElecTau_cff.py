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

elecTauPairCutBgEstPreselection = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string("elecTauPairCutBgEstPreselection"),
    pluginType = cms.string("OrEventSelector"),
    selectors = cms.VPSet(
        cms.PSet(
            pluginName = cms.string("elecTauPairCutBgEstPreselZee"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecTauPairsForBgEstZeeEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecTauPairCutBgEstPreselWplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecTauPairsForBgEstWplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecTauPairCutBgEstPreselTTplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecTauPairsForBgEstTTplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecTauPairCutBgEstPreselQCD"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecTauPairsForBgEstQCDenriched'),
            minNumber = cms.uint32(1)
        )
    )
) 

produceBoolEventSelFlags = cms.Sequence( elecTauPairCutBgEstPreselection )

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        #cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('elecTauPairCutBgEstPreselection')
    )
)
