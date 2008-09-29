import FWCore.ParameterSet.Config as cms
import copy

from TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import * 
from PhysicsTools.PatAlgos.recoLayer0.jetMETCorrections_cff import *
from PhysicsTools.PatAlgos.cleaningLayer0.caloMetCleaner_cfi  import *
from PhysicsTools.PatAlgos.producersLayer1.metProducer_cff import *
allLayer1METs.addTrigMatch = cms.bool(False)

patMET=cms.Sequence(patAODJetMETCorrections * allLayer0METs  * layer1METs)

 






