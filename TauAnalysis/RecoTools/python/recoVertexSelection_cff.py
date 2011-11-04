import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Select candidates for "the" event vertex.
#
# Selection based on PhysicsTools/PatAlogos/plugins/PATSingleVertexSelector.
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string
#
# NOTE: offlinePrimaryVerticesWithBS collection is sorted
#       in order of decreasing sum of Pt of tracks fitted to each vertex
#--------------------------------------------------------------------------------

# CV: ndof >= 4 if using 'offlinePrimaryVertices',
#     ndof >= 7 if using 'offlinePrimaryVerticesWithBS' as input
selectedPrimaryVertexQuality = cms.EDFilter("VertexSelector",
    src = cms.InputTag('offlinePrimaryVertices'),
    cut = cms.string("isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0"), # CV: cut >= 4 if using 'offlinePrimaryVertices',
                                                                         #         >= 7 if using 'offlinePrimaryVerticesWithBS' as input
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexPosition = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    cut = cms.string("abs(z) < 24 & abs(position.Rho) < 2."),
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
