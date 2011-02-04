import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
                 "file:onia2MuMuPAT.root"
   )
)

process.hltMuF = cms.EDFilter("HLTHighLevel",
                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                 HLTPaths = cms.vstring("HLT_DoubleMu0"),
                 eventSetupPathsKey = cms.string(''),
                 andOr = cms.bool(True),
                 throw = cms.bool(False)
)


# filter on good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4),
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT',

    src = cms.InputTag("onia2MuMuPatTrkTrk"),
    srcWithCaloMuons = cms.InputTag("onia2MuMuPatGlbCal"),

    writeTree = cms.bool(True),
    treeFileName = cms.string("testTree.root"),

    writeDataSet = cms.bool(True),                 
    dataSetName = cms.string("testDataSet.root"),
    triggerForDataset = cms.string("HLT_DoubleMu0"),

    massMin = cms.double(2.6),
    massMax = cms.double(3.5),
    pTBinRanges = cms.vdouble(0.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
    etaBinRanges = cms.vdouble(0.0, 1.3, 2.5),
    onlyTheBest = cms.bool(True),		
    applyCuts = cms.bool(True),
    applyExpHitCuts = cms.bool(False),
    applyDiMuonCuts = cms.bool(True),                          
    useBeamSpot = cms.bool(True),
    useCaloMuons = cms.untracked.bool(False),
    removeSignalEvents = cms.untracked.bool(False),
    removeTrueMuons = cms.untracked.bool(False),
    storeWrongSign = cms.untracked.bool(True),
    writeOutCandidates = cms.untracked.bool(False),
    massCorrectionMode = cms.int32(3),    # mode 0 no correction, mode 1 constant corr, mode 2 pt dependent corr, mode 3 pt and eta dependent corr
    oniaPDG = cms.int32(443),
    genParticles = cms.InputTag("genMuons"),
    isMC = cms.untracked.bool(True),
    storeAllMCEvents = cms.untracked.bool(True),
    isPromptMC = cms.untracked.bool(True),
    TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT")
)

## no filter
# process.p = cms.Path(process.demo)

## filter on vertex
process.p = cms.Path(process.primaryVertexFilter*process.demo)

## filter on vertex and HLT
# process.p = cms.Path(process.primaryVertexFilter*process.hltMuF*process.demo)
