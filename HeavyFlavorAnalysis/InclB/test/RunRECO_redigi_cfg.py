import FWCore.ParameterSet.Config as cms

process = cms.Process("ana")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("HeavyFlavorAnalysis.InclB.HFGenerator_cff")
process.genDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTracks_cff")
process.trkDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFSimTracks_cff")
process.simtrkDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFSignal_cff")
process.signalDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFJets_cff")
process.jetDump.doflavortagging = 1
process.jetDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTrackJets_cff")
process.trackjetDump.doflavortagging = 0
process.trackjetDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFVertex_cff")

process.load("HeavyFlavorAnalysis.InclB.HFTrigger_redigi_cff")
process.triggerDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFCandidate_cff")
process.candidateDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFCandidateNew_cff")
process.candidateDumpNew.verbose = 0

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START3X_V26::All" # or "" or...


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V25B_356ReReco-v1/0007/FE90A396-233C-DF11-8106-002618943898.root')
    
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)
process.tree = cms.EDFilter("HFTree",
    fileName = cms.string('mc.root')
)

process.histo_begin = cms.EDFilter("HFHisto",
    fileName = cms.string('histo_begin.root')
)

process.histo_end = cms.EDFilter("HFHisto",
    fileName = cms.string('histo_end.root')
)

process.filterpkame = cms.EDFilter("PKAME",
    tracksLabel = cms.untracked.string('generalTracks')
)

process.myPartons = cms.EDFilter("PartonSelector",
    
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
    
)

# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# different choices of trigger conditions:
# bsc minbias
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41)')
# bsc minbias and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
# bsc minbias in coincidence with bptx
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41)')
# bsc minbias in coinidence with bptx and veto on beam halo
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


#filter based on L1 decision
#process.load('L1Trigger.Skimmer.l1Filter_cfi')
#process.l1Filter.algorithms = cms.vstring('L1_SingleJet6','L1_SingleForJet2')



#process.p = cms.Path(process.hltLevel1GTSeed + process.filterpkame*process.histo_begin*process.GenDump*process.myPartons*process.CandidateDump*process.CandidateDumpNew*process.JetDump*process.GenJetDump*process.TrackJetDump*process.TrackDump*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)
process.p = cms.Path(process.filterpkame*process.histo_begin*process.GenDump*process.SimTrackDump*process.myPartons*process.CandidateDump*process.CandidateDumpNew*process.GenJetDump*process.TrackJetDump*process.TrackDump*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)






