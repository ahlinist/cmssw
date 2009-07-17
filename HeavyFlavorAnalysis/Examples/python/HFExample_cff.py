import FWCore.ParameterSet.Config as cms

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
from Configuration.StandardSequences.MagneticField_cff import *
from SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoMET.Configuration.GenMETParticles_cff import *

goodTracks = cms.EDFilter("CandSelector",
    filter = cms.bool(False),
    src = cms.InputTag("generalTracks"),
    cut = cms.string('charge != 0')
)

caloJetCollectionClone = cms.EDProducer("CaloJetShallowCloneProducer",
    src = cms.InputTag('iterativeCone5CaloJets')
)

caloJetSele = cms.EDFilter("PtMinCandSelector",
    src = cms.InputTag("caloJetCollectionClone"),
    ptMin = cms.double(10.0)
)

tagJetAlgo = cms.EDFilter("JetFlavourIdentifier",
    jets = cms.InputTag("caloJetSele"),
    debug = cms.bool(False),
    coneSizeToAssociate = cms.double(0.3),
    vetoFlavour = cms.vstring(),
    physicsDefinition = cms.bool(False),
    srcByReference = cms.InputTag("caloJetSele")
)

tagJetPhys = cms.EDFilter("JetFlavourIdentifier",
    jets = cms.InputTag("caloJetSele"),
    debug = cms.bool(False),
    coneSizeToAssociate = cms.double(0.3),
    vetoFlavour = cms.vstring(),
    physicsDefinition = cms.bool(True),
    srcByReference = cms.InputTag("caloJetSele")                                                       
)

genJetCollectionClone = cms.EDProducer("GenJetShallowCloneProducer",
    src = cms.InputTag("iterativeCone5GenJets")
)

genJetSele = cms.EDFilter("PtMinCandSelector",
    src = cms.InputTag("genJetCollectionClone"),
    ptMin = cms.double(10.0)
)

gentagJetAlgo = cms.EDFilter("JetFlavourIdentifier",
    jets = cms.InputTag("genJetSele"),
    debug = cms.bool(False),
    coneSizeToAssociate = cms.double(0.3),
    vetoFlavour = cms.vstring(),
    physicsDefinition = cms.bool(False),
    srcByReference = cms.InputTag("genJetSele")                                                          
)

gentagJetPhys = cms.EDFilter("JetFlavourIdentifier",
    jets = cms.InputTag("genJetSele"),
    debug = cms.bool(False),
    coneSizeToAssociate = cms.double(0.3),
    vetoFlavour = cms.vstring(),
    physicsDefinition = cms.bool(True),
    srcByReference = cms.InputTag("genJetSele")                             
)

tree = cms.EDFilter("HFTree",
    fileName = cms.string('test.root')
)

genDump = cms.EDFilter("HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('evtgenproducer')
)

trkDump = cms.EDFilter("HFDumpTracks",
    simTracksLabel = cms.untracked.string('famosSimHits'),
    verbose = cms.untracked.int32(0),
    generatorEventLabel = cms.untracked.string('evtgenproducer'),
    muonsLabel = cms.untracked.InputTag("muons"),
    trackingParticlesLabel = cms.untracked.string('trackingParticles'),
    associatorLabel = cms.untracked.string('TrackAssociatorByChi2'),
    doTruthMatching = cms.untracked.int32(2),
    tracksLabel = cms.untracked.string('generalTracks')
)

signalDump = cms.EDFilter("HFDumpSignal",
    deltaR = cms.untracked.double(1.5),
    verbose = cms.untracked.int32(1),
    muonsLabel = cms.untracked.InputTag("muons"),
    muonPt = cms.untracked.double(3.0),
    kaonPt = cms.untracked.double(1.0),
    tracksLabel = cms.untracked.string('generalTracks')
)

jetDump = cms.EDFilter("HFDumpJets",
    fDisc1 = cms.untracked.string('softMuonBJetTags'),
    fDisc3 = cms.untracked.string('softElectronBJetTags'),
    fDisc2 = cms.untracked.string('softMuonNoIPBJetTags'),
    fDisc5 = cms.untracked.string('trackCountingHighPurBJetTags'),
    fDisc4 = cms.untracked.string('trackCountingHighEffBJetTags'),
    fDisc7 = cms.untracked.string('combinedSecondaryVertexBJetTags'),
    jetsLabel = cms.untracked.string('iterativeCone5CaloJets'),
    genjetsLabel = cms.untracked.string('iterativeCone5GenJets'),
    fDisc6 = cms.untracked.string('jetProbabilityBJetTags'),
    JetPartonMapAlgo = cms.InputTag("tagJetAlgo"),
    GenJetPartonMapAlgo = cms.InputTag("gentagJetAlgo"),
    JetPartonMapPhys = cms.InputTag("tagJetPhys"),
    GenJetPartonMapPhys = cms.InputTag("gentagJetPhys")

#    JetPartonMapAlgo = cms.InputTag("TrackCountingHighEffBJetTags")                       
)

stuffDump = cms.EDFilter("HFDumpStuff",
    genEventScale = cms.untracked.string(''),
    METLabel = cms.untracked.string('corMetType1Icone5'),
    GenMETLabel = cms.untracked.string('genMet'),
    CandidateLabel = cms.untracked.string('allMuons')
)

# this is commented in the .cff file
triggerDump = cms.EDFilter("HFDumpTrigger",
    verbose = cms.untracked.int32(1),
    particleMap = cms.untracked.string("fastL1extraParticleMap"),
    L1MuLabel = cms.untracked.string("l1extraParticles"),
    L1TriggerName = cms.untracked.string("L1_DoubleMu3"),
    hltLabel = cms.InputTag("TriggerResults::HLT"),
    HLTriggerName = cms.untracked.string("HLTBJPsiMuMu"),

    HLTfiltObj0 = cms.untracked.string("muTracks"),
    HLTfiltObj1 = cms.untracked.string("CandFromMumu"),
    HLTfiltObj2 = cms.untracked.string("JpsitoMumuL1Seed"),
    HLTfiltObj3 = cms.untracked.string("JpsitoMumuL2Filtered"),
    HLTfiltObj4 = cms.untracked.string("displacedJpsitoMumuFilter"),
)



