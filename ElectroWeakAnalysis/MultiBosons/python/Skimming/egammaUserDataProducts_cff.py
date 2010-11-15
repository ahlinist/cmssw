import FWCore.ParameterSet.Config as cms

def egammaUserDataFloats(moduleName):
  return [
    cms.InputTag(moduleName, "e1x3"),
    cms.InputTag(moduleName, "e3x1"),
    cms.InputTag(moduleName, "e1x5"),
    cms.InputTag(moduleName, "e2x2"),
    cms.InputTag(moduleName, "e3x2"),
    cms.InputTag(moduleName, "e3x3"),
    cms.InputTag(moduleName, "e4x4"),
    cms.InputTag(moduleName, "e5x5"),

    cms.InputTag(moduleName, "e2x5Right"),
    cms.InputTag(moduleName, "e2x5Left"),
    cms.InputTag(moduleName, "e2x5Top"),
    cms.InputTag(moduleName, "e2x5Bottom"),
    cms.InputTag(moduleName, "e2x5Max"),

    cms.InputTag(moduleName, "eLeft"),
    cms.InputTag(moduleName, "eRight"),
    cms.InputTag(moduleName, "eTop"),
    cms.InputTag(moduleName, "eBottom"),

    cms.InputTag(moduleName, "eMax"),
    cms.InputTag(moduleName, "e2nd"),

    cms.InputTag(moduleName, "etaLat"),
    cms.InputTag(moduleName, "phiLat"),
    cms.InputTag(moduleName, "lat"),

    cms.InputTag(moduleName, "covEtaEta"),
    cms.InputTag(moduleName, "covEtaPhi"),
    cms.InputTag(moduleName, "covPhiPhi"),

    cms.InputTag(moduleName, "covIEtaIEta"),
    cms.InputTag(moduleName, "covIEtaIPhi"),
    cms.InputTag(moduleName, "covIPhiIPhi"),

    cms.InputTag(moduleName, "zernike20"),
    cms.InputTag(moduleName, "zernike42"),

    cms.InputTag(moduleName, "seedTime"),
    cms.InputTag(moduleName, "seedOutOfTimeChi2"),
    cms.InputTag(moduleName, "seedChi2"),
    cms.InputTag(moduleName, "seedSwissCross"),
    cms.InputTag(moduleName, "seedE1OverE9"),
    
    cms.InputTag(moduleName, "scRoundness"),
    cms.InputTag(moduleName, "scAngle"),

    cms.InputTag(moduleName, "sMin"),
    cms.InputTag(moduleName, "sMaj"),
    cms.InputTag(moduleName, "alphaSMinMaj"),

    cms.InputTag(moduleName, "E2overE9")
  ]
# egammaUserDataFloats(moduleName) <------------------------------------------

def egammaUserDataInts(moduleName):
  return [
    cms.InputTag(moduleName, "seedRecoFlag"),
    cms.InputTag(moduleName, "seedSeverityLevel"),
  ]
# egammaUserDataInts(moduleName) <------------------------------------------
