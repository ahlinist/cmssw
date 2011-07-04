import FWCore.ParameterSet.Config as cms

TreesEle = cms.EDAnalyzer('SingleTopSystematicsTreesDumper',                              
systematics = cms.untracked.vstring("BTagUp","BTagDown","MisTagUp","MisTagDown","JESUp","JESDown","UnclusteredMETUp","UnclusteredMETDown"),
#systematics = cms.untracked.vstring(""),
doBScan = cms.untracked.bool(True),
#rateSystematics = cms.untracked.vstring("WLightRate","TTBarRate","WqqRate","WqRate"),
rateSystematics = cms.untracked.vstring("WLightRateUp",
                                        "WLightRateDown",
                                        "TTBarRateUp",
                                        "Ttbarratedown ",
                                        "WHFRateUp",
                                        "WHFRateDown"),
#rateSystematics = cms.untracked.vstring(""),
leptonsID = cms.InputTag("nTupleElectrons","tightElectronsSimpleEleId70cIso"),  
#mode = cms.untracked.string("pt"),
maxPtCut = cms.untracked.double("45"),

channelInfo = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
#    originalEvents = cms.untracked.double(14800000),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(14.5),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    loosePtCut = cms.untracked.double(30.0),#Default 30.0 GeV
    RelIsoCut = cms.untracked.double(0.1)
    ),


#Part of the kin quantities:
leptonsEta = cms.InputTag("nTupleElectrons","tightElectronsEta"),  
leptonsPt = cms.InputTag("nTupleElectrons","tightElectronsPt"),  
leptonsPhi = cms.InputTag("nTupleElectrons","tightElectronsPhi"),  
leptonsEnergy = cms.InputTag("nTupleElectrons","tightElectronsE"),  
leptonsCharge = cms.InputTag("nTupleElectrons","tightElectronsCharge"),  
leptonsRelIso = cms.InputTag("nTupleElectrons","tightElectronsRelIso"),  
leptonsDB = cms.InputTag("nTupleElectrons","tightElectronsRelIso"),  

looseElectronsRelIso = cms.InputTag("nTupleLooseElectrons","looseElectronsRelIso"),  
looseMuonsRelIso = cms.InputTag("nTupleLooseMuons","looseMuonsRelIso"),  

leptonsFlavour = cms.untracked.string("electron"),

jetsPt = cms.InputTag("nTupleTopJetsPF","topJetsPFPt"),  
jetsPhi = cms.InputTag("nTupleTopJetsPF","topJetsPFPhi"),  
jetsEta = cms.InputTag("nTupleTopJetsPF","topJetsPFEta"),  
jetsEnergy = cms.InputTag("nTupleTopJetsPF","topJetsPFE"),  

jetsBTagAlgo = cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighPur"),  
jetsAntiBTagAlgo =  cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighEff"),  
jetsFlavour = cms.InputTag("nTupleTopJetsPF","topJetsPFFlavour"),   

jetsCorrTotal = cms.InputTag("nTupleTopJetsPF","topJetsPFJetCorrTotal"),   

METPhi = cms.InputTag("nTuplePatMETsPF","patMETsPFPhi"),
METPt = cms.InputTag("nTuplePatMETsPF","patMETsPFPt"),

nvertices = cms.InputTag("NVertices","PileUpSync"),
UnclusteredMETPx = cms.InputTag("UnclusteredMETPF","UnclusteredMETPx"),
UnclusteredMETPy = cms.InputTag("UnclusteredMETPF","UnclusteredMETPy"),

)


TreesMu = TreesEle.clone(
    channelInfo = cms.PSet(
        crossSection = cms.untracked.double(20.93),
        channel = cms.untracked.string("TChannel"),
        #    originalEvents = cms.untracked.double(14800000),
        originalEvents = cms.untracked.double(480000),
        finalLumi = cms.untracked.double(14.5),
        MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
        RelIsoCut = cms.untracked.double(0.05),
        ),
    leptonsEta = cms.InputTag("nTupleMuons","tightMuonsEta"),  
    leptonsPt = cms.InputTag("nTupleMuons","tightMuonsPt"),  
    leptonsPhi = cms.InputTag("nTupleMuons","tightMuonsPhi"),  
    leptonsEnergy = cms.InputTag("nTupleMuons","tightMuonsE"),  
    leptonsCharge = cms.InputTag("nTupleMuons","tightMuonsCharge"),  
    leptonsRelIso = cms.InputTag("nTupleMuons","tightMuonsRelIso"),  
    leptonsDB = cms.InputTag("nTupleMuons","tightMuonsRelIso"),  
    leptonsID = cms.InputTag("nTupleElectrons","tightElectronsSimpleEleId70cIso"),  
    leptonsFlavour = cms.untracked.string("muon"),

    
    )

