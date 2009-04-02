import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")

process.load("Geometry.RPCGeometry.rpcGeometry_cfi")

process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

process.MessageLogger = cms.Service("MessageLogger")

process.demo = cms.EDFilter("RPCMonitorEfficiency",
    fileName = cms.untracked.string('/tmp/carrillo/first.root'),
    fileOut = cms.untracked.string('/tmp/carrillo/second.root'),

    dosD = cms.untracked.bool(True),
    CLSandBXY = cms.untracked.bool(True),

    debug = cms.untracked.bool(False),

    cosmics = cms.untracked.bool(False),
    makehtml = cms.untracked.bool(True),
    prodimages = cms.untracked.bool(True),
    threshold = cms.untracked.double(80.0),

    barrel = cms.untracked.bool(True),
    endcap = cms.untracked.bool(True)
)

process.p = cms.Path(process.demo)
