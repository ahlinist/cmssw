import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
                            skipEvents = cms.untracked.uint32(0),
                            fileNames = cms.untracked.vstring(
    # One file from /BtoJPsiMuMu/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO
    'file:/nfs/data35/cms/keith/D2600DC8-15AC-DE11-B0FE-00221982D6CC.root'
   )  
)

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V1::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load("PhysicsTools.PatAlgos.cleaningLayer1.genericTrackCleaner_cfi")
process.cleanLayer1Tracks.checkOverlaps.muons.requireNoOverlaps = cms.bool(False)
process.cleanLayer1Tracks.checkOverlaps.electrons.requireNoOverlaps = cms.bool(False)

from PhysicsTools.PatAlgos.tools.trackTools import *
makeTrackCandidates(process, 
        label='TrackCands',                   # output collection will be 'allLayer0TrackCands', 'allLayer1TrackCands', 'selectedLayer1TrackCands'
        tracks=cms.InputTag('generalTracks'), # input track collection
        particleType="pi+",                   # particle type (for assigning a mass)
        preselection='pt > 0.1',              # preselection cut on candidates. Only methods of 'reco::Candidate' are available
        selection='pt > 0.1',                 # Selection on PAT Layer 1 objects ('selectedLayer1TrackCands')
        isolation={},                         # Isolations to use ('source':deltaR; set to {} for None)
        mcAs='muon'            # Replicate MC match as the one used for Muons
        );                                    #  you can specify more than one collection for this


process.p = cms.Path( process.patDefaultSequence )  #does not include cleaning for now

process.mkobjects = cms.EDAnalyzer("PATObjectExample")

process.ntup = cms.Path( process.mkobjects )

# Output module configuration
process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule("PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('output.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
)
process.outpath = cms.EndPath(process.out)
# save PAT Layer 1 output
from PhysicsTools.PatAlgos.patEventContent_cff import *
process.out.outputCommands += patEventContentNoLayer1Cleaning
process.out.outputCommands += patEventContent

process.out.outputCommands.append('keep *_selectedLayer1TrackCands_*_*')
process.out.outputCommands.append('keep *_cleanLayer1TrackCands_*_*')
