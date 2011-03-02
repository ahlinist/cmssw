import FWCore.ParameterSet.Config as cms

rpcdigidqm = cms.EDAnalyzer("RPCMonitorDigi",
                            SaveRootFile = cms.untracked.bool(False),
                            RootFileName = cms.untracked.string('RPCMonitorDigi.root'),
                            UseMuon =  cms.untracked.bool(True),
                            MuonPtCut = cms.untracked.double(3.0),
                            MuonEtaCut= cms.untracked.double(1.6),
                            MuonLabel =  cms.InputTag('muons'),
                            RPCFolder = cms.untracked.string('RPC'),
                            GlobalFolder = cms.untracked.string('SummaryHistograms'),
                            RecHitLabel = cms.InputTag("rpcRecHits"),
                            NumberOfEndcapDisks = cms.untracked.int32(3),
                            NumberOfInnermostEndcapRings = cms.untracked.int32(2),
                            NoiseFolder  = cms.untracked.string("Noise"),
                            MuonFolder = cms.untracked.string("Muon")
                            )


