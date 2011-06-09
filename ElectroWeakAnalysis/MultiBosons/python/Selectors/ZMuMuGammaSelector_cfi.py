import FWCore.ParameterSet.Config as cms

ZMuMuGammaSelection_Fsr2011Apr11 = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum near muon HCAL isolation
    maxNearMuonHcalIso = cms.double(1),
    ## 2. maximum far muon ECAL isolation
    maxFarMuonTrackIso = cms.double(3),
    ## 3. maximum far muon ECAL isolation
    maxFarMuonEcalIso = cms.double(1),
    ## 4. maximum photon tracker isolation (near muon removed) = const + slope * pt
    photonTrackIsoConst = cms.double(2),
    photonTrackIsoSlope = cms.double(0.001),
    ## 5. maximum Delta R distance between the photon and the near muon
    maxDeltaRNear = cms.double(0.8),
    ## 6. minimum far muon transverse momentum
    minFarMuonPt = cms.double(30),
    ## 7. minimum invariant mass
    minMass = cms.double(70),
    ## 8. maximum invariant mass
    maxMass = cms.double(110),
    cutsToIgnore = cms.vstring("maxFarMuonTrackIso", "photonTrackIso")
)

ZMuMuGammaSelection_Fsr2011Apr11_PixelMatchVeto = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum near muon HCAL isolation
    maxNearMuonHcalIso = cms.double(1),
    ## 2. maximum far muon ECAL isolation
    maxFarMuonTrackIso = cms.double(3),
    ## 3. maximum far muon ECAL isolation
    maxFarMuonEcalIso = cms.double(1),
    ## 4. maximum photon tracker isolation (near muon removed) = const + slope * pt
    photonTrackIsoConst = cms.double(2),
    photonTrackIsoSlope = cms.double(0.001),
    ## 5. maximum Delta R distance between the photon and the near muon
    maxDeltaRNear = cms.double(1),
    ## 6. minimum far muon transverse momentum
    minFarMuonPt = cms.double(15),
    ## 7. minimum invariant mass
    minMass = cms.double(60),
    ## 8. maximum invariant mass
    maxMass = cms.double(120),
    #cutsToIgnore = cms.vstring("maxNearMuonHcalIso", "maxFarMuonEcalIso")
)

ZMuMuGammaSelection_Fsr2011Jun8_energyScale = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum near muon HCAL isolation
    maxNearMuonHcalIso = cms.double(1),
    ## 2. maximum far muon ECAL isolation
    maxFarMuonTrackIso = cms.double(3),
    ## 3. maximum far muon ECAL isolation
    maxFarMuonEcalIso = cms.double(1),
    ## 4. maximum photon tracker isolation (near muon removed) = const + slope * pt
    photonTrackIsoConst = cms.double(2),
    photonTrackIsoSlope = cms.double(0.001),
    ## 5. maximum Delta R distance between the photon and the near muon
    maxDeltaRNear = cms.double(0.8),
    ## 6. minimum far muon transverse momentum
    minFarMuonPt = cms.double(30),
    ## 7. minimum invariant mass
    minMass = cms.double(50),
    ## 8. maximum invariant mass
    maxMass = cms.double(130),
    cutsToIgnore = cms.vstring("maxFarMuonTrackIso", "photonTrackIso")
)

