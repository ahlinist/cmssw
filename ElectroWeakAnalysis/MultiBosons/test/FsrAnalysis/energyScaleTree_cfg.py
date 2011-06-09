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

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.PmvTreeMaker_cfi")

process.pmvTree.isMC = options.isMC

## Pileup
if options.isMC:
    process.pmvTree.pileupInfoSrc = cms.untracked.InputTag("addPileupInfo")
    process.pmvTree.lumiReWeighting = cms.untracked.PSet(
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
        )
    )

process.p = cms.Path(
    process.selectionSequence *
    process.pmvTree
)

process.options.wantSummary = True

if __name__ == "__main__": import user

