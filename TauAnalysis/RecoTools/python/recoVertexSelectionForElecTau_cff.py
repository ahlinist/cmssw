import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Select candidates for "the" event vertex.
# "The" event vertex is selected by requiring electron + tau-jet pair
# to be associated to this vertex.
#
# Selection based on PhysicsTools/PatAlogos/plugins/PATSingleVertexSelector;
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string
#
# NOTE: offlinePrimaryVerticesWithBS collection is sorted
#       in order of decreasing sum of Pt of tracks fitted to each vertex
#--------------------------------------------------------------------------------

selectedPrimaryVertexForElecTau = cms.EDFilter("VertexByParticleMatchSelector",
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    srcParticles = cms.VInputTag(
        'selectedPatElectronsForElecTauTrkIPcumulative',
        'selectedPatTausForElecTauMuonVetoCumulative',
    ),
    dZ = cms.double(0.2),                                         
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexQualityForElecTau = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexForElecTau'),
    cut = cms.string("isValid & ndof >= 4"),
    filter = cms.bool(False)                                          
)

selectedPrimaryVertexPositionForElecTau = cms.EDFilter("VertexSelector",
    src = cms.InputTag('selectedPrimaryVertexQualityForElecTau'),
    cut = cms.string("z > -24 & z < +24 & position.Rho < 2."),
    filter = cms.bool(False)                                           
)

selectedPrimaryVertexHighestPtTrackSumForElecTau = cms.EDFilter("PATSingleVertexSelector",
    mode = cms.string('firstVertex'),
    vertices = cms.InputTag('selectedPrimaryVertexPositionForElecTau'),
    filter = cms.bool(False)                                                    
)

selectPrimaryVertexForElecTau = cms.Sequence(
    selectedPrimaryVertexForElecTau
   * selectedPrimaryVertexQualityForElecTau
   * selectedPrimaryVertexPositionForElecTau
   * selectedPrimaryVertexHighestPtTrackSumForElecTau
)

#--------------------------------------------------------------------------------
# Define additional collections of event vertices
# matching electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedPrimaryVertexForElecTauLooseElectronIsolation = selectedPrimaryVertexForElecTau.clone(
    srcParticles = cms.VInputTag(
        'selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative',
        'selectedPatTausForElecTauMuonVetoCumulative',
    )
)    

selectedPrimaryVertexQualityForElecTauLooseElectronIsolation = selectedPrimaryVertexQualityForElecTau.clone(
    src = cms.InputTag('selectedPrimaryVertexForElecTauLooseElectronIsolation')
    )    

selectedPrimaryVertexPositionForElecTauLooseElectronIsolation = selectedPrimaryVertexPositionForElecTau.clone(
    src = cms.InputTag('selectedPrimaryVertexQualityForElecTauLooseElectronIsolation')
)    

selectedPrimaryVertexHighestPtTrackSumForElecTauLooseElectronIsolation = selectedPrimaryVertexHighestPtTrackSumForElecTau.clone(
    vertices = cms.InputTag('selectedPrimaryVertexPositionForElecTauLooseElectronIsolation')
)    

selectPrimaryVertexForElecTauLooseElectronIsolation = cms.Sequence(
    selectedPrimaryVertexForElecTauLooseElectronIsolation
   * selectedPrimaryVertexQualityForElecTauLooseElectronIsolation
   * selectedPrimaryVertexPositionForElecTauLooseElectronIsolation
   * selectedPrimaryVertexHighestPtTrackSumForElecTauLooseElectronIsolation
)

selectPrimaryVertexForElecTauAll = cms.Sequence(selectPrimaryVertexForElecTau * selectPrimaryVertexForElecTauLooseElectronIsolation)
