import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIMEVENTS')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

sourcePath = "rfio:/castor/cern.ch/cms/store/data/Run2010A/Mu/RECO/v4/000/142/933/"

fileList = """
C043478A-79A7-DF11-BB4C-001D09F2426D.root
""".split()

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    [sourcePath + f for f in fileList]
   ),
   ## First baseline Z(->mumu)gamma candidate reported
   ## by Lindsey Gray on Aug 24 V-Gamma meeting
   eventsToProcess = cms.untracked.VEventRange("142933:20579653")
)

process.output = cms.OutputModule("PoolOutputModule",
   fileName = cms.untracked.string('InterestingEvents.root')
)

process.endpath = cms.EndPath(process.output)


