import FWCore.ParameterSet.Config as cms
import copy

from RecoJets.Configuration.RecoJets_cff import *

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

from PhysicsTools.PatAlgos.tools.coreTools import *

from TauAnalysis.RecoTools.postPatProduction_cff import produceGenObjects

from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

def switchToData(process):

	# remove MC matching from standard PAT sequences
	removeMCMatching(process, ["All"])

	# remove modules from pre-PAT production running on genParticles
	process.producePrePat.remove(ak5CaloJets)
	process.producePrePat.remove(ak5CaloJetsPUCorr)

	process.producePrePat.remove(genParticlesForJets)
	process.producePrePat.remove(ak5GenJets)

	# remove modules from post-PAT production running on genParticles
	process.producePostPat.remove(produceGenObjects)

	# remove modules from producePatTupleZtoElecTauSpecific sequence which run on genParticles
	if hasattr(process,'allDiTauPairs'): 
		process.allDiTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allElecTauPairs"):
		process.allElecTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allElecTauPairsLooseElectronIsolation"):
		process.allElecTauPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allElecMuPairs"):
		process.allElecMuPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allElecMuPairsLooseElectronIsolation"): 
		process.allElecMuPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allMuTauPairs"):
		process.allMuTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process,"allMuTauPairsLooseMuonIsolation"): 
		process.allMuTauPairsLooseMuonIsolation.srcGenParticles = cms.InputTag('')

	# remove modules from the Z->e+tau-jet analysis sequence which run on GEN collections
	if hasattr(process,"analyzeZtoElecTauEvents"):
		process.analyzeZtoElecTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
		removeAnalyzer(process.analyzeZtoElecTauEvents.analysisSequence,"genPhaseSpaceEventInfoHistManager")
		process.analyzeZtoElecTauEvents.eventDumps[0].doGenInfo = cms.bool(False)
		process.electronHistManager.genParticleSource = cms.InputTag('')
		process.tauHistManager.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')
	

