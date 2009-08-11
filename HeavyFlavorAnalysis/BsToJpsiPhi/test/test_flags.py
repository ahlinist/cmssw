import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V3::All'
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("SimGeneral.TrackingAnalysis.trackingParticles_cfi")
process.load("SimGeneral.MixingModule.mixNoPU_cfi") # VIR-pertrucc

process.allKTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                                    src = cms.InputTag("generalTracks"),
                                    particleType = cms.string('K+')
                                    )

process.allPiTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
                                    src = cms.InputTag("generalTracks"),
                                    particleType = cms.string('pi+')
                                    )

process.kTracks = cms.EDFilter("CandViewRefSelector",
                               src = cms.InputTag("allKTracks"),
                               cut = cms.string("pt > 0.3 & abs(eta) < 2.5")	
                               )

process.piTracks = cms.EDFilter("CandViewRefSelector",
                               src = cms.InputTag("allPiTracks"),
                               cut = cms.string("pt > 0.3 & abs(eta) < 2.5")	
                               )

process.bsVertexAnalysis = cms.EDAnalyzer("BsToJpsiPhiAnalysis",	
                                          genParticlesLabel  = cms.InputTag("genParticles"),
                                          TrackLabel_K = cms.InputTag("kTracks"),
                                          TrackLabel_pi = cms.InputTag("piTracks"),
                                          Tracks1 = cms.InputTag("allKTracks"),
                                          Tracks2 = cms.InputTag("allPiTracks"),
                                          useStrip           = cms.bool(True),
                                          usePixel           = cms.bool(True),
                                          TrajectoryTracks = cms.InputTag("generalTracks"),
                                          TriggerTag = cms.InputTag("TriggerResults::HLT"),
                                          MuonTag = cms.InputTag("muons"),
                                          outputFile = cms.untracked.string("RelVal_test.root"),
                                         )

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
                              fileNames = cms.untracked.vstring(
                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/1/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/2/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/3/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/4/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/5/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/6/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/7/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/8/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/9/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/10/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/11/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/12/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/13/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/14/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/15/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/16/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/17/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/18/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/19/BsToJpsiPhi_full_1.root',
#                              'file:../../../HeavyFlavorAnalysis/Examples/python/April09_MCproduction/mc/reco/20/BsToJpsiPhi_full_1.root',
                              ),
)

process.myout = cms.OutputModule("PoolOutputModule",
                                 outputCommands = cms.untracked.vstring('drop *', 'keep *_*_*_ANALYSIS', 'keep *_muons_*_*'),
                                 fileName = cms.untracked.string('test.root')
                                 )

process.p = cms.Path(process.mix*process.allPiTracks*process.allKTracks*process.kTracks*process.piTracks*process.trackingParticles*process.bsVertexAnalysis)

process.outpath = cms.EndPath(process.myout)
