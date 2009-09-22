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

elecMuPairCutBgEstPreselection = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string("elecMuPairCutBgEstPreselection"),
    pluginType = cms.string("OrEventSelector"),
    selectors = cms.VPSet(
        cms.PSet(
            pluginName = cms.string("elecMuPairCutBgEstPreselZmumu"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecMuPairsForBgEstZmumuEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecMuPairCutBgEstPreselWplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecMuPairsForBgEstWplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecMuPairCutBgEstPreselTTplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecMuPairsForBgEstTTplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("elecMuPairCutBgEstPreselQCD"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('elecMuPairsForBgEstQCDenriched'),
            minNumber = cms.uint32(1)
        )
    )
) 

produceBoolEventSelFlags = cms.Sequence( elecMuPairCutBgEstPreselection )

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('elecMuPairCutBgEstPreselection')
    )
)
