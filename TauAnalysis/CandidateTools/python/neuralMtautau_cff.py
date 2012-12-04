import FWCore.ParameterSet.Config as cms

neuralMtautauSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# produce collections of jets not overlapping with leptons
# and with (corrected) Pt > 20 GeV
selectedPatJetsNotOverlappingWithLeptonsForNeuralMtautau = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag('patJets'),
    # CV: set 'srcNotToBeFiltered' to collections of electrons, muons and taus passing analysis specific selection criteria
    srcNotToBeFiltered = cms.VInputTag(
        'genMatchedElectrons',
        'genMatchedMuons',
        'genMatchedTaus'
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)      
)
neuralMtautauSequence += selectedPatJetsNotOverlappingWithLeptonsForNeuralMtautau

selectedPatJetsPt20ForNeuralMtautau = cms.EDFilter("PATJetSelector",
    src = cms.InputTag('selectedPatJetsNotOverlappingWithLeptonsForNeuralMtautau'),                                   
    cut = cms.string('pt > 20.'), 
    filter = cms.bool(False)
)
neuralMtautauSequence += selectedPatJetsPt20ForNeuralMtautau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select ak5PFJets corresponding to selected pat::Jets
ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20ForNeuralMtautau = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag('selectedPatJetsPt20'),
    dRmin = cms.double(1.e-1),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
neuralMtautauSequence += ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20ForNeuralMtautau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select PFCandidates ("unclustered energy") not within jets
from CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi import pfNoJet
pfCandsNotInSelectedJetsForNeuralMtautau = pfNoJet.clone(
    topCollection = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20ForNeuralMtautau'),
    bottomCollection = cms.InputTag('particleFlow')
)
neuralMtautauSequence += pfCandsNotInSelectedJetsForNeuralMtautau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce genMET
from RecoMET.Configuration.GenMETParticles_cff import *
genParticlesForMETAllVisibleForNeuralMtautau = genParticlesForMETAllVisible.clone()
neuralMtautauSequence += genParticlesForMETAllVisibleForNeuralMtautau

from RecoMET.METProducers.genMetTrue_cfi import *
genMetFromGenParticlesForNeuralMtautau = genMetTrue.clone(
    src = cms.InputTag('genParticlesForMETAllVisibleForNeuralMtautau'),
    alias = cms.string('genMetFromGenParticles')
)
neuralMtautauSequence += genMetFromGenParticlesForNeuralMtautau
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix
from RecoMET.METProducers.METSigParams_cfi import *
pfMEtSignCovMatrixForNeuralMtautau = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'genMatchedElectrons',
        'genMatchedMuons',
        'genMatchedTaus',                                   
        'ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20ForNeuralMtautau',
        'pfCandsNotInSelectedJetsForNeuralMtautau'
    )                                
)
neuralMtautauSequence += pfMEtSignCovMatrixForNeuralMtautau
#--------------------------------------------------------------------------------

from RecoMET.METProducers.METSigParams_cfi import *
neuralMtautauNtupleProducer = cms.EDProducer("NeuralMtautauNtupleProducer",
    srcGenTauPair = cms.InputTag(''),
    srcGenMEt = cms.InputTag('genMetFromGenParticlesForNeuralMtautau'), 
    srcGenTaus = cms.InputTag(''),                                         
    srcGenParticles = cms.InputTag('genParticles'),
                                             
    srcRecPFJets = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20ForNeuralMtautau'),
    srcRecPFCandidatesNotWithinJets = cms.InputTag('pfCandsNotInSelectedJetsForNeuralMtautau'),
    srcRecLeg1 = cms.InputTag('genMatchedMuons'), # collection of reconstructed tau decay products (1st leg)
    srcRecLeg2 = cms.InputTag('genMatchedTaus'),  # collection of reconstructed tau decay products (2nd leg)
    srcRecMEt = cms.InputTag('patType1CorrectedPFMet'),
                                             
    srcPFMEtCovMatrix = cms.InputTag('pfMEtSignCovMatrixForNeuralMtautau')
)                                         
neuralMtautauSequence += neuralMtautauNtupleProducer
