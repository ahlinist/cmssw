import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.messageLogger_cfi')

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SingleDiffractiveWmunuPlus_10TeV_StageA43Bx_cff_py_RAW2DIGI_RECO.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorReco_cff')
process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.analysisSequences_cff')

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.outputModule_cfi')
process.output.fileName = 'edmDump_WMuNu.root'
process.output.SelectEvents.SelectEvents = cms.vstring('selection_step')

process.add_(cms.Service("TFileService",
                fileName = cms.string("analysisWMuNu_histos.root")
	)
)

process.reco_step = cms.Path(process.castorReco)
process.selection_step = cms.Path(process.wmunuSelectionSequence)
process.edmDump_step = cms.Path(process.edmDumpSequence)

process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.reco_step,process.selection_step,process.wmunuAnalysis,process.wmunu0PU,process.wmunu1PU,process.wmunu2PU,process.wmunu3PU,process.wmunu4PU,process.wmunuAfterFilter,process.edmDump_step,process.out_step)
