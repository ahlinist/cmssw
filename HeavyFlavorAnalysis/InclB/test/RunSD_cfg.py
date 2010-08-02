import FWCore.ParameterSet.Config as cms

process = cms.Process("ana")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.load("HeavyFlavorAnalysis.InclB.HFGenerator_cff")

process.load("HeavyFlavorAnalysis.InclB.HFPartons_cff")

process.load("HeavyFlavorAnalysis.InclB.HFTracks_cff")
process.trkDump.doTruthMatching = 0
process.trkDump.verbose = 0


process.load("HeavyFlavorAnalysis.InclB.HFSignal_cff")
process.signalDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTrackJets_cff")
process.trackjetDump.doflavortagging = 0

process.load("HeavyFlavorAnalysis.InclB.HFVertex_cff")
process.vertexDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFTrigger_cff")
process.triggerDump.dol1 = 2
process.triggerDump.verbose = 0

process.load("HeavyFlavorAnalysis.InclB.HFCandidate_cff")

process.load("HeavyFlavorAnalysis.InclB.HFCandidateNew_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_35X_V7::All" # or "" or...
#process.GlobalTag.globaltag = "GR09_R_35X_V4::All" # or "" or...

process.source = cms.Source("PoolSource",
   
    #fileNames = cms.untracked.vstring('file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/user/caminada/cmssw/CMSSW_3_1_4/copy/copy_2.root')
    fileNames = cms.untracked.vstring(
  
    'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v9/000/135/523/B492B232-8063-DF11-82F5-00E0817917DF.root'
 ##        'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/FC99FA82-8E45-DF11-B2B7-00E081791853.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F800D72C-9145-DF11-8DFA-003048670A06.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F0BE2343-8C45-DF11-86BD-00E08178C02A.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F01AF433-8D45-DF11-BAAC-003048635B86.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/EEAE2138-8D45-DF11-88AD-002481E1510C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/EC88A2DF-9145-DF11-B006-0025B31E3D3C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/E02134D0-9145-DF11-B848-003048D476C6.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/D0B2ADA4-8B45-DF11-8F4F-002481E15000.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/BC985DCF-9345-DF11-9FAF-00304866C51E.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/AA45B342-8A45-DF11-B952-00E0817918D3.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/A00FCD08-8046-DF11-85EA-003048670A0C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/9A7B9127-8D45-DF11-BC77-002481E14D64.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/9636AE2C-9145-DF11-B731-002481E1510C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/84886DC2-8D45-DF11-B9DD-003048673FE6.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/7C0F1A7E-9245-DF11-A5D6-003048D479FC.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/5EEEF385-9045-DF11-9ADC-00E0817918C3.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/56C3B805-8945-DF11-A1E2-003048673F1C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/54B205AC-8F45-DF11-B1AD-002481E150EE.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/4ABC065B-8F45-DF11-9C8B-0025B3E0653C.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3EF0D727-8D45-DF11-A879-00E081791859.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3CA9ABC9-8D45-DF11-8165-00E081791819.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3A4E7084-8E45-DF11-9635-0030486361F2.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/38F5689B-D445-DF11-B866-0025B3E06480.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/383CE239-9445-DF11-8ECA-0025B3E05CF8.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/383A957B-8845-DF11-B9B5-002481E14E62.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/34719668-8F45-DF11-9F5C-003048673F54.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/267ABE5D-7146-DF11-A703-002481E14E56.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/18A39F75-EF45-DF11-BC34-0025B3E06468.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/06824BCD-8246-DF11-9AA6-00E08178C129.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/025BBF90-8B45-DF11-9969-003048673FE6.root',
##         'file:dcap://t3se01.psi.ch:22125/pnfs/psi.ch/cms/trivcat/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/0219956C-8F45-DF11-B238-0025B3E05CF8.root',
    
    )
    
                          
   

)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.tree = cms.EDFilter("HFTree",
    fileName = cms.string('data.root')
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

# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# different choices of trigger conditions:
# bsc minbias
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41)')
# bsc minbias and veto on beam halo
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
# bsc minbias in coincidence with bptx
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41)')
# bsc minbias in coinidence with bptx and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


# filter on lumisections
#from HeavyFlavorAnalysis.InclB.goodLumiSectionList_cfi import *
#process.source.lumisToProcess = goodLumisToProcess

process.skimming = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
)


#filter based on L1 decision
process.load('L1Trigger.Skimmer.l1Filter_cfi')
process.l1Filter.algorithms = cms.vstring('L1_SingleJet6','L1_SingleForJet2')


#process.p = cms.Path(process.hltLevel1GTSeed + process.filterpkame*process.histo_begin*process.CandidateDump*process.CandidateDumpNew*process.TrackJetDumpAOD*process.TrackDumpAOD*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)

process.p = cms.Path(process.filterpkame*process.histo_begin*process.CandidateDump*process.CandidateDumpNew*process.TrackJetDumpAOD*process.TrackDumpAOD*process.SignalDump*process.VertexDump*process.TriggerDump*process.tree*process.histo_end)






