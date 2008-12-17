import FWCore.ParameterSet.Config as cms

process = cms.Process("H4l")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:events.root")
)  

process.load("PhysicsTools/RecoAlgos/allTracks_cfi")

process.goodTracks = cms.EDFilter("CandSelector",
    src = cms.InputTag("allTracks"),
    cut = cms.string("pt > 3.0"),
    filter = cms.bool(False)                                  
)

process.Ztoll = cms.EDProducer("CandCombiner",
    decay = cms.string("goodTracks@+ goodTracks@-"),
    cut = cms.string("70.0 < mass < 110.0")                                
)

process.HiggsCandidates = cms.EDProducer("CandCombiner",
    decay = cms.string("Ztoll Ztoll"),
    cut = cms.string("mass < 600.0")
)                                                                       

process.out = cms.OutputModule("PoolOutputModule",
    string = cms.untracked.string("candidates.root"),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep recoTracks_*_*_*",
        "keep recoCandidatesOwned_*_*_*"
    )                                 
)

process.evtInfo = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(process.allTracks * process.goodTracks * process.Ztoll * process.HiggsCandidates)

process.ep = cms.EndPath( process.evtInfo + process.out)


