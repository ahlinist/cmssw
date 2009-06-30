# The following comments couldn't be translated into the new config version:

#keep the logging output to a nice level

import FWCore.ParameterSet.Config as cms

process = cms.Process("RPCSegmentEff")

process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
process.load("Geometry.RPCGeometry.rpcGeometry_cfi")
process.load("Geometry.CSCGeometry.cscGeometry_cfi")
process.load("Geometry.DTGeometry.dtGeometry_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")
process.load("DQMServices.Core.DQM_cfg")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_31X::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/h/hyteng/muongun_endcap/MuonMC.999.root')
    fileNames = cms.untracked.vstring('file:/tmp/carrillo/mcEndCap3_1_X.root')
)

process.museg = cms.EDFilter("MuonSegmentEff",

    incldt = cms.untracked.bool(False),
    incldtMB4 = cms.untracked.bool(False),
    inclcsc = cms.untracked.bool(True),

    debug = cms.untracked.bool(True),
    inves = cms.untracked.bool(True),
    
    DuplicationCorrection = cms.untracked.int32(1),

    manualalignment = cms.untracked.bool(False),
    AliFileName = cms.untracked.string('/afs/cern.ch/user/c/carrillo/endcap/CMSSW_3_0_0_pre10/src/DQM/RPCMonitorModule/data/Alignment69912.dat'),
	
    rangestrips = cms.untracked.double(4.),
    rangestripsRB4 = cms.untracked.double(4.),
    MinCosAng = cms.untracked.double(0.99),
    MaxD = cms.untracked.double(80.0),
    MaxDrb4 = cms.untracked.double(150.0),

    muonRPCDigis = cms.untracked.string('muonRPCDigis'),
    cscSegments = cms.untracked.string('cscSegments'),
    dt4DSegments = cms.untracked.string('dt4DSegments'),

    EffSaveRootFile = cms.untracked.bool(True),
    EffRootFileName = cms.untracked.string('/tmp/carrillo/first.root'),
    EffSaveRootFileEventsInterval = cms.untracked.int32(100)
)
process.p = cms.Path(process.museg)

process.DQM.collectorHost = ''
process.DQM.collectorPort = 9090
process.DQM.debug = False


