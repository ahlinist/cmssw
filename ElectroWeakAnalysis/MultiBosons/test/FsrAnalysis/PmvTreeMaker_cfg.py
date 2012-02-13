import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
    "Cert_160404-165542_7TeV_PromptReco_Collisions11_JSON.txt", # default value
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
#options.inputFiles = ["file:/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu/VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_1_of_4.root"]
options.outputFile = "pmvTree.root"
options.jsonFile = "Cert_160404-163869_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("TEST")

## Load standard sequence for crack corrections
process.load('CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi')
# process.load('Configuration.StandardSequences.Services_cff')
# process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
# process.load('Configuration.StandardSequences.Reconstruction_cff')
# process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

## Global tag
# process.GlobalTag.globaltag = options.globalTag

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
## Enable LogInfo
# process.MessageLogger.cerr.INFO.limit = 100
## Enable LogDebug
### Remember to recompile with:
### scramv1 b USER_CXXFLAGS="-g\ -D=EDM_ML_DEBUG"
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


from ElectroWeakAnalysis.MultiBosons.Selectors.muonSelector_cfi \
    import muonSelection_FsrApr082011_PixelMatchVeto as muonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi \
    import diMuonSelection_Fsr2011Apr11_PixelMatchVeto as diMuonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi \
    import photonSelection_Fsr2011Apr11_PixelMatchVeto as photonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.ZMuMuGammaSelector_cfi \
    import ZMuMuGammaSelection_Fsr2011Apr11_PixelMatchVeto as ZMuMuGammaSelection

process.selectedMuons = cms.EDFilter("VGammaMuonFilter",
    filterParams = muonSelection,
    src = cms.InputTag("cleanPatMuonsTriggerMatch","","PAT"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.goodDiMuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
#process.goodDiMuons = cms.EDProducer("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"), ## dummy cut
    decay = cms.string("selectedMuons selectedMuons"),
    roles = cms.vstring("muon1", "muon2")
)

process.selectedDiMuons = cms.EDFilter("VGammaDiLeptonFilter",
    filterParams = diMuonSelection,
    src = cms.InputTag("goodDiMuons"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

process.selectedPhotons = cms.EDFilter("VGammaPhotonFilter",
    filterParams = photonSelection,
    src = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

#process.vertexedDiMuons = cms.EDProducer("KalmanVertexFitCompositeCandProducer",
    #src = cms.InputTag("selectedDiMuons")
#)

process.goodZMuMuGammas = cms.EDProducer("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"), ## dummy cut
    decay = cms.string("selectedDiMuons selectedPhotons"),
    roles = cms.vstring("dimuon", "photon")
)

process.selectedZMuMuGammas = cms.EDFilter("ZMuMuGammaFilter",
    filterParams = ZMuMuGammaSelection,
    src = cms.InputTag("goodZMuMuGammas"),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)

## Loosen the invariant mass window
process.selectedZMuMuGammas.filterParams.minMass = 50
process.selectedZMuMuGammas.filterParams.maxMass = 130

process.selectionSequence = cms.Sequence(
    process.selectedMuons *
    process.goodDiMuons *
    process.selectedDiMuons *
    process.selectedPhotons *
    #process.vertexedDiMuons *
    process.goodZMuMuGammas *
    process.selectedZMuMuGammas
)

#process.mmgTree = cms.EDAnalyzer("MuMuGammaTreeMaker",
    #photonSrc   = cms.untracked.InputTag("selectedPhotons"),
    #muonSrc     = cms.untracked.InputTag("selectedMuons"),
    #dimuonSrc   = cms.untracked.InputTag("selectedDiMuons"),
    #beamSpotSrc = cms.untracked.InputTag("offlineBeamSpot"),
    #primaryVertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
    #ebClusterSrc = cms.untracked.InputTag("islandBasicClusters", "islandBarrelBasicClusters"),
    #ebRecHitsSrc = cms.untracked.InputTag("ecalRecHit", "EcalRecHitsEB"),
    #eeRecHitsSrc = cms.untracked.InputTag("ecalRecHit", "EcalRecHitsEE"),
    #genParticleSrc = cms.untracked.InputTag("prunedGenParticles"),
    #isMC        = cms.untracked.bool(False),
    #)

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.PmvTreeMaker_cfi")

process.pmvTree.isMC = options.isMC

## Pileup
if options.isMC:
    process.pmvTree.pileupInfoSrc = cms.untracked.InputTag("addPileupInfo")
    process.pmvTree.lumiReWeighting = cms.untracked.PSet(
        mcDistribution   = cms.vdouble(
            ## from the gamma+jet sample (no filter)
            ## 21 numbers
            # 257141., 295755., 263008., 286909., 282291., 281067.,
            # 295777., 297075., 250569., 299795., 256528., 248686.,
            # 203484., 137833., 117686., 76877., 62815., 35462.,
            # 8381., 10012., 4233.

            ## from the S4 gamma + jet sample (no filter)
            ## 51 numbers, use only first 36
            1.15148e+06, 582849, 629204, 642292, 658930, 666227,
            668263, 649863, 623035, 588189, 528601, 478063,
            412804, 351588, 285862, 231776, 181493, 139729,
            104007, 77262, 55684, 39053, 27132, 18393,
            12278, 8039, 5393, 3301, 2152, 1321,
            875, 482, 317, 195, 98, 75,
#            44, 22, 15, 5, 7, 2,
#             0, 1, 0, 0, 0, 0,
#             0, 0, 0,
        ),
        dataDistribution = cms.vdouble(
            ## the length has to be exactly the same as for the MC!

            ## from https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/
            ##+ certification/Collisions11/7TeV/PileUp/*.pileup_v2.root
            ##+ Run 2011A and 2011B combined
#             1.34465e+07, 5.90653e+07, 1.40903e+08, 2.41301e+08, 3.33745e+08, 3.98711e+08,
#             4.30106e+08, 4.32283e+08, 4.1382e+08, 3.82846e+08, 3.45164e+08, 3.04344e+08,
#             2.62555e+08, 2.21331e+08, 1.81983e+08, 1.4569e+08, 1.13413e+08, 8.57789e+07,
#             6.30124e+07, 4.49596e+07, 3.1169e+07, 2.10079e+07, 1.37759e+07, 8.79641e+06,
#             5.47442e+06, 3.32378e+06, 1.97064e+06, 1.14204e+06, 647539, 359547,
#             195673, 104460, 54745.2, 28185.6, 28005.5, 0.008,

            ## Run 2011A only
#             1.29654e+07, 5.58514e+07, 1.29329e+08, 2.12134e+08, 2.76138e+08, 3.03604e+08,
#             2.93258e+08, 2.55633e+08, 2.0497e+08, 1.53264e+08, 1.07936e+08, 7.21006e+07,
#             4.5913e+07, 2.797e+07, 1.63426e+07, 9.17598e+06, 4.95861e+06, 2.58239e+06,
#             1.2977e+06, 629975, 295784, 134470, 59260.1, 25343.9,
#             10530.1, 4255.05, 1673.95, 641.776, 240.022, 87.6504,
#             31.281, 10.9195, 3.73146, 1.24923, 0.602368, 0.008,

            ## Run 2011B only
            481142, 3.21393e+06, 1.15733e+07, 2.91676e+07, 5.76072e+07, 9.51074e+07,
            1.36849e+08, 1.7665e+08, 2.0885e+08, 2.29582e+08, 2.37228e+08, 2.32243e+08,
            2.16642e+08, 1.93361e+08, 1.6564e+08, 1.36514e+08, 1.08455e+08, 8.31965e+07,
            6.17147e+07, 4.43296e+07, 3.08733e+07, 2.08734e+07, 1.37166e+07, 8.77106e+06,
            5.46389e+06, 3.31952e+06, 1.96896e+06, 1.1414e+06, 647299, 359460,
            195642, 104449, 54741.4, 28184.3, 28004.9, 0,

            ## from estimatePileupD.py for golden JSON up to run 173244
#             2.66037e+07, 6.20837e+07, 1.28931e+08, 2.00545e+08, 2.5334e+08, 2.73133e+08,
#             2.5988e+08, 2.23527e+08, 1.76897e+08, 1.30515e+08, 9.06582e+07, 5.972e+07,
#             3.75081e+07, 2.2549e+07, 1.30131e+07, 7.2248e+06, 3.86533e+06, 1.99552e+06,
#             995277, 480084, 224189, 101452, 44532.8, 18979.4,
#             7860.96, 3167.1, 1242.31, 474.86, 177.025, 64.4158,
#             22.8974, 7.95686, 2.70506, 0.900305, 0.293541, 0.0938176,
#             0.02941, 0.0090478, 0.00273311, 0.000811054, 0.000236549, 6.78354e-05,

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

            ## from estimatePileupD.py for golden JSON up to run 166502
            #3.36441e+06, 6.50754e+06, 1.57837e+07, 2.75468e+07, 3.78054e+07, 4.31307e+07,
            #4.2414e+07, 3.68867e+07, 2.8917e+07, 2.07353e+07, 1.37572e+07, 8.52297e+06,
            #4.9674e+06, 2.74032e+06, 1.43822e+06, 721206, 346808, 160424,
            #71576.4, 30874.3, 12901.2, #5231.58, 2061.91, 790.889,

            ## from estimatePileupD.py for golden JSON up to run 165542
            #4.49103e+06, 7.50711e+06, 1.7013e+07, 2.77526e+07, 3.56721e+07, 3.82648e+07,
            #3.55386e+07, 2.93206e+07, 2.18974e+07, 1.50169e+07, 9.56312e+06, 5.70622e+06,
            #3.21393e+06, 1.71936e+06, 878374, 430566, 203380, 92934.5,
            #41228.6, 17815.2, 7520.35,# 3109.37, 1262.01, 503.739,
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

if __name__ == "__main__": import user

