import FWCore.ParameterSet.Config as cms

patMuonPFIsolationSelector = cms.PSet(
    # define particle-flow based isolation applied to muons in Z --> muon + tau-jet channel;
    # c.f. https://twiki.cern.ch/twiki/bin/view/CMS/Z2tautau2mujetFirstData#Default_event_selection
    
    pfCandidateSource = cms.InputTag('pfNoPileUp'),

    chargedHadronIso = cms.PSet(
        ptMin = cms.double(1.0),        
        dRvetoCone = cms.double(-1.),
        dRisoCone = cms.double(0.4)
    ),

    neutralHadronIso = cms.PSet(
        ptMin = cms.double(1.0),        
        dRvetoCone = cms.double(0.08),        
        dRisoCone = cms.double(0.4)
    ),

    photonIso = cms.PSet(
        ptMin = cms.double(1.0),        
        dPhiVeto = cms.double(-1.),  # asymmetric Eta x Phi veto region 
        dEtaVeto = cms.double(-1.),  # to account for photon conversions in electron isolation case        
        dRvetoCone = cms.double(0.05),
        dRisoCone = cms.double(0.4)
    ),

    sumPtMax = cms.double(0.10),
    sumPtMethod = cms.string("relative") # either "relative" or "absolute"
)
