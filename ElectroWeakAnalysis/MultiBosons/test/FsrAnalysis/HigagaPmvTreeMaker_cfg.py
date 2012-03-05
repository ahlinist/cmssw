import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
    "Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v3.txt", # default value
    VarParsing.VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.VarParsing.varType.string,          # string, int, or float
    "JSON file to be applied."
)

options.register("globalTag",
                 "GR_R_39X_V6::All", # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string,         # string, int, or float
                 "Global tag to be used."
                 )

options.register("isMC",
                 False, # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.bool,         # string, int, or float
                 "Is this MC?"
                 )

options.setupTags(tag = "of%d",
                  ifCond = "totalSections != 0",
                  tagArg = "totalSections")

options.setupTags(tag = "job%d",
                  ifCond = "section != 0",
                  tagArg = "section")

## setup any defaults you want
options.maxEvents = 10 # -1 means all events
options.inputFiles = ["file:/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu/VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_1_of_4.root"]
options.outputFile = "pmvTree.root"


## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("TEST")

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
## Enable LogInfo
process.MessageLogger.cerr.INFO.limit = 100
#process.MessageLogger.debugModules = ["pmvTree"]
#process.MessageLogger.cerr.threshold = "DEBUG"

### Geometry, Detector Conditions and Pythia Decay Tables (needed for the vertexing)
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = options.globalTag
#process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring() + options.inputFiles
)

# JSON file
if not options.isMC and options.jsonFile != "":
    myLumis = \
        LumiList.LumiList(filename = options.jsonFile
                          ).getCMSSWString().split(',')
    process.source.lumisToProcess = \
        CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

from ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi \
    import photonSelection_Fsr2011Apr11_PixelMatchVeto as photonSelection

process.selectedPhotons = cms.EDFilter("VGammaPhotonFilter",
    filterParams = photonSelection,
    src = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.selectionSequence = cms.Sequence(
    process.selectedPhotons
)

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.HigagaPmvTreeMaker_cfi")

process.pmvTree.isMC = options.isMC

## Pileup
if options.isMC:
    process.pmvTree.pileupInfoSrc = cms.untracked.InputTag("addPileupInfo")
    process.pmvTree.lumiReWeighting = cms.untracked.PSet(
        mcDistribution   = cms.vdouble(
            ## In-time Poisson smeared Distribution for Fall 2011 S6 MC
            ## see https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities#Sample_Input_Distributions
              0.0145837,     0.025683,    0.0384606,    0.0494145,    0.0569311,
              0.0611828,    0.0625346,    0.0614769,    0.0586775,    0.0554499, #10
              0.0515491,     0.047621,    0.0439238,    0.0405691,    0.0374147,
               0.034227,    0.0314377,    0.0288256,     0.026219,    0.0237271, #20
              0.0213656,    0.0191874,    0.0169728,    0.0149206,     0.013039,
              0.0112938,   0.00961247,   0.00819356,   0.00688805,   0.00571524, #30
             0.00471123,   0.00386993,   0.00315452,   0.00254742,   0.00202471,
             0.00157441,   0.00124581,  0.000955206,  0.000735305,  0.000557304, #40
             0.000412503,  0.000305502,  0.000231002,  0.000165701,  0.000121201,
             9.30006e-05,  6.40004e-05,  4.22003e-05,  2.85002e-05,  1.96001e-05, #50
#             1.59001e-05,  1.01001e-05,  8.50006e-06,  6.60004e-06,  2.70002e-06  #55

            ## from the S3 gamma+jet sample (no filter)
            ## 21 numbers
            # 257141., 295755., 263008., 286909., 282291., 281067.,
            # 295777., 297075., 250569., 299795., 256528., 248686.,
            # 203484., 137833., 117686., 76877., 62815., 35462.,
            # 8381., 10012., 4233.

            ## from the S4 gamma + jet sample (no filter)
            ## 51 numbers, use only first 42
#             1.15148e+06, 582849, 629204, 642292, 658930, 666227,
#             668263, 649863, 623035, 588189, 528601, 478063,
#             412804, 351588, 285862, 231776, 181493, 139729,
#             104007, 77262, 55684, 39053, 27132, 18393,
#             12278, 8039, 5393, 3301, 2152, 1321,
#             875, 482, 317, 195, 98, 75,
#             44, 22, 15, 5, 7, 2,
#             0, 1, 0, 0, 0, 0,
#             0, 0, 0,
        ),
        dataDistribution = cms.vdouble(
            ## the length has to be exactly the same as for the MC!

            ## From pileupCalc using the analysis_AN-12-048_HggMVA.json
            ##+ This is the intersection of the certified lumi and lumi in files
            ##+ for the AN-12-048 (Hgg MVA).
            ##+ https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData
            9.31279e+06, 4.37466e+07, 1.10154e+08, 1.9781e+08, 2.85437e+08, 3.5408e+08, # 6
            3.94674e+08, 4.07888e+08, 3.99865e+08, 3.77884e+08, 3.47865e+08, 3.137e+08, # 12
            2.77658e+08, 2.41091e+08, 2.05009e+08, 1.70366e+08, 1.38101e+08, 1.0904e+08,# 18
            8.3782e+07, 6.26145e+07, 4.55102e+07, 3.21754e+07, 2.21349e+07, 1.4825e+07, # 24
            9.67233e+06, 6.15145e+06, 3.81622e+06, 2.31105e+06, 1.36714e+06, 790595,    # 30
            447235, 247660, 134340, 71427.6, 37249.4, 19064.9, # 36
            9582.42, 4732.62, 2298.08, 1097.79, 516.183, 239.036, # 42
            109.078, 49.0747, 21.7801, 9.54063, 4.127, 1.76384, # 48
            0.745197, 0.31138, # 50

            ## from estimatePileupD.py for golden JSON for runs 160404-166861
#             1.00826e+07, 1.9655e+07, 4.58762e+07, 7.63478e+07, 9.9728e+07, 1.0842e+08,
#             1.01847e+08, 8.48512e+07, 6.39051e+07, 4.41459e+07, 2.82916e+07, 1.69742e+07,
#             9.60532e+06, 5.15841e+06, 2.64284e+06, 1.29755e+06, 612859, 279413,
#             123331, 52841.1, 22026.7, 8951.4, 3552.86, 1379.43,
#             524.638, 195.694, 71.6639, 25.7868, 9.12372, 3.17583,
#             1.088, 0.36694, 0.121851, 0.0398426, 0.0128274, 0.00406596,
#             0.00126871, 0.000389638, 0.000117757, 3.50154e-05, 1.02425e-05, 2.94689e-06,
#             8.33821e-07, 2.32e-07, 6.34707e-08, 1.7073e-08, 4.51528e-09, 1.17408e-09,
#             3.00169e-10, 2.00066e-07, 0,

            ## from estimatePileupD.py for golden JSON for runs 136033-166861
#             1.56121e+07, 2.87272e+07, 5.46463e+07, 8.25868e+07, 1.03348e+08, 1.10229e+08,
#             1.02651e+08, 8.51755e+07, 6.40254e+07, 4.41874e+07, 2.8305e+07, 1.69782e+07,
#             9.60647e+06, 5.15872e+06, 2.64292e+06, 1.29757e+06, 612863, 279414,
#             123331, 52841.1, 22026.7,

            ## from estimatePileupD.py for golden JSON up to run 165542
            # 4.49103e+06, 7.50711e+06, 1.7013e+07, 2.77526e+07, 3.56721e+07, 3.82648e+07,
            # 3.55386e+07, 2.93206e+07, 2.18974e+07, 1.50169e+07, 9.56312e+06, 5.70622e+06,
            # 3.21393e+06, 1.71936e+06, 878374, 430566, 203380, 92934.5,
            # 41228.6, 17815.2, 7520.35,# 3109.37, 1262.01, 503.739,
            #198.015, 76.7276, 29.3217, 11.0527, 4.10876, 1.50569,
            #0.543606, 0.193229, 0.0675766, 0.0232364, 0.00785103, 0.0026052,
            #0.000848637, 0.000271282, 8.50798e-05, 2.61736e-05, 7.8975e-06, 2.33716e-06,
            #6.78371e-07, 1.93133e-07, 5.39384e-08, 1.47793e-08, 3.97367e-09, 1.04856e-09,
            #2.71605e-10, 6.92423e-08, 0,



            ## from estimatePileupD.py for golden JSON up to run 163869
            #3.6124e+06, 5.7606e+06, 1.3047e+07, 2.12065e+07, 2.71345e+07, 2.89995e+07,
            #2.68765e+07, 2.21641e+07, 1.65695e+07, 1.13875e+07, 7.27332e+06, 4.35533e+06,
            #2.46294e+06, 1.32354e+06, 679618, 335115, 159402, 73447,
            #32906.5, 14384.3, 6152.9, #2581.8, 1064.77, 432.206,
            #172.826, 68.1079, 26.4529, 10.1234, 3.81552, 1.4155,
            #0.51655, 0.185307, 0.0653117, 0.0226036, 0.00767821, 0.00255903,
            #0.000836568, 0.000268193, 8.43057e-05, 2.59835e-05, 7.85175e-06, 2.32636e-06,
            #6.75872e-07, 1.92565e-07, 5.3812e-08, 1.47516e-08, 3.96773e-09, 1.0473e-09,
            #2.71346e-10, 5.26651e-08, 0.
        )
        #mcFile = cms.FileInPath('pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11.root'),
        #mcHist = cms.string('pudist'),
        #dataFile = cms.FileInPath('pudist_160404-163869_Cert_JSON.root'),
        #dataHist = cms.string('pileup'),
    )

process.p = cms.Path(
    process.selectionSequence *
    process.pmvTree
)

process.options.wantSummary = False

process.MessageLogger.debugModules = ["pmvTree"]
process.MessageLogger.cerr.threshold = "DEBUG"


if __name__ == "__main__": import user

