import FWCore.ParameterSet.Config as cms
import copy

# module to select candidates for "the primary event vertex".
# Selection based on PhysicsTools/PatAlogos/plugins/PATSingleVertexSelector;
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string

# Note: offlinePrimaryVerticesWithBS collection is sorted
#       in order of decreasing sum of Pt of tracks fitted to each vertex

selectedPrimaryVertexQuality = cms.EDFilter("VertexSelector",
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    cut = cms.string("isValid & ndof >= 4"),
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexPosition = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    cut = cms.string("z > -24 & z < +24 & position.Rho < 2."),
    filter = cms.bool(False)                                           
)

selectedPrimaryVertexHighestPtTrackSum = cms.EDFilter("PATSingleVertexSelector",
    mode = cms.string('firstVertex'),
    vertices = cms.InputTag('selectedPrimaryVertexPosition'),
    filter = cms.bool(False)                                                    
)

selectPrimaryVertex = cms.Sequence(
    selectedPrimaryVertexQuality
   * selectedPrimaryVertexPosition
   * selectedPrimaryVertexHighestPtTrackSum
)
