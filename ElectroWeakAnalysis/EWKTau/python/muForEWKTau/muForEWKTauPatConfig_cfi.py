# The following comments couldn't be translated into the new config version:

# set isolation but don't reject non-isolated electrons
import FWCore.ParameterSet.Config as cms

allLayer0MuForEWKTau = cms.EDFilter("PATMuonCleaner",
    ## reco muon input source
    muonSource = cms.InputTag("muons"), 

    # selection (e.g. ID)
    selection = cms.PSet(
        type = cms.string('none')
    ),
    # Other possible selections:
    ## Reco-based muon selection)
    # selection = cms.PSet( type = cms.string("globalMuons") ) # pick only globalMuons
    ## ID-based selection (maybe only tracker muons?)
    # selection = cms.PSet(                                   
    #     type = cms.string("muonPOG")
    #     flag = cms.string("TMLastStationLoose")     # flag for the muon id algorithm
    #                      # "TMLastStationLoose", "TMLastStationTight"  
    #                      # "TM2DCompatibilityLoose", "TM2DCompatibilityTight" 
    #     minCaloCompatibility    = cms.double(0.0)     # cut on calo compatibility
    #     minSegmentCompatibility = cms.double(0.0)     # cut on muon segment match to tracker
    # )
    ## Custom cut-based selection (from SusyAnalyzer))
    # selection = cms.PSet( type = cms.string("custom")  
    #                       dPbyPmax = cms.double(0.5)
    #                       chi2max  = cms.double(3.0)
    #                       nHitsMin = cms.double(13) )


    markItems    = cms.bool(True), ## write the status flags in the output items
    saveAll      = cms.string(''), ## set this to a non empty label to save a list of all items both passing and failing
    saveRejected = cms.string(''), ## set this to a non empty label to save the list of items which fail
)



allLayer1MuForEWKTau = cms.EDProducer("PATMuonProducer",
    # General configurables
    muonSource = cms.InputTag("allLayer0MuForEWKTau"),

    embedTrack          = cms.bool(False), ## whether to embed in AOD externally stored tracker track
    embedCombinedMuon   = cms.bool(False), ## whether to embed in AOD externally stored combined muon track
    embedStandAloneMuon = cms.bool(False), ## whether to embed in AOD externally stored standalone muon track

    # isolation configurables
    isolation = cms.PSet(
        hcal = cms.PSet(
            src = cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowershcal"),
            deltaR = cms.double(0.3)
        ),
        tracker = cms.PSet(
            src = cms.InputTag("layer0MuonIsolations","muIsoDepositTk"),
            deltaR = cms.double(0.3)
        ),
        user = cms.VPSet(cms.PSet(
            src = cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowersho"),
            deltaR = cms.double(0.3)
        ), 
            cms.PSet(
                src = cms.InputTag("layer0MuonIsolations","muIsoDepositJets"),
                deltaR = cms.double(0.3)
            )),
        ecal = cms.PSet(
            src = cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowersecal"),
            deltaR = cms.double(0.3)
        )
    ),
    # embed IsoDeposits to recompute isolation easily
    isoDeposits = cms.PSet(
        tracker = cms.InputTag("layer0MuonIsolations","muIsoDepositTk"),
        ecal    = cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowersecal"),
        hcal    = cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowershcal"),
        user    = cms.VInputTag(
                     cms.InputTag("layer0MuonIsolations","muIsoDepositCalByAssociatorTowersho"), 
                     cms.InputTag("layer0MuonIsolations","muIsoDepositJets")
                  ),
    ),

    # Muon ID configurables
    addMuonID = cms.bool(False), ## DEPRECATED OLD TQAF muon ID. 

    # Resolution configurables
    addResolutions = cms.bool(True),
    muonResoFile = cms.string('PhysicsTools/PatUtils/data/Resolutions_muon.root'),
    useNNResolutions = cms.bool(False), ## use the neural network approach?

    # Trigger matching configurables
    addTrigMatch = cms.bool(True),
    trigPrimMatch = cms.VInputTag(cms.InputTag("muonTrigMatchHLT1MuonNonIso"), cms.InputTag("muonTrigMatchHLT1MuonIso")),

    # MC matching configurables
    addGenMatch = cms.bool(False),
    embedGenMatch = cms.bool(False),
    genParticleMatch = cms.InputTag("muonMatch"), ## particles source to be used for the matching

    # Efficiencies
    addEfficiencies = cms.bool(False),
    efficiencies    = cms.PSet(),

)

