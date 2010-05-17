import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
       'file:onia2MuMuPAT.root'
    )
)

# process.options = cms.untracked.PSet(
#    IgnoreCompletely = cms.untracked.vstring( "ProductNotFound" )
# )

process.hltL1MuOpen = cms.EDFilter("HLTHighLevel",
                      TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                      HLTPaths = cms.vstring("HLT_L1MuOpen"),
                      eventSetupPathsKey = cms.string(''),
                      andOr = cms.bool(True),
                      throw = cms.bool(False)
)

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT',

    src = cms.InputTag("onia2MuMuPatTrkTrk"),
    srcWithCaloMuons = cms.InputTag("onia2MuMuPatGlbCal"),

    histFileName = cms.string("Histos.root"),   
    dataSetName = cms.string("DataSet.root"),
                              
    # pTBinRanges = cms.vdouble(3.0, 6.0, 8.0, 9.0, 10.0, 12.0, 15.0, 40.0),
    pTBinRanges = cms.vdouble(0.0, 40.0),
    # etaBinRanges = cms.vdouble(0.0, 1.6, 2.5),
    etaBinRanges = cms.vdouble(0.0, 3.0),
    onlyTheBest = cms.bool(True),		
    applyCuts = cms.bool(True),			
    storeEfficiency = cms.bool(False),	
    useBeamSpot = cms.bool(True),
    useRapidity = cms.bool(True),
    useCaloMuons = cms.untracked.bool(False),
    removeSignalEvents = cms.untracked.bool(False),
    removeTrueMuons = cms.untracked.bool(False),
    storeWrongSign = cms.untracked.bool(True),
    writeOutCandidates = cms.untracked.bool(True),
    includePsiPrime = cms.untracked.bool(False)
)


process.p = cms.Path(process.demo)
