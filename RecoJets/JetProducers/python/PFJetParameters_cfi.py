import FWCore.ParameterSet.Config as cms

PFJetParameters = cms.PSet(
    src            = cms.InputTag('particleFlow'),
    srcPVs         = cms.InputTag(''),
    jetType        = cms.string('PFJet'),
    jetPtMin       = cms.double(1.0),
    inputEMin      = cms.double(0.0),
    inputEtMin     = cms.double(0.0),
    doPVCorrection = cms.bool(False),
    # pileup
    doPUOffsetCorr = cms.bool(False),
    doPUFastjet    = cms.bool(False),
      # if doPU is false, these are not read:
      Active_Area_Repeats = cms.int32(5),
      GhostArea = cms.double(0.01),
      Ghost_EtaMax = cms.double(6.0)
    )
