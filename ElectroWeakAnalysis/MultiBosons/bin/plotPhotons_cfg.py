import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.inputs = cms.PSet (
    fileNames = cms.vstring(
        # Your data goes here:
#         '/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_1.root'
#         'file:zmm_pat.root'
#         'file:/tmp/veverka/tmp.YPPUW29322/CMSSW_3_6_2/src/PhysicsTools/SelectorUtils/bin/wm_pat.root'
  "file:/afs/cern.ch/cms/cit/veverka/vgamma/skims/CMSSW_3_6_3/src/ElectroWeakAnalysis/MultiBosons/test/Skimming/MuonPhotonSkim_numEvent10.root"
        )
)

process.outputs = cms.PSet (
    outputName = cms.string('photonPlots.root')
)

# process.load("ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff")

## Convert cms.EDAnalyzer to cms.PSet
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
process.photonHistos = cms.PSet(
  src = cms.InputTag(photonHistos.src.value() ),
  histograms = photonHistos.histograms.copy() +
    [
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(100),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("e3x3User"),
        description = cms.untracked.string("e3x3 user"),
        plotquantity = cms.untracked.string('userFloat("photonUserData:e3x3")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(100),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("e3x3Photon"),
        description = cms.untracked.string("e3x3 photon"),
        plotquantity = cms.untracked.string('e3x3')
      )
    ],
  eventWeight = cms.double(1.0),
  outputDirectory = cms.string('test')
)


# process.wplusjetsAnalysis.muonSrc = "cleanPatMuons"
# process.wplusjetsAnalysis.electronSrc = "cleanPatElectrons"
# process.wplusjetsAnalysis.jetSrc = "cleanPatJets"
#
# process.wplusjetsAnalysis.minJets = 2

