import FWCore.ParameterSet.Config as cms

from FastSimulation.ForwardDetectors.CastorFastTowerProducer_cfi import *  
#from RecoLocalCalo.Castor.Castor_cfi import *

castorReco = cms.Sequence(CastorFastTowerReco)
