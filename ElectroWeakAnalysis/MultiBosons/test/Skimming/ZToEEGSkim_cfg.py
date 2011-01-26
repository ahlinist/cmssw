import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## setup tags
options.setupTags (tag = 'of_%d',
    ifCond = 'totalSections > 0',
    tagArg = 'totalSections'
)
options.setupTags (tag = '%03d',
    ifCond = 'totalSections > 0',
    tagArg = 'section'
)

## setup any defaults you want
options.outputFile = "ZToEEGSkim.root"
options.maxEvents = -1 # -1 means all events
sourcePath = "/store/data/Run2010B/Electron/RECO/PromptReco-v2/000/149/291/"
sourceFiles = """
    E6289EB1-DAE4-DF11-91E8-000423D9997E.root
    E42C8C6C-E4E4-DF11-8C68-0019DB29C5FC.root
    DA8F57A1-F4E4-DF11-B899-003048F118C6.root
    CEE37FBF-E3E4-DF11-BE0B-0019B9F581C9.root
    CC974202-DCE4-DF11-81A8-001D09F24691.root
    CAE1A9C1-EAE4-DF11-9591-00304879FC6C.root
    C6B717C7-D7E4-DF11-926F-000423D98950.root
    B6951DB4-DAE4-DF11-8E1F-001D09F23C73.root
    B0A6EA2A-D2E4-DF11-B65D-003048F024DC.root
    AE440884-D8E4-DF11-B173-001D09F24489.root
    A815934E-E0E4-DF11-BF1D-003048F118DE.root
    A66C89CA-D7E4-DF11-87E1-003048F11CF0.root
    A66C07D9-D2E4-DF11-9732-003048F118D2.root
    A0EE3DC7-D7E4-DF11-9A59-001617E30D4A.root
    A0924DC8-D7E4-DF11-886F-003048F118DE.root
    9E843CB7-DCE4-DF11-8DA6-003048F11C5C.root
    9097562A-D2E4-DF11-93AB-0019B9F72D71.root
    88737CDA-D2E4-DF11-B810-003048F118DE.root
    82FA37CB-D7E4-DF11-9451-001D09F2545B.root
    7C7999EE-DEE4-DF11-AA4D-0016177CA778.root
    701C1129-D2E4-DF11-908E-003048F11DE2.root
    5AEC476C-E4E4-DF11-8830-001617C3B76A.root
    5A1D36B1-DAE4-DF11-A12F-001617DBD224.root
    54BA1E4F-E0E4-DF11-8916-003048F11C28.root
    4A86BC83-D8E4-DF11-BB3E-0019B9F709A4.root
    4876D7EC-DEE4-DF11-BE90-001617E30F50.root
    481745AD-DDE4-DF11-AA83-000423D94908.root
    46FAA3EE-DEE4-DF11-BD03-0019B9F581C9.root
    403E2700-DAE4-DF11-870C-001D09F28EC1.root
    3E4D2903-DCE4-DF11-AEA1-001D09F25208.root
    3E13C425-E5E4-DF11-BF80-001617C3B6C6.root
    30D2344F-E0E4-DF11-8D04-003048F118C2.root
    248C25F3-D4E4-DF11-BADC-001D09F29146.root
    1AD8A9D5-E5E4-DF11-AD95-001617C3B6C6.root
    1863A6EA-D7E4-DF11-BF56-003048F117EA.root
    164C079B-DFE4-DF11-B91A-001D09F2532F.root
    104F12EE-DEE4-DF11-9ACC-001617E30CD4.root
    """.split()
options.inputFiles = ",".join([sourcePath + f for f in sourceFiles])

# get and parse the command line arguments
options.parseArguments()


process = cms.Process('SKIM')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
if options.maxEvents < 0:
    process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZToEEGSkim_cff")
process.ZToEEGSkimFilterPath = cms.Path(process.ZToEEGSkimFilterSequence)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("ZToEEGSkimFilterPath")
    ),
    outputCommands = cms.untracked.vstring(
        "keep *",
        "drop *_*_*_SKIM"
    )
)

process.endpath = cms.EndPath(process.out)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

if __name__ == "__main__": import user
