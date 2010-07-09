import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger.HLTfilters.hltLevel1GTSeed_cfi')

#############  Primary Vertex Filter  ################
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
       vertexCollection = cms.InputTag('offlinePrimaryVertices'),
       minimumNDOF = cms.uint32(4) ,
       maxAbsZ = cms.double(15),	
       maxd0 = cms.double(2)	
)
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2010A/JetMETTau/USER/CS_DiJetAve-Jul6thSkim_v1/0156/4E94F3D3-0D8A-DF11-B467-001A92971B20.root' 
    )
)

process.TFileService = cms.Service("TFileService",fileName = cms.string('DBTree.root'))
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('RecoJets.JetAssociationProducers.trackExtrapolator_cfi')
process.load('RecoJets.JetPlusTracks.JetPlusTrackCorrections_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')
#############   hlt filter ###################################
process.HLTfilter = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('HLT_DiJetAve15U_8E29','HLT_DiJetAve30U_8E29'),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(True)
)

#############   dijet balance tree producer ##########
process.ak5calo = cms.EDAnalyzer("CaloDBTreeProducer",
    jets               = cms.string('ak5CaloJets'),
    jetsID             = cms.string('ak5JetID'),
    jetType            = cms.string('Calo'),
    dijetPtCut         = cms.double(1.0),
    barrelEtaCut       = cms.double(1.3),
    jetTriggerNames    = cms.vstring('HLT_DiJetAve15U_8E29','HLT_DiJetAve30U_8E29'),
    triggerProcessName = cms.string('HLT'), 
    triggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    isMCarlo           = cms.bool(False)
)

process.ak5calol2l3    = process.ak5calo.clone(jets = 'ak5CaloJetsL2L3',jetType = 'CaloCor')
process.ak5calol2l3res = process.ak5calo.clone(jets = 'ak5CaloJetsL2L3Residual',jetType = 'CaloCor')

process.ak5jpt = cms.EDAnalyzer("JPTDBTreeProducer",
    jets               = cms.string('JetPlusTrackZSPCorJetAntiKt5'),
    jetsID             = cms.string('ak5JetID'),
    jetType            = cms.string('JPT'),
    dijetPtCut         = cms.double(1.0),
    barrelEtaCut       = cms.double(1.3),
    jetTriggerNames    = cms.vstring('HLT_DiJetAve15U_8E29','HLT_DiJetAve30U_8E29'),
    triggerProcessName = cms.string('HLT'), 
    triggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    isMCarlo           = cms.bool(False)
)

process.ak5jptl2l3    = process.ak5jpt.clone(jets = 'ak5JPTJetsL2L3')
process.ak5jptl2l3res = process.ak5jpt.clone(jets = 'ak5JPTJetsL2L3Residual')

process.ak5pf = cms.EDAnalyzer("PFDBTreeProducer",
    jets               = cms.string('ak5PFJets'),
    jetsID             = cms.string(''),
    jetType            = cms.string('PF'),
    dijetPtCut         = cms.double(1.0),
    barrelEtaCut       = cms.double(1.3),
    jetTriggerNames    = cms.vstring('HLT_DiJetAve15U_8E29','HLT_DiJetAve30U_8E29'),
    triggerProcessName = cms.string('HLT'),
    triggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    isMCarlo           = cms.bool(False)
)

process.ak5pfl2l3 = process.ak5pf.clone(jets = 'ak5PFJetsL2L3')
process.ak5pfl2l3res = process.ak5pf.clone(jets = 'ak5PFJetsL2L3Residual')

process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0')

#############   Path       ###########################
process.p = cms.Path(
       #----- apply trigger selection and PV filter ------
       process.hltLevel1GTSeed * process.HLTfilter * process.primaryVertexFilter *
       #----- create collections -------------------------
       #process.trackExtrapolator * process.JetPlusTrackCorrectionsAntiKt5 * 
       #----- create corrected jet collections -----------
       process.ak5CaloJetsL2L3 * process.ak5PFJetsL2L3 * process.ak5JPTJetsL2L3 *
       #----- create residual corrected jet collections --
       process.ak5CaloJetsL2L3Residual * process.ak5PFJetsL2L3Residual * process.ak5JPTJetsL2L3Residual *
       #----- process raw jets ---------------------------
       process.ak5calo * process.ak5pf * process.ak5jpt *
       #----- process corrected jets ---------------------
       process.ak5calol2l3 * process.ak5pfl2l3 * process.ak5jptl2l3 *
       #----- process residual corrected jets ------------
       process.ak5calol2l3res * process.ak5pfl2l3res * process.ak5jptl2l3res
)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
