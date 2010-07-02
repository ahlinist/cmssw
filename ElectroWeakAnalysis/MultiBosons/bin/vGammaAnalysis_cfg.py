import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.inputs = cms.PSet (
    fileNames = cms.vstring('file:/tmp/veverka/tmp.YPPUW29322/CMSSW_3_6_2/src/PhysicsTools/SelectorUtils/bin/wm_pat.root'
                            )
)

process.outputs = cms.PSet (
    outputName = cms.string('analysisPlots.root')
)

## Define the selector configuration you want to use in FWLite
from ElectroWeakAnalysis.MultiBosons.Selectors.vGammaSelector_cfi import sw_commissioning_selection

process.SelectorConfig = sw_commissioning_selection.copy()

## Create the histogram definitions for the FWLite program
from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import muonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import photonHistos
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import mmgHistos

process.ZMuMuGamma = cms.PSet(
    muonHistos = cms.PSet(src = cms.InputTag(muonHistos.src.value() ),
                          histograms = muonHistos.histograms.copy(),
                          outputDirectory = cms.string('ZMuMuGamma/Muons')
                          ),
    photonHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
                            histograms = photonHistos.histograms.copy(),
                            outputDirectory = cms.string('ZMuMuGamma/Photons')
                            ),
    ZMuMuGammaHistos = cms.PSet(src = cms.InputTag(photonHistos.src.value()),
                                histograms = photonHistos.histograms.copy(),
                                outputDirectory = cms.string('ZMuMuGamma')
                                )
    
    )

