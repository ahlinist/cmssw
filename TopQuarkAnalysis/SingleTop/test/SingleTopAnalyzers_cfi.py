import FWCore.ParameterSet.Config as cms

PlotsEle = cms.EDAnalyzer('SingleTopSystematicsDumper',                              
systematics = cms.untracked.vstring("BTagUp","BTagDown","MisTagUp","MisTagDown","JESUp","JESDown","UnclusteredMETUp","UnclusteredMETDown"),
rateSystematics = cms.untracked.vstring("WLightRate","TTBarRate","WqqRate","WqRate"),
channelInfo = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
#    originalEvents = cms.untracked.double(14800000),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(14.5),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    loosePtCut = cms.untracked.double(30.0),#Default 30.0 GeV
    ),


#Part of the kin quantities:
leptonsPz = cms.InputTag("nTupleElectrons","tightElectronsPz"),  
leptonsPx = cms.InputTag("nTupleElectrons","tightElectronsPx"),  
leptonsPy = cms.InputTag("nTupleElectrons","tightElectronsPy"),  
leptonsEnergy = cms.InputTag("nTupleElectrons","tightElectronsE"),  
leptonsCharge = cms.InputTag("nTupleElectrons","tightElectronsCharge"),  

jetsPx = cms.InputTag("nTupleTopJetsPF","topJetsPFPx"),  
jetsPy = cms.InputTag("nTupleTopJetsPF","topJetsPFPy"),  
jetsPz = cms.InputTag("nTupleTopJetsPF","topJetsPFPz"),  
jetsEnergy = cms.InputTag("nTupleTopJetsPF","topJetsPFE"),  

jetsBTagAlgo = cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighPur"),  
jetsAntiBTagAlgo =  cms.InputTag("nTupleTopJetsPF","topJetsPFTrackCountingHighEff"),  
jetsFlavour = cms.InputTag("nTupleTopJetsPF","topJetsPFFlavour"),   

jetsCorrTotal = cms.InputTag("nTupleTopJetsPF","topJetsPFJetCorrTotal"),   

METPhi = cms.InputTag("nTuplePatMETsPF","patMETsPFPhi"),
METPt = cms.InputTag("nTuplePatMETsPF","patMETsPFPt"),

UnclusteredMETPx = cms.InputTag("UnclusteredMETPF","UnclusteredMETPx"),
UnclusteredMETPy = cms.InputTag("UnclusteredMETPF","UnclusteredMETPy"),

)


PlotsMu = PlotsEle.clone(
    channelInfo = cms.PSet(
        crossSection = cms.untracked.double(20.93),
            channel = cms.untracked.string("TChannel"),
        #    originalEvents = cms.untracked.double(14800000),
            originalEvents = cms.untracked.double(480000),
            finalLumi = cms.untracked.double(14.5),
            MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
            ),

leptonsPz = cms.InputTag("nTupleMuons","tightMuonsPz"),  
leptonsPx = cms.InputTag("nTupleMuons","tightMuonsPx"),  
leptonsPy = cms.InputTag("nTupleMuons","tightMuonsPy"),  
leptonsEnergy = cms.InputTag("nTupleMuons","tightMuonsE"),  
leptonsCharge = cms.InputTag("nTupleMuons","tightMuonsCharge"),  

    
    )

