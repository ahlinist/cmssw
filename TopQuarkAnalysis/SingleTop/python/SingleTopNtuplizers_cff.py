import FWCore.ParameterSet.Config as cms

nTupleEventsPF = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("recoTChanEventsPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("tChanCandidate"),
    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("topMass"),
    quantity = cms.untracked.string("daughter('Top').mass()")
    ),

    cms.PSet(
    tag = cms.untracked.string("bJetTrackCountingHighPur"),
    quantity = cms.untracked.string("daughter('Top').daughter('BJet').bDiscriminator('trackCountingHighPurBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("forwardJetTrackCountingHighEff"),
    quantity = cms.untracked.string("daughter('LightJet').bDiscriminator('trackCountingHighEffBJetTags')")
    ),

    cms.PSet(
    tag = cms.untracked.string("forwardJetEta"),
    quantity = cms.untracked.string("daughter('LightJet').eta()")
    ),


    cms.PSet(
    tag = cms.untracked.string("bJetPt"),
    quantity = cms.untracked.string("daughter('Top').daughter('BJet').pt()")
    ),
    cms.PSet(
    tag = cms.untracked.string("forwardJetPt"),
    quantity = cms.untracked.string("daughter('LightJet').pt()")
    ),
    

    )
    )

nTupleEventsAntiIsoPF = nTupleEventsPF.clone(
    src = cms.InputTag("recoTChanEventsAntiIsoPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("tChanCandidateAntiIso"),
    )

nTupleTopJetsPF = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("topJetsPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topJetsPF"),
    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("TrackCountingHighPur"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighPurBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("TrackCountingHighEff"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighEffBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("Eta"),
    quantity = cms.untracked.string("eta")
    ),
    )
)

nTupleTopJetsAntiIsoPF = nTupleTopJetsPF.clone(
    src = cms.InputTag("topJetsAntiIsoPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topJetsAntiIsoPF"),
)
nTupleForwardJetsPF= nTupleTopJetsPF.clone(
    src = cms.InputTag("forwardJetsPF"),
    prefix = cms.untracked.string("forwardJetsPF"),
    )

nTupleBJetsPF= nTupleTopJetsPF.clone(
    src = cms.InputTag("bJetsPF"),
    prefix = cms.untracked.string("bJetsPF"),
    )

nTuplePatMETsPF = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("patMETsPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("patMETsPF"),
    variables = cms.VPSet(

    cms.PSet(
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),

    cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
    ),

    )
    )

nTupleElectrons = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("topElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topElectrons"),
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

    )
    )

nTupleMuons = nTupleElectrons.clone(
    src = cms.InputTag("topMuons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topMuons"),
    )

nTupleMuonsAntiIso = nTupleMuons.clone(
    src = cms.InputTag("topMuonsAntiIso"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topMuonsAntiIso"),
    )

nTupleElectronsAntiIso = nTupleMuons.clone(
    src = cms.InputTag("topElectronsAntiIso"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topElectronsAntiIso"),
    )

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



nTuples = cms.Sequence(
    nTupleTopJetsPF *
    #nTupleBJetsPF *
    #nTupleForwardJetsPF *
    nTuplePatMETsPF *
    nTupleEventsPF *
    nTupleElectrons *
    nTupleMuons
    )

nTuplesAntiIso = cms.Sequence(
    nTupleTopJetsAntiIsoPF *
    #nTupleBJetsAntiIsoPF *
    #nTupleForwardJetsAntiIsoPF *
    nTuplePatMETsPF *
    nTupleEventsAntiIsoPF *
    nTupleElectronsAntiIso *
    nTupleMuonsAntiIso
    )

saveNTuples = cms.untracked.vstring(
    'drop *',

    'keep *_cFlavorHistoryProducer_*_*',
    'keep *_bFlavorHistoryProducer_*_*',


    'keep *_singleTopObservablesTSamplePF_*_*',    
    'keep floats_nTupleEventsPF_*_*',
    'keep floats_nTuplePatMETsPF_*_*',
    'keep floats_nTupleTopJetsPF_*_*',

    
    )

saveNTuplesAntiIso = cms.untracked.vstring(
    'drop *',
    
    'keep *_cFlavorHistoryProducer_*_*',
    'keep *_bFlavorHistoryProducer_*_*',


    'keep *_singleTopObservablesAntiIsoPF_*_*',
    'keep floats_nTupleEventsAntiIsoPF_*_*',
    'keep floats_nTuplePatMETsPF_*_*',
    'keep floats_nTupleTopJetsAntiIsoPF_*_*',

    
    )


saveNTuplesMu = saveNTuples
saveNTuplesEle = saveNTuples

saveNTuplesMuAntiIso = saveNTuplesAntiIso
saveNTuplesEleAntiIso = saveNTuples

saveNTuplesMu.extend(['keep floats_nTupleMuons_*_*'])
saveNTuplesEle.extend(['keep floats_nTupleElectrons_*_*'])

saveNTuplesMuAntiIso.extend(['keep floats_nTupleMuonsAntiIso_*_*'])
saveNTuplesEleAntiIso.extend(['keep floats_nTupleElectronsAntiIso_*_*'])
