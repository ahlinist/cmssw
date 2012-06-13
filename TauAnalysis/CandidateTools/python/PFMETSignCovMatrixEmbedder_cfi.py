'''

Fragments of modules to embed an externally computed significance matrix
into reco:: and pat:: METs.

Author: Evan K. Friis, UW

'''

import FWCore.ParameterSet.Config as cms

patMETSignEmbedder = cms.EDProducer(
    "PatPFMEtSignCovMatrixEmbedder",
    src = cms.InputTag("patMETs"),
    srcCov = cms.InputTag("covMatrix"),
)

recoMETSignEmbedder = cms.EDProducer(
    "RecoPFMEtSignCovMatrixEmbedder",
    src = cms.InputTag("met"),
    srcCov = cms.InputTag("covMatrix"),
)
