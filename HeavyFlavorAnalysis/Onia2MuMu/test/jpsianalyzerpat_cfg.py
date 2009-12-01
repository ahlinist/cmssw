import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:SkimmingOnia2MuMuPAT_output.root'
    )
)

# process.options = cms.untracked.PSet(
#    IgnoreCompletely = cms.untracked.vstring( "ProductNotFound" )
# )

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT',

    histFileName = cms.string("Histos.root"),		
    dataSetName = cms.string("DataSet.root"),
                              
    pTBinRanges = cms.vdouble(3.0, 5.0, 6.0, 7.0, 8.0, 9.0, 11.0, 14.0, 18.0, 25.0, 35.0, 60.0),
    etaBinRanges = cms.vdouble(0.0, 1.1, 2.5), 
    onlyTheBest = cms.bool(True),		
    applyCuts = cms.bool(True),			
    storeEfficiency = cms.bool(False),	
    useBeamSpot = cms.bool(False),
    useCaloMuons = cms.untracked.bool(False), 				
    TriggerResultsLabel = cms.InputTag("TriggerResults","","HLT8E29")
)


process.p = cms.Path(process.demo)
