# The following comments couldn't be translated into the new config version:

#include "Configuration/JetMET/data/calorimetry-jetmet-gen.cff" 
#replace genParticleCandidates.src="source" 

import FWCore.ParameterSet.Config as cms

#Particle data table, Magnetic Field, CMS geometry, Tracker geometry, Calo geometry
#include "FastSimulation/Configuration/data/CommonInputs.cff"
# Primary vertex smearing.
from IOMC.EventVertexGenerators.VtxSmearedGauss_cfi import *
# Conversion to GenParticleCandidates 
from PhysicsTools.HepMCCandAlgos.genParticleCandidatesFast_cfi import *
# Famos PileUp Producer
from FastSimulation.PileUpProducer.PileUpProducer_cff import *
# Famos SimHits producer
from FastSimulation.EventProducer.FamosSimHits_cff import *
# Mixing module 
from FastSimulation.Configuration.mixNoPU_cfi import *
# Gaussian Smearing RecHit producer
from FastSimulation.TrackingRecHitProducer.SiTrackerGaussianSmearingRecHitConverter_cfi import *
# Rec Hit Tranlator to the Full map with DeTId'
from FastSimulation.TrackingRecHitProducer.TrackingRecHitTranslator_cfi import *
# CTF and Iterative tracking (contains pixelTracks and pixelVertices)
# 1) Common algorithms and configuration taken from full reconstruction
# Note: The runge-kutta propagator is not used here 
# (because no magnetic field inhomogeneities are simulated between layers)
from FastSimulation.Tracking.GSTrackFinalFitCommon_cff import *
# 3) Fast Simulation tracking sequences
from FastSimulation.Tracking.GlobalPixelTracking_cff import *
from FastSimulation.Tracking.IterativeTracking_cff import *
# Calo RecHits producer (with no HCAL miscalibration by default)
from FastSimulation.CaloRecHitsProducer.CaloRecHits_cff import *
# ECAL clusters
from RecoEcal.Configuration.RecoEcal_cff import *
# Calo Towers
from RecoJets.Configuration.CaloTowersRec_cff import *
# Particle Flow
from RecoParticleFlow.PFClusterProducer.towerMakerPF_cff import *
from RecoParticleFlow.PFClusterProducer.particleFlowCluster_cff import *
from RecoParticleFlow.PFTracking.particleFlowTrack_cff import *
from RecoParticleFlow.PFBlockProducer.particleFlowSimParticle_cff import *
from RecoParticleFlow.PFBlockProducer.particleFlowBlock_cff import *
from RecoParticleFlow.PFProducer.particleFlow_cff import *
import FastSimulation.Tracking.TrackCandidateProducer_cfi
fsGsfElCandidates = FastSimulation.Tracking.TrackCandidateProducer_cfi.trackCandidateProducer.clone()
import TrackingTools.GsfTracking.GsfElectronFit_cfi
fsgsfPFtracks = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
# Reco Jets and MET
from RecoJets.Configuration.RecoJets_cff import *
from RecoJets.Configuration.RecoPFJets_cff import *
from RecoMET.Configuration.RecoMET_cff import *
# Gen Jets
from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *
from RecoMET.Configuration.GenMETParticles_cff import *
from RecoMET.Configuration.RecoGenMET_cff import *
# Muon parametrization
from FastSimulation.ParamL3MuonProducer.ParamL3Muon_cfi import *
# Muon simHit sequence 
from FastSimulation.MuonSimHitProducer.MuonSimHitProducer_cfi import *
# Muon Digi sequence
from SimMuon.Configuration.SimMuon_cff import *
# Muon RecHit sequence
from RecoLocalMuon.Configuration.RecoLocalMuon_cff import *
# Muon reconstruction sequence
#include "RecoMuon/Configuration/data/RecoMuon.cff"
from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
from RecoMuon.TrackingTools.MuonTrackLoader_cff import *
from RecoMuon.MuonSeedGenerator.standAloneMuonSeeds_cfi import *
from RecoMuon.StandAloneMuonProducer.standAloneMuons_cfi import *
from FastSimulation.Configuration.globalMuons_cff import *
#Muon identification sequence
#include "RecoMuon/MuonIdentification/data/muonIdProducerSequence.cff"
from FastSimulation.Configuration.muonIdentification_cff import *
#Muon isolation
from RecoMuon.MuonIsolationProducers.muIsolation_cff import *
# Electron reconstruction
from FastSimulation.Tracking.GlobalMixedSeedProducer_cff import *
from FastSimulation.EgammaElectronAlgos.electronGSPixelSeeds_cfi import *
from FastSimulation.EgammaElectronAlgos.electronGSGsfTrackCandidates_cff import *
from RecoEgamma.EgammaElectronProducers.pixelMatchGsfElectrons_cff import *
from TrackingTools.GsfTracking.GsfElectronFit_cff import *
import TrackingTools.GsfTracking.GsfElectronFit_cfi
pixelMatchGsfFit = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
from RecoEgamma.ElectronIdentification.electronIdSequence_cff import *
# Photon reconstruction
from RecoEgamma.EgammaPhotonProducers.photonSequence_cff import *
from RecoEgamma.PhotonIdentification.photonId_cff import *
# Both for electrons and photons
from RecoEgamma.EgammaIsolationAlgos.egammaIsolationSequence_cff import * 
# B tagging
from RecoJets.JetAssociationProducers.ic5JetTracksAssociatorAtVertex_cfi import *
from RecoVertex.Configuration.RecoVertex_cff import *
from RecoVertex.BeamSpotProducer.BeamSpot_cff import *
from RecoBTag.Configuration.RecoBTag_cff import *
#Tau tagging
from RecoTauTag.Configuration.RecoTauTag_cff import *
from RecoTauTag.Configuration.RecoPFTauTag_cff import *
famosParticleFlowSequence = cms.Sequence(caloTowersPFRec+particleFlowCluster+elecpreid+fsGsfElCandidates+fsgsfPFtracks+pfTrackElec+particleFlowBlock+particleFlow)
caloJetMet = cms.Sequence(recoJets+metreco)
PFJetMet = cms.Sequence(recoPFJets)
caloJetMetGen = cms.Sequence(genParticles+genJetParticles+recoGenJets+genMETParticles+recoGenMET)
famosMuonSequence = cms.Sequence(muonDigi+muonlocalreco+MuonSeed+standAloneMuons+globalMuons)
#replace muIsoDepositTk.ExtractorPSet = { using MIsoTrackExtractorGsBlock }
famosMuonIdAndIsolationSequence = cms.Sequence(sisCone5CaloJets+muonIdProducerSequence+muIsolation)
famosElectronSequence = cms.Sequence(globalMixedSeeds+electronGSPixelSeeds+electronGSGsfTrackCandidates+pixelMatchGsfFit+pixelMatchGsfElectrons+eIdSequence)
famosPhotonSequence = cms.Sequence(photonSequence+photonIDSequence)
famosBTaggingSequence = cms.Sequence(btagging)
famosTauTaggingSequence = cms.Sequence(tautagging)
#replace pfRecoTauProducer.PVProducer = "offlinePrimaryVerticesFromCTFTracks"
famosPFTauTaggingSequence = cms.Sequence(PFTau)
# The sole simulation sequence
famosSimulationSequence = cms.Sequence(offlineBeamSpot+famosPileUp+famosSimHits+MuonSimHits+mix)
# Famos pre-defined sequences
famosWithTrackerHits = cms.Sequence(famosSimulationSequence+siTrackerGaussianSmearingRecHits)
famosWithTrackerAndCaloHits = cms.Sequence(famosWithTrackerHits+caloRecHits)
famosWithTracks = cms.Sequence(famosWithTrackerHits+iterativeTracking)
famosWithTracksAndMuonHits = cms.Sequence(famosSimulationSequence+siTrackerGaussianSmearingRecHits+iterativeTracking+famosMuonSequence)
famosWithTracksAndMuons = cms.Sequence(famosSimulationSequence+siTrackerGaussianSmearingRecHits+iterativeTracking+famosMuonSequence+caloRecHits+caloTowersRec+famosMuonIdAndIsolationSequence)
famosWithCaloHits = cms.Sequence(famosSimulationSequence+caloRecHits)
famosWithEcalClusters = cms.Sequence(famosWithCaloHits+ecalClusters)
famosWithTracksAndCaloHits = cms.Sequence(famosWithTracks+caloRecHits)
famosWithTracksAndEcalClusters = cms.Sequence(famosWithTracksAndCaloHits+ecalClusters)
famosWithParticleFlow = cms.Sequence(famosWithTracksAndCaloHits+famosParticleFlowSequence+PFJetMet)
famosWithCaloTowers = cms.Sequence(famosWithCaloHits+caloTowersRec)
famosWithJets = cms.Sequence(famosWithCaloTowers+caloJetMetGen+caloJetMet)
famosWithTracksAndCaloTowers = cms.Sequence(famosWithTracksAndCaloHits+caloTowersRec)
famosWithTracksAndJets = cms.Sequence(famosWithTracksAndCaloTowers+caloJetMetGen+caloJetMet)
famosWithCaloTowersAndParticleFlow = cms.Sequence(famosWithParticleFlow+caloTowersRec)
famosWithMuons = cms.Sequence(famosWithTracks+paramMuons)
famosWithMuonsAndIsolation = cms.Sequence(famosWithTracksAndCaloTowers+paramMuons+sisCone5CaloJets+muIsolation_ParamGlobalMuons)
famosWithElectrons = cms.Sequence(famosWithTrackerHits+caloRecHits+ecalClusters+famosElectronSequence+egammaIsolationSequence)
famosWithPhotons = cms.Sequence(famosWithTrackerHits+caloRecHits+ecalClusters+famosPhotonSequence+egammaIsolationSequence)
famosWithElectronsAndPhotons = cms.Sequence(famosWithTrackerHits+caloRecHits+ecalClusters+famosElectronSequence+famosPhotonSequence+egammaIsolationSequence)
famosWithBTagging = cms.Sequence(famosWithTracksAndCaloTowers+vertexreco+iterativeCone5CaloJets+ic5JetTracksAssociatorAtVertex+ecalClusters+famosMuonSequence+famosBTaggingSequence)
famosWithTauTagging = cms.Sequence(famosWithTracksAndCaloTowers+vertexreco+iterativeCone5CaloJets+ic5JetTracksAssociatorAtVertex+ecalClusters+famosTauTaggingSequence)
famosWithPFTauTagging = cms.Sequence(famosWithCaloTowersAndParticleFlow+vertexreco+famosPFTauTaggingSequence)
# The simulation sequence
simulationWithFamos = cms.Sequence(famosSimulationSequence+siTrackerGaussianSmearingRecHits+caloRecHits)
# The reconstruction sequence
reconstructionWithFamos = cms.Sequence(iterativeTracking+vertexreco+caloTowersRec+ecalClusters+famosElectronSequence+famosPhotonSequence+egammaIsolationSequence+famosMuonSequence+famosMuonIdAndIsolationSequence+famosParticleFlowSequence+caloJetMetGen+caloJetMet+PFJetMet+paramMuons+muIsolation_ParamGlobalMuons+ic5JetTracksAssociatorAtVertex+famosBTaggingSequence+famosTauTaggingSequence+famosPFTauTaggingSequence)
famosWithEverything = cms.Sequence(simulationWithFamos+reconstructionWithFamos)
# 2) Specific cuts
# Add a chi**2 cut to retain/reject hits
KFFittingSmoother.EstimateCut = 15.0
# Request three hits to make a track
KFFittingSmoother.MinNumberOfHits = 3
islandBasicClusters.barrelHitProducer = 'caloRecHits'
islandBasicClusters.endcapHitProducer = 'caloRecHits'
hybridSuperClusters.ecalhitproducer = 'caloRecHits'
correctedHybridSuperClusters.recHitProducer = cms.InputTag("caloRecHits","EcalRecHitsEB")
correctedIslandBarrelSuperClusters.recHitProducer = cms.InputTag("caloRecHits","EcalRecHitsEB")
correctedIslandEndcapSuperClusters.recHitProducer = cms.InputTag("caloRecHits","EcalRecHitsEE")
correctedEndcapSuperClustersWithPreshower.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
preshowerClusterShape.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
reducedEcalRecHitsEB.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEB")
reducedEcalRecHitsEE.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEE")
interestingEcalDetIdEB.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEB")
interestingEcalDetIdEE.recHitsLabel = cms.InputTag("caloRecHits","EcalRecHitsEE")
multi5x5BasicClusters.barrelHitProducer = 'caloRecHits'
multi5x5BasicClusters.endcapHitProducer = 'caloRecHits'
multi5x5PreshowerClusterShape.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
multi5x5SuperClustersWithPreshower.preshRecHitProducer = cms.InputTag("caloRecHits","EcalRecHitsES")
correctedMulti5x5SuperClustersWithPreshower.recHitProducer = cms.InputTag("caloRecHits","EcalRecHitsEE")
towerMaker.ecalInputs = cms.VInputTag(cms.InputTag("caloRecHits","EcalRecHitsEB"), cms.InputTag("caloRecHits","EcalRecHitsEE"))
towerMaker.hbheInput = 'caloRecHits'
towerMaker.hfInput = 'caloRecHits'
towerMaker.hoInput = 'caloRecHits'
towerMakerPF.ecalInputs = cms.VInputTag(cms.InputTag("caloRecHits","EcalRecHitsEB"), cms.InputTag("caloRecHits","EcalRecHitsEE"))
towerMakerPF.hbheInput = 'caloRecHits'
towerMakerPF.hoInput = 'caloRecHits'
towerMakerPF.hfInput = 'caloRecHits'
particleFlowRecHitECAL.ecalRecHitsEB = cms.InputTag("caloRecHits","EcalRecHitsEB")
particleFlowRecHitECAL.ecalRecHitsEE = cms.InputTag("caloRecHits","EcalRecHitsEE")
particleFlowRecHitPS.ecalRecHitsES = cms.InputTag("caloRecHits","EcalRecHitsES")
particleFlowSimParticle.sim = 'famosSimHits'
elecpreid.NHitsInSeed = 1
fsGsfElCandidates.SeedProducer = cms.InputTag("elecpreid","SeedsForGsf")
fsGsfElCandidates.TrackProducers = []
fsGsfElCandidates.MinNumberOfCrossedLayers = 5
fsgsfPFtracks.src = 'fsGsfElCandidates'
fsgsfPFtracks.TTRHBuilder = 'WithoutRefit'
fsgsfPFtracks.TrajectoryInEvent = True
pfTrackElec.GsfTrackModuleLabel = 'fsgsfPFtracks'
calotoweroptmaker.hbheInput = 'caloRecHits'
calotoweroptmaker.hoInput = 'caloRecHits'
calotoweroptmaker.hfInput = 'caloRecHits'
calotoweroptmaker.ecalInputs = cms.VInputTag(cms.InputTag("caloRecHits","EcalRecHitsEB"), cms.InputTag("caloRecHits","EcalRecHitsEE"))
genCandidatesForMET.verbose = False
simMuonCSCDigis.strips.doCorrelatedNoise = False ## Saves a little bit of time

simMuonCSCDigis.InputCollection = 'MuonSimHitsMuonCSCHits'
simMuonDTDigis.InputCollection = 'MuonSimHitsMuonDTHits'
simMuonRPCDigis.InputCollection = 'MuonSimHitsMuonRPCHits'
#replace  DTParametrizedDriftAlgo_CSA07.recAlgoConfig.tTrigModeConfig.doT0Correction = false
csc2DRecHits.stripDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigi")
csc2DRecHits.wireDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")
rpcRecHits.rpcDigiLabel = 'simMuonRPCDigis'
dt1DRecHits.dtDigiLabel = 'simMuonDTDigis'
globalMuons.TrackerCollectionLabel = 'generalTracks'
globalMuons.GLBTrajBuilderParameters.TrackRecHitBuilder = 'WithoutRefit'
globalMuons.GLBTrajBuilderParameters.TrackTransformer.TrackerRecHitBuilder = 'WithoutRefit'
# Use FastSim tracks and calo hits for muon id
muons.inputCollectionLabels = cms.VInputTag('generalTracks', 'globalMuons', cms.InputTag("standAloneMuons","UpdatedAtVtx"))
muons.TrackAssociatorParameters.EBRecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEB")
muons.TrackAssociatorParameters.EERecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEE")
muons.TrackAssociatorParameters.CaloTowerCollectionLabel = 'towerMaker'
muons.TrackAssociatorParameters.HBHERecHitCollectionLabel = 'caloRecHits'
muons.TrackAssociatorParameters.HORecHitCollectionLabel = 'caloRecHits'
#replace muons.TrackExtractorPSet = { using MIsoTrackExtractorGsBlock }
# Use FastSim tracks and calo hits for calo muon id
calomuons.inputTracks = 'generalTracks'
calomuons.TrackAssociatorParameters.EBRecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEB")
calomuons.TrackAssociatorParameters.EERecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEE")
calomuons.TrackAssociatorParameters.CaloTowerCollectionLabel = 'towerMaker'
calomuons.TrackAssociatorParameters.HBHERecHitCollectionLabel = 'caloRecHits'
calomuons.TrackAssociatorParameters.HORecHitCollectionLabel = 'caloRecHits'
electronGSPixelSeeds.SeedConfiguration.initialSeeds = cms.InputTag("globalMixedSeeds","GlobalMixed")
pixelMatchGsfFit.src = 'electronGSGsfTrackCandidates'
pixelMatchGsfFit.TTRHBuilder = 'WithoutRefit'
# 
# replace pixelMatchGsfElectrons.hbheModule = "caloRecHits"
# replace pixelMatchGsfElectrons.hbheInstance = ""
# replace pixelMatchGsfElectrons.SCLBarrelLabel = "electronGSPixelSeeds"
# replace pixelMatchGsfElectrons.SCLEndcapLabel = "electronGSPixelSeeds"
# 
pixelMatchGsfElectrons.hcalRecHits = 'caloRecHits'
pixelMatchGsfElectrons.barrelSuperClusters = cms.InputTag("correctedHybridSuperClusters","electronGSPixelSeeds")
pixelMatchGsfElectrons.endcapSuperClusters = cms.InputTag("correctedEndcapSuperClustersWithPreshower","electronGSPixelSeeds")
photons.barrelEcalHits = cms.InputTag("caloRecHits","EcalRecHitsEB")
photons.endcapEcalHits = cms.InputTag("caloRecHits","EcalRecHitsEE")
photons.hbheModule = 'caloRecHits'
photons.hbheInstance = ''
photons.pixelSeedProducer = 'electronGSPixelSeeds'
PhotonIDProd.barrelEcalRecHitProducer = 'caloRecHits'
PhotonIDProd.endcapEcalRecHitProducer = 'caloRecHits'
PhotonIDProd.HcalRecHitProducer = 'caloRecHits'
EgammaIsoEcalFromHitsExtractorBlock.barrelRecHits = 'caloRecHits:EcalRecHitsEB'
EgammaIsoEcalFromHitsExtractorBlock.endcapRecHits = 'caloRecHits:EcalRecHitsEE'
EgammaIsoHcalFromHitsExtractorBlock.hcalRecHits = 'caloRecHits'
ic5JetTracksAssociatorAtVertex.tracks = 'generalTracks'
ic5PFJetTracksAssociatorAtVertex.tracks = 'generalTracks'
offlinePrimaryVerticesWithBS.TrackLabel = 'generalTracks'
btagSoftElectrons.HBHERecHitTag = 'caloRecHits'
btagSoftElectrons.TrackAssociatorParameters.EBRecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEB")
btagSoftElectrons.TrackAssociatorParameters.EERecHitCollectionLabel = cms.InputTag("caloRecHits","EcalRecHitsEE")
btagSoftElectrons.TrackAssociatorParameters.CaloTowerCollectionLabel = 'towerMaker'
btagSoftElectrons.TrackAssociatorParameters.HBHERecHitCollectionLabel = 'caloRecHits'
#replace combinedTauTag.PVSrc = "offlinePrimaryVerticesFromCTFTracks"
#replace caloRecoTauProducer.PVProducer = "offlinePrimaryVerticesFromCTFTracks"
caloRecoTauTagInfoProducer.EBRecHitsSource = cms.InputTag("caloRecHits","EcalRecHitsEB")
caloRecoTauTagInfoProducer.EERecHitsSource = cms.InputTag("caloRecHits","EcalRecHitsEE")

