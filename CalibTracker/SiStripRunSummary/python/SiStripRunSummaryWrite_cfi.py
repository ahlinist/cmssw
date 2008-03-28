import FWCore.ParameterSet.Config as cms

modSiStripRSFWrite = cms.EDFilter("SiStripRunSummaryWrite",
    # [Parent] ConditionDBWriter configuration
    IOVMode = cms.string('Job'),
    SinceAppendMode = cms.bool(True),
    # Module configuration
    oFileXMLIn = cms.untracked.string('archive.xml'),
    doStoreOnDB = cms.bool(True),
    Record = cms.string('SiStripRunSummaryRcd')
)


