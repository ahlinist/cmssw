import FWCore.ParameterSet.Config as cms

process = cms.Process("EWKTauIDAnalysis")

####### Load the necessary files (startup conditions, etc) #######

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("DQMOffline.Trigger.Tau.HLTTauDQMOffline_cff")
process.load("ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatProducer_cff")
process.load("ElectroWeakAnalysis.EWKTau.metForETauPatConfig_cff")
process.load("ElectroWeakAnalysis.EWKTau.tauForETauMod.tauForETauPFPatProducer_cff")
process.pfRecoTauTagInfoProducerForETauEff.ChargedHadrCand_tkminPt = 0.5
process.pfRecoTauProducerForETauEff.MatchingConeSizeFormula = "0.2"
process.pfRecoTauProducerForETauEff.MatchingConeSize_max = 0.2
process.pfRecoTauProducerForETauEff.TrackerSignalConeSizeFormula = "0.15"
process.pfRecoTauProducerForETauEff.TrackerSignalConeSize_min = 0.0
process.pfRecoTauProducerForETauEff.TrackerSignalConeSize_max = 0.15
process.pfRecoTauProducerForETauEff.LeadTrack_minPt = 6.0
process.pfRecoTauProducerForETauEff.LeadChargedHadrCand_minPt = 6.0
process.pfRecoTauProducerForETauEff.ChargedHadrCand_minPt = 0.5
process.pfRecoTauLdgTrkPtCutForETauEff.MinPtLeadingTrack = 6.0

process.selectorPfTausForETauKinEtaEff = cms.EDProducer("PATTauSelector",
    src = cms.InputTag("layer1PfTausForETauEff"),
    cut = cms.string("abs(eta)<=2.5"),
    filter = cms.bool(False)
)
process.selectorPfTausForETauKinEtaEtEff = cms.EDProducer("PATTauSelector",
    src = cms.InputTag("selectorPfTausForETauKinEtaEff"),
    cut = cms.string("et>=10.0"),
    filter = cms.bool(False)
)
process.selectorPfTausForETauKinEtEff = cms.EDProducer("PATTauSelector",
    src = cms.InputTag("layer1PfTausForETauEff"),
    cut = cms.string("et>=10.0"),
    filter = cms.bool(False)
)

####### Define the input data sample and the number of events to process #######

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/12F2BEA9-F97D-DD11-8262-000423D9890C.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/1A683EA7-F97D-DD11-9A4E-000423D6C8E6.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/3C84A911-FA7D-DD11-BC5E-000423D990CC.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/405E200D-FA7D-DD11-B587-000423D952C0.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/4CD31C08-FA7D-DD11-A305-000423D99AAE.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/5AA51868-FA7D-DD11-A2AB-000423D98FBC.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/7C20F9DB-F97D-DD11-A8E3-000423D8FA38.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/8A7D56DC-F97D-DD11-9591-000423D99AAE.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/90DEE0A4-F97D-DD11-B223-000423D98800.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/A8D09913-FA7D-DD11-B9EE-000423D951D4.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/BC85BA74-FA7D-DD11-934D-000423D952C0.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/CC7D48D0-F97D-DD11-8C7E-000423D98F98.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/E212380D-FA7D-DD11-A5C2-000423D99896.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/F0ADB46E-FA7D-DD11-A599-000423D94524.root',
        '/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/F484E210-FA7D-DD11-A42A-000423D94534.root'
     )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

####### Define the selector module that chooses muons based on whether it is a global muon #######

process.selectedLayer1GlobalMuons = cms.EDProducer("PATMuonSelector",
     src = cms.InputTag("allLayer1MuForEWKTau"),
     cut = cms.string('isGlobalMuon()'),
     filter = cms.bool(False)
)

####### Define the selector module that chooses muons based on pseudorapidity #######

process.selectedLayer1MuonsEta21 = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("selectedLayer1GlobalMuons"),
  cut = cms.string('abs(eta) <= 2.1'),
  filter = cms.bool(False)
)

####### Define the selector module that chooses muons based on pt #######

process.selectedLayer1MuonsPt20 = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("selectedLayer1MuonsEta21"),
  cut = cms.string('pt >= 20'),
  filter = cms.bool(False)
)

process.selectedLayer1MuonsSeperatedFromPATTau = cms.EDProducer("PATLeptonPATTauSeperationSelector",
     src = cms.InputTag("selectedLayer1MuonsPt20"),
     LeptonType = cms.string('Muon'),
     TauSource = cms.InputTag("selectorPfTausForETauKinEtaEtEff"),
     DeltaRMaxCut = cms.double(999.0),
     DeltaRMinCut = cms.double(0.7),
)

####### Define the selector module that chooses muons based on segment & calo compatibilities #######

process.selectedLayer1MuonsAntiPionCut = cms.EDProducer("PATMuonAntiPionSelection",
     src = cms.InputTag("selectedLayer1MuonsSeperatedFromPATTau"),
     CaloCompCoefficient = cms.double(0.8),
     SegmCompCoefficient = cms.double(1.2),
     AntiPionCut = cms.double(1.0),
     filter = cms.bool(False)
)

####### Define the selector module that chooses muons based on impact parameter #######

process.selectedLayer1MuonsIP = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("selectedLayer1MuonsAntiPionCut"),
  cut = cms.string('track.d0 < 0.01'),
  filter = cms.bool(False)
)

####### Define the selector module that chooses muons based on PFGamma isolation #######

process.selectedLayer1MuonsPFGammaIso10 = cms.EDProducer("PATLeptonPFParticleIsolationSelector",
     src = cms.InputTag("selectedLayer1MuonsIP"),
     LeptonType = cms.string('Muon'),
     PFCandidateProd = cms.string('particleFlow'),
     PFParticleCode = cms.double(4.0),
     OuterIsolationConeSize = cms.double(0.8),
     InnerIsolationConeSize = cms.double(0.0),
     IsolationMinCut = cms.double(0.0),
     IsolationMaxCut = cms.double(1.0)
)

####### Define the selector module that chooses muons based on track isolation #######

process.selectTrkIsoMuForEWKTau = cms.EDProducer("PATMuonSelector",
     src = cms.InputTag("selectedLayer1MuonsPFGammaIso10"),   
     cut = cms.string('trackIso < 1.0'),
     filter = cms.bool(False)
)

####### Define the b jet rejection module #######

process.selectedLayer1MuonsBJetRejection = cms.EDProducer("PATLeptonSVAssociatorSelector",
     src = cms.InputTag("selectTrkIsoMuForEWKTau"),
     LeptonType = cms.string('Muon'),
     TrackProducer = cms.string('generalTracks'),
     VertexProducer = cms.string('offlinePrimaryVertices'),
     VFParameters = cms.PSet(
       finder = cms.string('avr'),
       primcut = cms.double(1.8),
       seccut = cms.double(6.0),
       minweight = cms.double(0.5),
       weightthreshold = cms.double(0.001),
       smoothing = cms.bool(False)
     ),
     VertexMathingDistance = cms.double(0.005),
     VertexMuonMaxDistance = cms.double(0.00),
     MinAssociatedSVCut = cms.double(0.0),
     MaxAssociatedSVCut = cms.double(0.0),
)

####### Define the muon-met delphi selector module #######

process.selectedLayer1MuonsSatisfyingMetDelPhi = cms.EDProducer("PATLeptonPATMetDelPhiSelector",
     src = cms.InputTag("selectedLayer1MuonsBJetRejection"),
     LeptonType = cms.string('Muon'),
     MetSource = cms.InputTag("allLayer1METs"),
     DelPhiMaxCut = cms.double(999.0),
     DelPhiMinCut = cms.double(-999.0),
)

process.selectedLayer1MuonsSatisfyingMassCut = cms.EDProducer("PATMuonPATTauMassSelection",
     src = cms.InputTag("selectedLayer1MuonsSatisfyingMetDelPhi"),
     TauSource = cms.InputTag("layer1TausForETauLdgTrkEff"),
     MassMinCut = cms.double(40.0),
     MassMaxCut = cms.double(100.0),
     filter = cms.bool(False)
)

#process.load("ElectroWeakAnalysis.EWKTau.analyzerForEWKTauID_cfi")

process.patTausEWKTauID = cms.Sequence(
                                   process.PFTauForETauEff
                                  *process.PFTauForETauEffPat
                                  *process.selectorPfTausForETauKinEtaEff
                                  *process.selectorPfTausForETauKinEtaEtEff
                                  *process.selectorPfTausForETauKinEtEff)

process.patLayer1MuonsEWKTauID = cms.Sequence(process.layer0MuForEWKTau *
                                              process.layer1MuForEWKTau *
#                                              process.patMET *
                                              process.selectedLayer1GlobalMuons *
                                              process.selectedLayer1MuonsEta21 *
                                              process.selectedLayer1MuonsPt20 *
                                              process.selectedLayer1MuonsSeperatedFromPATTau *
                                              process.selectedLayer1MuonsAntiPionCut *
                                              process.selectedLayer1MuonsIP *
                                              process.selectedLayer1MuonsPFGammaIso10 *
                                              process.selectTrkIsoMuForEWKTau *
                                              process.selectedLayer1MuonsBJetRejection
#                                              process.selectedLayer1MuonsSatisfyingMetDelPhi *
#                                              process.selectedLayer1MuonsSatisfyingMassCut
)

####### Output module configuration #######

process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = cms.untracked.vstring('keep *_*_*_*'),
  fileName = cms.untracked.string('test.root')

  ####### save only events passing the full path #######

#  SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
)

####### Define the path #######

#process.p = cms.Path(process.myHltHighLevel * process.patTausEWKTauID * process.patLayer1MuonsEWKTauID)
process.p = cms.Path(process.patTausEWKTauID * process.patLayer1MuonsEWKTauID)
#process.p = cms.Path(process.patTausEWKTauID * process.patLayer1MuonsEWKTauID * process.defaultEWKTauIDAnalyzer)

process.outpath = cms.EndPath(process.out)
