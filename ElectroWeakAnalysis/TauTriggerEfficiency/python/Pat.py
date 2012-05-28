import FWCore.ParameterSet.Config as cms

import PhysicsTools.PatAlgos.tools.coreTools as coreTools
import PhysicsTools.PatAlgos.tools.tauTools as tauTools
import PhysicsTools.PatAlgos.tools.jetTools as jetTools

import ElectroWeakAnalysis.TauTriggerEfficiency.MuonPFIsolation as MuonPFIsolation

def addPat(process, isData, doRECO, doTTEffShrinkingConePFTau):
    sequence = cms.Sequence()

    # If running together with RECO
    # We get another kt6PFJets from PAT, and as-is they clash producing an exception
    runningWithReco = hasattr(process, "recoAllPFJets")
    if runningWithReco:
        process.recoAllPFJets.remove(process.kt6PFJets)
        process.recoPFJets.remove(process.kt6PFJets)
    
    process.load("PhysicsTools.PatAlgos.patSequences_cff")

    if runningWithReco:
        process.recoAllPFJets.replace(process.kt4PFJets, process.kt4PFJets*process.kt6PFJets)
        process.recoPFJets.replace(process.kt4PFJets, process.kt4PFJets*process.kt6PFJets)

#    process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")

    # Calculate PF isolation of the muon
    #process.muonPFIsolationSequence = MuonPFIsolation.addMuonPFIsolation(process, "muons", process.patMuons)
    #sequence += process.muonPFIsolationSequence
    
    if doRECO:
        process.pfPileUpIso.PFCandidates = "particleFlowTmp" # I don't know why I have to do this
        process.pfNoPileUpIso.bottomCollection = "particleFlowTmp"

    jetCorr = ["L1FastJet", "L2Relative", "L3Absolute"]
    if isData:
        jetCorr.append("L2L3Residual")
#        coreTools.removeMCMatching(process, ["All"], outputInProcess=False)
	coreTools.removeMCMatching(process, ["All"], outputModules = [])
####    coreTools.removeCleaning(process, False)
        coreTools.removeCleaning(process, outputModules = [])
####    coreTools.removeSpecificPATObjects(process, ["Electrons", "Photons", "METs"], False)
        coreTools.removeSpecificPATObjects(process, ["Electrons", "Photons", "METs"], outputModules = [])
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
                                 outputModules = [])
    
    # If the reference tau should be the old TTEff shrinking cone
    # if doTTEffShrinkingConePFTau:
    #     process.patTaus.tauSource = "TTEffShrinkingConePFTauProducer"
    #     for module in [process.tauIsoDepositPFCandidates, process.tauIsoDepositPFChargedHadrons, process.tauIsoDepositPFGammas, process.tauIsoDepositPFNeutralHadrons]:
    #         module.src = "TTEffShrinkingConePFTauProducer"
    #         module.ExtractorPSet.tauSource = "TTEffShrinkingConePFTauProducer"
    #     process.tauMatch.src = "TTEffShrinkingConePFTauProducer"
    #     process.patTaus.tauIDSources.leadingPionPtCut = "TTEffPFTauDiscriminationByLeadingPionPtCut"
    #     process.patTaus.tauIDSources.byIsolationUsingLeadingPion = "TTEffPFTauDiscriminationByIsolationUsingLeadingPion"
    #     process.patTaus.tauIDSources.leadingTrackFinding = "TTEffPFTauDiscriminationByLeadingTrackFinding"
    #     process.patTaus.tauIDSources.againstMuon = "TTEffPFTauDiscriminationAgainstMuon"
    #     process.patTaus.tauIDSources.againstElectron = "TTEffPFTauDiscriminationAgainstElectron"
    #     process.selectedPatTaus.cut = "pt() > 15 && abs(eta()) < 2.5 && tauID('leadingPionPtCut')"

    sequence += process.patDefaultSequence
    return sequence
