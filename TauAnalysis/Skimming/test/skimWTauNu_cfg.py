import FWCore.ParameterSet.Config as cms

process = cms.Process("muTauSkim")

from TauAnalysis.Skimming.EventContent_cff import *

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        '/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
    )
)

#--------------------------------------------------------------------------------
# select met and tau-jets
#--------------------------------------------------------------------------------

process.selectedPFMet = cms.EDFilter("PFMEtSelector",
                                     src = cms.InputTag('pfMet'),
                                     cut = cms.string("pt > 15"),
                                     filter = cms.bool(True)
                                     )

process.selectedCaloMet = cms.EDFilter("CaloMEtSelector",
                                       src = cms.InputTag('met'),
                                       cut = cms.string("pt > 15"),
                                       filter = cms.bool(True)
                                       )

process.selectedPFTaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('shrinkingConePFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 15."),
    filter = cms.bool(True)
)

process.selectedCaloTaus = cms.EDFilter("CaloTauSelector",
    src = cms.InputTag('caloRecoTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut"),
            selectionCut = cms.double(0.5)
        )
    ),
    cut = cms.string("abs(eta) < 2.5 & leadTrack().isNonnull() & leadTrack().pt() > 15."),
    filter = cms.bool(True)
)

#--------------------------------------------------------------------------------
# keep event in case it passed either the MET + pfTau or MET + caloTau selection
#--------------------------------------------------------------------------------

process.PFTauNuSkimPath = cms.Path(
    (process.selectedPFTaus + process.selectedPFMet)
)

process.CaloTauNuSkimPath = cms.Path(
    (process.selectedCaloTaus + process.selectedCaloMet)
)

wTauNuEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('PFTauNuSkimPath', 'CaloTauNuSkimPath')
    )
)

process.wTauNuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    tauAnalysisEventContent,                                               
    wTauNuEventSelection,
    fileName = cms.untracked.string('wTauNuSkim.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.wTauNuSkimOutputModule )

