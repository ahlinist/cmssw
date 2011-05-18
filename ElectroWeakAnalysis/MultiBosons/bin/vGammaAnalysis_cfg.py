import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FWLitePlots")

castorDir = "/raid2/veverka/WJets_7TeV-madgraph-tauola/MuonPhotonVGammaSkim/28b662cc3f390786caf43da2275d2425/"
pathPrefix = "file:" + castorDir + "/"
filelist = os.popen("ls " + castorDir).read().split()[:10]

process.inputs = cms.PSet (
    fileNames = cms.vstring(          "file:/scratch/lgray/CMSSW_4_1_4/src/ElectroWeakAnalysis/MultiBosons/test/Skimming/VGammaPAT_MuonPhotonSkim_testMC414_100outEvents.root")
)

#process.inputs.fileNames = [pathPrefix + file for file in filelist]

process.inputs.fileNames = """file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_1_1_jGh.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_2_1_p2n.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_3_1_Hcz.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_4_1_jHi.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_5_1_EJd.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_6_1_R31.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_7_1_seT.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_8_1_0rV.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_9_1_Y9D.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_10_1_XUJ.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_11_1_Dbl.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_12_1_CzG.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_13_1_v94.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_14_1_GQb.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_15_1_taJ.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_16_1_PbT.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_17_1_IxC.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_18_1_kzh.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_19_1_bhf.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_20_1_1qu.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_21_1_PJd.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_22_1_g5x.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_23_1_D5M.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_24_1_3K4.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_25_1_9YP.root
    file:/mnt/tier2/store/user/veverka/Zgamma/ElectronPhotonVGammaSkim/a81f4ee10ef188fe6a5700e98d7864f7/VGammaPAT_ElectronPhotonSkim_26_1_iPq.root
    """.split()


process.outputs = cms.PSet (
    outputName = cms.string('VGammaHistos.root')
)

process.maxEvents = cms.PSet(input = cms.untracked.int64(-1) )

process.heartbeat = cms.PSet(
    updateEvery = cms.double(1.0) # update the heartbeat every X percent
    )


## Define the selector configuration you want to use in FWLite
from ElectroWeakAnalysis.MultiBosons.Selectors.vGammaSelector_cfi import sw_commissioning_selection

process.SelectorConfig = sw_commissioning_selection.copy()

process.SelectorConfig.cutsToIgnore.extend([
    "ZEEGamma",
    #"ZMuMuGamma",
    "WMuNuGamma",
    "WENuGamma",
    "ZInvisibleGamma",
])

## Modify the selection for Wgamma FSR
process.SelectorConfig.cutsToIgnore.remove("max DR(g,l_near)")
process.SelectorConfig.cutsToIgnore.append("== 1 Tight Photon")
process.SelectorConfig.maxDeltaRPhotonNearLepton = 0.5
process.SelectorConfig.minLeptonPhotonDeltaR = -1.
## End of Wgamma FSR changes

process.SelectorConfig.cutsToIgnore = cms.vstring(#"ZEEGamma",
                                                  "ZMuMuGamma",
                                                  "WMuNuGamma",
                                                  "WENuGamma",
                                                  "ZInvisibleGamma")

## Create the histogram definitions for the FWLite program
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import mmgHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.zGammaSpecialHistos_cff import zGammaSpecialHistos

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
                                specializedHistograms = zGammaSpecialHistos.copy(),
                                histograms = mmgHistos.histograms.copy(),
                                outputDirectory = cms.string('ZMuMuGamma'),
                                eventWeight = cms.double(1.0)
                                )

    )

#process.WMuNuGamma = cms.PSet(
#   muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
#                         histograms = muonHistos.histograms.copy(),
#                         outputDirectory = cms.string('WMuNuGamma/Muons'),
#                         eventWeight = cms.double(1.0)
#                         ),
#   photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
#                           histograms = photonHistos.histograms.copy(),
#                           outputDirectory = cms.string('WMuNuGamma/Photons'),
#                           eventWeight = cms.double(1.0)
#                           ),
#   WMuNuGammaHistos = cms.PSet(src = cms.InputTag(mmgHistos.src.value()),
#                               histograms = mmgHistos.histograms.copy(),
#                               outputDirectory = cms.string('WMuNuGamma'),
#                               eventWeight = cms.double(1.0)
#                               )
#
#   )

if __name__ == "__main__": import user
#process.inputs.fileNames = ["file:../test/Skimming/VGammaSkim_v2.root"]
