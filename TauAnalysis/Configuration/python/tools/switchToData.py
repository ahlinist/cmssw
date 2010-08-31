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
    if hasattr(process, "patPFMETs"):
        process.patPFMETs.addGenMET = cms.bool(False)


    # remove modules from the Z->e+tau-jet analysis sequence which run on GEN collections
    if hasattr(process,"analyzeZtoElecTauEvents"):
        process.analyzeZtoElecTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
        removeAnalyzer(process.analyzeZtoElecTauEvents.analysisSequence,"genPhaseSpaceEventInfoHistManager")
        process.analyzeZtoElecTauEvents.eventDumps[0].doGenInfo = cms.bool(False)
        process.analyzeZtoElecTauEvents.eventDumps[0].genParticleSource = cms.InputTag('')
        process.electronHistManager.genParticleSource = cms.InputTag('')
        process.tauHistManager.genParticleSource = cms.InputTag('')
        process.jetHistManager.genParticleSource = cms.InputTag('')
        process.elecTauPairZeeHypotheses.genLeptonsFromZsSource = cms.InputTag('')
        process.elecTauPairZeeHypothesesLooseElectronIsolation.genLeptonsFromZsSource = cms.InputTag('')
        #process.selectedDiTauPairs1stTauChargeCumulative.srcGenParticles = cms.InputTag('')    
        #process.selectedDiTauPairs1stTauChargeIndividual.srcGenParticles = cms.InputTag('')

    # remove modules from the W->tau nu analysis sequence which run on GEN collections
    if hasattr(process,"analyzeWtoTauNuEvents"):
        process.analyzeWtoTauNuEvents.eventDumps[0].doGenInfo = cms.bool(False)
        process.analyzeWtoTauNuEvents.eventDumps[0].genParticleSource = cms.InputTag('')
        process.electronHistManager.genParticleSource = cms.InputTag('')
        process.tauHistManager.genParticleSource = cms.InputTag('')
        process.jetHistManager.genParticleSource = cms.InputTag('')

    for module_name in ['analyzeAHtoMuTauEvents_woBtag', 'analyzeAHtoMuTauEvents_wBtag']:
        # Loop over relevant generic analyzers
        process.muTauPairZmumuHypothesesForAHtoMuTau.genLeptonsFromZsSource = cms.InputTag('')
        process.muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')
        process.muTauPairZmumuHypotheses.genLeptonsFromZsSource = cms.InputTag('')
        process.muTauPairZmumuHypothesesLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')

        if hasattr(process, module_name):
            module = getattr(process, module_name)

            # Remove genPhaseSpace info
            if process.genPhaseSpaceEventInfoHistManager in module.analyzers:
                module.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)

            # Remove genParticles
            for analyzer in module.analyzers:
                if hasattr(analyzer , 'genParticleSource'):
                    analyzer.genParticleSource = cms.InputTag('')
                    #del analyzer.genParticleSource

                if hasattr(analyzer, 'histManagers'):
                    for histManager in analyzer.histManagers:
                        if hasattr(histManager , 'genParticleSource'):
                            histManager.genParticleSource = cms.InputTag('')
                            #del histManager.genParticleSource

            removeAnalyzer(module.analysisSequence,"genPhaseSpaceEventInfoHistManager")
            #module.eventDumps[0].doGenInfo = cms.bool(False)
            #module.eventDumps[0].genParticleSource = cms.InputTag('')
            process.muonHistManager.genParticleSource = cms.InputTag('')
            process.tauHistManager.genParticleSource = cms.InputTag('')
            process.jetHistManager.genParticleSource = cms.InputTag('')

