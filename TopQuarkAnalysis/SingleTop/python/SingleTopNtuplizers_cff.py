import FWCore.ParameterSet.Config as cms

nTupleTopJetsPF = cms.EDProducer(
    "CandViewNtpProducer",
    src = cms.InputTag("topJetsPF"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("topJetsPF"),
    
    variables = cms.VPSet(
    cms.PSet(
    #B-Tagging
    tag = cms.untracked.string("TrackCountingHighPur"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighPurBJetTags')"),
    ),
    cms.PSet(
    tag = cms.untracked.string("TrackCountingHighEff"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighEffBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SecondaryVertexHighEffBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('simpleSecondaryVertexHighEffBJetTags')"),
    ),
    cms.PSet(
    tag = cms.untracked.string("SecondaryVertexHighPurBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('simpleSecondaryVertexHighPurBJetTags')"),
    ),
    ##    4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Flavour
    cms.PSet(
    tag = cms.untracked.string("Flavour"),
    quantity = cms.untracked.string("partonFlavour")
    ),
    #JEC factor to uncorrected jet
    cms.PSet(
    tag = cms.untracked.string("JetCorrTotal"),
    quantity = cms.untracked.string("jecFactor('Uncorrected')")
    ),
    )
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
    src = cms.InputTag("tightElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("tightElectrons"),
    variables = cms.VPSet(
    #4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Charge
    cms.PSet(
    tag = cms.untracked.string("Charge"),
    quantity = cms.untracked.string("charge")
    ),
    #Iso
    cms.PSet(
    tag = cms.untracked.string("RelIso"),
    quantity = cms.untracked.string("(dr03TkSumPt() + max(0., dr03EcalRecHitSumEt - 1.) + dr03HcalTowerSumEt)/et")
    ),
    cms.PSet(
    tag = cms.untracked.string("PFRelIso"),
    quantity = cms.untracked.string('(chargedHadronIso+ neutralHadronIso + photonIso )/pt'),
    ),
    #ID and other parameters
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId70cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId70cIso')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId60cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId60cIso')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId95cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId95cIso')")
    ),
    cms.PSet(
    tag = cms.untracked.string("EidRobustLoose"),
    quantity = cms.untracked.string("electronID('eidRobustLoose')")
    ),
    cms.PSet(
    tag = cms.untracked.string("EidRobustTight"),
    quantity = cms.untracked.string("electronID('eidRobustTight')")
    ),
    cms.PSet(
    tag = cms.untracked.string("EidRobustHighEnergy"),
    quantity = cms.untracked.string("electronID('eidRobustHighEnergy')")
    ),
#    cms.PSet(
#    tag = cms.untracked.string("SuperClusterEta"),
#    quantity = cms.untracked.string("superCluster.eta")
#    ),
    cms.PSet(
    tag = cms.untracked.string("AbsoluteDB"),
    quantity = cms.untracked.string("dB"),
    ),
#    cms.PSet(
#    tag = cms.untracked.string("TrackerExpectedInnerHits"),
#    quantity = cms.untracked.string("gsfTrack().trackerExpectedHitsInner.numberOfHits")
#    ),
    )
    )

nTupleMuons = nTupleElectrons.clone(
    src = cms.InputTag("tightMuons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("tightMuons"),
    variables = cms.VPSet(
    #4-momentum
    #4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Charge
    cms.PSet(
    tag = cms.untracked.string("Charge"),
    quantity = cms.untracked.string("charge")
    ),
    #Iso
    cms.PSet(
    tag = cms.untracked.string("RelIso"),
    quantity = cms.untracked.string("(isolationR03.sumPt + isolationR03.emEt + isolationR03.hadEt)/pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("PFRelIso"),
    quantity = cms.untracked.string('(chargedHadronIso+ neutralHadronIso + photonIso )/pt'),
    ),

    #ID and other parameters
#    cms.PSet(
#    tag = cms.untracked.string("IsGlobalMuonPromptTight"),
#    quantity = cms.untracked.string("muonID('GlobalMuonPromptTight')")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("IsGlobalMuon"),
#    quantity = cms.untracked.string("isGlobalMuon")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("IsTrackerMuon"),
#    quantity = cms.untracked.string("isTrackerMuon")
#    ),
    cms.PSet(
    tag = cms.untracked.string("AbsoluteDB"),
    quantity = cms.untracked.string("dB"),
    ),
#    cms .PSet(
#    tag = cms.untracked.string("TrackerValidInnerHits"),
#    quantity = cms.untracked.string("innerTrack.numberOfValidHits")
#    ),
    )
    )

nTupleLooseMuons = nTupleMuons.clone(
    src = cms.InputTag("looseMuons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("looseMuons"),
    variables = cms.VPSet(
    #4-momentum
    #4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Charge
    cms.PSet(
    tag = cms.untracked.string("Charge"),
    quantity = cms.untracked.string("charge")
    ),
    #Iso
    cms.PSet(
    tag = cms.untracked.string("RelIso"),
    quantity = cms.untracked.string("(isolationR03.sumPt + isolationR03.emEt + isolationR03.hadEt)/pt")
    ),
    cms.PSet(
    tag = cms.untracked.string("PFRelIso"),
    quantity = cms.untracked.string('(chargedHadronIso+ neutralHadronIso + photonIso )/pt'),
    ),

    #ID and other parameters
#    cms.PSet(
#    tag = cms.untracked.string("IsGlobalMuonPromptTight"),
#    quantity = cms.untracked.string("muonID('GlobalMuonPromptTight')")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("IsGlobalMuon"),
#    quantity = cms.untracked.string("isGlobalMuon")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("IsTrackerMuon"),
#    quantity = cms.untracked.string("isTrackerMuon")
#    ),
    cms.PSet(
    tag = cms.untracked.string("AbsoluteDB"),
    quantity = cms.untracked.string("dB"),
    ),
    #    cms .PSet(
    #    tag = cms.untracked.string("TrackerValidInnerHits"),
    #    quantity = cms.untracked.string("innerTrack.numberOfValidHits")
    #    ),
    
    )
    )

nTupleAllMuons = nTupleLooseMuons.clone(
    src = cms.InputTag("selectedPatMuons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("allMuons"),
    )

nTupleLooseElectrons = nTupleElectrons.clone(
    src = cms.InputTag("looseElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("looseElectrons"),
    variables = cms.VPSet(
    #4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Charge
    cms.PSet(
    tag = cms.untracked.string("Charge"),
    quantity = cms.untracked.string("charge")
    ),
    #Iso
    cms.PSet(
    tag = cms.untracked.string("RelIso"),
    quantity = cms.untracked.string("(dr03TkSumPt() + max(0., dr03EcalRecHitSumEt - 1.) + dr03HcalTowerSumEt)/et")
    ),
    cms.PSet(
    tag = cms.untracked.string("PFRelIso"),
    quantity = cms.untracked.string('(chargedHadronIso+ neutralHadronIso + photonIso )/pt'),
    ),
    #ID and other parameters
    cms.PSet(
    tag = cms.untracked.string("EidRobustLoose"),
    quantity = cms.untracked.string("electronID('eidRobustLoose')")
    ),
    cms.PSet(
    tag = cms.untracked.string("EidRobustTight"),
    quantity = cms.untracked.string("electronID('eidRobustTight')")
    ),
    cms.PSet(
    tag = cms.untracked.string("EidRobustHighEnergy"),
    quantity = cms.untracked.string("electronID('eidRobustHighEnergy')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId60cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId60cIso')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId70cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId70cIso')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SimpleEleId95cIso"),
    quantity = cms.untracked.string("electronID('simpleEleId95cIso')")
    ),
#    cms.PSet(
#    tag = cms.untracked.string("SuperClusterEta"),
#    quantity = cms.untracked.string("superCluster.eta")
#    ),
    cms.PSet(
    tag = cms.untracked.string("AbsoluteDB"),
    quantity = cms.untracked.string("dB"),
    ),
#    cms.PSet(
#    tag = cms.untracked.string("TrackerExpectedInnerHits"),
#    quantity = cms.untracked.string("gsfTrack().trackerExpectedHitsInner.numberOfHits")
#    ),
    )
    )

nTupleAllElectrons = nTupleLooseElectrons.clone(
    src = cms.InputTag("selectedPatElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("allElectrons"),
    )

nTupleZVetoElectrons = nTupleLooseElectrons.clone(
    src = cms.InputTag("zVetoElectrons"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("zVetoElectrons"),
    )

nTupleAllJets = nTupleTopJetsPF.clone(
    src = cms.InputTag("selectedPatJets"),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string("allJets"),
    variables = cms.VPSet(
    cms.PSet(
    #B-Tagging
    tag = cms.untracked.string("TrackCountingHighPur"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighPurBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("TrackCountingHighEff"),
    quantity = cms.untracked.string("bDiscriminator('trackCountingHighEffBJetTags')")
    ),
    cms.PSet(
    tag = cms.untracked.string("SecondaryVertexHighEffBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('simpleSecondaryVertexHighEffBJetTags')"),
    ),
    cms.PSet(
    tag = cms.untracked.string("SecondaryVertexHighPurBJetTags"),
    quantity = cms.untracked.string("bDiscriminator('simpleSecondaryVertexHighPurBJetTags')"),
    ),
    #4-momentum
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
    tag = cms.untracked.string("E"),
    quantity = cms.untracked.string("energy")
    ),
    #Flavour
    cms.PSet(
    tag = cms.untracked.string("Flavour"),
    quantity = cms.untracked.string("partonFlavour")
    ),
    #ID 
#    cms.PSet(
#    tag = cms.untracked.string("NumberOfDaughters"),
#    quantity = cms.untracked.string("numberOfDaughters")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("ChargedMultiplicity"),
#    quantity = cms.untracked.string("chargedMultiplicity")
#    ),
#    cms.PSet(
#    tag = cms.untracked.string("ChargedHadronEnergyFraction"),
 #   quantity = cms.untracked.string("chargedHadronEnergyFraction")
 #   ),
 #   cms.PSet(
 #   tag = cms.untracked.string("ChargedEmEnergyFraction"),
 #   quantity = cms.untracked.string("chargedEmEnergyFraction")
 #   ),
 #   cms.PSet(
 #   tag = cms.untracked.string("NeutralHadronEnergyFraction"),
 #   quantity = cms.untracked.string("neutralHadronEnergyFraction")
 #   ),
 #   cms.PSet(
 #   tag = cms.untracked.string("NeutralEmEnergyFraction"),
 #   quantity = cms.untracked.string("neutralEmEnergyFraction")
 #   ),
 #   #JEC factor to uncorrected jet
    cms.PSet(
    tag = cms.untracked.string("JetCorrTotal"),
    quantity = cms.untracked.string("jecFactor('Uncorrected')")
    ),
    )
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
    tag = cms.untracked.string("Pt"),
    quantity = cms.untracked.string("pt")
    ),

        cms.PSet(
    tag = cms.untracked.string("Phi"),
    quantity = cms.untracked.string("phi")
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
 


nTuplesSkim = cms.Sequence(
    nTupleTopJetsPF +
    nTupleAllJets +
    nTuplePatMETsPF +
    nTupleAllElectrons +
    nTupleAllMuons +
    nTupleLooseElectrons +
    nTupleLooseMuons +
    nTupleElectrons +
    nTupleZVetoElectrons +
    nTupleMuons
    )

saveNTuplesSkim = cms.untracked.vstring(
    'drop *',
#    'keep *_nTupleGenerator_*_*',
    'keep *_PDFInfo_*_*',
    
    'keep *_cFlavorHistoryProducer_*_*',
    'keep *_bFlavorHistoryProducer_*_*',

    'keep floats_nTupleAllJets_*_*',
    'keep floats_nTuplePatMETsPF_*_*',
    'keep floats_nTupleTopJetsPF_*_*',
    'keep *_UnclusteredMETPF_*_*',
    )



saveNTuplesSkimMu = cms.untracked.vstring(saveNTuplesSkim)
saveNTuplesSkimEle = cms.untracked.vstring(saveNTuplesSkim)

saveNTuplesSkimLoose = cms.untracked.vstring(saveNTuplesSkim)

saveNTuplesSkimMu.append('keep floats_nTupleMuons_*_*')
saveNTuplesSkimEle.append('keep floats_nTupleElectrons_*_*')


##Skimmed Ntuple
saveNTuplesSkimLoose.append('keep floats_nTupleMuons_*_*')
saveNTuplesSkimLoose.append('keep floats_nTupleElectrons_*_*')

saveNTuplesSkimLoose.append('keep floats_nTupleAllMuons_*_*')
saveNTuplesSkimLoose.append('keep floats_nTupleAllElectrons_*_*')

saveNTuplesSkimLoose.append('keep floats_nTupleLooseMuons_*_*')
saveNTuplesSkimLoose.append('keep floats_nTupleLooseElectrons_*_*')
saveNTuplesSkimLoose.append('keep floats_nTupleZVetoElectrons_*_*')

saveNTuplesSkimLoose.append('keep *_TriggerResults_*_*')


