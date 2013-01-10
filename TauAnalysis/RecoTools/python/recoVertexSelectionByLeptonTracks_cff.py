import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Select candidates for "the" event vertex.
#
# The vertex is chosen to be the one most compatible
# with a collection of leptons given as input.
#--------------------------------------------------------------------------------

selectedPrimaryVertexQuality = cms.EDFilter("VertexSelector",
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    cut = cms.string("isValid & ndof >= 7 & chi2 > 0 & tracksSize > 0"), # CV: apply ndof cut >= 4 if using 'offlinePrimaryVertices',
                                                                         #                    >= 7 if using 'offlinePrimaryVerticesWithBS' as input
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexPosition = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    cut = cms.string("abs(z) < 24 & abs(position.Rho) < 2."),
    filter = cms.bool(False)                                           
)

from CommonTools.RecoUtils.pf_pu_assomap_cfi import Tracks2Vertex
selectedPrimaryVertexByLeptonMatch = cms.EDFilter("VertexByLeptonTrackMatchSelector",                                                    
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    srcLeptons = cms.VInputTag(), # NOTE: you need to set this to collections of electrons, muons and tau-jets
                                  #       passing the lepton reconstruction & identification criteria applied in your analysis                                         
    srcTrackToVertexAssociations = cms.InputTag('Tracks2Vertex'),
    dZ = cms.double(0.2), # cm
    minAssocProb = cms.double(-1.),
    filter = cms.bool(False)                                                    
)

selectPrimaryVertexByLeptonTracks = cms.Sequence(
    selectedPrimaryVertexQuality
   * selectedPrimaryVertexPosition
   * Tracks2Vertex
   * selectedPrimaryVertexByLeptonMatch
)
