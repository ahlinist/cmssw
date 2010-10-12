import FWCore.ParameterSet.Config as cms

tauPFIsolationHistManager = cms.PSet(    
    pluginName = cms.string('tauPFIsolationHistManager'),
    pluginType = cms.string('TauPFIsolationHistManager'),

    dqmDirectory_store = cms.string('TauPFIsolationQuantities'),

    leptonSource = cms.InputTag("patTaus"),
    ptMin = cms.double(20.),
    ptMax = cms.double(100.),
    etaMin = cms.double(-2.1),
    etaMax = cms.double(+2.1),

    dRisoCone = cms.double(0.5),

    pfCandidateSource = cms.InputTag("particleFlow"),

    genLeptonMatch = cms.PSet(
        genParticleSource = cms.InputTag("genParticles"),
        dRmatch = cms.double(0.5),
        genTauJetSource = cms.InputTag("tauGenJets"),                                              
        tauDecayModes = cms.vstring(
##--- define names of tau lepton decay modes
##   ( using names defined in PhysicsTools/JetMCUtils/src/JetMCTag.cc )            
            "oneProng0Pi0",
            "oneProng1Pi0",
            "oneProng2Pi0",
            "oneProngOther",
            "threeProng0Pi0",
            "threeProng1Pi0",
            "threeProngOther",
            "rare"
        )
    )
)

muonPFIsolationHistManager = tauPFIsolationHistManager.clone(
    pluginName = cms.string('muonPFIsolationHistManager'),
    pluginType = cms.string('MuonPFIsolationHistManager'),

    dqmDirectory_store = cms.string('MuonPFIsolationQuantities'),

    leptonSource = cms.InputTag("patMuons"),

    genLeptonMatch = cms.PSet(
        genParticleSource = cms.InputTag("genParticles"),
        dRmatch = cms.double(0.5)
    )
)

electronPFIsolationHistManager = tauPFIsolationHistManager.clone(
    pluginName = cms.string('electronPFIsolationHistManager'),
    pluginType = cms.string('ElectronPFIsolationHistManager'),

    dqmDirectory_store = cms.string('ElectronPFIsolationQuantities'),

    leptonSource = cms.InputTag("patElectrons"),

    genLeptonMatch = cms.PSet(
        genParticleSource = cms.InputTag("genParticles"),
        dRmatch = cms.double(0.5)
    )
)


