import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.inputs = cms.PSet (
    fileNames = cms.vstring(
        # Your data goes here:
#         '/Volumes/MyBook/Data/TTbar/shyft_35x_v3/ljmet_1.root'
#         'file:zmm_pat.root'
#         'file:/tmp/veverka/tmp.YPPUW29322/CMSSW_3_6_2/src/PhysicsTools/SelectorUtils/bin/wm_pat.root'
#           "file:/afs/cern.ch/cms/cit/veverka/vgamma/skims/CMSSW_3_6_3/src/ElectroWeakAnalysis/MultiBosons/test/Skimming/MuonPhotonSkim_numEvent10.root"
          "file:VGammaPAT_MuonPhotonSkim_testWinter10.root"

        )
)

process.outputs = cms.PSet (
    outputName = cms.string('muonPlots.root')
)

# process.load("ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff")

## Convert cms.EDAnalyzer to cms.PSet
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
process.muonHistos = cms.PSet(
  src = cms.InputTag("cleanPatMuonsTriggerMatch"),
  histograms = muonHistos.histograms.copy(),
  eventWeight = cms.double(1.0),
  outputDirectory = cms.string('test')
)

# process.wplusjetsAnalysis.muonSrc = "cleanPatMuons"
# process.wplusjetsAnalysis.electronSrc = "cleanPatElectrons"
# process.wplusjetsAnalysis.jetSrc = "cleanPatJets"
#
# process.wplusjetsAnalysis.minJets = 2

