import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIMEVENTS')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

sourcePath = "rfio:/castor/cern.ch/cms/store/data/Run2010A/Mu/RECO/v4/000/"

fileList = """
142/933/C043478A-79A7-DF11-BB4C-001D09F2426D.root
143/657/A8367E04-73AE-DF11-9989-001D09F297EF.root
144/089/8C537DF5-72B3-DF11-8321-0030487CD840.root
144/112/90F5E085-FBB3-DF11-B6E8-0030486780B8.root
""".split()

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    [sourcePath + f for f in fileList]
   ),
   ## First baseline Z(->mumu)gamma candidate reported
   ## by Lindsey Gray on Aug 24 V-Gamma meeting
   eventsToProcess = cms.untracked.VEventRange("142933:20579653",
                                               "143657:101543051",
                                               "144089:1126044362",
                                               "144112:1393130648")
                            )

process.output = cms.OutputModule("PoolOutputModule",
   fileName = cms.untracked.string('InterestingEvents.root')
)

process.endpath = cms.EndPath(process.output)


