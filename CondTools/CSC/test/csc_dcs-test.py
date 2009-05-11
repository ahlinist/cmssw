import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.MessageLogger=cms.Service("MessageLogger",
  destinations=cms.untracked.vstring("cout"),
  cout=cms.untracked.PSet(
    treshold=cms.untracked.string("INFO")
  )
)

#process.CondDBCommon.connect = cms.string('oracle://cms_orcoff_prep/CMS_COND_30X_DQM_SUMMARY')
#process.CondDBCommon.DBParameters.authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = cms.string('sqlite_file:csc_dcs.db')
process.CondDBCommon.DBParameters.messageLevel = cms.untracked.int32(1)

process.source = cms.Source("EmptyIOVSource",
  timetype = cms.string('timestamp'),
  firstValue = cms.uint64(1),
  lastValue = cms.uint64(1),
  interval = cms.uint64(1)
)

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
  process.CondDBCommon,
  timetype = cms.untracked.string('timestamp'),
  toPut = cms.VPSet(
    cms.PSet(
      record = cms.string('CSCDCSDataRcd'),
      tag = cms.string('CSCDCSData')
    ),
  ),
  logconnect= cms.untracked.string('sqlite_file:csc_dcs_log.db')
)

process.CSCDCSData = cms.EDAnalyzer("CSCDCSDataPopConAnalyzer",
  record = cms.string('CSCDCSDataRcd'),
  loggingOn = cms.untracked.bool(True),
  SinceAppendMode = cms.bool(True),
  Source = cms.PSet(
    name = cms.untracked.string('CSCDCSDataHandler'),
    db_connect = cms.untracked.string('oracle://cms_orcon_prod/cms_csc_pvss_cond_r'),
    db_auth = cms.untracked.string('/nfshome0/valdo/private/'),
    log_level = cms.untracked.uint32(1)
  )
)

process.p = cms.Path(process.CSCDCSData)

