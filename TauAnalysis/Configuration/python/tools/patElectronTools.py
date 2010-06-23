import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import *
#from PhysicsTools.PatAlgos.mcMatchLayer0.electronMatch_cfi import *
#from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

#from TauAnalysis.RecoTools.patElectronConfig_cfi import *

def producePatElectronIsolation(process):
	process.makePatElectrons = cms.Sequence(
		patElectronIsolation +
		process.electronMatch +
		process.patElectrons
	)

def producePatElectronIsolationAndSimpleId(process):
	process.load("TauAnalysis.RecoTools.simpleEleIdSequence_cff")
	process.patElectrons.electronIDSources.simpleEleId95relIso = cms.InputTag("simpleEleId95relIso")
	process.patElectrons.electronIDSources.simpleEleId90relIso = cms.InputTag("simpleEleId90relIso")
	process.patElectrons.electronIDSources.simpleEleId85relIso = cms.InputTag("simpleEleId85relIso")
	process.patElectrons.electronIDSources.simpleEleId80relIso = cms.InputTag("simpleEleId80relIso")
	process.patElectrons.electronIDSources.simpleEleId70relIso = cms.InputTag("simpleEleId70relIso")
	process.patElectrons.electronIDSources.simpleEleId60relIso = cms.InputTag("simpleEleId60relIso")
	process.patElectrons.electronIDSources.simpleEleId95cIso = cms.InputTag("simpleEleId95cIso")
	process.patElectrons.electronIDSources.simpleEleId90cIso = cms.InputTag("simpleEleId90cIso")
	process.patElectrons.electronIDSources.simpleEleId85cIso = cms.InputTag("simpleEleId85cIso")
	process.patElectrons.electronIDSources.simpleEleId80cIso = cms.InputTag("simpleEleId80cIso")
	process.patElectrons.electronIDSources.simpleEleId70cIso = cms.InputTag("simpleEleId70cIso")
	process.patElectrons.electronIDSources.simpleEleId60cIso = cms.InputTag("simpleEleId60cIso")
	process.makePatElectrons = cms.Sequence(
		process.simpleEleIdSequence +
		patElectronIsolation +
		process.electronMatch +
		process.patElectrons
	)
