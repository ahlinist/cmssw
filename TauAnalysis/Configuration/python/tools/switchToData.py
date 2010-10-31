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

	# add data-quality cuts which work on "real" data only
	if hasattr(process, "dataQualityFilters"):
		process.dataQualityFilters._seq = process.dataQualityFilters._seq * process.hltPhysicsDeclared
		process.dataQualityFilters._seq = process.dataQualityFilters._seq * process.dcsstatus

	if hasattr(process, "patPFMETs"):
		process.patPFMETs.addGenMET = cms.bool(False)

	# remove modules from pre-PAT production running on genParticles
	process.producePrePat.remove(genParticlesForJets)
	process.producePrePat.remove(ak5GenJets)

	# remove modules from post-PAT production running on genParticles
	process.producePostPat.remove(produceGenObjects)

	if hasattr(process, "allDiTauPairs"): 
		process.allDiTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allElecTauPairs"):
		process.allElecTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allElecTauPairsLooseElectronIsolation"):
		process.allElecTauPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allElecMuPairs"):
		process.allElecMuPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allElecMuPairsLooseElectronIsolation"): 
		process.allElecMuPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allMuTauPairs"):
		process.allMuTauPairs.srcGenParticles = cms.InputTag('')
	if hasattr(process, "allMuTauPairsLooseMuonIsolation"): 
		process.allMuTauPairsLooseMuonIsolation.srcGenParticles = cms.InputTag('')
	diTauPairCollectionNames = [
			"allDiTauPairs",
			"selectedDiTauPairs1stTauEta21Individual", "selectedDiTauPairs1stTauEta21Cumulative",
			"selectedDiTauPairs1stTauEta21Individual", "selectedDiTauPairs1stTauEta21Cumulative",
			"selectedDiTauPairs1stTauPt20Individual", "selectedDiTauPairs1stTauPt20Cumulative",
			"selectedDiTauPairs1stTauLeadTrkIndividual", "selectedDiTauPairs1stTauLeadTrkCumulative",
			"selectedDiTauPairs1stTauLeadTrkPtIndividual", "selectedDiTauPairs1stTauLeadTrkPtCumulative",
			"selectedDiTauPairs1stTauTaNCdiscrIndividual", "selectedDiTauPairs1stTauTaNCdiscrCumulative",
			"selectedDiTauPairs1stTauTrkIsoIndividual", "selectedDiTauPairs1stTauTrkIsoCumulative",
			"selectedDiTauPairs1stTauEcalIsoIndividual", "selectedDiTauPairs1stTauEcalIsoCumulative",
			"selectedDiTauPairs1stTauProngIndividual", "selectedDiTauPairs1stTauProngCumulative",
			"selectedDiTauPairs1stTauChargeIndividual", "selectedDiTauPairs1stTauChargeCumulative",
			"selectedDiTauPairs1stTauMuonVetoIndividual", "selectedDiTauPairs1stTauMuonVetoCumulative",
			"selectedDiTauPairs1stTauElectronVetoIndividual", "selectedDiTauPairs1stTauElectronVetoCumulative",
			"selectedDiTauPairs2ndTauEta21Individual", "selectedDiTauPairs2ndTauEta21Cumulative",
			"selectedDiTauPairs2ndTauPt20Individual", "selectedDiTauPairs2ndTauPt20Cumulative",
			"selectedDiTauPairs2ndTauLeadTrkIndividual", "selectedDiTauPairs2ndTauLeadTrkCumulative",
			"selectedDiTauPairs2ndTauLeadTrkPtIndividual", "selectedDiTauPairs2ndTauLeadTrkPtCumulative",
			"selectedDiTauPairs2ndTauLeadTrkPtLooseIndividual", "selectedDiTauPairs2ndTauLeadTrkPtLooseCumulative",
			"selectedDiTauPairs2ndTauTaNCdiscrIndividual", "selectedDiTauPairs2ndTauTaNCdiscrCumulative",
			"selectedDiTauPairs2ndTauTaNCdiscrLooseIndividual", "selectedDiTauPairs2ndTauTaNCdiscrLooseCumulative",
			"selectedDiTauPairs2ndTauTrkIsoIndividual", "selectedDiTauPairs2ndTauTrkIsoCumulative",
			"selectedDiTauPairs2ndTauTrkIsoLooseIndividual", "selectedDiTauPairs2ndTauTrkIsoLooseCumulative",
			"selectedDiTauPairs2ndTauEcalIsoIndividual", "selectedDiTauPairs2ndTauEcalIsoCumulative",
			"selectedDiTauPairs2ndTauEcalIsoLooseIndividual", "selectedDiTauPairs2ndTauEcalIsoLooseCumulative",
			"selectedDiTauPairs2ndTauProngIndividual", "selectedDiTauPairs2ndTauProngCumulative",
			"selectedDiTauPairs2ndTauProngLooseIndividual", "selectedDiTauPairs2ndTauProngLooseCumulative",
			"selectedDiTauPairs2ndTauChargeIndividual", "selectedDiTauPairs2ndTauChargeCumulative",
			"selectedDiTauPairs2ndTauChargeLooseIndividual", "selectedDiTauPairs2ndTauChargeLooseCumulative",
			"selectedDiTauPairs2ndTauMuonVetoIndividual", "selectedDiTauPairs2ndTauMuonVetoCumulative",
			"selectedDiTauPairs2ndTauMuonVetoLooseIndividual", "selectedDiTauPairs2ndTauMuonVetoLooseCumulative",
			"selectedDiTauPairs2ndTauElectronVetoIndividual", "selectedDiTauPairs2ndTauElectronVetoCumulative",
			"selectedDiTauPairs2ndTauElectronVetoLooseIndividual", "selectedDiTauPairs2ndTauElectronVetoLooseCumulative" ]
	for diTauPairCollectionName in diTauPairCollectionNames:
		if hasattr(process, diTauPairCollectionName):
			diTauPairCollection = getattr(process, diTauPairCollectionName)
			diTauPairCollection.srcGenParticles = cms.InputTag('')
	if hasattr(process, "patPFMETs"):
		process.patPFMETs.addGenMET = cms.bool(False)

	# for modules in producePatTupleZtoElecTauSpecific sequence, remove references to genParticles 
	if hasattr(process, "producePatTupleZtoElecTauSpecific"):
		process.producePatTupleZtoElecTauSpecific.remove(process.produceElecTauPairsLooseElectronIsolation)
		process.producePatTupleZtoElecTauSpecific.remove(process.selectElecTauPairsLooseElectronIsolation)
		process.produceElecTauPairZeeHypotheses.remove(process.elecTauPairZeeHypothesesLooseElectronIsolation)
		process.produceElecTauPairZeeHypotheses.remove(process.selectedElecTauPairZeeHypothesesLooseElectronIsolation)
		process.produceElecTauPairZeeHypotheses.remove(process.elecTauPairVisMassHypothesesLooseElectronIsolation)
		process.elecTauPairsAfterTauPt.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterElectronTrkIso.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterElectronEcalIso.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterElectronConversionVeto.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterElectronTrkIP.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauLeadTrkFind.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauLeadTrkPt.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauTaNC.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauTrkIso.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauEcalIso.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauProng.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauCharge.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauElectronVeto.srcGenParticles = cms.InputTag('')
		process.elecTauPairsAfterTauEcalCrackVeto.srcGenParticles = cms.InputTag('')

	# remove modules from the Z->e+tau-jet analysis sequence which run on GEN collections
	if hasattr(process, "analyzeZtoElecTauEvents"):
		process.analyzeZtoElecTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
		removeAnalyzer(process.analyzeZtoElecTauEvents.analysisSequence, "genPhaseSpaceEventInfoHistManager")
		process.analyzeZtoElecTauEvents.eventDumps[0].doGenInfo = cms.bool(False)
		process.analyzeZtoElecTauEvents.eventDumps[0].genParticleSource = cms.InputTag('')
		process.electronHistManager.genParticleSource = cms.InputTag('')
		process.tauHistManager.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')
		process.elecTauPairZeeHypotheses.genLeptonsFromZsSource = cms.InputTag('')
		process.elecTauPairZeeHypothesesLooseElectronIsolation.genLeptonsFromZsSource = cms.InputTag('')

	# remove modules from the Z --> muon + tau-jet analysis sequence which run on GEN collections
	if hasattr(process, "analyzeZtoMuTauEvents"):
		process.analyzeZtoMuTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
		removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "genPhaseSpaceEventInfoHistManager")
		process.analyzeZtoMuTauEvents.analyzers.remove(process.modelBinnerForMuTauGenTauLeptonPairAcc)
		process.analyzeZtoMuTauEvents.analyzers.remove(process.modelBinnerForMuTauWrtGenTauLeptonPairAcc)
		removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "modelBinnerForMuTauGenTauLeptonPairAcc")
		removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "modelBinnerForMuTauWrtGenTauLeptonPairAcc")
		process.analyzeZtoMuTauEvents.analyzers_systematic = cms.VPSet()
		removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "sysUncertaintyBinnerForMuTau")
                for eventDump in process.analyzeZtoMuTauEvents.eventDumps:
                    eventDump.doGenInfo = cms.bool(False)
		    eventDump.genParticleSource = cms.InputTag('')
		process.muonHistManager.genParticleSource = cms.InputTag('')
		process.tauHistManager.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')
		process.diTauCandidateHistManagerForMuTau.genParticleSource = cms.InputTag('')
		process.diTauCandidateCollinearApproxHistManagerForMuTau.genParticleSource = cms.InputTag('')
		removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, "diTauCandidateCollinearApproxHistManagerBinnedForMuTau")
		process.diTauCandidateSVfitHistManagerForMuTau.genParticleSource = cms.InputTag('')
		process.diTauCandidateEventActivityHistManagerForMuTau.genParticleSource = cms.InputTag('')
		process.diTauCandidateZmumuHypothesisHistManagerForMuTau.genParticleSource = cms.InputTag('')
		process.muPairHistManagerByMass.genParticleSource = cms.InputTag('')
		process.muPairHistManagerByLooseIsolationAndCharge.genParticleSource = cms.InputTag('')
		process.muTauPairZmumuHypotheses.genLeptonsFromZsSource = cms.InputTag('')
		process.muTauPairZmumuHypothesesLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')

	# remove modules from the Z --> tau-jet + tau-jet analysis sequence which run on GEN collections
	if hasattr(process, "analyzeZtoDiTauEvents"):
		# Remove genPhaseSpace info
		process.analyzeZtoDiTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
		removeAnalyzer(process.analyzeZtoDiTauEvents.analysisSequence, "genPhaseSpaceEventInfoHistManager")

		# Remove genParticles/genTauJets
		for eventDump in process.analyzeZtoDiTauEvents.eventDumps:
			eventDump.doGenInfo = cms.bool(False)
			eventDump.genParticleSource = cms.InputTag('')
		process.tauHistManager1.genParticleSource = cms.InputTag('')
		process.tauHistManager2.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')
		process.diTauCandidateHistManagerForDiTau.genParticleSource = cms.InputTag('')
		process.diTauCandidateCollinearApproxHistManagerForDiTau.genParticleSource = cms.InputTag('')		
		process.diTauCandidateSVfitHistManagerForDiTau.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')

	# remove modules from the W --> tau-jet nu analysis sequence which run on GEN collections
	if hasattr(process, "analyzeWtoTauNuEvents"):
		process.analyzeWtoTauNuEvents.eventDumps[0].doGenInfo = cms.bool(False)
		process.analyzeWtoTauNuEvents.eventDumps[0].genParticleSource = cms.InputTag('')
		process.electronHistManager.genParticleSource = cms.InputTag('')
		process.tauHistManager.genParticleSource = cms.InputTag('')
		process.jetHistManager.genParticleSource = cms.InputTag('')    

	# remove modules from the A/H --> mu + tau-jet analysis sequence which run on GEN collections
	for module_name in [ 'analyzeAHtoMuTauEvents_woBtag', 'analyzeAHtoMuTauEvents_wBtag' ]:
		if hasattr(process, module_name):
			module = getattr(process, module_name)

			# Remove genPhaseSpace info
			if process.genPhaseSpaceEventInfoHistManager in module.analyzers:
				module.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)

			# Remove genParticles
			for analyzer in module.analyzers:
				if hasattr(analyzer , 'genParticleSource'):
					analyzer.genParticleSource = cms.InputTag('')

				if hasattr(analyzer, 'histManagers'):
					for histManager in analyzer.histManagers:
						if hasattr(histManager , 'genParticleSource'):
							histManager.genParticleSource = cms.InputTag('')

			removeAnalyzer(module.analysisSequence, "genPhaseSpaceEventInfoHistManager")
			
			for eventDump in module.eventDumps:
				eventDump.doGenInfo = cms.bool(False)
				eventDump.genParticleSource = cms.InputTag('')

			if process.modelBinnerForMuTauGenTauLeptonPairAcc in module.analyzers:
				module.analyzers.remove(process.modelBinnerForMuTauGenTauLeptonPairAcc)
			removeAnalyzer(module.analysisSequence, "modelBinnerForMuTauGenTauLeptonPairAcc")
			if process.modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc in module.analyzers:
				module.analyzers.remove(process.modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc)
			removeAnalyzer(module.analysisSequence, "modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc")
			if process.modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc in module.analyzers:
				module.analyzers.remove(process.modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc)
			removeAnalyzer(module.analysisSequence, "modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc")

			process.muonHistManager.genParticleSource = cms.InputTag('')
			process.tauHistManager.genParticleSource = cms.InputTag('')
			process.jetHistManager.genParticleSource = cms.InputTag('')

			process.diTauCandidateHistManagerForMuTau.genParticleSource = cms.InputTag('')
			process.diTauCandidateCollinearApproxHistManagerForMuTau.genParticleSource = cms.InputTag('')
			process.diTauCandidateSVfitHistManagerForMuTau.genParticleSource = cms.InputTag('')
			process.diTauCandidateZmumuHypothesisHistManagerForMuTau.genParticleSource = cms.InputTag('')

			process.muPairHistManagerByMass.genParticleSource = cms.InputTag('')
			process.muPairHistManagerByLooseIsolationAndCharge.genParticleSource = cms.InputTag('')

			process.muTauPairZmumuHypothesesForAHtoMuTau.genLeptonsFromZsSource = cms.InputTag('')
			process.muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')
			process.muTauPairZmumuHypotheses.genLeptonsFromZsSource = cms.InputTag('')
			process.muTauPairZmumuHypothesesLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')    
      
	# remove modules from the Z --> e + mu analysis sequence which run on GEN collections
	if hasattr(process, "analyzeZtoElecMuEvents"):
                process.analyzeZtoElecMuEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
                removeAnalyzer(process.analyzeZtoElecMuEvents.analysisSequence,"genPhaseSpaceEventInfoHistManager")
                process.analyzeZtoElecMuEvents.eventDumps[0].doGenInfo = cms.bool(False)
                process.analyzeZtoElecMuEvents.eventDumps[0].genParticleSource = cms.InputTag('')
                process.electronHistManager.genParticleSource = cms.InputTag('')
                process.muonHistManager.genParticleSource = cms.InputTag('')
                process.jetHistManager.genParticleSource = cms.InputTag('')
  
