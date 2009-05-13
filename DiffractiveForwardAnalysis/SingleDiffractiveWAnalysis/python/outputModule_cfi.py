import FWCore.ParameterSet.Config as cms

MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
MyEventContent.outputCommands.append('keep *_*_*_Analysis')
MyEventContent.outputCommands.append('keep *_genParticles_*_*')
MyEventContent.outputCommands.append('keep recoMuons_muons_*_*')
MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
MyEventContent.outputCommands.append('keep *_offlinePrimaryVerticesWithBS_*_*')
MyEventContent.outputCommands.append('keep *_pixelVertices_*_*')

output = cms.OutputModule("PoolOutputModule",
    MyEventContent,
    fileName = cms.untracked.string(''),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('')
    )
)
