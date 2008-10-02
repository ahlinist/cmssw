import FWCore.ParameterSet.Config as cms

pfRecoTauTagInfoProducer = cms.EDProducer("PFRecoTauTagInfoProducer",
    tkminTrackerHitsn = cms.int32(3),
    #string PVProducer                         = "pixelVertices"
    PVProducer = cms.string('offlinePrimaryVertices'),
    tkmaxChi2 = cms.double(100.0),
    # parameters of the considered charged hadr. PFCandidates, based on their rec. Track properties :
    ChargedHadrCand_AssociationCone = cms.double(0.8),
    ChargedHadrCand_tkminTrackerHitsn = cms.int32(3),
    ChargedHadrCand_tkmaxChi2 = cms.double(100.0),
    tkPVmaxDZ = cms.double(0.2), ##considered if UsePVconstraint=true    

    tkminPixelHitsn = cms.int32(0),
    # parameters of the considered rec. Tracks (these ones were catched through a JetTracksAssociation object, not through the charged hadr. PFCandidates inside the PFJet ; the motivation for considering them is the need for checking that a selection by the charged hadr. PFCandidates is equivalent to a selection by the rec. Tracks.) :
    tkminPt = cms.double(1.0),
    PFCandidateProducer = cms.string('particleFlow'),
    ChargedHadrCand_tkminPt = cms.double(1.0),
    #
    UsePVconstraint = cms.bool(False),
    ChargedHadrCand_tkmaxipt = cms.double(0.03),
    # parameters of the considered neutral hadr. PFCandidates, based on their rec. HCAL cluster properties : 
    NeutrHadrCand_HcalclusminE = cms.double(1.0),
    ChargedHadrCand_tkminPixelHitsn = cms.int32(0),
    # parameters of the considered gamma PFCandidates, based on their rec. ECAL cluster properties :
    GammaCand_EcalclusminE = cms.double(1.0),
    PFJetTracksAssociatorProducer = cms.string('ic5PFJetTracksAssociatorAtVertex'),
    smearedPVsigmaY = cms.double(0.0015),
    smearedPVsigmaX = cms.double(0.0015),
    smearedPVsigmaZ = cms.double(0.005),
    ChargedHadrCand_tkPVmaxDZ = cms.double(0.2), ##considered if UsePVconstraint=true    	

    tkmaxipt = cms.double(0.03)
)


