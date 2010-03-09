import FWCore.ParameterSet.Config as cms

process = cms.Process("write")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBSetup,
    # Writing to oracle needs the following shell variable setting (in zsh):
    # export CORAL_AUTH_PATH=/afs/cern.ch/cms/DB/conddb
    # string connect = "oracle://cms_orcoff_int2r/CMS_COND_ALIGNMENT"
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('sqlite_file:output.db'),
    # untracked uint32 authenticationMethod = 1
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('GlobalPositionRcd'),
        tag = cms.string('IdealGeometry')
    ))
)

process.GlobalPositionRcdWrite = cms.EDAnalyzer("GlobalPositionRcdWrite",
    hcal = cms.PSet(
        beta = cms.double(0.0),
        alpha = cms.double(0.0),
        y = cms.double(0.0),
        x = cms.double(0.0),
        z = cms.double(0.0),
        gamma = cms.double(0.0)
    ),
    muon = cms.PSet(
        beta = cms.double(0.0),
        alpha = cms.double(0.0),
        y = cms.double(0.0),
        x = cms.double(0.0),
        z = cms.double(0.0),
        gamma = cms.double(0.0)
    ),
    tracker = cms.PSet(
        beta = cms.double(0.0),
        alpha = cms.double(0.0),
        y = cms.double(0.0),
        x = cms.double(0.0),
        z = cms.double(0.0),
        gamma = cms.double(0.0)
    ),
    ecal = cms.PSet(
        beta = cms.double(0.0),
        alpha = cms.double(0.0),
        y = cms.double(0.0),
        x = cms.double(0.0),
        z = cms.double(0.0),
        gamma = cms.double(0.0)
    ),
    calo = cms.PSet(
        beta = cms.double(0.0),
        alpha = cms.double(0.0),
        y = cms.double(0.0),
        x = cms.double(0.0),
        z = cms.double(0.0),
        gamma = cms.double(0.0)
    )
)

process.p = cms.Path(process.GlobalPositionRcdWrite)
process.CondDBSetup.DBParameters.messageLevel = 2


