import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

pathPrefix = "/scratch/lgray/vgamma_skims/VGammaSkim_v2/WJets-madgraph/"

filelist = """
VGammaSkim_v2_100_1_LZo.root
VGammaSkim_v2_10_1_zQR.root
VGammaSkim_v2_11_1_G6B.root
VGammaSkim_v2_12_1_Fk2.root
VGammaSkim_v2_13_1_Bbc.root
VGammaSkim_v2_14_1_RHP.root
VGammaSkim_v2_15_1_BoS.root
VGammaSkim_v2_16_1_qeX.root
VGammaSkim_v2_17_1_AVr.root
VGammaSkim_v2_18_1_iqk.root
VGammaSkim_v2_19_1_aoR.root
VGammaSkim_v2_1_1_AJ7.root
VGammaSkim_v2_20_1_lu8.root
VGammaSkim_v2_21_1_OBS.root
VGammaSkim_v2_22_1_DDb.root
VGammaSkim_v2_23_1_LgK.root
VGammaSkim_v2_24_1_FRg.root
VGammaSkim_v2_25_1_p5t.root
VGammaSkim_v2_26_1_aH0.root
VGammaSkim_v2_27_1_qVO.root
VGammaSkim_v2_28_1_J4g.root
VGammaSkim_v2_29_1_VyD.root
VGammaSkim_v2_2_1_iZB.root
VGammaSkim_v2_30_1_3Cd.root
VGammaSkim_v2_31_1_0C6.root
VGammaSkim_v2_32_1_FhG.root
VGammaSkim_v2_33_1_JHo.root
VGammaSkim_v2_34_1_PZB.root
VGammaSkim_v2_35_1_j7U.root
VGammaSkim_v2_36_1_XUV.root
VGammaSkim_v2_37_1_J7T.root
VGammaSkim_v2_38_1_8rg.root
VGammaSkim_v2_39_1_wT2.root
VGammaSkim_v2_3_1_UST.root
VGammaSkim_v2_40_1_8oX.root
VGammaSkim_v2_41_1_jO4.root
VGammaSkim_v2_42_1_MfI.root
VGammaSkim_v2_43_1_uiN.root
VGammaSkim_v2_44_1_RiZ.root
VGammaSkim_v2_45_1_UZ7.root
VGammaSkim_v2_46_1_Fuu.root
VGammaSkim_v2_47_1_0Hb.root
VGammaSkim_v2_48_1_aB9.root
VGammaSkim_v2_49_1_x4d.root
VGammaSkim_v2_4_1_52t.root
VGammaSkim_v2_50_1_ttT.root
VGammaSkim_v2_51_1_Bpj.root
VGammaSkim_v2_52_1_oTQ.root
VGammaSkim_v2_53_1_y0i.root
VGammaSkim_v2_54_1_OH6.root
VGammaSkim_v2_55_1_pyz.root
VGammaSkim_v2_56_1_JTK.root
VGammaSkim_v2_57_1_lvV.root
VGammaSkim_v2_58_1_Ojq.root
VGammaSkim_v2_59_1_Szg.root
VGammaSkim_v2_5_1_CEe.root
VGammaSkim_v2_60_1_a7O.root
VGammaSkim_v2_61_1_Pgw.root
VGammaSkim_v2_62_1_AX7.root
VGammaSkim_v2_63_1_cyS.root
VGammaSkim_v2_64_1_NP3.root
VGammaSkim_v2_65_1_kuQ.root
VGammaSkim_v2_66_1_lH0.root
VGammaSkim_v2_67_1_8o3.root
VGammaSkim_v2_68_1_P69.root
VGammaSkim_v2_69_1_LqO.root
VGammaSkim_v2_6_1_0fz.root
VGammaSkim_v2_70_1_tbE.root
VGammaSkim_v2_71_1_x3s.root
VGammaSkim_v2_72_1_lOi.root
VGammaSkim_v2_73_1_FRG.root
VGammaSkim_v2_74_1_Otj.root
VGammaSkim_v2_75_1_7Dp.root
VGammaSkim_v2_76_1_kdf.root
VGammaSkim_v2_77_1_PWh.root
VGammaSkim_v2_78_1_CfN.root
VGammaSkim_v2_79_1_hyL.root
VGammaSkim_v2_7_1_Iaa.root
VGammaSkim_v2_80_1_yEa.root
VGammaSkim_v2_81_1_3p6.root
VGammaSkim_v2_82_1_NEO.root
VGammaSkim_v2_83_1_S9b.root
VGammaSkim_v2_84_1_xJB.root
VGammaSkim_v2_85_1_Qrg.root
VGammaSkim_v2_86_1_6d3.root
VGammaSkim_v2_87_1_cZI.root
VGammaSkim_v2_88_1_Kr1.root
VGammaSkim_v2_89_1_s1R.root
VGammaSkim_v2_8_1_Lfg.root
VGammaSkim_v2_90_1_kQP.root
VGammaSkim_v2_91_1_tR2.root
VGammaSkim_v2_92_1_NhO.root
VGammaSkim_v2_93_1_iFe.root
VGammaSkim_v2_94_1_MfV.root
VGammaSkim_v2_95_1_PGu.root
VGammaSkim_v2_96_1_5Qd.root
VGammaSkim_v2_97_1_KtL.root
VGammaSkim_v2_98_1_kTX.root
VGammaSkim_v2_99_1_DQn.root
VGammaSkim_v2_9_1_Ypt.root
""".split()

process.inputs = cms.PSet (
    fileNames = cms.vstring( )
)

process.inputs.fileNames = [pathPrefix + file for file in filelist]



process.outputs = cms.PSet (   
    outputName = cms.string('analysisPlots_ttbar.root')
)

process.maxEvents = cms.PSet(input = cms.untracked.int64(-1) )

process.heartbeat = cms.PSet(
    updateEvery = cms.double(1.0) # update the heartbeat every X percent
    )
                             

## Define the selector configuration you want to use in FWLite
from ElectroWeakAnalysis.MultiBosons.Selectors.vGammaSelector_cfi import sw_commissioning_selection

process.SelectorConfig = sw_commissioning_selection.copy()

process.SelectorConfig.cutsToIgnore = cms.vstring("ZEEGamma",
                                                  "WMuNuGamma",
                                                  "WENuGamma",
                                                  "ZInvisibleGamma")

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

#process.WMuNuGamma = cms.PSet(
#    muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
#                          histograms = muonHistos.histograms.copy(),
#                          outputDirectory = cms.string('WMuNuGamma/Muons'),
#                          eventWeight = cms.double(1.0)
#                          ),
#    photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
#                            histograms = photonHistos.histograms.copy(),
#                            outputDirectory = cms.string('WMuNuGamma/Photons'),
#                            eventWeight = cms.double(1.0)
#                            ),
#    WMuNuGammaHistos = cms.PSet(src = cms.InputTag(mmgHistos.src.value()),
#                                histograms = mmgHistos.histograms.copy(),
#                                outputDirectory = cms.string('WMuNuGamma'),
#                                eventWeight = cms.double(1.0)
#                                )
#    
#    )

