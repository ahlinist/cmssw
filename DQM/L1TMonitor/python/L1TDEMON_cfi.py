import FWCore.ParameterSet.Config as cms

l1demon = cms.EDFilter("L1TDEMON",
    HistFolder = cms.untracked.string('L1TEMU/'),
    VerboseFlag = cms.untracked.int32(0),
    DataEmulCompareSource = cms.InputTag("l1compare"),
    DQMStore = cms.untracked.bool(True),
    HistFile = cms.untracked.string('')
)


