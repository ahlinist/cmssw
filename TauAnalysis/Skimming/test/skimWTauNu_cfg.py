import FWCore.ParameterSet.Config as cms

process = cms.Process("wTauNuSkim1")

from TauAnalysis.Skimming.EventContent_cff import *

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(10000)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
'file:ttbarSkim.root'
#    'file:../../Configuration/test/QCD_PtTrack15_FASTSIM_1000.root'
#  'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_1.root'  
  )
)

#--------------------------------------------------------------------------------
# select taus and met
#--------------------------------------------------------------------------------


process.selectedPFMet = cms.EDFilter("PFMEtSelector",
  src = cms.InputTag('pfMet'),
  cut = cms.string("pt > 10"),
  filter = cms.bool(True)
)

process.selectedCaloMet = cms.EDFilter("CaloMEtSelector",
   src = cms.InputTag('met'),
   cut = cms.string("pt > 10"),
   filter = cms.bool(True)
)

                           
process.leadTrPFTaus = cms.EDFilter("PFTauSelector",
  src = cms.InputTag('pfRecoTauProducer'),
  discriminators = cms.VPSet(
    cms.PSet(
      discriminator = cms.InputTag("pfRecoTauDiscriminationByLeadingTrackPtCut"),
      selectionCut = cms.double(0.5)
    )
  ),
  cut = cms.string("abs(eta) < 2.5 & leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 15 "),                                    
  filter = cms.bool(True)
)

process.leadTrCaloTaus = cms.EDFilter("CaloTauSelector",
  src = cms.InputTag('caloRecoTauProducer'),
  discriminators = cms.VPSet(
    cms.PSet(
      discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
      selectionCut = cms.double(0.5)
    )
  ),
  cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 15"),
  filter = cms.bool(True)
)


#--------------------------------------------------------------------------------
# keep event in case it passed either the pfTau or caloTau selection
#--------------------------------------------------------------------------------

process.PFTauSkimPath = cms.Path(
    (process.leadTrPFTaus + process.selectedPFMet)
)

process.CaloTauSkimPath = cms.Path(
    (process.leadTrCaloTaus  + process.selectedCaloMet)
)

wTauNuEventSelection = cms.untracked.PSet(
  SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('PFTauSkimPath','CaloTauSkimPath')
  )
)

process.wTauNuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
  tauAnalysisEventContent,                                               
  wTauNuEventSelection,
  fileName = cms.untracked.string('ttbarSkim.root')
)

process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.wTauNuSkimOutputModule )
