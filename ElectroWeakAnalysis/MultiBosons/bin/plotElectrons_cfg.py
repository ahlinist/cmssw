import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.inputs = cms.PSet (
    fileNames = cms.vstring(
        # Your data goes here:
#         '/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_1.root'
#         'file:zmm_pat.root'
#         'file:/tmp/veverka/tmp.YPPUW29322/CMSSW_3_6_2/src/PhysicsTools/SelectorUtils/bin/wm_pat.root'
  "file:VGammaSkim_numEvent200.root"
        )
)

process.outputs = cms.PSet (
    outputName = cms.string('electronPlots.root')
)

# process.load("ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff")

## Convert cms.EDAnalyzer to cms.PSet
# from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
process.electronHistos = cms.PSet(
  src = cms.InputTag(electronHistos.src.value() ),
  histograms = cms.VPSet() + #electronHistos.histograms.copy() +
    [
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("swissCross"),
        description = cms.untracked.string("swiss cross"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:swissCross")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(-0.5),
        max = cms.untracked.double(1.5),
        nbins = cms.untracked.int32(2),
        name = cms.untracked.string("isOutOfTime"),
        description = cms.untracked.string("is out of time"),
        plotquantity = cms.untracked.string('userInt("electronUserData:isOutOfTime")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("E1OverE9"),
        description = cms.untracked.string("E1/E9 severity"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:E1OverE9")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("r19"),
        description = cms.untracked.string("eMax / e3x3"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:eMax")/' +
          'userFloat("electronUserData:e3x3")'
          )
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(100),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("e3x3User"),
        description = cms.untracked.string("e3x3 user"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:e3x3")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(100),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("e3x3Electron"),
        description = cms.untracked.string("e3x3 electron"),
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

