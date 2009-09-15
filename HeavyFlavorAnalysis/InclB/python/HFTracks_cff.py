import FWCore.ParameterSet.Config as cms

from SimGeneral.TrackingAnalysis.trackingParticles_cfi import *
from Configuration.StandardSequences.MagneticField_38T_cff import *
from SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi import *
from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from PhysicsTools.RecoAlgos.allTracks_cfi import *
from PhysicsTools.RecoAlgos.allTrackCandidates_cfi import *
from PhysicsTools.HepMCCandAlgos.allTracksGenParticlesMatch_cfi import *
allTracksGenParticlesMatch.src = "allTrackCandidates"



matching = cms.EDProducer( "MCTruthDeltaRMatcherNew",
                           src = cms.InputTag("allTrackCandidates"),
                           matched = cms.InputTag("genParticles"),
                           distMin = cms.double(0.15)
                         )

anal = cms.EDAnalyzer("EventContentAnalyzer")

trkDump = cms.EDFilter("HFDumpTracks",
    
    tracksLabel = cms.untracked.string('generalTracks'),
    muonsLabel = cms.untracked.InputTag("muons"),
    verbose = cms.untracked.int32(0),
    doTruthMatching = cms.untracked.int32(3),
                       
    generatorEventLabel = cms.untracked.string('source'),              #famos matching
    simTracksLabel = cms.untracked.string('famosSimHits'),             #famos matching

    trackingParticlesLabel = cms.untracked.string('trackingParticles'),#reco matching
    associatorLabel = cms.untracked.string('TrackAssociatorByHits'),   #reco matching
  
   
    #associatorLabel3 = cms.untracked.string('allTracksGenParticlesMatch'), #aod matching
    associatorLabel3 = cms.untracked.string('matching'),                    #aod matching
    genParticlesLabel = cms.untracked.string('genParticles'),               #aod matching
    trackcandsLabel = cms.untracked.string('allTrackCandidates')            #aod matching
)


TrackDump = cms.Sequence(allTrackCandidates*matching*trkDump)
#TrackDump = cms.Sequence(trkDump)
