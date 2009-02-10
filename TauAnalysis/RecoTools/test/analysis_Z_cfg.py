

import FWCore.ParameterSet.Config as cms


process = cms.Process("ANA")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source(
    "PoolSource",
    #                            fileNames = cms.untracked.vstring('file:PATLayer1_ZToTaus.root')
    fileNames = cms.untracked.vstring('file:PATLayer1_fromAOD.root')
    )


# gen level analysis
process.load("TauAnalysis.GenSimTools.gen_cff")
process.load("TauAnalysis.GenSimTools.genZllReconstruction_cff")

process.particleListDrawer.maxEventsToPrint = cms.untracked.int32(10)
process.particleListDrawer.printOnlyHardInteraction = True


# reco level analysis 
# process.load("TauAnalysis.GenSimTools.recoDiTauReconstruction_cff")
process.load("TauAnalysis.RecoTools.recoZllReconstruction_cff")


process.recoZll.verbosity = 0

process.p1 = cms.Path(
    process.genParticlesPrint +
#    process.recoDiTauReconstruction + 
    process.genZllReconstruction +
    process.recoZllReconstruction
    )


process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *'),
    fileName = cms.untracked.string('analysis_Z.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring("p1")
        )
)

process.load("TauAnalysis.RecoTools.recoEventContent_cff")
process.out.outputCommands.extend( process.TauAnalysisRecoToolsEC.outputCommands )
process.load("TauAnalysis.GenSimTools.genEventContent_cff")
process.out.outputCommands.extend( process.TauAnalysisGenSimToolsEC.outputCommands )


process.outpath = cms.EndPath( process.out )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATLayer0Summary')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    PATLayer0Summary = cms.untracked.PSet( limit = cms.untracked.int32(10) )
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
