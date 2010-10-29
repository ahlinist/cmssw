import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# preselection of events considered in data-driven background estimation methods
#--------------------------------------------------------------------------------

genPhaseSpaceFilter = cms.EDFilter("EventSelPluginFilter",
    selector = cms.PSet(
        pluginName = cms.string('genPhaseSpaceCut'),
        pluginType = cms.string('BoolEventSelector'),
        src = cms.InputTag('genPhaseSpaceCut')
    )
)

muonsLooseZtoMuTauNtupleForZmumuHypotheses = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string('isGlobalMuon | isStandAloneMuon | pt > 5.'),
    filter = cms.bool(False)
)

muonsTightZtoMuTauNtupleForZmumuHypotheses = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string('isGlobalMuon | isStandAloneMuon'),
    filter = cms.bool(False)
)

allDiMuPairZtoMuTauNtupleZmumuHypotheses = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTightZtoMuTauNtupleForZmumuHypotheses'),
    srcLeg2 = cms.InputTag('muonsLooseZtoMuTauNtupleForZmumuHypotheses'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                        
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZtoMuTauNtupleZmumuHypotheses = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag("allDiMuPairZtoMuTauNtupleZmumuHypotheses"),                                   
    cut = cms.string('p4Vis.mass > 70. & p4Vis.mass < 110.'),
    filter = cms.bool(False)
)

muTauPairCutBgEstPreselection = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string("muTauPairCutBgEstPreselection"),
    pluginType = cms.string("AndEventSelector"),
    selectors = cms.VPSet(
        cms.PSet(            
            pluginName = cms.string("muTauPairCutBgEstPreselection"),
            pluginType = cms.string("PATCandViewMinEventSelector"),
            src = cms.InputTag('muTauPairsZtoMuTauNtuplePzetaDiff'),
            minNumber = cms.uint32(1)
        ),
        cms.PSet(
            pluginName = cms.string('diMuPairZmumuHypothesisVeto'),
            pluginType = cms.string('PATCandViewMaxEventSelector'),
            src = cms.InputTag('selectedDiMuPairZtoMuTauNtupleZmumuHypotheses'),
            maxNumber = cms.uint32(0)
        ),
        cms.PSet(
            pluginName = cms.string('uniqueMuonCandidateCutTauIdEffZtoMuTauCombinedFit'),
            pluginType = cms.string('PATCandViewMaxEventSelector'),
            src = cms.InputTag('muonsTightZtoMuTauNtupleForZmumuHypotheses'),
            maxNumber = cms.uint32(1)
        )
    )
) 

produceBoolEventSelFlags = cms.Sequence(
    muonsLooseZtoMuTauNtupleForZmumuHypotheses * muonsTightZtoMuTauNtupleForZmumuHypotheses
   * allDiMuPairZtoMuTauNtupleZmumuHypotheses * selectedDiMuPairZtoMuTauNtupleZmumuHypotheses
   * muTauPairCutBgEstPreselection
)

selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertex'),
        cms.InputTag('primaryEventVertexQuality'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muTauPairCutBgEstPreselection')
    )
)
