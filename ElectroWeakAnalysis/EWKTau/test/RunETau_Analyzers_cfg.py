import FWCore.ParameterSet.Config as cms

process = cms.Process("ETauPat")

######## MESSAGE LOGGER #####################
process.load("FWCore.MessageService.MessageLogger_cfi")  
process.MessageLogger.cerr.FwkReport.reportEvery = 100

################## SERVICES  ######################
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = cms.bool(True)
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'  
process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoParticleFlow.Configuration.RecoParticleFlow_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

############### PAT PRODUCERS #####################
process.load("ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauPatProducer_cff")
process.load("ElectroWeakAnalysis.EWKTau.tauForETauMod.tauForETauPFPatProducer_cff")
process.load("ElectroWeakAnalysis.EWKTau.metForETauPatConfig_cff")

############## SETUP ANALYZERS ###################
process.load("ElectroWeakAnalysis.EWKTau.electronForETauAnalyzer_cfi")
process.analyzeElectronForETau.ElecInputTags = (
   "allLayer1ElecForETau"
  ,"selectKinElecForETau"
  ,"selectMchHLTElecForETau"
  ,"selectTrkIsoAfterMchHLTElecForETau"
  ,"selectEcalIsoAfterTrkIsoElecForETau"
  ,"selectHcalIsoAfterEcalIsoElecForETau"
  ,"selectIdAfterIsoElecForETau"
  ,"selectIpAfterIsoElecForETau"
)
process.analyzeElectronForETau.RefInputTags = ("tauMCProducer:LeptonicTauElectrons")
process.analyzeElectronForETau.DoMatching = False
process.analyzeElectronForETau.DoTuning = True
process.analyzeElectronForETau.DoVeto = True
process.load("ElectroWeakAnalysis.EWKTau.tauForETauAnalyzer_cfi")
process.analyzeTauForETau.TauInputTags = (
   "selectorPfTausForETauKinEff",
   "selectorPfTausForETauLdgTrkEff",
   "selectorPfTausForETauLdgTrkPtEff",
   "selectorPfTausForETauEclIsodEff",
   "selectorPfTausForETauTrkIsodEff",
   "selectorPfTausForETauElecRejEff",
   "selectorPfTausForETauProngEff"
)
process.analyzeTauForETau.FinalElecTag = "selectIpAfterIsoElecForETau"
process.analyzeTauForETau.RefInputTags = ("tauMCProducer:HadronicTauOneAndThreeProng")
process.analyzeTauForETau.DoMatching = False
process.analyzeTauForETau.DoTuning = True
process.analyzeTauForETau.DoTauIdAfterElecId = True

############### SKIM FILTERS ###########################                                                   
process.myHltHighLevel = cms.EDFilter("HLTHighLevel",
         HLTPaths = cms.vstring("HLT_IsoEle15_L1I"), ## provide list of HLT paths you want
         andOr = cms.bool(True), ## true: OR of those on your list                                          
         TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
)
############### POOL SOURCE #######################
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_2_1_2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_10TeV_v1/0005/08CE7EE3-5F74-DD11-ACC8-001617E30CA4.root'
   )
)
process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)         
)

###### TFile Service ########
process.TFileService = cms.Service("TFileService",         
    fileName = cms.string('RootFiles/ETauAnalyzerOutput.root')
)

############## SEQUENCES AND PATHS ################
process.pflow = cms.Sequence(      
                       process.siPixelRecHits
                      *process.siStripMatchedRecHits
                      *process.ckftracks
                      *process.electronSequence
                      *process.conversionSequence                  
                      *process.vertexreco
                      *process.particleFlowReco
                      *process.iterativeCone5PFJets          
                    )


process.patElec = cms.Sequence(
                       process.layer0ElecForETau
                      *process.layer1ElecForETau
                      )
process.patTau = cms.Sequence(
                       process.PFTauForETauPreamble
                      *process.PFTauForETauEff
                      *process.PFTauForETauEffPat
                     )
process.patMet = cms.Sequence(
                       process.patMET              
                      )
process.p = cms.Path(
                       process.myHltHighLevel
                     #*process.pflow
                      *process.patElec
                      *process.patTau
                      *process.patMet
                      *process.analyzeElectronForETau
                      *process.analyzeTauForETau
                     )


