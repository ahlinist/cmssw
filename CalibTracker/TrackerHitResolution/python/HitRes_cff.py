import FWCore.ParameterSet.Config as cms

hitres = cms.EDAnalyzer("HitRes",
                        usePXB = cms.bool(False),
                        usePXF = cms.bool(False),
                        useTIB = cms.bool(True),
                        useTOB = cms.bool(True),
                        useTID = cms.bool(False),
                        useTEC = cms.bool(False),
                        ROUList = cms.vstring('TrackerHitsTIBLowTof',
                                              'TrackerHitsTIBHighTof',
                                              'TrackerHitsTOBLowTof',
                                              'TrackerHitsTOBHighTof'),        
                        trajectories = cms.InputTag("generalTracks"),
                        associatePixel = cms.bool(False),
                        associateStrip = cms.bool(False),
                        associateRecoTracks = cms.bool(False),
                        tracks = cms.InputTag("generalTracks"),
                        barrelOnly = cms.bool(False),
                        minPt = cms.double(5.0)
                        )

HitResNTuple = cms.Sequence( hitres )
