import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
    "Cert_160404-165970_7TeV_PromptReco_Collisions11_JSON.txt", # default value
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
options.outputFile = "tree.root"
options.jsonFile = "Cert_160404-163869_7TeV_PromptReco_Collisions11_JSON_MuonPhys.txt"

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("ESCALE")

## Load standard sequence for crack corrections
# process.load('CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi')
# process.load('Configuration.StandardSequences.Geometry_cff')

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
## Enable LogInfo
process.MessageLogger.cerr.INFO.limit = 100
## Enable LogDebug
### Remember to recompile with:
### scramv1 b USER_CXXFLAGS="-g\ -D=EDM_ML_DEBUG"
#process.MessageLogger.debugModules = ["tree"]
#process.MessageLogger.cerr.threshold = "DEBUG"

## Geometry, Detector Conditions and Pythia Decay Tables (needed for the vertexing)
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = options.globalTag
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
    import muonSelection_FsrApr082011 as muonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.diLeptonSelector_cfi \
    import diMuonSelection_Fsr2011Apr11 as diMuonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.photonSelector_cfi \
    import photonSelection_Fsr2011Apr11 as photonSelection

from ElectroWeakAnalysis.MultiBosons.Selectors.ZMuMuGammaSelector_cfi \
    import ZMuMuGammaSelection_Fsr2011Apr11 as ZMuMuGammaSelection

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
process.selectedDiMuons.filterParams.maxMass = 120

# process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.defaultPhotons_cfi")

process.selectedPhotons = cms.EDFilter("VGammaPhotonFilter",
    filterParams = photonSelection,
#     src = cms.InputTag("defaultPhotons"),
    src = cms.InputTag('cleanPatPhotonsTriggerMatch', '', 'PAT'),
    filter = cms.bool(True),
    verbosity = cms.untracked.uint32(2)
)
process.selectedPhotons.filterParams.cutsToIgnore.append('minPt')

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
process.selectedZMuMuGammas.filterParams.minMass = 50
process.selectedZMuMuGammas.filterParams.maxMass = 130

process.combinatoricFilter = cms.EDFilter("CandViewCountRangeFilter",
    src = cms.InputTag("selectedZMuMuGammas"),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1),
    )


process.selectionSequence = cms.Sequence(
    process.selectedMuons *
    process.goodDiMuons *
    process.selectedDiMuons *
#     process.defaultPhotons *
    process.selectedPhotons *
    #process.vertexedDiMuons *
    process.goodZMuMuGammas *
    process.selectedZMuMuGammas *
    process.combinatoricFilter
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

#process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.energyScaleTree_cfi")

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.PmvTreeMaker_cfi")
process.tree = process.pmvTree.clone()
process.tree.isMC = options.isMC


## Pileup
if options.isMC:
    process.tree.pileupInfoSrc = cms.untracked.InputTag("addPileupInfo")
    process.tree.lumiReWeighting = cms.untracked.PSet(
        mcDistribution   = cms.vdouble(
            ## from the gamma+jet sample (no filter)
            ## 21 numbers
#             257141., 295755., 263008., 286909., 282291., 281067.,
#             295777., 297075., 250569., 299795., 256528., 248686.,
#             203484., 137833., 117686., 76877., 62815., 35462.,
#             8381., 10012., 4233.

            ## from the S4 gamma + jet sample (no filter)
            ## 51 numbers, use only first 42
            1.15148e+06, 582849, 629204, 642292, 658930, 666227,
            668263, 649863, 623035, 588189, 528601, 478063,
            412804, 351588, 285862, 231776, 181493, 139729,
            104007, 77262, 55684, 39053, 27132, 18393,
            12278, 8039, 5393, 3301, 2152, 1321,
            875, 482, 317, 195, 98, 75,
            44, 22, 15, 5, 7, 2,
        ),
        dataDistribution = cms.vdouble(
            ## the length has to be exactly the same as for the MC!

            ## from estimatePileupD.py for golden JSON up to run 165970
            #5.42138e+06, 9.26849e+06, 2.10364e+07, 3.47247e+07, 4.546e+07, 4.98579e+07,
            #4.74552e+07, 4.01812e+07, 3.08212e+07, 2.17147e+07, 1.42032e+07, 8.69835e+06,
            #5.02211e+06, 2.74918e+06, 1.43373e+06, 715277., 342608., 158070.,
            #70454.9, 30421.1, 12757.2

            ## from estimatePileupD.py for golden JSON up to run 166861
#             1.00826e+07, 1.9655e+07, 4.58762e+07, 7.63478e+07, 9.9728e+07, 1.0842e+08,
#             1.01847e+08, 8.48512e+07, 6.39051e+07, 4.41459e+07, 2.82916e+07, 1.69742e+07,
#             9.60532e+06, 5.15841e+06, 2.64284e+06, 1.29755e+06, 612859, 279413,
#             123331, 52841.1, 22026.7

            ## from estimatePileupD.py for golden JSON up to run 173244
#             2.66037e+07, 6.20837e+07, 1.28931e+08, 2.00545e+08, 2.5334e+08, 2.73133e+08,
#             2.5988e+08, 2.23527e+08, 1.76897e+08, 1.30515e+08, 9.06582e+07, 5.972e+07,
#             3.75081e+07, 2.2549e+07, 1.30131e+07, 7.2248e+06, 3.86533e+06, 1.99552e+06,
#             995277, 480084, 224189, 101452, 44532.8, 18979.4,
#             7860.96, 3167.1, 1242.31, 474.86, 177.025, 64.4158,
#             22.8974, 7.95686, 2.70506, 0.900305, 0.293541, 0.0938176,
#             0.02941, 0.0090478, 0.00273311, 0.000811054, 0.000236549, 6.78354e-05,

            ## from Poter's pudist_160431to166967_Poter.root for 0.875/fb Vg analysis
            1.10793e+07, 2.31206e+07, 5.42952e+07, 9.04771e+07, 1.18068e+08, 1.28036e+08,
            1.19846e+08, 9.94131e+07, 7.45008e+07, 5.11836e+07, 3.26078e+07, 1.94407e+07,
            1.09285e+07, 5.8289e+06, 2.96543e+06, 1.44558e+06, 677890, 306859,
            134492, 57224.5, 23693.1, 9565.65, 3772.73, 1455.95,
            550.55, 204.24, 74.4102, 26.6473, 9.38663, 3.2542,
            1.11079, 0.373414, 0.123646, 0.0403285, 0.012956, 0.00409919,
            0.0012771, 0.000391712, 0.000118258, 3.51339e-05, 1.02699e-05, 2.9531e-06,
#             8.35198e-07, 2.32299e-07, 6.35345e-08, 1.70863e-08, 4.518e-09, 1.17463e-09,
#             3.00277e-10, 2.3274e-07, 0,


            ## from Poter's pudist_160431to173692_Poter.root for 2.15/fb Vg analysis
#             2.1911e+07, 5.4998e+07, 1.25265e+08, 2.03756e+08, 2.63771e+08, 2.89181e+08,
#             2.79196e+08, 2.43752e+08, 1.96063e+08, 1.47256e+08, 1.04271e+08, 7.00899e+07,
#             4.49454e+07, 2.75913e+07, 1.62566e+07, 9.21086e+06, 5.02652e+06, 2.64559e+06,
#             1.34465e+06, 660766, 314306, 144888, 64802.3, 28153.3,
#             11894.4, 4892.33, 1961.18, 767.018, 292.964, 109.386,
#             39.9615, 14.2963, 5.01243, 1.72359, 0.581655, 0.192754,
#             0.0627596, 0.0200861, 0.00632161, 0.00195716, 0.000596234, 0.000178775,
# #             5.27696e-05, 1.53362e-05, 4.38902e-06, 1.23703e-06, 3.43391e-07, 9.38925e-08,
# #             2.5289e-08, 6.90399e-07, 0,
        )
    )

process.p = cms.Path(
    process.selectionSequence *
    process.tree
)

process.options.wantSummary = True

## Turn off the delta R cut
# process.selectedZMuMuGammas.maxDeltaRNear = 99

if __name__ == "__main__": import user

