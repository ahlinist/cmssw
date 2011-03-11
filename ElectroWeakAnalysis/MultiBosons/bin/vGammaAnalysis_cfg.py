import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FWLitePlots")

castorDir = "/scratch/lgray/vgamma_skims/data/WJets-madgraph-Full"
pathPrefix = "file:" + castorDir + "/"
filelist = os.popen("ls " + castorDir).read().split()[:10]

process.inputs = cms.PSet (
    fileNames = cms.vstring(          "file:/afs/cern.ch/cms/cit/veverka/vgamma/skims/CMSSW_3_6_3/src/ElectroWeakAnalysis/MultiBosons/test/Skimming/MuonPhotonSkim_numEvent10.root")
)

process.inputs.fileNames = [pathPrefix + file for file in filelist]



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
    "ZMuMuGamma",
    #"WMuNuGamma",
    "WENuGamma",
    "ZInvisibleGamma",
])

## Modify the selection for Wgamma FSR
process.SelectorConfig.cutsToIgnore.remove("max DR(g,l_near)")
process.SelectorConfig.cutsToIgnore.append("== 1 Tight Photon")
process.SelectorConfig.maxNearLeptonPhotonDeltaR = 0.5
process.SelectorConfig.minLeptonPhotonDeltaR = -1.
## End of Wgamma FSR changes


## Create the histogram definitions for the FWLite program
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import mmgHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.zGammaSpecialHistos_cff import zGammaSpecialHistos

# process.ZMuMuGamma = cms.PSet(
#     muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
#                           histograms = muonHistos.histograms.copy(),
#                           outputDirectory = cms.string('ZMuMuGamma/Muons'),
#                           eventWeight = cms.double(1.0)
#                           ),
#     photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
#                             histograms = photonHistos.histograms.copy(),
#                             outputDirectory = cms.string('ZMuMuGamma/Photons'),
#                             eventWeight = cms.double(1.0)
#                             ),
#     ZMuMuGammaHistos = cms.PSet(src = cms.InputTag(mmgHistos.src.value()),
#                                 specializedHistograms = zGammaSpecialHistos.copy(),
#                                 histograms = mmgHistos.histograms.copy(),
#                                 outputDirectory = cms.string('ZMuMuGamma'),
#                                 eventWeight = cms.double(1.0)
#                                 )
#
#     )

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

if __name__ == "__main__": import user
#process.inputs.fileNames = ["file:../test/Skimming/VGammaSkim_v2.root"]
