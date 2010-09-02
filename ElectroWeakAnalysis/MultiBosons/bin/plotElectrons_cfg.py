import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.inputs = cms.PSet (
    fileNames = cms.vstring(
        # Your data goes here:
#         '/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_1.root'
#         'file:zmm_pat.root'
#         'file:/tmp/veverka/tmp.YPPUW29322/CMSSW_3_6_2/src/PhysicsTools/SelectorUtils/bin/wm_pat.root'
  "file:VGammaSkim_testMC_numEvent100.root"
        )
)

process.outputs = cms.PSet (
    outputName = cms.string('electronPlots.root')
)

# process.load("ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff")

## Convert cms.EDAnalyzer to cms.PSet
# from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
process.electronHistos = cms.PSet(
  src = cms.InputTag("cleanPatElectrons"),
  histograms = cms.VPSet() + #electronHistos.histograms.copy() +
    [
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(-5),
        max = cms.untracked.double(5),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("seedTime"),
        description = cms.untracked.string("Electrons:seed time (ns):events / bin"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:seedTime")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("seedOutOfTimeChi2"),
        description = cms.untracked.string("Electrons:seed out of time #chi^{2}:events / bin"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:seedSwissCross")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("seedChi2"),
        description = cms.untracked.string("Electrons:seed #chi^{2}:events / bin"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:seedSwissCross")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("seedSwissCross"),
        description = cms.untracked.string("Electrons:seed swiss cross 1-S4/S1, events / bin"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:seedSwissCross")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(-0.5),
        max = cms.untracked.double(20.5),
        nbins = cms.untracked.int32(21),
        name = cms.untracked.string("seedRecoFlag"),
        description = cms.untracked.string("Electrons:seed reco flag:events per flag"),
        plotquantity = cms.untracked.string('userInt("electronUserData:seedRecoFlag")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(-0.5),
        max = cms.untracked.double(20.5),
        nbins = cms.untracked.int32(21),
        name = cms.untracked.string("seedSeverityLevel"),
        description = cms.untracked.string("Electrons:Seed severity level:events"),
        plotquantity = cms.untracked.string('userInt("electronUserData:seedSeverityLevel")')
      ),
      cms.PSet(
        itemsToPlot = cms.untracked.int32(-1),
        min = cms.untracked.double(0),
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("seedE1OverE9"),
        description = cms.untracked.string("Electrons:Seed E1/E9 from severity level:events"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:seedE1OverE9")')
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
        max = cms.untracked.double(1),
        nbins = cms.untracked.int32(100),
        name = cms.untracked.string("covIetaIeta"),
        description = cms.untracked.string("#sigma_{i#eta i#eta}"),
        plotquantity = cms.untracked.string('userFloat("electronUserData:covIEtaIEta")')
      ),
    ],
  eventWeight = cms.double(1.0),
  outputDirectory = cms.string('test')
)


# process.wplusjetsAnalysis.muonSrc = "cleanPatMuons"
# process.wplusjetsAnalysis.electronSrc = "cleanPatElectrons"
# process.wplusjetsAnalysis.jetSrc = "cleanPatJets"
#
# process.wplusjetsAnalysis.minJets = 2

