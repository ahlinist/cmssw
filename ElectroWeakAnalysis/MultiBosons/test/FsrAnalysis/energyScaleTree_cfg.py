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

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.defaultPhotons_cfi")

process.selectedPhotons = cms.EDFilter("VGammaPhotonFilter",
    filterParams = photonSelection,
    src = cms.InputTag("defaultPhotons"),
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

process.combinatoricFilter = cms.EDFilter("CandViewCountRangeFilter",
    src = cms.InputTag("selectedZMuMuGammas"),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1),
    )


process.selectionSequence = cms.Sequence(
    process.selectedMuons *
    process.goodDiMuons *
    process.selectedDiMuons *
    process.defaultPhotons *
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

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.energyScaleTree_cfi")

## Pileup
if options.isMC:
    process.tree.pileupInfoSrc = cms.untracked.InputTag("addPileupInfo")
    process.tree.lumiReWeighting = cms.untracked.PSet(
        mcDistribution   = cms.vdouble(
            ## from the gamma+jet sample (no filter)
            ## 21 numbers
            257141., 295755., 263008., 286909., 282291., 281067.,
            295777., 297075., 250569., 299795., 256528., 248686.,
            203484., 137833., 117686., 76877., 62815., 35462.,
            8381., 10012., 4233.
        ),
        dataDistribution = cms.vdouble(
            ## the length has to be exactly the same as for the MC!
            ## from estimatePileupD.py for golden JSON up to run 165970
            5.42138e+06, 9.26849e+06, 2.10364e+07, 3.47247e+07, 4.546e+07, 4.98579e+07,
            4.74552e+07, 4.01812e+07, 3.08212e+07, 2.17147e+07, 1.42032e+07, 8.69835e+06,
            5.02211e+06, 2.74918e+06, 1.43373e+06, 715277., 342608., 158070.,
            70454.9, 30421.1, 12757.2
            ## from estimatePileupD.py for golden JSON up to run 165542
  #           4.49103e+06, 7.50711e+06, 1.7013e+07, 2.77526e+07, 3.56721e+07, 3.82648e+07,
  #           3.55386e+07, 2.93206e+07, 2.18974e+07, 1.50169e+07, 9.56312e+06, 5.70622e+06,
  #           3.21393e+06, 1.71936e+06, 878374, 430566, 203380, 92934.5,
  #           41228.6, 17815.2, 7520.35,# 3109.37, 1262.01, 503.739,
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
    process.tree
)

process.options.wantSummary = True

if __name__ == "__main__": import user

