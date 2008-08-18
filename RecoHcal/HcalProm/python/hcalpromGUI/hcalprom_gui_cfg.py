import FWCore.ParameterSet.Config as cms

from FastSimulation.Calorimetry.HcalResponse_cfi import *
from FastSimulation.Calorimetry.HSParameters_cfi import *

process = cms.Process("Demo")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("MagneticField.Engine.volumeBasedMagneticField_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff")

###<<<<<<<<<< Don't remove this line -- it's used by the gui when updating/replacing file names!
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:///uscms_data/d1/cmstb04/GlobalCosmic/data/GlobalMar08_38429_Pass4.root')
)
###>>>>>>>>>>>  Don't remove this line!


process.MessageLogger = cms.Service("MessageLogger")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('prompt_out.root')
)


process.prom = cms.EDFilter("HcalProm",
    printRecHitHTML = cms.untracked.bool(True),
    printJetMetHTML = cms.untracked.bool(True),
    printCaloTowerHTML = cms.untracked.bool(False),
    HCALResponseBlock = cms.PSet(
        HCALResponse = cms.PSet(
            eResponseCoefficient = cms.double(1.0),
            HadronEndcapResolution_Noise = cms.double(0.0),
            HadronForwardResolution_Stochastic = cms.double(1.82),
            ElectronForwardResolution_Constant = cms.double(0.05),
            HadronBarrelResolution_Noise = cms.double(0.0),
            HadronForwardResolution_Constant = cms.double(0.09),
            HadronBarrelResolution_Stochastic = cms.double(1.22),
            HadronEndcapResolution_Constant = cms.double(0.05),
            eResponseExponent = cms.double(1.0),
            HadronForwardResolution_Noise = cms.double(0.0),
            HadronBarrelResolution_Constant = cms.double(0.05),
            HadronEndcapResolution_Stochastic = cms.double(1.3),
            eResponseCorrection = cms.double(1.0),
            eResponseScaleHB = cms.double(3.0),
            eResponseScaleHF = cms.double(3.0),
            eResponseScaleHE = cms.double(3.0),
            ElectronForwardResolution_Stochastic = cms.double(1.38),
            eResponsePlateauHE = cms.double(0.95),
            eResponsePlateauHF = cms.double(0.95),
            eResponsePlateauHB = cms.double(0.95),
            energyBias = cms.double(0.0),
            ElectronForwardResolution_Noise = cms.double(0.0)
        )
    ),
    baseHtmlDir = cms.untracked.string('.'),
    printPromptHTML = cms.untracked.bool(True),
    MuonAcceptance = cms.untracked.double(100.0),
#    HSParameterBlock = cms.PSet(
#        HSParameters = cms.PSet(
#            nTRsteps = cms.int32(40),
#            lossesOpt = cms.int32(0),
#            depthStep = cms.double(0.5),
#            balanceEH = cms.double(0.9),
#            eSpotSize = cms.double(0.2),
#            hcalDepthFactor = cms.double(1.1),
#            transRparam = cms.double(1.0),
#            nDepthSteps = cms.int32(10),
#            maxTRfactor = cms.double(4.0),
#            criticalHDEnergy = cms.double(3.0)
#        )
#    ),
    IPR = cms.untracked.double(20.0),
    MuonSelectionAlgoType = cms.untracked.int32(1),
    printDigiHTML = cms.untracked.bool(False),
    printHPDNoiseHTML = cms.untracked.bool(False),
    IPZ = cms.untracked.double(20.0),
    printMuonHTML = cms.untracked.bool(True),
    FamosCalorimetryBlock = cms.PSet(
        Calorimetry = cms.PSet(
            HSParameterBlock,
            HCALResponseBlock,
            ECAL = cms.PSet(
                CoreIntervals = cms.vdouble(100.0, 0.1),
                RTFactor = cms.double(1.0),
                RCFactor = cms.double(1.0),
                TailIntervals = cms.vdouble(1.0, 0.1, 100.0, 1.0),
                FrontLeakageProbability = cms.double(1.0),
                GridSize = cms.int32(7),
                RadiusFactor = cms.double(1.096),
                Debug = cms.untracked.bool(False),
                SpotFraction = cms.double(-1.0),
                GapLossProbability = cms.double(0.9)
            ),
            CalorimeterProperties = cms.PSet(
                PreshowerLayer2_thickness = cms.double(0.38),
                ECALEndcap_LightCollection = cms.double(0.023),
                PreshowerLayer1_thickness = cms.double(1.6),
                PreshowerLayer1_mipsPerGeV = cms.double(35.7),
                PreshowerLayer2_mipsPerGeV = cms.double(59.5),
                ECALBarrel_LightCollection = cms.double(0.03),
                HCAL_Sampling = cms.double(0.0035),
                HCAL_PiOverE = cms.double(0.2)
            ),
            UnfoldedMode = cms.untracked.bool(False),
            HCAL = cms.PSet(
                SimMethod = cms.int32(0),
                GridSize = cms.int32(7),
                SimOption = cms.int32(2)
            )
        )
    )
)

process.l1filter = cms.EDFilter("L1GTFilter",
    trigger = cms.string('L1_SingleMu3')
)

process.p = cms.Path(process.l1filter*process.prom)

