# $Id: common_dump_config.py,v 1.2 2011/05/21 16:13:53 rahatlou Exp $
#
#  common configuration to dump ntuples in MC and data
#    all changes affecting the path and additional modules msut be done here
#

import FWCore.ParameterSet.Config as cms
import RecoJets.JetProducers.JetIDParams_cfi
theJetIDParams = RecoJets.JetProducers.JetIDParams_cfi.JetIDParams.clone()
from RecoJets.Configuration.RecoGenJets_cff import *
from RecoMET.Configuration.RecoGenMET_cff import *
from RecoMET.Configuration.GenMETParticles_cff import *

process = cms.Process("myprocess")

process.load("FWCore.MessageLogger.MessageLogger_cfi")


process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticleCandidates_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load('Configuration/StandardSequences/Reconstruction_cff')




#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(
'file:/cmsrm/pc24_2/meridian/data/GluGluToHToGG_M-115_7TeV-powheg-pythia6_AODSIM.root'
)

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
#    SkipEvent = cms.untracked.vstring('ProductNotFound')
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10

#monster track event cleaning
process.monster = cms.EDFilter(
   "FilterOutScraping",
   applyfilter = cms.untracked.bool(True),
   debugOn = cms.untracked.bool(False),
   numtrack = cms.untracked.uint32(10),
   thresh = cms.untracked.double(0.2)
)


###########  EB SPIKE CLEANING BEGIN #####################

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START311_V2::All')

###########  EB SPIKE CLEANING END   #####################

## produce JPT jets
#process.load('RecoJets.Configuration.RecoJPTJets_cff')

#############   Include the corrections ##########
process.load("RecoMET.Configuration.RecoGenMET_cff")
process.load("RecoMET.Configuration.GenMETParticles_cff")
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")
process.load("JetMETCorrections.Type1MET.MetType1Corrections_cff")

process.ak5CaloL1Offset.useCondDB = False
#process.ak5PFL1Offset.useCondDB = False
process.ak5JPTL1Offset.useCondDB = False

process.ak5CaloL1Fastjet.useCondDB = False
process.ak5PFL1Fastjet.useCondDB = False
process.ak5JPTL1Fastjet.useCondDB = False

process.ak5PFJets.doAreaFastjet = True
#process.ak7PFJets.doAreaFastjet = True

process.metMuonJESCorAK5 = process.metJESCorAK5CaloJet.clone()
process.metMuonJESCorAK5.inputUncorJetsLabel = "ak5CaloJets"
process.metMuonJESCorAK5.corrector = "ak5CaloL2L3"
process.metMuonJESCorAK5.inputUncorMetLabel = "corMetGlobalMuons"
#process.metMuonJESCorAK5.hasMuonsCorr = True
#process.metMuonJESCorAK5.useTypeII = True
 
process.metCorSequence = cms.Sequence(process.metMuonJESCorAK5)

# configure B-tagging to be run on ak5PFJets
process.myBtag = cms.Sequence(process.ak5JetTracksAssociatorAtVertex*process.btagging)
process.ak5JetTracksAssociatorAtVertex.jets = cms.InputTag("ak5PFJets")
process.softMuonTagInfos.jets =  cms.InputTag("ak5PFJets")
process.softElectronTagInfos.jets =  cms.InputTag("ak5PFJets")


## dumper module
process.myanalysis = cms.EDAnalyzer("GammaJetAnalyzer",
    debug = cms.bool(False),
    dumpBeamHaloInformations = cms.untracked.bool(True),
    dumpAKT5Jets=cms.untracked.bool(True),
    dumpAKT7Jets=cms.untracked.bool(True),
    dumpJPTAKT5Jets=cms.untracked.bool(False),
    dumpJPTAKT7Jets=cms.untracked.bool(False),
    dumpPFAKT5Jets=cms.untracked.bool(True),
    dumpPFAKT7Jets=cms.untracked.bool(True),                                                                        
    dumpKT4Jets=cms.untracked.bool(False),
    dumpKT6Jets=cms.untracked.bool(False),                                    
    recoProducer = cms.string('ecalRecHit'),
    PUSummaryInfoCollection = cms.InputTag("addPileupInfo"),
    MCTruthCollection = cms.untracked.InputTag("source"),
    genMet = cms.untracked.InputTag("genMetTrue"),
    met = cms.untracked.InputTag("met"),
    tracks = cms.untracked.InputTag("generalTracks"),
    Electronsrc = cms.untracked.InputTag("gsfElectrons"),
    Photonsrc = cms.untracked.InputTag("photons"),
    Conversionsrc = cms.untracked.InputTag("allConversions"),
    recoCollection = cms.string('reducedEcalRecHitsEB'),
    JetCorrectionService_akt5 = cms.string('ak5CaloL1L2L3'),
    JetCorrectionService_akt7 = cms.string('ak7CaloL2L3'),
    JetCorrectionService_jptak5 = cms.string('ak5JPTL1L2L3'),
    JetCorrectionService_jptak7 = cms.string('ak7JPTL2L3'),
    JetCorrectionService_pfakt5 = cms.string('ak5PFL1FastL2L3'),
    JetCorrectionService_pfakt7 = cms.string('ak7PFL2L3'),
    jetskt4 = cms.untracked.InputTag("kt4CaloJets"),
    jetskt6 = cms.untracked.InputTag("kt6CaloJets"),
    jetsakt5 = cms.untracked.InputTag("ak5CaloJets"),
    jetsakt7 = cms.untracked.InputTag("ak7CaloJets"),
    jetsjptak5 = cms.untracked.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
    jetspfkt4 = cms.untracked.InputTag("kt4PFJets"),
    jetspfkt6 = cms.untracked.InputTag("kt6PFJets"),
    jetspfakt5 = cms.untracked.InputTag("ak5PFJets"),
    jetspfakt7 = cms.untracked.InputTag("ak7PFJets"),
    hbhits = cms.untracked.InputTag("hbhereco"),
    jetsgenkt4 = cms.untracked.InputTag("kt4GenJets"),
    jetsgenkt6 = cms.untracked.InputTag("kt6GenJets"),
    jetsgenakt5 = cms.untracked.InputTag("ak5GenJets"),
    jetsgenakt7 = cms.untracked.InputTag("ak7GenJets"),
    TriggerTag = cms.untracked.InputTag("TriggerResults::HLT"),
    vertices = cms.untracked.InputTag("offlinePrimaryVertices"),
                                    
    genjetptthr = cms.double(5.),
    calojetptthr = cms.double(3.),
    pfjetptthr = cms.double(4.),
    jptjetptthr = cms.double(4.),
    genjetnmin = cms.int32(10),
    pfjetnmin = cms.int32(10),
    jptjetnmin = cms.int32(10),
    JetIDParams = theJetIDParams,
    Xsec = cms.double(1.)
)

# compute rho with PF candidates
process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True,  doAreaFastjet = cms.bool(True), voronoiRfact = cms.double(0.9) )
process.kt6PFJets = process.kt6PFJets.clone( rParam = 0.6, doRhoFastjet = True )

#process.load('RecoJets.JetProducers.kt6PFJets_cfi')
#process.kt6PFJetsForIso = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True,  doAreaFastjet = cms.bool(True), voronoiRfact = cms.double(0.9) )
process.kt6PFJetsForIso = process.kt6PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIso.Rho_EtaMax = cms.double(2.5)

# compute rho with calo towers
#process.load('RecoJets.JetProducers.kt6CaloJets_cfi')
#process.kt6CaloJetsForIso = process.kt4CaloJets.clone( rParam = 0.6, doRhoFastjet = True, doAreaFastjet = cms.bool(True), voronoiRfact = cms.double(0.9) )
process.kt6CaloJetsForIso = process.kt6CaloJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6CaloJetsForIso.Rho_EtaMax = cms.double(2.5)

# re-reconstructing the primary vertices with the Deterministic Annealing (DA) vertex finder
# from B. Mangano studies
from RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi import *
import RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi

process.offlinePrimaryVerticesDA = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi.offlinePrimaryVerticesDA.clone()
process.offlinePrimaryVerticesDA.useBeamConstraint = cms.bool(True)
process.offlinePrimaryVerticesDA.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
process.offlinePrimaryVerticesDA.TkClusParameters.TkDAClusParameters.vertexSize = cms.double(0.01) 

# high purity tracks
process.highPurityTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("generalTracks"),
    cut = cms.string('quality("highPurity")')
)

process.analysisSequence = cms.Sequence( process.monster * process.offlinePrimaryVerticesDA * process.highPurityTracks * process.kt6PFJets * process.ak5PFJets *process.kt6PFJetsForIso * process.kt6CaloJetsForIso * process.myBtag * process.metCorSequence * process.myanalysis )
