import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorGen_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
xiTowerNoMET = xiTower.clone(UseMETInfo = False)
castorGenNoThreshold = castorGen.clone(CASTORParticleEnergyThreshold = 0.0)
