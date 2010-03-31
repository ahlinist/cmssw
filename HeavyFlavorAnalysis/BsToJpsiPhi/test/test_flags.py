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
                                          TriggerTag = cms.InputTag("TriggerResults::HLT"),
                                          MuonTag = cms.InputTag("muons"),
                                          StoreDeDxInfo = cms.bool( False ),  #needs to be false on AODSIM, can be true on RECO
                                          
                                          JpsiMassWindowBeforeFit = cms.double(0.150),
                                          JpsiMassWindowAfterFit = cms.double(0.150),
                                          JpsiPtCut      = cms.double(3),
                                          KaonTrackPtCut = cms.double(0.5),
                                          PhiMassWindowBeforeFit  = cms.double(0.05),
                                          PhiMassWindowAfterFit  = cms.double(0.02),
                                          BsLowerMassCutBeforeFit = cms.double(4.5),
                                          BsUpperMassCutBeforeFit = cms.double(6),
                                          BsLowerMassCutAfterFit  = cms.double(5.2),
                                          BsUpperMassCutAfterFit  = cms.double(5.7),
                                          
                                          outputFile = cms.untracked.string("BsToJpsiPhi.root"),
                                         )

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
                              fileNames = cms.untracked.vstring(
#                              'file:/test/mc/1/fromHLT/Bs2JpsiPhi_1.root',
'file:/nfs/data5/cms/store/mc/Summer09/BtoJPsiMuMu/AODSIM/MC_31X_V3_AODSIM-v1/0021/465107F5-E988-DE11-BA12-003048C91B0E.root'

                              ),
)

process.myout = cms.OutputModule("PoolOutputModule",
                                 outputCommands = cms.untracked.vstring('drop *', 'keep *_*_*_ANALYSIS', 'keep *_muons_*_*'),
                                 fileName = cms.untracked.string('test.root')
                                 )

process.p = cms.Path(process.mix*process.allPiTracks*process.allKTracks*process.kTracks*process.piTracks*process.bsVertexAnalysis)

process.outpath = cms.EndPath(process.myout)
