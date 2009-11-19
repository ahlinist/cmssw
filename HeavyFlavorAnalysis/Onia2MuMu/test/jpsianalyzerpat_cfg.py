import FWCore.ParameterSet.Config as cms

process = cms.Process("ANAPAT")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/hadoop/cms/store/group/tracker-pog/JPsiMuMu/TRK-JPsi_JPsiMuMu-SKIM_test4/b6694ead8530d72ae99d3d30b3d3cb8b/myOutputFile_16.root'
    )
)

process.demo = cms.EDAnalyzer('JPsiAnalyzerPAT'
)


process.p = cms.Path(process.demo)
