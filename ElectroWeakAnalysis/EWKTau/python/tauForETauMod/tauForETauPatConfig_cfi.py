import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.cleaningLayer0.pfTauCleaner_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import * 

#####################  PAT LAYER 0  #########################

allLayer0PfTausForETau = copy.deepcopy(allLayer0Taus)
allLayer0PfTausForETau.tauSource = cms.InputTag("pfRecoTauProducerForETau")
allLayer0PfTausForETau.tauDiscriminatorSource = cms.InputTag("pfRecoTauIsoDiscrForETau")
allLayer0PfTausForETau.removeOverlaps = cms.PSet()
allLayer0PfTausForETau.markItems = cms.bool(True)
allLayer0PfTausForETau.bitsToIgnore = cms.vstring('Core/Preselection')
allLayer0PfTausForETau.saveRejected = cms.string('')
allLayer0PfTausForETau.saveAll      = cms.string('')

#####################  PAT LAYER 0 FOR EFF TAU ID MEASUREMENT ###########

allLayer0PfTausForETauEff= copy.deepcopy(allLayer0Taus)
allLayer0PfTausForETauEff.tauSource = cms.InputTag("pfRecoTauProducerForETauEff")
allLayer0PfTausForETauEff.tauDiscriminatorSource = cms.InputTag("pfRecoTauIsoDiscrForETauEff")
allLayer0PfTausForETauEff.removeOverlaps = cms.PSet()
allLayer0PfTausForETauEff.markItems = cms.bool(True)
allLayer0PfTausForETauEff.bitsToIgnore = cms.vstring('Core/Preselection')
allLayer0PfTausForETauEff.saveRejected = cms.string('')
allLayer0PfTausForETauEff.saveAll      = cms.string('')

#################### PAT LAYER 1 ##########################

layer1PfTausForETau = copy.deepcopy(allLayer1Taus)
layer1PfTausForETau.tauSource = "allLayer0PfTausForETau"
layer1PfTausForETau.tauIDSources = cms.PSet(
    leadingTrackFinding = cms.InputTag("pfRecoTauLdgTrkFindForETau"),
    leadingTrackPtCut = cms.InputTag("pfRecoTauLdgTrkPtCutForETau"),
    trackIsolation = cms.InputTag("pfRecoTauTrkIsoDiscrForETau"),
    ecalIsolation = cms.InputTag("pfRecoTauEclIsoDiscrForETau"),
    #byIsolation = cms.InputTag("pfRecoTauIsoDiscrForETau"),
    againstElectron = cms.InputTag("pfRecoTauElecRejDiscrForETau"),
    againstMuon = cms.InputTag("pfRecoTauMuonRejDiscrForETau")
    )

layer1PfTausForETau.addTrigMatch = False
layer1PfTausForETau.addGenMatch = False
layer1PfTausForETau.addGenJetMatch = False

#################### PAT LAYER 1 FOR TAU ID EFF MEASUREMENT ###########

layer1PfTausForETauEff = copy.deepcopy(allLayer1Taus)
layer1PfTausForETauEff.tauSource = "allLayer0PfTausForETauEff"
layer1PfTausForETauEff.tauIDSources = cms.PSet(
    leadingTrackFinding = cms.InputTag("pfRecoTauLdgTrkFindForETauEff"),
    leadingTrackPtCut = cms.InputTag("pfRecoTauLdgTrkPtCutForETauEff"),
    trackIsolation = cms.InputTag("pfRecoTauTrkIsoDiscrForETauEff"),
    ecalIsolation = cms.InputTag("pfRecoTauEclIsoDiscrForETauEff"),
    #byIsolation = cms.InputTag("pfRecoTauIsoDiscrForETauEff"),
    againstElectron = cms.InputTag("pfRecoTauElecRejDiscrForETauEff"),
    againstMuon = cms.InputTag("pfRecoTauMuonRejDiscrForETauEff")
    )
layer1PfTausForETauEff.addTrigMatch = False
layer1PfTausForETauEff.addGenMatch = False
layer1PfTausForETauEff.addGenJetMatch = False

#################### PAT SELECTORS FOR TAU  ###########

selectorPfTausForETauKin = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1PfTausForETau"),
    cut = cms.string("et>15. & abs(eta)<2.5"),
)
selectorPfTausForETauLdgTrk = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauKin"),
    cut = cms.string('tauID("leadingTrackFinding")>0.'),
)
selectorPfTausForETauLdgTrkPt = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauLdgTrk"),
    cut = cms.string('tauID("leadingTrackPtCut")>0.'),
)
selectorPfTausForETauEclIsod = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauLdgTrkPt"),
    cut = cms.string('tauID("ecalIsolation")>0.'),
)
selectorPfTausForETauTrkIsod = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauEclIsod"),
    cut = cms.string('tauID("trackIsolation")>0.'),
)
selectorPfTausForETauElecRej = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauTrkIsod"),
    cut = cms.string('tauID("againstElectron")>0.'),
)
selectorPfTausForETauMuonRej = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauElecRej"),
    cut = cms.string('tauID("againstMuon")>0.'),
)
selectorPfTausForETauProng = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauElecRej"),
    cut = cms.string("signalTracks.size()=1|signalTracks.size()=3")
)


#################### PAT SELECTORS FOR TAU ID EFF MEASUREMENT ###########

selectorPfTausForETauKinEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("layer1PfTausForETauEff"),
    cut = cms.string("et>15. & abs(eta)<2.5"),
)
selectorPfTausForETauLdgTrkEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauKinEff"),
    cut = cms.string('tauID("leadingTrackFinding")>0.'),
)
selectorPfTausForETauLdgTrkPtEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauLdgTrkEff"),
    cut = cms.string('tauID("leadingTrackPtCut")>0.'),
)
selectorPfTausForETauEclIsodEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauLdgTrkPtEff"),
    cut = cms.string('tauID("ecalIsolation")>0.'),
)
selectorPfTausForETauTrkIsodEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauEclIsodEff"),
    cut = cms.string('tauID("trackIsolation")>0.'),
)
selectorPfTausForETauElecRejEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauTrkIsodEff"),
    cut = cms.string('tauID("againstElectron")>0.'),
)
selectorPfTausForETauMuonRejEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauElecRejEff"),
    cut = cms.string('tauID("againstMuon")>0.'),
)

selectorPfTausForETauProngEff = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauElecRejEff"),
    cut = cms.string("signalTracks.size()=1|signalTracks.size()=3")
)




#allLayer0PfTausForETauEff = cms.EDFilter("PATPFTauCleaner",
#     tauSource              = cms.InputTag("pfRecoTauProducerForETauEff"),
#     tauDiscriminatorSource = cms.InputTag("pfRecoTauIsoDiscrForETauEff"),
#     removeOverlaps = cms.PSet(),
#     markItems    = cms.bool(True),
#     bitsToIgnore = cms.vstring('Core/Preselectio'),
#     saveRejected = cms.string(''),
#     saveAll      = cms.string(''),
# )

#allLayer0PfTausForETau = cms.EDFilter("PATPFTauCleaner",
#     tauSource              = cms.InputTag("pfRecoTauProducerForETau"),
#     tauDiscriminatorSource = cms.InputTag("pfRecoTauIsoDiscrForETau"),
# 
#     removeOverlaps = cms.PSet(),
#
#     markItems    = cms.bool(True),
#     bitsToIgnore = cms.vstring('Core/Preselection'), 
#     saveRejected = cms.string(''),
#     saveAll      = cms.string(''),
# )


