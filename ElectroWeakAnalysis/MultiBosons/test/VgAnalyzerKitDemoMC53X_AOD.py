import FWCore.ParameterSet.Config as cms

process = cms.Process("VgKit")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START53_V7G::All')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    #'file:/data3/ncuhep/52X_vgamma/testfiles/WGToLNuG_TuneZ2star_8TeV_PU_S7_START52_V9-v2.root'),
    'file:/data2/poter/MC_CMSSW537_vgamma/testfiles/DYJetsToLL_M-50_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1.root'),
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )

## The iso-based HBHE noise filter ___________________________________________||
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

## The HCAL laser filter _____________________________________________________||
process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
process.hcalLaserEventFilter.taggingMode = cms.bool(True)

## The ECAL dead cell trigger primitive filter _______________________________||
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.taggingMode = cms.bool(True)

## The EE bad SuperCrystal filter ____________________________________________||
process.load('RecoMET.METFilters.eeBadScFilter_cfi')
process.eeBadScFilter.taggingMode = cms.bool(True)

## The ECAL laser correction filter __________________________________________||
process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')
process.ecalLaserCorrFilter.taggingMode = cms.bool(True)

## The Good vertices collection needed by the tracking failure filter ________||
process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

## The tracking failure filter _______________________________________________||
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
process.trackingFailureFilter.taggingMode = cms.bool(True)

## The tracking POG filters __________________________________________________||
process.load('RecoMET.METFilters.trackingPOGFilters_cff')
process.manystripclus53X.taggedMode = cms.untracked.bool(True)
process.manystripclus53X.forcedValue = cms.untracked.bool(False)
process.toomanystripclus53X.taggedMode = cms.untracked.bool(True)
process.toomanystripclus53X.forcedValue = cms.untracked.bool(False)
process.logErrorTooManyClusters.taggedMode = cms.untracked.bool(True)
process.logErrorTooManyClusters.forcedValue = cms.untracked.bool(False)

process.load("PhysicsTools.PatAlgos.patSequences_cff")

# Trigger matching
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi")
process.load("ElectroWeakAnalysis.MultiBosons.VgTriggerMatcher_cfi")
process.patTriggerEvent.patTriggerMatches  = cms.VInputTag(
    "electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL",
    "electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL",
    "electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL",
    "electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50",
    "electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50",
    "electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50",
    "electronTriggerMatchHLTEle27WP80PFMETMT50",
    "electronTriggerMatchHLTEle27WP80",
    "electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT",
    "electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT",
    "electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT",
    "electronTriggerMatchHLTEle8Mass50",
    "electronTriggerMatchHLTSC4Mass50",
    "electronTriggerMatchHLTSC17Mass50",
    "photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50",
    "photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50",
    "photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50",
    "photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT",
    "photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT",
    "photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT",
    "photonTriggerMatchHLTEle8Mass50",
    "photonTriggerMatchHLTSC4Mass50",
    "photonTriggerMatchHLTSC17Mass50",
    "photonTriggerMatchHLTPhoton20CaloIdVLIsoL",
    "photonTriggerMatchHLTPhoton26Photon18",
    "photonTriggerMatchHLTPhoton30CaloIdVL",
    "photonTriggerMatchHLTPhoton30CaloIdVLIsoL",
    "photonTriggerMatchHLTPhoton36Photon22",
    "photonTriggerMatchHLTPhoton36CaloId10Iso50Photon22CaloId10Iso50",
    "photonTriggerMatchHLTPhoton50CaloIdVL",
    "photonTriggerMatchHLTPhoton50CaloIdVLIsoL",
    "photonTriggerMatchHLTPhoton75CaloIdVL",
    "photonTriggerMatchHLTPhoton75CaloIdVLIsoL",
    "photonTriggerMatchHLTPhoton90CaloIdVL",
    "photonTriggerMatchHLTPhoton90CaloIdVLIsoL",
    "photonTriggerMatchHLTPhoton135",
    "photonTriggerMatchHLTPhoton150",
    "muonTriggerMatchHLTIsoMu24",
    "muonTriggerMatchHLTIsoMu30",
    "muonTriggerMatchHLTIsoMu24eta2p1",
    "muonTriggerMatchHLTIsoMu30eta2p1",
    "muonTriggerMatchHLTIsoMu34eta2p1",
    "muonTriggerMatchHLTMu17Mu8",
    "muonTriggerMatchHLTMu17TkMu8",
    "muonTriggerMatchHLTMu17forMu17Mu8",
    "muonTriggerMatchHLTMu17forMu17TkMu8",
    "muonTriggerMatchHLTMu8forMu17Mu8",
    "muonTriggerMatchHLTTkMu8forMu17TkMu8",
    "jetTriggerMatchHLTPFJet40",
    "jetTriggerMatchHLTPFJet80",
    "jetTriggerMatchHLTPFJet140",
    "jetTriggerMatchHLTPFJet200",
    "jetTriggerMatchHLTPFJet260",
    "jetTriggerMatchHLTPFJet320",
    "jetTriggerMatchHLTPFJet400"
    )

process.patTrigger.processName = 'HLT'
process.patTriggerEvent.processName = 'HLT'

process.vgTriggerSequence = cms.Sequence(
     process.patTrigger * process.vgTriggerMatcher * process.patTriggerEvent
    )

#================================================
# ADD OFFICIAL ELECTRON ID FROM EG group!
#================================================
# Simple cut based selection in 2010
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.electronMatch*process.patElectrons)
process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
    )

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.patJetCorrFactors.useRho = cms.bool(True)

process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets25 = process.kt6PFJets.clone( doAreaFastjet=True, doRhoFastjet=True, rParam=0.6 )
process.kt6PFJets25.Rho_EtaMax = cms.double(2.5)
process.fjSequence = cms.Sequence( process.kt6PFJets25 )

from RecoJets.JetProducers.kt4PFJets_cfi import *
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)
process.fjSequence += process.kt6PFJetsForIsolation

process.ak5PFJets.doAreaFastjet = True

process.patJetCorrFactors.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
process.patJetCorrFactors.rho = cms.InputTag('kt6PFJets','rho')

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])),
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True,
                 outputModules = []
                 )

process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3")
process.load("JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfJetMETcorr', 'type1')        
)

# load the coreTools of PAT
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')
process.patMETsPF.metSource = cms.InputTag("pfMet")

process.patPFMETsTypeIcorrected = process.patMETs.clone(
    metSource = cms.InputTag('pfType1CorrectedMet'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
)
process.metAnalysisSequence = cms.Sequence( process.patPFMETsTypeIcorrected )

# apply type I/type I + II PFMEt corrections to pat::MET object 
# and estimate systematic uncertainties on MET
from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
runMEtUncertainties(process, doApplyType0corr = True, doSmearJets = True)

process.smearedPFType1CorrectedMet = process.patMETs.clone(
    metSource = cms.InputTag('patType1CorrectedPFMet'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(False)
)
process.metAnalysisSequence += process.smearedPFType1CorrectedMet

process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = cms.bool(False)

from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFPhotonIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.phoIsoSequence = setupPFPhotonIso(process, 'photons')

process.load("ElectroWeakAnalysis.MultiBosons.VgAnalyzerKit_cfi")
process.VgAnalyzerKit.doStoreJets = cms.bool(True)
process.VgAnalyzerKit.doJetHLTMatch = cms.bool(True)
process.VgAnalyzerKit.doStoreSCs = cms.bool(False)
process.VgAnalyzerKit.triggerResults = cms.InputTag("TriggerResults::HLT")
process.VgAnalyzerKit.rhoLabel = cms.InputTag("kt6PFJets25", "rho")
process.VgAnalyzerKit.sigmaLabel = cms.InputTag("kt6PFJets25", "sigma")
process.VgAnalyzerKit.rho2011Label = cms.InputTag("kt6PFJetsForIsolation", "rho")
process.VgAnalyzerKit.rho2012Label = cms.InputTag("kt6PFJets", "rho")
process.VgAnalyzerKit.TypeIpfMETLabel = cms.InputTag("patPFMETsTypeIcorrected")
process.VgAnalyzerKit.SmearedTypeIpfMETLabel = cms.InputTag("smearedPFType1CorrectedMet")

process.VgAnalyzerKit.doSkim = cms.bool(False)

process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree.root'))

process.p = cms.Path(process.HBHENoiseFilterResultProducer*
		     process.hcalLaserEventFilter*
		     process.EcalDeadCellTriggerPrimitiveFilter*
		     process.eeBadScFilter*
		     process.ecalLaserCorrFilter*
		     process.goodVertices* 
		     process.trackingFailureFilter*
		     process.trkPOGFilters*
		     process.pfParticleSelectionSequence*
		     process.eleIsoSequence*
		     process.phoIsoSequence*
		     process.fjSequence*
		     process.type0PFMEtCorrection*
		     process.producePFMETCorrections*
                     process.patDefaultSequence*
                     process.metAnalysisSequence*
                     process.vgTriggerSequence*
                     process.VgAnalyzerKit)

