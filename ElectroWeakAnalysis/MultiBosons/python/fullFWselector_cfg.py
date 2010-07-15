import FWCore.ParameterSet.Config as cms

## Create the histogram definitions for the FWLite program
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import mmgHistos
## Define the selector configuration you want to use in FWLite
from ElectroWeakAnalysis.MultiBosons.Selectors.vGammaSelector_cfi import sw_commissioning_selection

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

pathPrefix = "file:/scratch/lgray/vgamma_patskims/TTbarJets-madgraph/"

filelist = """
VGammaSkim_10_1_ZVP.root
VGammaSkim_11_1_wx6.root
VGammaSkim_12_1_SWz.root
VGammaSkim_13_1_hhz.root
VGammaSkim_15_1_35M.root
VGammaSkim_16_1_Csq.root
VGammaSkim_17_1_NYr.root
VGammaSkim_18_1_Ubh.root
VGammaSkim_19_1_XV9.root
VGammaSkim_1_1_gjy.root
VGammaSkim_20_1_pwS.root
VGammaSkim_21_1_5sx.root
VGammaSkim_22_1_aTX.root
VGammaSkim_23_1_QDA.root
VGammaSkim_24_1_TFD.root
VGammaSkim_25_1_72O.root
VGammaSkim_26_1_OKs.root
VGammaSkim_27_1_6Yr.root
VGammaSkim_28_1_rsk.root
VGammaSkim_29_1_gNK.root
VGammaSkim_2_1_oRG.root
VGammaSkim_30_1_E8w.root
VGammaSkim_31_1_GdW.root
VGammaSkim_3_1_Tc8.root
VGammaSkim_4_1_3eb.root
VGammaSkim_5_1_IQe.root
VGammaSkim_6_1_0i9.root
VGammaSkim_7_1_y81.root
VGammaSkim_8_1_C1K.root
VGammaSkim_9_1_TfC.root
""".split()

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1) )

process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring( )
                             )

process.source.fileNames = [pathPrefix + file for file in filelist]

process.selector = cms.EDAnalyzer("FullFWSelector",
                                  outputs = cms.PSet(outputName = cms.string('analysisPlots_ttbar.root')),
                                  SelectorConfig = sw_commissioning_selection.copy(),
                                  ZMuMuGamma = cms.PSet(muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
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
                                  )

process.selector.SelectorConfig.cutsToIgnore = cms.vstring("ZEEGamma",
                                                           "WMuNuGamma",
                                                           "WENuGamma",
                                                           "ZInvisibleGamma")


process.p = cms.Path(process.selector)

process.schedule = cms.Schedule(process.p)

