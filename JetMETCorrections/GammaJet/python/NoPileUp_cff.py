import FWCore.ParameterSet.Config as cms

# Run PFNoPileUp and create corresponding object (jets, met)

# Get a list of good primary vertices, in 42x, these are DAF vertices
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
)

#--------------------------------------------------------------------------------
# produce rho parameters needed for L1FastJet corrections
from RecoJets.JetProducers.kt4CaloJets_cfi import kt4CaloJets
#kt6CaloJetsNoPU = kt4CaloJets.clone(
#    src = cms.InputTag('pfNoPileUp'),
#    rParam = cms.double(0.6),
#    doRhoFastjet = cms.bool(True),
#    Rho_EtaMax = cms.double(2.5)
#)

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
kt6PFJetsNoPU = kt4PFJets.clone(
    src = cms.InputTag('pfNoPileUp'),
    rParam = cms.double(0.6),
    doRhoFastjet = cms.bool(True),
    doAreaFastjet = cms.bool(True)
)

kt6PFJetsNoPUForIso = kt4PFJets.clone(
    src = cms.InputTag('pfNoPileUp'),
    rParam = cms.double(0.6),
    doRhoFastjet = cms.bool(True),
    doAreaFastjet = cms.bool(True),
    Rho_EtaMax = cms.double(2.5)
)




# L1FastJetCorrectors
from JetMETCorrections.Configuration.JetCorrectionServices_cff import ak5PFL1Fastjet, ak5PFL2Relative, ak5PFL3Absolute, ak5PFL2L3, ak5PFL1FastL2L3, ak5CaloL2Relative, ak5CaloL3Absolute
#ak5CaloL1FastjetNoPU = ak5CaloL1Fastjet.clone(
#    srcRho = cms.InputTag('kt6CaloJetsNoPU', 'rho')
#    )

ak5PFL1FastjetNoPU = ak5PFL1Fastjet.clone(
    algorithm = "AK5PFchs",
    srcRho = cms.InputTag('kt6PFJetsNoPU', 'rho')
    )

# L2 (relative eta-conformity) Correction Services
ak5PFL2RelativeNoPU = ak5CaloL2Relative.clone( algorithm = 'AK5PFchs' )

# L3 (absolute) Correction Services
ak5PFL3AbsoluteNoPU = ak5CaloL3Absolute.clone( algorithm = 'AK5PFchs' )

# Multiple corrections
ak5PFL2L3NoPU = ak5PFL2L3.clone( correctors = ('ak5PFL2RelativeNoPU','ak5PFL3AbsoluteNoPU') )
ak5PFL1FastL2L3NoPU = ak5PFL1FastL2L3.clone( correctors = ('ak5PFL1FastjetNoPU','ak5PFL2RelativeNoPU','ak5PFL3AbsoluteNoPU') )

from CommonTools.ParticleFlow.pfNoPileUp_cff import *
pfPileUp.checkClosestZVertex = cms.bool(False)
pfPileUp.Vertices = cms.InputTag('offlinePrimaryVertices')
pfPileUp.PFCandidates = cms.InputTag("particleFlow")

# Jets
from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
ak5PFJetsNoPU = ak5PFJets.clone(
    src = cms.InputTag("pfNoPileUp"),
    doAreaFastjet = cms.bool(True)
    )

ak7PFJetsNoPU = ak5PFJetsNoPU.clone( rParam = 0.7 )

# MET
from RecoMET.METProducers.PFMET_cfi import pfMet
pfMetNoPU = pfMet.clone(
    src = cms.InputTag("pfNoPileUp"),
    jets = cms.InputTag("ak5PFJetsNoPU")
    )

pfNoPileUp.bottomCollection = cms.InputTag("particleFlow")

producePFNoPileUp = cms.Sequence(
    goodOfflinePrimaryVertices
#  * ak5CaloL1FastjetNoPU
#  * ak5PFL1FastjetNoPU
#  * ak5JPTL1FastjetNoPU
  * pfNoPileUpSequence
#  * kt6CaloJetsNoPU
  * kt6PFJetsNoPU
  * kt6PFJetsNoPUForIso
  * ak5PFJetsNoPU
  * ak7PFJetsNoPU
  * pfMetNoPU
    )
