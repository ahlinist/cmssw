import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("RecoParticleFlow.PFAnalyses.pflowProcessTestbeam_cff")


runs = cms.untracked.vstring("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029732.combined.OutServ_0.0-cmsswreco.root", )

process.TFileService.fileName = cms.string("demo_outputtree_7GeV.root")
process.finishup.fileName = cms.untracked.string("demo_reprocessed_7GeV.root")

# This should be updated as necessary
process.extraction.runinfo_cuts = cms.string("/castor/cern.ch/user/b/ballin/tbv2/testbeam_cuts.root")
process.faketracks.runinfo_cuts = cms.string("/castor/cern.ch/user/b/ballin/tbv2/testbeam_cuts.root")

process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(2000)
)


process.source = cms.Source("PoolSource",
        fileNames = runs
)

process.p1 = cms.Path(process.pflowProcessTestbeam)
process.outpath = cms.EndPath(process.finishup)
