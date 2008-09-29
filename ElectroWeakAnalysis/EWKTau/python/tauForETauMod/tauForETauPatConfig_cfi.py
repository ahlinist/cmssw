import FWCore.ParameterSet.Config as cms
import copy
from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import * 

#####################  PAT LAYER 0  #########################

allLayer0TausForETauKin = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("pfRecoTauProducerForETau"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauKin:passKin"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''), 
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )


allLayer0TausForETauLdgTrk = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauKin"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauLdgTrk:foundLdgTrk"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''), 
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauLdgTrkPt = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauLdgTrk"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauLdgTrkCut:passLdgTrkPt"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )


allLayer0TausForETauTrkIsod = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauLdgTrkPt"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauTrkIsoDiscrForETau"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauEclIsod = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauTrkIsod"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauEclIsoDiscrForETau"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauElecRej = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauEclIsod"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauElecRejDiscrForETau"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )


#####################  PAT LAYER 0 FOR EFF TAU ID MEASUREMENT #########################

allLayer0TausForETauKinEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("pfRecoTauProducerForETauEff"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauEffKin:passKin"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauLdgTrkEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauKinEff"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauEffLdgTrk:foundLdgTrk"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''), 
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauLdgTrkPtEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauLdgTrkEff"),
     tauDiscriminatorSource = cms.InputTag("ldgTrkDiscrForETauEffLdgTrkCut:passLdgTrkPt"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauTrkIsodEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauLdgTrkPtEff"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauTrkIsoDiscrForETauEff"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauEclIsodEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauTrkIsodEff"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauEclIsoDiscrForETauEff"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )

allLayer0TausForETauElecRejEff = cms.EDFilter("PATPFTauCleaner",
     tauSource              = cms.InputTag("allLayer0TausForETauEclIsodEff"),
     tauDiscriminatorSource = cms.InputTag("pfRecoTauElecRejDiscrForETauEff"),
 
     removeOverlaps = cms.PSet(),

     markItems    = cms.bool(True),
     bitsToIgnore = cms.vstring(''),
     saveRejected = cms.string(''),
     saveAll      = cms.string(''),
 )





#################### PAT LAYER 1 ##########################

layer1TausForETauKin = copy.deepcopy(allLayer1Taus)
layer1TausForETauKin.tauSource = "allLayer0TausForETauKin"
layer1TausForETauKin.addTrigMatch = False
layer1TausForETauKin.addGenMatch = False

layer1TausForETauLdgTrk = copy.deepcopy(allLayer1Taus)
layer1TausForETauLdgTrk.tauSource = "allLayer0TausForETauLdgTrk"
layer1TausForETauLdgTrk.addTrigMatch = False
layer1TausForETauLdgTrk.addGenMatch = False

layer1TausForETauLdgTrkPt = copy.deepcopy(allLayer1Taus)
layer1TausForETauLdgTrkPt.tauSource = "allLayer0TausForETauLdgTrkPt"
layer1TausForETauLdgTrkPt.addTrigMatch = False
layer1TausForETauLdgTrkPt.addGenMatch = False

layer1TausForETauTrkIsod = copy.deepcopy(allLayer1Taus)
layer1TausForETauTrkIsod.tauSource = "allLayer0TausForETauTrkIsod"
layer1TausForETauTrkIsod.addTrigMatch = False
layer1TausForETauTrkIsod.addGenMatch = False

layer1TausForETauEclIsod = copy.deepcopy(allLayer1Taus)
layer1TausForETauEclIsod.tauSource = "allLayer0TausForETauEclIsod"
layer1TausForETauEclIsod.addTrigMatch = False
layer1TausForETauEclIsod.addGenMatch = False

layer1TausForETauElecRej = copy.deepcopy(allLayer1Taus)
layer1TausForETauElecRej.tauSource = "allLayer0TausForETauElecRej"
layer1TausForETauElecRej.addTrigMatch = False
layer1TausForETauElecRej.addGenMatch = False


#################### PAT LAYER 1 FOR TAU ID EFF MEASUREMENT ###########

layer1TausForETauKinEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauKinEff.tauSource = "allLayer0TausForETauKinEff"
layer1TausForETauKinEff.addTrigMatch = False
layer1TausForETauKinEff.addGenMatch = False

layer1TausForETauLdgTrkEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauLdgTrkEff.tauSource = "allLayer0TausForETauLdgTrkEff"
layer1TausForETauLdgTrkEff.addTrigMatch = False
layer1TausForETauLdgTrkEff.addGenMatch = False

layer1TausForETauLdgTrkPtEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauLdgTrkPtEff.tauSource = "allLayer0TausForETauLdgTrkPtEff"
layer1TausForETauLdgTrkPtEff.addTrigMatch = False
layer1TausForETauLdgTrkPtEff.addGenMatch = False

layer1TausForETauTrkIsodEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauTrkIsodEff.tauSource = "allLayer0TausForETauTrkIsodEff"
layer1TausForETauTrkIsodEff.addTrigMatch = False
layer1TausForETauTrkIsodEff.addGenMatch = False

layer1TausForETauEclIsodEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauEclIsodEff.tauSource = "allLayer0TausForETauEclIsodEff"
layer1TausForETauEclIsodEff.addTrigMatch = False
layer1TausForETauEclIsodEff.addGenMatch = False

layer1TausForETauElecRejEff = copy.deepcopy(allLayer1Taus)
layer1TausForETauElecRejEff.tauSource = "allLayer0TausForETauElecRejEff"
layer1TausForETauElecRejEff.addTrigMatch = False
layer1TausForETauElecRejEff.addGenMatch = False



#################### PAT SELECTORS FOR TAU  ###########

select1Or3SigTrackTausForETau = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1TausForETauElecRej"),
    cut = cms.string("signalTracks.size()=1|signalTracks.size()=3")
)

select1Or3SigChargedHadTausForETau = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1TausForETauElecRej"),
    cut = cms.string("signalPFChargedHadrCands.size()=1|signalPFChargedHadrCands.size()=3")
)


#################### PAT SELECTORS FOR TAU ID EFF MEASUREMENT ###########

select1Or3SigTrackTausForETauEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1TausForETauElecRejEff"),
    cut = cms.string("signalTracks.size()=1|signalTracks.size()=3")
)

select1Or3SigChargedHadTausForETauEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1TausForETauElecRejEff"),
    cut = cms.string("signalPFChargedHadrCands.size()=1|signalPFChargedHadrCands.size()=3")
)
