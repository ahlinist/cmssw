import FWCore.ParameterSet.Config as cms


treeDump = cms.EDAnalyzer("HFTree",
    verbose = cms.untracked.int32(0),
    requireSignal = cms.untracked.int32(0),
    pfmetLabel = cms.untracked.string('pfMet'),
    calometLabel = cms.untracked.string('met'),
    genmetLabel = cms.untracked.string('genMet'),
    fileName = cms.string('mcout.root')
                              
)

TreeDump = cms.Sequence(treeDump)





 
