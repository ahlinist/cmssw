import FWCore.ParameterSet.Config as cms

ZMuMuGammaSelection_Fsr2011Apr11 = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum near muon HCAL isolation
    maxNearMuonHcalIso = cms.double(1),
    ## 2. maximum far muon ECAL isolation
    maxFarMuonEcalIso = cms.double(1),
    ## 3. maximum Delta R distance between the photon and the near muon
    maxDeltaRNear = cms.double(0.8),
    ## 4. minimum far muon transverse momentum
    minFarMuonPt = cms.double(30),
    ## 5. minimum invariant mass
    minMass = cms.double(70),
    ## 6. maximum invariant mass
    maxMass = cms.double(110),
)
