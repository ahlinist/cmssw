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
options.outputFile = "ZToMMGSkim.root"
options.maxEvents = -1 # -1 means all events
## All files from run 149291 (last big run in 2010)
sourcePath = "/store/data/Run2010B/Mu/RECO/Nov4ReReco_v1/0000/"
sourceFiles = """
    FE80786F-E1E9-DF11-96A3-E0CB4E29C507.root
    F84230E3-62EA-DF11-AE54-485B39800B94.root
    E631865A-FAE9-DF11-86D9-E0CB4E4408D9.root
    E2397BC0-FFE9-DF11-B382-0019BB3F73A4.root
    DCE71D65-CAE9-DF11-89D9-E0CB4E19F969.root
    D640ADD7-36EC-DF11-B121-90E6BA442F1C.root
    D2F32612-9EEA-DF11-9DC6-E0CB4EA0A8E8.root
    D23B3B33-8FEA-DF11-8AE9-E0CB4E29C50F.root
    D0F1FF0B-FDE9-DF11-A1DF-90E6BA442F21.root
    C8FF3FC5-9DEA-DF11-AEB4-E0CB4E29C4BB.root
    C43C9D93-E4E9-DF11-AF71-485B39800C32.root
    B242852D-0FEA-DF11-B4B1-E0CB4E553676.root
    AC76DDD4-D2E9-DF11-8A80-485B39800B62.root
    A6C20F6F-15EA-DF11-AA84-E0CB4E19F989.root
    A4B8220A-0FEA-DF11-8ED7-E0CB4E55367A.root
    9E13C32B-02EA-DF11-A5B5-E0CB4E1A119A.root
    94D3181F-F4E9-DF11-AC85-E0CB4E29C4E5.root
    92EC528E-36EC-DF11-8AF7-E0CB4EA0A91C.root
    9002948E-9CEA-DF11-ACDC-90E6BA0D09E2.root
    88613917-0EEA-DF11-9DFC-90E6BA0D09EC.root
    86601C02-DFE9-DF11-9A16-E0CB4E553665.root
    8454090C-0FEA-DF11-880A-90E6BA0D09AF.root
    826DC429-60EA-DF11-A4B0-E0CB4E29C4B6.root
    808FCA8C-6DEA-DF11-B06C-E0CB4E4408CD.root
    7E51879E-7FEA-DF11-97F7-E0CB4E29C4B7.root
    7258C2EB-F7E9-DF11-A766-90E6BA19A22B.root
    6E58D935-F5E9-DF11-B950-E0CB4E29C4BF.root
    6A838104-3EEA-DF11-8E65-90E6BA19A20E.root
    5C425115-A7EA-DF11-ADAE-E0CB4E29C4D2.root
    589FF616-A7EA-DF11-963E-E0CB4E19F9A3.root
    52F84CB6-0EEA-DF11-AAEC-00261834B580.root
    32550F9A-17EA-DF11-8958-0022198F5AF3.root
    2E6A5ADB-C0E9-DF11-8BAD-E0CB4E1A117E.root
    2A80CA9B-55EA-DF11-880F-00261834B59B.root
    28860159-08EA-DF11-8613-E0CB4E29C4F9.root
    28538E3D-D9E9-DF11-8F88-E0CB4E55368D.root
    24C23ED6-36EC-DF11-AC3E-E0CB4E553659.root
    22FF9843-D7E9-DF11-9FF8-E0CB4E553659.root
    22B94FE4-88EA-DF11-AB13-90E6BA19A25E.root
    20EF9703-11EA-DF11-907B-001EC9D81A4A.root
    1ED9DF4E-B9E9-DF11-BCE2-90E6BA442F00.root
    1EC13C0C-F3E9-DF11-A73B-E0CB4E553666.root
    1E6D1129-0FEA-DF11-86B2-90E6BA442F21.root
    16F10202-0FEA-DF11-A9AC-90E6BA442F13.root
    14776F21-FAE9-DF11-A268-E0CB4E29C4E4.root
    14012967-66EA-DF11-9820-90E6BA0D0998.root
    0A33D328-0FEA-DF11-B65B-E0CB4E29C4D5.root
    08CDCBB0-0FEA-DF11-82C9-E0CB4E553646.root
    0865E2D6-DBE9-DF11-BFF1-90E6BA442F42.root
    06A829BA-FCE9-DF11-847B-90E6BA442F2D.root
    045726BC-BFE9-DF11-99E0-E0CB4E1A1169.root
    """.split()
options.inputFiles = ",".join([sourcePath + f for f in sourceFiles])

# get and parse the command line arguments
options.parseArguments()


process = cms.Process("SKIM")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
if options.maxEvents < 0:
    process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZToMMGSkim_cff")
process.ZToMMGSkimFilterPath = cms.Path(process.ZToMMGSkimFilterSequence)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("ZToMMGSkimFilterPath")
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
