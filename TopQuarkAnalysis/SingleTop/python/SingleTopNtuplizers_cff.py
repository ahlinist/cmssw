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
   
        cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
    ),

        cms.PSet(
    tag = cms.untracked.string("Px"),
    quantity = cms.untracked.string("px")
    ),

        cms.PSet(
    tag = cms.untracked.string("Py"),
    quantity = cms.untracked.string("py")
    ),

        cms.PSet(
    tag = cms.untracked.string("Pz"),
    quantity = cms.untracked.string("pz")
    ),

        cms.PSet(
    tag = cms.untracked.string("P"),
    quantity = cms.untracked.string("p")
    ),

        cms.PSet(
    tag = cms.untracked.string("Theta"),
    quantity = cms.untracked.string("theta")
    ),

        cms.PSet(
    tag = cms.untracked.string("TrackCountingHighPurBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighPurBJetTags')")
    ),

        cms.PSet(
    tag = cms.untracked.string("TrackCountingHighEffBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighEffBJetTags')")
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


        cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
    ),

        cms.PSet(
    tag = cms.untracked.string("Px"),
    quantity = cms.untracked.string("px")
    ),

        cms.PSet(
    tag = cms.untracked.string("Py"),
    quantity = cms.untracked.string("py")
    ),

        cms.PSet(
    tag = cms.untracked.string("Pz"),
    quantity = cms.untracked.string("pz")
    ),


        cms.PSet(
    tag = cms.untracked.string("P"),
    quantity = cms.untracked.string("p")
    ),

        cms.PSet(
    tag = cms.untracked.string("Theta"),
    quantity = cms.untracked.string("theta")
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
    tag = cms.untracked.string("LeptonPx"),
    quantity = cms.untracked.string("daughter('Lepton').px")
    ),
    cms.PSet(
    tag = cms.untracked.string("LeptonPy"),
    quantity = cms.untracked.string("daughter('Lepton').py")
    ),
    cms.PSet(
    tag = cms.untracked.string("LeptonPz"),
    quantity = cms.untracked.string("daughter('Lepton').pz")
    ),

    cms.PSet(
    tag = cms.untracked.string("LeptonPhi"),
    quantity = cms.untracked.string("daughter('Lepton').phi")
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
    tag = cms.untracked.string("METPhi"),
    quantity = cms.untracked.string("daughter('MET').phi")
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


singleTopMCNeutrinos = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("MCTruthParticles","topNeutrinos"),
    prefix = cms.untracked.string("mcNeutrinos"),

    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("PdgId"),
    quantity = cms.untracked.string("pdgId")
    ),


    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),

        cms.PSet(
    tag = cms.untracked.string("Eta"),
    quantity = cms.untracked.string("eta")
    ),

        cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
    ),

        cms.PSet(
    tag = cms.untracked.string("Px"),
    quantity = cms.untracked.string("px")
    ),

        cms.PSet(
    tag = cms.untracked.string("Py"),
    quantity = cms.untracked.string("py")
    ),

        cms.PSet(
    tag = cms.untracked.string("Pz"),
    quantity = cms.untracked.string("pz")
    ),

        cms.PSet(
    tag = cms.untracked.string("P"),
    quantity = cms.untracked.string("p")
    ),

        cms.PSet(
    tag = cms.untracked.string("Theta"),
    quantity = cms.untracked.string("theta")
    ),


    ),

)

singleTopMCLeptons = singleTopMCNeutrinos.clone( src = cms.InputTag("MCTruthParticles","topLeptons"), prefix = cms.untracked.string("mcLeptons"))

singleTopMCRecoilQuark = singleTopMCNeutrinos.clone( src = cms.InputTag("MCTruthParticles","singleTopRecoilQuark"), prefix = cms.untracked.string("mcRecoilQuark"))
singleTopMCBQuark = singleTopMCNeutrinos.clone( src = cms.InputTag("MCTruthParticles","bGenParticles"), prefix = cms.untracked.string("mcBQuark"))
 


singleTopPreJetsCutsTops = singleTopFinalTops.clone(prefix = cms.untracked.string("preJetsCutsTops") )

singleTopPreselectedMETs = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("preselectedMETs"),
    prefix = cms.untracked.string("allMETs"),

    variables = cms.VPSet(
    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),

        cms.PSet(
    tag = cms.untracked.string("Eta"),
    quantity = cms.untracked.string("eta")
    ),

        cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
    ),

        cms.PSet(
    tag = cms.untracked.string("Px"),
    quantity = cms.untracked.string("px")
    ),

        cms.PSet(
    tag = cms.untracked.string("Py"),
    quantity = cms.untracked.string("py")
    ),

        cms.PSet(
    tag = cms.untracked.string("Pz"),
    quantity = cms.untracked.string("pz")
    ),

        cms.PSet(
    tag = cms.untracked.string("P"),
    quantity = cms.untracked.string("p")
    ),

        cms.PSet(
    tag = cms.untracked.string("Theta"),
    quantity = cms.untracked.string("theta")
    ),
    )
    
    )
