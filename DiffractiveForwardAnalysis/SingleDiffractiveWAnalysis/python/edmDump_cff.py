import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorGen_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromJets_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import *

xiFromJets.ParticlesTag = "ak5PFJets"
xiTowerNoMET = xiTower.clone(UseMETInfo = False)
xiFromJetsNoMET = xiFromJets.clone(UseMETInfo = False)

castorGenNoThreshold = castorGen.clone(CASTORParticleEnergyThreshold = 0.0)
selectTracksAssociatedToPV.src = cms.InputTag("selectGoodTracks")
selectTracksAssociatedToPV.MaxDistanceFromVertex = 0.2
selectTracksAwayFromPV = selectTracksAssociatedToPV.clone()
selectTracksAwayFromPV.MinDistanceFromVertex = 1.2
selectTracksAwayFromPV.MaxDistanceFromVertex = 999.9
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityAwayFromPV = trackMultiplicity.clone(TracksTag = "selectTracksAwayFromPV")

edmDumpTracksAssociatedToPV = cms.Sequence(selectTracksAssociatedToPV*trackMultiplicityAssociatedToPV) 
edmDumpTracksAwayFromPV = cms.Sequence(selectTracksAwayFromPV*trackMultiplicityAwayFromPV)
edmDumpTracks = cms.Sequence(selectGoodTracks*edmDumpTracksAssociatedToPV+edmDumpTracksAwayFromPV)

edmDumpAllNoPileUp = cms.Sequence(castorGen + 
                          castorGenNoThreshold + 
                          castorTower +
                          hfTower + 
                          xiTower +
                          xiTowerNoMET +
                          xiFromJets +
                          xiFromJetsNoMET +
                          trackMultiplicity +
                          edmDumpTracks)

edmDumpAll = cms.Sequence(edmDumpAllNoPileUp+pileUpInfo)
