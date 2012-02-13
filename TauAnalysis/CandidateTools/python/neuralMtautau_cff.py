import FWCore.ParameterSet.Config as cms

neuralMtautauSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# produce collections of jets not overlapping with leptons
# and with (corrected) Pt > 20 GeV
selectedPatJetsNotOverlappingWithLeptons = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag('patJets'),
    # CV: set 'srcNotToBeFiltered' to collections of electrons, muons and taus passing analysis specific selection criteria
    srcNotToBeFiltered = cms.VInputTag(
        'patElectrons',
        'patMuons',
        'patTaus',
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)      
)
neuralMtautauSequence += selectedPatJetsNotOverlappingWithLeptons

selectedPatJetsPt20 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag('selectedPatJetsNotOverlappingWithLeptons'),                                   
    cut = cms.string('pt > 20.'), 
    filter = cms.bool(False)
)
neuralMtautauSequence += selectedPatJetsPt20
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select ak5PFJets corresponding to selected pat::Jets
ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20 = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag('selectedPatJetsPt20'),
    dRmin = cms.double(1.e-1),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
neuralMtautauSequence += ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select PFCandidates ("unclustered energy") not within jets
from CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi import pfNoJet
pfCandsNotInSelectedJets = pfNoJet.clone(
    topCollection = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20'),
    bottomCollection = cms.InputTag('particleFlow')
)
neuralMtautauSequence += pfCandsNotInSelectedJets
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce genMET
from RecoMET.Configuration.GenMETParticles_cff import *
neuralMtautauSequence += genParticlesForMETAllVisible

from RecoMET.METProducers.genMetTrue_cfi import *
genMetFromGenParticles = genMetTrue.clone(
    src = cms.InputTag('genParticlesForMETAllVisible'),
    alias = cms.string('genMetFromGenParticles')
)
neuralMtautauSequence += genMetFromGenParticles
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix
from RecoMET.METProducers.METSigParams_cfi import *
pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'genMatchedElectrons',
        'genMatchedMuons',
        'genMatchedTaus',                                   
        'ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20',
        'pfCandsNotInSelectedJets'
    )                                
)
neuralMtautauSequence += pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

from RecoMET.METProducers.METSigParams_cfi import *
neuralMtautauNtupleProducer = cms.EDProducer("NeuralMtautauNtupleProducer",
    srcGenTauPair = cms.InputTag(''),
    srcGenMEt = cms.InputTag('genMetFromGenParticles'), 

    srcRecPFJets = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20'),
    srcRecPFCandidatesNotWithinJets = cms.InputTag('pfCandsNotInSelectedJets'),
    srcRecLeg1 = cms.InputTag('genMatchedMuons'), # collection of reconstructed tau decay products (1st leg)
    srcRecLeg2 = cms.InputTag('genMatchedTaus'),  # collection of reconstructed tau decay products (2nd leg)
    srcRecMEt = cms.InputTag('patType1CorrectedPFMet'),
                                             
    srcPFMEtCovMatrix = cms.InputTag('pfMEtSignCovMatrix')
)                                         
neuralMtautauSequence += neuralMtautauNtupleProducer
