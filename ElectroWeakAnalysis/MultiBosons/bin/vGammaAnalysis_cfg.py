import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")

process.inputs = cms.PSet (
    fileNames = cms.vstring('file:/scratch/lgray/patskim_test/VGammaSkimPAT.root'
                            )
)

process.outputs = cms.PSet (
    outputName = cms.string('analysisPlots.root')
)

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

