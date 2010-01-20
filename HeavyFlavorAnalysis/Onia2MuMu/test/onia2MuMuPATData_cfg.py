import FWCore.ParameterSet.Config as cms

process = cms.Process("SkimmingOnia2MuMuPAT")

### standard includes
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

### global tag
process.GlobalTag.globaltag = "GR09_R_V5::All"

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
         '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/FCFEFB8D-E0F0-DE11-A01D-0015178C6B88.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/F8269247-E0F0-DE11-9431-00151796D9C4.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/F4E10D24-E0F0-DE11-870D-0024E8767DAD.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/EAFCDCA2-E0F0-DE11-AF3D-0015178C6B8C.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/E2F75B8B-E0F0-DE11-8435-0024E8768D34.root',
        '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Dec19thSkim_336p3_v1/0008/DE4AFC37-E0F0-DE11-A291-001D0967DC3D.root'
   )
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

#############REAL DATA######################
# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# bsc minbias and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# this is for filtering on HLT physics bit
process.hltPhysicsDeclared = cms.EDFilter("HLTHighLevel",
                                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                 HLTPaths = cms.vstring("HLT_PhysicsDeclared"
                                                        ),
                                 eventSetupPathsKey = cms.string(''),
                                 andOr = cms.bool(True),
                                 throw = cms.bool(True)
                                 )

# this is for filtering on HLT MinBiasBSC bit
process.hltMinBiasBSC = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                     HLTPaths = cms.vstring("HLT_MinBiasBSC"
                                                            ),
                                     eventSetupPathsKey = cms.string(''),
                                     andOr = cms.bool(True),
                                     throw = cms.bool(True)
                                     )

# filter on good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNumberOfTracks = cms.uint32(3) ,
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )

# filter to remove scraping ("monster") events
process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.2)
                                      )

###############################################



### load Onia2MuMu PAT
process.load("HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff")
# it will define:
#   - patMuonSequence: makes the 'patMuons' for Onia2MuMu
#   - onia2MuMuPat<X>: makes di-muons. <X> can be GlbGlb, GlbTrk, GlbCal, TrkTrk, TrkCal.
#                      categories are inclusive: Cal includes Trk which includes Glb.
#                      just run the loosest one you need.

### skim requiring at least one (inclusive) tracker+tracker di-muon
process.selectedEvents = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('onia2MuMuPatTrkTrk'),
    minNumber = cms.uint32(1),
) 

### path
process.p = cms.Path(
        process.hltLevel1GTSeed +
        process.hltPhysicsDeclared +
        process.hltMinBiasBSC +
        process.primaryVertexFilter +
        process.scrapingFilter +
        process.patMuonSequence +     # produce PAT muons for Onia2MuMu (includes merging with CaloMuons)
        process.onia2MuMuPatTrkTrk   # make J/Psi's (inclusively down to tracker+tracker)
        #process.selectedEvents        # select events with J/Psi's
)


### output
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('BeamCommissioning09_BSCNOBEAMHALO_Dec19thSkim_336p3.root'),
    outputCommands = cms.untracked.vstring('drop *',
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep patMuons_patMuons__SkimmingOnia2MuMuPAT',
        'keep patCompositeCandidates_*__SkimmingOnia2MuMuPAT'
    ),
    ## Uncomment to activate skimming!
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
)
process.e = cms.EndPath(process.out)

## If this is real data:
from HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff import onia2MuMu_isNotMC
onia2MuMu_isNotMC(process)
process.source.inputCommands = cms.untracked.vstring("keep *", "drop *_genParticles_*_*")

## If this is Summer09 MC
#process.patTrigger.processName    = "HLT8E29" 

