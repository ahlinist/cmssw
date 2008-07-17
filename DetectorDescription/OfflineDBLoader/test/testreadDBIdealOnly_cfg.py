import FWCore.ParameterSet.Config as cms

process = cms.Process("DBGeometryTest")
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )
process.source = cms.Source("EmptySource")

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                          loadAll = cms.bool(True),
                                          toGet = cms.VPSet(cms.PSet(
            record = cms.string('IdealGeometryRecord'),
                    tag = cms.string('IdealGeometry01')
                )),
                                          DBParameters = cms.PSet(
            messageLevel = cms.untracked.int32(0),
                    authenticationPath = cms.untracked.string('.')
                ),
                                          catalog = cms.untracked.string('file:PoolFileCatalog.xml'),
                                          timetype = cms.string('runnumber'),
                                          connect = cms.string('sqlite_file:testIdeal.db')
                                      )

process.myprint = cms.OutputModule("AsciiOutputModule")

process.prod = cms.EDAnalyzer("PerfectGeometryAnalyzer",
                                  dumpPosInfo = cms.untracked.bool(True),
                                  dumpSpecs = cms.untracked.bool(True),
                                  dumpGeoHistory = cms.untracked.bool(True)
                              )

process.p1 = cms.Path(process.prod)
process.e1 = cms.EndPath(process.myprint)


