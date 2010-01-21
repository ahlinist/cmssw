import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
      'file:myfile.root'
    )
)

# process.options = cms.untracked.PSet(
#    IgnoreCompletely = cms.untracked.vstring( "ProductNotFound" )
# )

process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 500

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT',

    src = cms.InputTag("onia2MuMuPatTrkTrk"),
    srcWithCaloMuons = cms.InputTag("onia2MuMuPatGlbCal"),

    histFileName = cms.string("Histos.root"),		
    dataSetName = cms.string("DataSet.root"),
                              
    # pTBinRanges = cms.vdouble(3.0, 5.0, 6.0, 7.0, 8.0, 9.0, 11.0, 14.0, 18.0, 25.0, 35.0, 60.0),
    pTBinRanges = cms.vdouble(0.5, 3.0, 6.0, 8.0, 10.0, 15.0, 35.0),
    # etaBinRanges = cms.vdouble(0.0, 1.1, 2.5),
    etaBinRanges = cms.vdouble(0.0, 2.5),
    onlyTheBest = cms.bool(True),		
    applyCuts = cms.bool(True),			
    storeEfficiency = cms.bool(False),	
    useBeamSpot = cms.bool(False),
    useCaloMuons = cms.untracked.bool(False),
    removeSignalEvents = cms.untracked.bool(False),
    TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT")           
    # For Summer09 only 
    # TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT8E29")
)


process.p = cms.Path(process.demo)
