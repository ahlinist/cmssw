import FWCore.ParameterSet.Config as cms

# import PAT muon isolation config
from PhysicsTools.PatAlgos.recoLayer0.muonIsolation_cff import *

# set PAT layer 0 muon collections to EWK tau specific collections
layer0MuonIsolations.collection = cms.InputTag("allLayer0MuForEWKTau")
layer0MuonIsolations.backrefs = cms.InputTag("allLayer0MuForEWKTau")
