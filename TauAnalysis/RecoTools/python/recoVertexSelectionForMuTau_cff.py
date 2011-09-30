import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Select candidates for "the" event vertex.
# "The" event vertex is selected by requiring muon + tau-jet pair
# to be associated to this vertex.
#
# Selection based on PhysicsTools/PatAlogos/plugins/PATSingleVertexSelector;
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string
#
# NOTE: offlinePrimaryVerticesWithBS collection is sorted
#       in order of decreasing sum of Pt of tracks fitted to each vertex
#--------------------------------------------------------------------------------

selectedPrimaryVertexForMuTau = cms.EDFilter("VertexByParticleMatchSelector",
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    srcParticles = cms.VInputTag(
        'selectedPatMuonsTrkIPcumulative',
        'selectedPatTausForMuTauElectronVetoCumulative',
    ),
    dZ = cms.double(0.2),                                         
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexQualityForMuTau = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexForMuTau'),
    cut = cms.string("isValid & ndof >= 7"),
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexPositionForMuTau = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexQualityForMuTau'),
    cut = cms.string("z > -24 & z < +24 & position.Rho < 2."),
    filter = cms.bool(False)                                           
)

selectedPrimaryVertexHighestPtTrackSumForMuTau = cms.EDFilter("PATSingleVertexSelector",
    mode = cms.string('firstVertex'),
    vertices = cms.InputTag('selectedPrimaryVertexPositionForMuTau'),
    filter = cms.bool(False)                                                    
)

selectPrimaryVertexForMuTau = cms.Sequence(
    selectedPrimaryVertexForMuTau
   * selectedPrimaryVertexQualityForMuTau
   * selectedPrimaryVertexPositionForMuTau
   * selectedPrimaryVertexHighestPtTrackSumForMuTau
)

#--------------------------------------------------------------------------------
# Define additional collections of event vertices
# matching muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedPrimaryVertexForMuTauLooseMuonIsolation = selectedPrimaryVertexForMuTau.clone(
    srcParticles = cms.VInputTag(
        'selectedPatMuonsTrkIPlooseIsolationCumulative',
        'selectedPatTausForMuTauElectronVetoCumulative',
    )
)    

selectedPrimaryVertexQualityForMuTauLooseMuonIsolation = selectedPrimaryVertexQualityForMuTau.clone(
    src = cms.InputTag('selectedPrimaryVertexForMuTauLooseMuonIsolation')
    )    

selectedPrimaryVertexPositionForMuTauLooseMuonIsolation = selectedPrimaryVertexPositionForMuTau.clone(
    src = cms.InputTag('selectedPrimaryVertexQualityForMuTauLooseMuonIsolation')
)    

selectedPrimaryVertexHighestPtTrackSumForMuTauLooseMuonIsolation = selectedPrimaryVertexHighestPtTrackSumForMuTau.clone(
    vertices = cms.InputTag('selectedPrimaryVertexPositionForMuTauLooseMuonIsolation')
)    

selectPrimaryVertexForMuTauLooseMuonIsolation = cms.Sequence(
    selectedPrimaryVertexForMuTauLooseMuonIsolation
   * selectedPrimaryVertexQualityForMuTauLooseMuonIsolation
   * selectedPrimaryVertexPositionForMuTauLooseMuonIsolation
   * selectedPrimaryVertexHighestPtTrackSumForMuTauLooseMuonIsolation
)

selectPrimaryVertexForMuTauAll = cms.Sequence(selectPrimaryVertexForMuTau * selectPrimaryVertexForMuTauLooseMuonIsolation)
