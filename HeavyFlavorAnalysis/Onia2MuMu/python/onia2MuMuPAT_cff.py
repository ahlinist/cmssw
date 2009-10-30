import FWCore.ParameterSet.Config as cms
import  HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi 

onia2MuMuPatGlbGlb = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()

onia2MuMuPatGlbTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatGlbTrk.lowerPuritySelection  = cms.int32(2)

onia2MuMuPatTrkTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatTrkTrk.higherPuritySelection  = cms.int32(2)
onia2MuMuPatTrkTrk.lowerPuritySelection   = cms.int32(2)
