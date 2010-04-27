import FWCore.ParameterSet.Config as cms

process = cms.Process("SkimmingOnia2MuMuPAT")

### standard includes
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

### global tag
process.GlobalTag.globaltag = "GR_R_35X_V7::All"

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/EC865CF0-1A2B-DF11-91F4-001CC47A52B6.root'
        #'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/jpsi/CS_Onia_Run133321_26238844-9749-DF11-B896-003048D47A64.root'
        '/store/data/Commissioning10/MinimumBias/USER/v8/000/133/321/269F73D7-9C49-DF11-9251-003048D46236.root',
   )
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#############REAL DATA######################
# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# bsc minbias and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
# set the L1MinBiasVetoBeamHalo bit
process.L1MinBiasVetoBeamHalo = cms.Path(process.hltLevel1GTSeed)

# this is for filtering on HLT physics bit
process.hltPhysicsDeclared = cms.EDFilter("HLTHighLevel",
                                 TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                 HLTPaths = cms.vstring("HLT_PhysicsDeclared"
                                                        ),
                                 eventSetupPathsKey = cms.string(''),
                                 andOr = cms.bool(True),
                                 throw = cms.bool(False)  # Avoid crashes if the job happens to process runs that don't have the HLT_PhysicsDeclared (e.g. 122294)
                                 )
# copy the PhysicsDeclared bit
process.PhysicsDeclared = cms.Path(process.hltPhysicsDeclared)

# this is for filtering on HLT MinBiasBSC bit
process.hltMinBiasBSC = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                     HLTPaths = cms.vstring("HLT_MinBiasBSC"
                                                            ),
                                     eventSetupPathsKey = cms.string(''),
                                     andOr = cms.bool(True),
                                     throw = cms.bool(True)
                                     )
# copy the MinBiasBSC bit
process.MinBiasBSC = cms.Path(process.hltMinBiasBSC)

# filter on good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4),
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )
# set the PrimaryVertex bit
process.PrimaryVertex = cms.Path(process.primaryVertexFilter)
 
# filter to remove scraping ("monster") events
process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.25)
                                      )
# set the Scraping bit
process.Scraping = cms.Path(process.scrapingFilter)

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
process.Onia2MuMuPatTrkTrk = cms.Path(
        # process.hltLevel1GTSeed +
        # process.hltPhysicsDeclared +
        # process.hltMinBiasBSC +
        # process.primaryVertexFilter +
        # process.scrapingFilter +
        process.patMuonSequence +     # produce PAT muons for Onia2MuMu (includes merging with CaloMuons)
        process.onia2MuMuPatTrkTrk +  # make J/Psi's (inclusively down to tracker+tracker)
        process.selectedEvents        # select events with J/Psi's
)


### output
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('onia2MuMuPATData.root'),
    outputCommands = cms.untracked.vstring('drop *',
        'keep patCompositeCandidates_*__SkimmingOnia2MuMuPAT', ## PAT di-muons
        'keep patMuons_patMuons__SkimmingOnia2MuMuPAT',        ## All PAT muons (note: not necessary if you use only the di-muons)
        'keep *_offlinePrimaryVertices_*_*',                   ## Primary vertices: you want these to compute impact parameters
        'keep edmTriggerResults_TriggerResults_*_*',           ## HLT info, per path (cheap)
        'keep l1extraL1MuonParticles_l1extraParticles_*_*',    ## L1 info (cheap)
        #'keep *_patTrigger_*_*',                               ## HLT info, per object (BIG. Keep only when debugging trigger match)
    ),
    ## Uncomment to activate skimming!
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Onia2MuMuPatTrkTrk') )
)
process.e = cms.EndPath(process.out)

##This is real data:
from HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff import onia2MuMu_isNotMC
onia2MuMu_isNotMC(process)

## If this is Summer09 MC
#process.patTrigger.processName    = "HLT8E29" 

