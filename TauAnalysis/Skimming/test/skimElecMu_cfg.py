import FWCore.ParameterSet.Config as cms

process = cms.Process("elecMuSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
#
# Z --> tau tau (all decay modes; simulated with TAUOLA)
# 9k events CMSSW_2_2_3 RelVal sample
#
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/A4A3988A-BCCB-DD11-A103-001617E30E28.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/D412FFFC-BCCB-DD11-8B20-000423D952C0.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/F01E4F34-BDCB-DD11-B87D-001617C3B77C.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/1CAA08F8-D3CB-DD11-ADF9-000423D6B358.root',
    '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/2800478C-08CC-DD11-94BB-0019B9F72BAA.root'
  )
)

#--------------------------------------------------------------------------------
# select muons and electrons
#--------------------------------------------------------------------------------

process.selectedMuons = cms.EDFilter("MuonSelector",
  src = cms.InputTag('muons'),
  cut = cms.string("pt > 8 & abs(eta) < 2.5"),
  filter = cms.bool(True)
)

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
  src = cms.InputTag("pixelMatchGsfElectrons"),
  cut = cms.string("pt > 8 & abs(eta) < 2.5 & eSuperClusterOverP>0.8 & eSuperClusterOverP<1.25"),
  filter = cms.bool(True)
)

#--------------------------------------------------------------------------------
# combine selected muons and electrons into pairs;
# disable cut on eta-phi distance between electron and muon
# (should not be needed to avoid double-counting,
#  as the electron and muon selections are rather exclusive)
#--------------------------------------------------------------------------------

process.elecMuPairs = cms.EDProducer("DiCandidatePairProducer",
  useLeadingTausOnly = cms.bool(False),
  srcLeg1 = cms.InputTag('selectedElectrons'),
  srcLeg2 = cms.InputTag('selectedMuons'),
  dRmin12 = cms.double(-1.),
  srcMET = cms.InputTag(''),
  recoMode = cms.string(""),
  verbosity = cms.untracked.int32(0)                                       
)

process.selectedElecMuPairs = cms.EDFilter("DiCandidatePairSelector",
  src = cms.InputTag('elecMuPairs'),
  cut = cms.string("dR12 > -1."),
  filter = cms.bool(True)                                     
)

#--------------------------------------------------------------------------------
# keep event in case it passed the muon + electron selection
#--------------------------------------------------------------------------------

process.elecMuSkimPath = cms.Path(
  (process.selectedElectrons + process.selectedMuons)
 * process.elecMuPairs
)

elecMuEventSelection = cms.untracked.PSet(
  SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('elecMuSkimPath')
  )
)

process.elecMuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
  tauAnalysisEventContent,                                               
  elecMuEventSelection,
  #fileName = cms.untracked.string('/castor/cern.ch/user/c/cerati/elecMuSkim.root')
  fileName = cms.untracked.string('elecMuSkim.root')                                                  
)

process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.elecMuSkimOutputModule )

