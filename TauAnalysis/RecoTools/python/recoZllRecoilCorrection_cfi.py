import FWCore.ParameterSet.Config as cms

recoZllRecoilCorrectionParameter = cms.PSet(
    parameter = cms.PSet(
        data = cms.PSet(
            d = cms.double(0.),
            k = cms.double(-0.92461),
            sigma1 = cms.double(9.16832),
            b1 = cms.double(-1.37362e-2),
            c1 = cms.double(+7.51767e-5),
            sigma2 = cms.double(7.4436),
            b2 = cms.double(-7.60317e-3),
            c2 = cms.double(-2.30955e-6)
        ),
        mc = cms.PSet(
            d = cms.double(0.),
            k = cms.double(-0.928701),
            sigma1 = cms.double(7.86255),
            b1 = cms.double(-1.08423e-2),
            c1 = cms.double(+4.26271e-5),
            sigma2 = cms.double(6.14236),
            b2 = cms.double(-1.05838e-2),
            c2 = cms.double(+2.51923e-5)
        )
    ),

    # flag indicating whether to correct MEt only 
    # or to recreate diTau candidates using new MEt too
    recreateDiTauCandidates = cms.bool(True),

    # configuration parameters needed for recreation of diTau objects
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),
    srcGenParticles = cms.InputTag('genParticles'),
    svFit = cms.PSet()
)    
