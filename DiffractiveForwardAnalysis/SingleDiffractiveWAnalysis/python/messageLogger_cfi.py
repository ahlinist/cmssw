import FWCore.ParameterSet.Config as cms

from FWCore.MessageService.MessageLogger_cfi import *
MessageLogger.debugModules = cms.untracked.vstring('')
MessageLogger.cerr.threshold = 'WARNING'
MessageLogger.categories.append('Analysis')
MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)
MessageLogger.cerr.INFO = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)
