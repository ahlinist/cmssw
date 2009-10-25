

import FWCore.ParameterSet.Config as cms

# Choose analysis mode:
# 0: Z->mu,mu 
# 1: Z->tau,tau->mu,tau-jet
mode = 1

process = cms.Process("ANA")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring('file:PATLayer1_fromAOD.root')
    )


# gen level analysis
process.load("TauAnalysis.GenSimTools.gen_cff")
if mode==0:
    print "*** Z->mu,mu analysis ***"
    process.load("TauAnalysis.GenSimTools.genZllReconstruction_cff")
    process.genZll.verbosity = 1
else:
    print "*** Z->tau,tau->mu,tau-jet analysis ***"
    process.load("TauAnalysis.GenSimTools.genDiTauReconstruction_cff")
    process.genDiTau.verbosity = 0
    
    
process.particleListDrawer.maxEventsToPrint = cms.untracked.int32(3)
process.particleListDrawer.printOnlyHardInteraction = True


process.p1 = cms.Path(
    process.genParticlesPrint
    )

# reco level analysis 
if mode==0:
    process.load("TauAnalysis.RecoTools.recoZllReconstruction_cff")
    process.p1 += ( process.genZllReconstruction +
                    process.recoZllReconstruction )
else:
    process.load("TauAnalysis.RecoTools.recoDiTauReconstruction_cff")
    process.p1 += ( process.genDiTauReconstruction +
                    process.recoDiTauReconstruction )


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


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
