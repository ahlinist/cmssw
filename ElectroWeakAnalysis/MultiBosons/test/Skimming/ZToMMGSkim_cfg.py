import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

sourcePath = "/store/data/Run2010A/Mu/RECO/v4/000/"
sourceFiles = """
    142/933/C043478A-79A7-DF11-BB4C-001D09F2426D.root
    142/933/C277D905-95A7-DF11-A70D-003048F024F6.root
    142/933/CC18C413-9CA7-DF11-91B8-003048F118AA.root
    142/933/D433F278-9FA7-DF11-8403-003048F11114.root
    """.split()
process.source.fileNames.extend([sourcePath + f for f in sourceFiles])

process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZToMMGSkim_cff")
process.ZToMMGSkimFilterPath = cms.Path(process.ZToMMGSkimFilterSequence)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("ZToMMGSkim.root"),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("ZToMMGSkimFilterPath")
    ),
)

process.endpath = cms.EndPath(process.out)

if __name__ == "__main__": import user