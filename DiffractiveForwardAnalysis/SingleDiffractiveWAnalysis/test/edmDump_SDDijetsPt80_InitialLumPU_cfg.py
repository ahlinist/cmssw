import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.messageLogger_cfi')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SingleDiffractiveWmunuPlus_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorReco_cff')
process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisSequencesDiJets_cff')

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.outputModule_cfi')
process.output.fileName = 'POMWIG_SDDijetsPt80_InitialLumPU_EdmDump.root'
#process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')
process.output.SelectEvents.SelectEvents = cms.vstring('')

process.reco_step = cms.Path(process.pileUpInfo+process.castorReco)
#process.selection_step = cms.Path()
process.edmDump_step = cms.Path(process.edmDumpSequence)

process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.reco_step,process.edmDump_step,process.out_step)
