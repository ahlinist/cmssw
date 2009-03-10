import FWCore.ParameterSet.Config as cms

from FastSimulation.ForwardDetectors.CastorTowerProducer_cfi import *  
from RecoLocalCalo.Castor.Castor_cfi import *

castorReco = cms.Sequence(CastorTowerReco)
