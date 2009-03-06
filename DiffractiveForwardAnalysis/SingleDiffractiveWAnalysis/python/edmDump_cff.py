import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorGen_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import *

xiTowerNoMET = xiTower.clone(UseMETInfo = False)
castorGenNoThreshold = castorGen.clone(CASTORParticleEnergyThreshold = 0.0)
selectTracksAwayFromPV = selectTracksAssociatedToPV.clone()
selectTracksAwayFromPV.MinDistanceFromVertex = 0.8
selectTracksAwayFromPV.MaxDistanceFromVertex = 999.9
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityAwayFromPV = trackMultiplicity.clone(TracksTag = "selectTracksAwayFromPV")

edmDumpTracksAssociatedToPV = cms.Sequence(selectTracksAssociatedToPV*trackMultiplicityAssociatedToPV) 
edmDumpTracksAwayFromPV = cms.Sequence(selectTracksAwayFromPV*trackMultiplicityAwayFromPV)
edmDumpAll = cms.Sequence(castorGen + 
                          castorGenNoThreshold + 
                          castorTower +
                          hfTower + 
                          xiTower +
                          xiTowerNoMET +
                          trackMultiplicity +
                          edmDumpTracksAssociatedToPV + 
                          edmDumpTracksAwayFromPV + 
                          pileUpInfo)
edmDumpAllNoPileUp = cms.Sequence(castorGen +
                          castorGenNoThreshold +
                          castorTower +
                          hfTower +
                          xiTower +
                          xiTowerNoMET +
                          trackMultiplicity +
                          edmDumpTracksAssociatedToPV +
                          edmDumpTracksAwayFromPV)
