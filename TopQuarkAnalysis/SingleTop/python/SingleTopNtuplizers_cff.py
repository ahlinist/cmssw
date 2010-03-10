import FWCore.ParameterSet.Config as cms


singleTopPreselectedJets = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("preselectedJets"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("bJets"),
    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("Eta"),
    quantity = cms.untracked.string("eta")
    ),
   
    ),

    )


singleTopPreLeptonsCutsJets = singleTopPreselectedJets.clone(src = cms.InputTag("topJets"),prefix = cms.untracked.string("preLeptonsCutsJets") )
singleTopJets = singleTopPreLeptonsCutsJets.clone(prefix = cms.untracked.string("topJets"))

singleTopBJets = singleTopPreselectedJets.clone(src = cms.InputTag("bJets"), prefix = cms.untracked.string("bJets") )
singleTopForwardJets = singleTopPreselectedJets.clone(src = cms.InputTag("forwardJets"), prefix = cms.untracked.string("forwardJets"))


singleTopFinalElectrons = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("topElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("finalElectrons"),
    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),

    cms.PSet(
    tag = cms.untracked.string("relIso"),
    quantity = cms.untracked.string("pt/(pt + trackIso + caloIso)")
    ),
    
    ),

    )

singleTopFinalMuons = singleTopFinalElectrons.clone(src = cms.InputTag("topMuons"),prefix = cms.untracked.string("finalMuons") )
singleTopPreJetsCutsElectrons = singleTopFinalElectrons.clone(prefix = cms.untracked.string("preJetsCutsElectrons") )
singleTopPreJetsCutsMuons = singleTopFinalMuons.clone(prefix = cms.untracked.string("preJetsCutsMuons"))

singleTopFinalTops = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("recoTops"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("finalTops"),
    variables = cms.VPSet(
    
    cms.PSet(
    tag = cms.untracked.string("Mass"),
    quantity = cms.untracked.string("mass")
    ),

    cms.PSet(
    tag = cms.untracked.string("LeptonPt"),
    quantity = cms.untracked.string("daughter('Lepton').pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("LeptonEta"),
    quantity = cms.untracked.string("daughter('Lepton').eta")
    ),

    cms.PSet(
    tag = cms.untracked.string("METPt"),
    quantity = cms.untracked.string("daughter('MET').pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("METPy"),
    quantity = cms.untracked.string("daughter('MET').py")
    ),
    cms.PSet(
    tag = cms.untracked.string("METPx"),
    quantity = cms.untracked.string("daughter('MET').px")
    ),    

    cms.PSet(
    tag = cms.untracked.string("WMass"),
    quantity = cms.untracked.string("daughter('W').mass")
    ),

    cms.PSet(
    tag = cms.untracked.string("BJetPt"),
    quantity = cms.untracked.string("daughter('BJet').pt")
    ),

    cms.PSet(
    tag = cms.untracked.string("BJetEta"),
    quantity = cms.untracked.string("daughter('BJet').eta")
    ),
    
    ),
    ) 


singleTopPreJetsCutsTops = singleTopFinalTops.clone(prefix = cms.untracked.string("preJetsCutsTops") )
