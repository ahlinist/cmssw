import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# compute neutral particle density for out-of-time pile-up reweighting

from CommonTools.ParticleFlow.pfNoPileUp_cff import *
pfPileUp.Enable = cms.bool(True)
pfPileUp.checkClosestZVertex = cms.bool(True)
pfPileUp.PFCandidates = cms.InputTag('particleFlow')
pfNoPileUp.bottomCollection = cms.InputTag('particleFlow')

from CommonTools.ParticleFlow.pfNoPileUpIso_cff import *

from CommonTools.ParticleFlow.ParticleSelectors.pfAllNeutralHadrons_cfi import pfAllNeutralHadrons
from CommonTools.ParticleFlow.ParticleSelectors.pfAllPhotons_cfi import pfAllPhotons
pfNeutralCandPdgIds = []
pfNeutralCandPdgIds.extend(pfAllNeutralHadrons.pdgId.value())
pfNeutralCandPdgIds.extend(pfAllPhotons.pdgId.value())
 
pfNeutralCands = cms.EDFilter("PdgIdPFCandidateSelector",
    src = cms.InputTag('particleFlow'),
    pdgId = cms.vint32(pfNeutralCandPdgIds)
)

from CommonTools.ParticleFlow.ParticleSelectors.pfAllChargedHadrons_cfi import pfAllChargedHadrons
pfChargedHadronCandPdgIds = []
pfChargedHadronCandPdgIds.extend(pfAllChargedHadrons.pdgId.value())

pfChargedHadronNoPileUpCands = pfNeutralCands.clone(
    src = cms.InputTag('pfNoPileUp'),
    pdgId = cms.vint32(pfChargedHadronCandPdgIds)
)

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
kt6PFNeutralJetsForVtxMultReweighting = kt4PFJets.clone(
    src = cms.InputTag('pfNeutralCands'),
    rParam = cms.double(0.6),
    doRhoFastjet = cms.bool(True),
    Rho_EtaMax = cms.double(2.5)
)

kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting = kt6PFNeutralJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfChargedHadronNoPileUpCands')
)    
#--------------------------------------------------------------------------------

vertexMultiplicityVsRhoPFNeutralReweight = cms.EDProducer("VertexMultiplicityVsRhoPFNeutralReweightProducer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoPFNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),
                                                          
    inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
    lutName = cms.string('histoReweight_fitted'),
                                                          
    minPUreweight = cms.double(1.e-1),
    maxPUreweight = cms.double(1.e+1)
)

produceVertexMultiplicityVsRhoPFNeutralReweights = cms.Sequence(
    pfNoPileUpSequence
   * pfNoPileUpIsoSequence
   * pfNeutralCands
   * kt6PFNeutralJetsForVtxMultReweighting
   * pfChargedHadronNoPileUpCands
   * kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting
   * vertexMultiplicityVsRhoPFNeutralReweight
)
