import FWCore.ParameterSet.Config as cms

process = cms.Process("skimTauEnergyScaleSample")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('MC_311_V2::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau/RelValTTbar_RECO.root'
    )
)

# run Vertex reconstruction via Deterministic annealing algorithm
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi")
process.offlinePrimaryVerticesDAwithBS = process.offlinePrimaryVerticesDA.clone()
process.offlinePrimaryVerticesDAwithBS.useBeamConstraint = cms.bool(True)
process.offlinePrimaryVerticesDAwithBS.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
process.offlinePrimaryVerticesDAwithBS.TkClusParameters.TkDAClusParameters.vertexSize = cms.double(0.01)

# rerun PFTau reconstruction
process.load('RecoTauTag/Configuration/RecoPFTauTag_cff')

# select events with at least one PFTau passing "loose" HPS tau id. 
process.selectedPFTaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('hpsPFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag('hpsPFTauDiscriminationByDecayModeFinding'),
            selectionCut = cms.double(0.5)
        ),
        cms.PSet(
            discriminator = cms.InputTag('hpsPFTauDiscriminationByLooseElectronRejection'),
            selectionCut = cms.double(0.5)
        ),
        cms.PSet(
            discriminator = cms.InputTag('hpsPFTauDiscriminationByTightMuonRejection'),
            selectionCut = cms.double(0.5)
        )                                  
    ),                          
    cut = cms.string("abs(eta) < 2.3 & pt > 20."),
    filter = cms.bool(True)
)

process.skimPath = cms.Path(
    process.offlinePrimaryVerticesDAwithBS
   + process.PFTau
   + process.selectedPFTaus
)

# use offlinePrimaryVerticesDAwithBS Vertex collection as input for PFTau reconstruction
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag 
massSearchReplaceAnyInputTag(process.skimPath, cms.InputTag("offlinePrimaryVerticesDA"), cms.InputTag('offlinePrimaryVerticesDAwithBS'))

skimEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('skimPath')
    )
)

process.load("Configuration.EventContent.EventContent_cff")
process.skimOutputModule = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    skimEventSelection,
    fileName = cms.untracked.string("skimmedSample_AOD.root")
)

process.o = cms.EndPath(process.skimOutputModule)

# print-out all python configuration parameter information
#print process.dumpPython()
