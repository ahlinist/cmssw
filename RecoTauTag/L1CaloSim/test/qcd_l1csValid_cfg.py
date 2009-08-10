import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("L1VAL")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
)


process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #'file:/home/chinhan/cms/cmssw/tteff_forNote/CMSSW_2_2_13/src/ElectroWeakAnalysis/TauTriggerEfficiency/test/rootfiles/ztt_summer08_reco_01.root'
    'file:/home/chinhan/cms/cmssw/tteff_forNote/CMSSW_2_2_13/src/ElectroWeakAnalysis/TauTriggerEfficiency/test/rootfiles/qcdpt30_summer08_reco_01.root'
    )
)


# L1CaloSim
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")
process.load("RecoTauTag.L1CaloSim.l1calosim_cfi")
process.l1CaloSim.AlgorithmSource = "RecHits"
process.l1CaloSim.EmInputs = cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"), cms.InputTag("ecalRecHit","EcalRecHitsEE"))
process.l1CaloSim.DoBitInfo = cms.bool(False)
process.l1CaloSim.EMActiveLevelIso = cms.double(4.0)
process.l1CaloSim.HadActiveLevelIso = cms.double(4.0)
process.l1CaloSim.IsolationEt = cms.double(2.0)
process.l1CaloSim.EMActiveLevel = cms.double(3.0)
process.l1CaloSim.HadActiveLevel = cms.double(3.0)
#process.l1CaloSim.GctIso = cms.bool(True)####


process.l1Validation = cms.EDAnalyzer("L1CaloSimValidation",
  L1extraTauJetSource	    = cms.InputTag("hltL1extraParticles", "Tau", "HLT"),
  L1extraCentralJetSource   = cms.InputTag("hltL1extraParticles", "Central", "HLT"),
  L1calosimTauJetSource	    = cms.InputTag("l1CaloSim", "Tau"),
  L1calosimCentralJetSource = cms.InputTag("l1CaloSim", "Central"),
  L1JetMatchingCone	    = cms.double(0.4)
)

process.TFileService = cms.Service("TFileService", 
    #fileName = cms.string('ztt_l1val.root')
    fileName = cms.string('qcdpt30_l1val.root')
)

process.runEDAna = cms.Path(
    process.l1CaloSim*
    process.l1Validation
) 

