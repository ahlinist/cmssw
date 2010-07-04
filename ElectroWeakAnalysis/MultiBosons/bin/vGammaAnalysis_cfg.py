import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

pathPrefix = "rfio:/castor/cern.ch/user/v/veverka/mc/Spring10/START3X_V26-v1/PAT/ZJets-madgraph/"

filelist = """
VGammaSkim_10_1_IgX.root
VGammaSkim_11_1_5GN.root
VGammaSkim_12_1_tkX.root
VGammaSkim_13_1_iW0.root
VGammaSkim_14_1_ldS.root
VGammaSkim_15_1_BXg.root
VGammaSkim_16_1_her.root
VGammaSkim_17_1_Mu7.root
VGammaSkim_18_1_3QB.root
VGammaSkim_19_1_aXX.root
VGammaSkim_1_1_thG.root
VGammaSkim_20_1_kfK.root
VGammaSkim_21_1_Uyn.root
VGammaSkim_22_1_dRH.root
VGammaSkim_2_1_uRr.root
VGammaSkim_3_1_ARY.root
VGammaSkim_4_1_3qg.root
VGammaSkim_5_1_6vV.root
VGammaSkim_6_1_PM5.root
VGammaSkim_7_1_vKk.root
VGammaSkim_8_1_Lvr.root
VGammaSkim_9_1_v8B.root
""".split()

process.inputs = cms.PSet (
    fileNames = cms.vstring( )
)

process.inputs.fileNames = [pathPrefix + file for file in filelist]

process.outputs = cms.PSet (   
    outputName = cms.string('analysisPlots.root')
)

#process.maxEvents = cms.PSet(output = cms.untracked.int64(5) )

## Define the selector configuration you want to use in FWLite
from ElectroWeakAnalysis.MultiBosons.Selectors.vGammaSelector_cfi import sw_commissioning_selection

process.SelectorConfig = sw_commissioning_selection.copy()

process.SelectorConfig.cutsToIgnore = cms.vstring("ZEEGamma",
                                                  "WENuGamma",
                                                  "ZNuNuGamma")

## Create the histogram definitions for the FWLite program
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import mmgHistos

process.ZMuMuGamma = cms.PSet(
    muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
                          histograms = muonHistos.histograms.copy(),
                          outputDirectory = cms.string('ZMuMuGamma/Muons'),
                          eventWeight = cms.double(1.0)
                          ),
    photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
                            histograms = photonHistos.histograms.copy(),
                            outputDirectory = cms.string('ZMuMuGamma/Photons'),
                            eventWeight = cms.double(1.0)
                            ),
    ZMuMuGammaHistos = cms.PSet(src = cms.InputTag(mmgHistos.src.value()),
                                histograms = mmgHistos.histograms.copy(),
                                outputDirectory = cms.string('ZMuMuGamma'),
                                eventWeight = cms.double(1.0)
                                )
    
    )

process.WMuNuGamma = cms.PSet(
    muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
                          histograms = muonHistos.histograms.copy(),
                          outputDirectory = cms.string('WMuNuGamma/Muons'),
                          eventWeight = cms.double(1.0)
                          ),
    photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
                            histograms = photonHistos.histograms.copy(),
                            outputDirectory = cms.string('WMuNuGamma/Photons'),
                            eventWeight = cms.double(1.0)
                            ),
    WMuNuGammaHistos = cms.PSet(src = cms.InputTag(mmgHistos.src.value()),
                                histograms = mmgHistos.histograms.copy(),
                                outputDirectory = cms.string('WMuNuGamma'),
                                eventWeight = cms.double(1.0)
                                )
    
    )

