import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------------
from Configuration.EventContent.EventContent_cff import *
import FWCore.ParameterSet.Config as cms
#------------------------------------------------------------------------------


source = cms.Source(
  "PoolSource",
  debugFlag = cms.untracked.bool(False),
  debugVebosity = cms.untracked.uint32(10),
  fileNames = cms.untracked.vstring("file:anInput.root")
  )

#Out = cms.OutputModule(
#  "PoolOutputModule",
#  fileName = cms.untracked.string('')
#  )

options = cms.untracked.PSet(
  FailModule = cms.untracked.vstring('ProductNotFound'),
  wantSummary = cms.untracked.bool(False)
  )


MessageLogger = cms.Service(
  "MessageLogger",
  debugModules = cms.untracked.vstring( 'DTL1slhcPlots'),
  destinations = cms.untracked.vstring('DTL1slhcPlots.msg')
  )


DTL1slhcPlots = cms.EDAnalyzer(
  'DTL1slhcPlots',
  Data_Source = cms.InputTag("DTL1slhcProd"),
  GaugeSampleSize = cms.untracked.uint32(1500),
  breath = cms.untracked.int32(5),
  desert = cms.untracked.double(0.0005),
  rootFileNamePlots = cms.untracked.string('')
)

