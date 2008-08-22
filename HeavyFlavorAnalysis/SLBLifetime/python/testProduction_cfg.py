import FWCore.ParameterSet.Config as cms

process = cms.Process("ana")
process.load("HeavyFlavorAnalysis.Examples.HFExample_cff")
#process.load("FastSimulation/Configuration/FamosSequences_cff")
#process.load("HeavyFlavorAnalysis.Examples.famos_cff")

process.load("RecoJets.Configuration.GenJetParticles_cff")

process.trkDump.tracksLabel = 'generalTracks'
process.trkDump.doTruthMatching = 2
process.signalDump.tracksLabel = 'generalTracks'
process.trkDump.verbose = 0
process.signalDump.verbose = 0

#process.genParticles.src = 'evtgenproducer'
process.genParticles.abortOnUnknownPDGCode = False


process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValbJpsiX_RECO.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValbJpsiX.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValJpsiMM_RECO.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValJpsiMM.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValBsMM_RECO.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValBsMM.root')
    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValUpsMM_RECO.root')
#    fileNames = cms.untracked.vstring('file:/export/data2/dambach/ValidationUrs/RelValUpsMM.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.p = cms.Path(process.genDump*
                     process.trkDump*
                     process.signalDump*
                     process.tree
                     )
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValbJpsiX_RECO.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValbJpsiX.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValJpsiMM_RECO.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValJpsiMM.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValBsMM_RECO.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValBsMM.root'
process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValUpsMM_RECO.root'
#process.tree.fileName = '/export/data2/dambach/ValidationUrs/ntplRelValUpsMM.root'
process.genParticles.abortOnUnknownPDGCode = False

