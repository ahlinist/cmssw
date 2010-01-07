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

muTauPairCutBgEstPreselection = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string("muTauPairCutBgEstPreselection"),
    pluginType = cms.string("OrEventSelector"),
    selectors = cms.VPSet(
        cms.PSet(
            pluginName = cms.string("muTauPairCutBgEstPreselZmumu"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsForBgEstZmumuEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("muTauPairCutBgEstPreselWplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsForBgEstWplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("muTauPairCutBgEstPreselTTplusJets"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsForBgEstTTplusJetsEnriched'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("muTauPairCutBgEstPreselQCDlooseMuonIsolation"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsForBgEstQCDenrichedLooseMuonIsolation'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string("muTauPairCutBgEstPreselQCDnoMuonIsolation"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsForBgEstQCDenrichedNoMuonIsolation'),
            minNumber = cms.uint32(1)
        )
    )
) 

produceBoolEventSelFlags = cms.Sequence( muTauPairCutBgEstPreselection )

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muTauPairCutBgEstPreselection')
    )
)
