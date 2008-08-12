import FWCore.ParameterSet.Config as cms

from FastSimulation.Configuration.CommonInputsFake_cff import *
from FastSimulation.Configuration.FamosSequences_cff import *
#famosPileUp.UseTRandomEngine = True
#famosSimHits.UseTRandomEngine = True
#siTrackerGaussianSmearingRecHits.UseTRandomEngine = True
#caloRecHits.UseTRandomEngine = True
#paramMuons.UseTRandomEngine = True
famosPileUp.PileUpSimulator.averageNumber = 0.0
#MagneticFieldESProducer.useParametrizedTrackerField = True
famosSimHits.SimulateCalorimetry = True
famosSimHits.SimulateTracking = True

