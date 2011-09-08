import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.tools.coreTools as coreTools
import PhysicsTools.PatAlgos.tools.tauTools as tauTools
import PhysicsTools.PatAlgos.tools.jetTools as jetTools

import ElectroWeakAnalysis.TauTriggerEfficiency.MuonPFIsolation as MuonPFIsolation
import ElectroWeakAnalysis.TauTriggerEfficiency.ZtoMuTauFilter_cfi as zmutau

def addPat(process, isData, doTTEffShrinkingConePFTau):
    sequence = cms.Sequence()

    process.load("PhysicsTools.PatAlgos.patSequences_cff")

    # Calculate PF isolation of the muon
    process.muonPFIsolationSequence = MuonPFIsolation.addMuonPFIsolation(process, "muons", process.patMuons)
    sequence += process.muonPFIsolationSequence
    
    jetCorr = ["L1FastJet", "L2Relative", "L3Absolute"]
    coreTools.removeMCMatching(process, ["All"], outputInProcess=False)
    if isData:
        jetCorr.append("L2L3Residual")
    coreTools.removeCleaning(process, False)
    coreTools.removeSpecificPATObjects(process, ["Electrons", "Photons", "METs"], False)
    tauTools.addTauCollection(process, cms.InputTag('hpsPFTauProducer'),
                              algoLabel = "hps", typeLabel = "PFTau")
    jetTools.switchJetCollection(process, cms.InputTag('ak5PFJets'),   
                                 doJTA            = True,            
                                 doBTagging       = True,            
                                 jetCorrLabel     = ('AK5PF', jetCorr),
                                 doType1MET       = False,            
                                 genJetCollection = cms.InputTag("ak5GenJets"),
                                 doJetID      = False,
                                 jetIdLabel   = "ak5",
                                 outputModule = "")

    # If the reference tau should be the old TTEff shrinking cone
    if doTTEffShrinkingConePFTau:
        process.patTaus.tauSource = "TTEffShrinkingConePFTauProducer"
        for module in [process.tauIsoDepositPFCandidates, process.tauIsoDepositPFChargedHadrons, process.tauIsoDepositPFGammas, process.tauIsoDepositPFNeutralHadrons]:
            module.src = "TTEffShrinkingConePFTauProducer"
            module.ExtractorPSet.tauSource = "TTEffShrinkingConePFTauProducer"
        process.tauMatch.src = "TTEffShrinkingConePFTauProducer"
        process.patTaus.tauIDSources.leadingPionPtCut = "TTEffPFTauDiscriminationByLeadingPionPtCut"
        process.patTaus.tauIDSources.byIsolationUsingLeadingPion = "TTEffPFTauDiscriminationByIsolationUsingLeadingPion"
        process.patTaus.tauIDSources.leadingTrackFinding = "TTEffPFTauDiscriminationByLeadingTrackFinding"
        process.patTaus.tauIDSources.againstMuon = "TTEffPFTauDiscriminationAgainstMuon"
        process.patTaus.tauIDSources.againstElectron = "TTEffPFTauDiscriminationAgainstElectron"
        process.selectedPatTaus.cut = "pt() > 15 && abs(eta()) < 2.5 && tauID('leadingPionPtCut')"

    # Set the default selection
    process.selectedPatTausHpsPFTau.cut = "pt() > 15 && abs(eta()) < 2.5 && tauID('byLooseIsolation') > 0.5"

    # Set the default selection the same as in 
    process.selectedPatMuons.cut = zmutau.selectedMuons.cut

    sequence += process.patDefaultSequence

    # Redo the mu+tau pairs in terms of PAT objects
    process.muTauPairs = zmutau.muTauPairs.clone(
        decay = "selectedPatMuons@+ selectedPatTaus@-"
    )
    sequence += process.muTauPairs

    return sequence
