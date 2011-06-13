import FWCore.ParameterSet.Config as cms

patMuonPFIsolationSelector = cms.PSet(
    # define particle-flow based isolation applied to muons in Z --> muon + tau-jet channel;
    # c.f. https://twiki.cern.ch/twiki/bin/view/CMS/Z2tautau2mujetFirstData#Default_event_selection
    
    pfCandidateSource = cms.InputTag('particleFlow'),

    # CV: configuration parameters 'vertexSource' and 'beamSpotSource'
    #     need to be set if using deltaBeta pile-up corrections
    vertexSource = cms.InputTag('offlinePrimaryVerticesWithBS'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),

    # CV: configuration parameters 'rhoFastJetSource' and 'ueRhoOffset'
    #     need to be set if using rho (FastJet) pile-up corrections
    #rhoFastJetSource = cms.InputTag('kt6PFJets', 'rho'),
        
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
    sumPtMethod = cms.string("relative"), # either "relative" or "absolute"

    pileUpCorr = cms.PSet(
        method = cms.string("deltaBeta"),
        ##method = cms.string("rho"),
        deltaZ = cms.double(0.2),
        chargedToNeutralFactor = cms.double(0.38), # (pi+ + pi-)/pi0 ratio in QCD/min. bias events
        ueRhoOffset = cms.double(0.), # [GeV], energy density offset to be subtracted from rho computed by FastJet algorithm
    )
)

patElectronPFIsolationSelector = cms.PSet(
    
    pfCandidateSource = cms.InputTag('pfNoPileUp'),

    vertexSource = cms.InputTag('offlinePrimaryVerticesWithBS'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),
    
    # CV: configuration parameters 'rhoFastJetSource' and 'ueRhoOffset'
    #     need to be set if using rho (FastJet) pile-up corrections
    #rhoFastJetSource = cms.InputTag('kt6PFJets', 'rho'),
        
    chargedHadronIso = cms.PSet(
        ptMin = cms.double(0.5),        
        dRvetoCone = cms.double(0.001),  # "sanity" check, to match Wisconsin's selection
        dRisoCone = cms.double(0.4),
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

    sumPtMaxEB = cms.double(0.08),
    sumPtMaxEE = cms.double(0.04),
    sumPtMethod = cms.string("relative"), # either "relative" or "absolute"
    
    pileUpCorr = cms.PSet(
        method = cms.string("deltaBeta"),
        deltaZ = cms.double(0.2),
        chargedToNeutralFactor = cms.double(0.38) # (pi+ + pi-)/pi0 ratio in QCD/min. bias events
    )
)
