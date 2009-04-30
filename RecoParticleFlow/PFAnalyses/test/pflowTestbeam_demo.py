import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("RecoParticleFlow.PFAnalyses.pflowProcessTestbeam_cff")

runs = cms.untracked.vstring("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029732.combined.OutServ_0.0-cmsswreco.root",)

process.TFileService.fileName = cms.string("demo_outputtree_7GeV.root")
process.finishup.fileName = cms.untracked.string("demo_reprocessed_7GeV.root")

# This should be updated as necessary
process.faketracks.runinfo_cuts = cms.string("/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_v1_3X.root")
process.extraction.runinfo_cuts = cms.string("/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_v1_3X.root")
process.particleFiltration.runinfo_cuts = cms.string("/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_v1_3X.root")

#process.extraction.debug = cms.int32(4)

process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(2000)
)
process.particleFlowCluster.verbose = cms.bool(True)
process.dump = cms.EDAnalyzer('EventContentAnalyzer')


process.source = cms.Source("PoolSource",
        fileNames=runs,
		inputCommands=cms.untracked.vstring('keep *', 'drop EBDataFramesSorted_*_*_*')

)


#process.MessageLogger = cms.Service("MessageLogger",
#    debugModules=cms.untracked.vstring('particleFiltration', 'extraction'),
#    cout=cms.untracked.PSet(
#        TestbeamFiltratrionDelegate=cms.untracked.PSet(
#            limit=cms.untracked.int32(10000000)
#        ),
#        TestbeamDelegate=cms.untracked.PSet(
#            limit=cms.untracked.int32(10000000)
#        ),
#        threshold=cms.untracked.string('INFO')
#    ),
#    log=cms.untracked.PSet(
#     
#        threshold=cms.untracked.string('INFO')
#    ),
#    categories=cms.untracked.vstring('TestbeamFiltrationDelegate',
#        'TestbeamDelegate'),
#    destinations=cms.untracked.vstring('cout', 'log.txt')
#)

process.p1 = cms.Path(process.pflowProcessTestbeam)
process.outpath = cms.EndPath(process.finishup)
