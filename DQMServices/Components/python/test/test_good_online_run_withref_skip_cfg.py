import FWCore.ParameterSet.Config as cms

process = cms.Process("TestDQMFileSaver")
process.load("DQMServices.Components.test.test_good_online_basic_cfi")

process.load("DQMServices.Components.test.MessageLogger_cfi")

process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("DQMServices.Core.DQM_cfg")

process.dqmmodules = cms.Path(process.dqmEnv+process.dqmSaver)
process.DQMStore.referenceFileName = 'Ref.root'
process.dqmSaver.referenceHandling = 'skip'
process.dqmSaver.convention = 'Online'
process.dqmEnv.subSystemFolder = 'TestSystem'


