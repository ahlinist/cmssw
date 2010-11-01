import FWCore.ParameterSet.Config as cms

"""
VARIOUS FILTERS TO CLEAN UP COLLISION DATA
"""

## Physics Declared bit selection
## https://twiki.cern.ch/twiki/bin/view/CMS/Collisions2010Recipes#Physics_Declared_bit_selection
## (Not used since Lindsey says it kills some good runs)
from HLTrigger.special.hltPhysicsDeclared_cfi import *
hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

## Removal of Beam Scraping Events
## https://twiki.cern.ch/twiki/bin/view/CMS/Collisions2010Recipes#Removal_of_Beam_Scraping_Events
noScraping = cms.EDFilter("FilterOutScraping",
  applyfilter = cms.untracked.bool(True),
  debugOn = cms.untracked.bool(False),
  numtrack = cms.untracked.uint32(10),
  thresh = cms.untracked.double(0.25)
)

## Good vertex selection
## https://twiki.cern.ch/twiki/bin/view/CMS/Collisions2010Recipes#Good_Vertex_selection
## https://hypernews.cern.ch/HyperNews/CMS/get/gaugeCoupling/349/1.html
primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
  vertexCollection = cms.InputTag('offlinePrimaryVertices'),
  minimumNDOF = cms.uint32(4),
  maxAbsZ = cms.double(24),
  maxd0 = cms.double(2)
)

goodCollisionDataSequence = cms.Sequence(
  hltPhysicsDeclared  + # kills some good runs?
  noScraping          + # crashes?
  primaryVertexFilter
)
