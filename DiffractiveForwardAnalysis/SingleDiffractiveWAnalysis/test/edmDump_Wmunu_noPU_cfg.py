import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.messageLogger_cfi')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorReco_cff')
process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisSequencesNoPileUp_cff')

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.outputModule_cfi')
process.output.fileName = 'PYTHIA6_Wmunu_EdmDump_noPU.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.add_(cms.Service("TFileService",
                fileName = cms.string("analysisWMuNu_histos_Wmunu_noPU.root")
	)
)

process.reco_step = cms.Path(process.castorReco)
process.selection_step = cms.Path(process.wmunuSelectionSequence)
process.edmDump_step = cms.Path(process.edmDumpSequence)

process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.reco_step,process.selection_step,process.wmunuAnalysis,process.wmunuAfterFilter,process.edmDump_step,process.out_step)
