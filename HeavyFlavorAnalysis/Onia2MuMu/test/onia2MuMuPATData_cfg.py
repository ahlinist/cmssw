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
process.GlobalTag.globaltag = "GR09_R_35X_V3::All"

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Latest processing; files are from BSC skim, run 124120 lumis 1-59 
	'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/EC865CF0-1A2B-DF11-91F4-001CC47A52B6.root',
	'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/DCEE6C1B-052B-DF11-ACD7-001E0B5FB53E.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/DAA107B9-162B-DF11-BD3D-00237DA14F86.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/C88851E0-0C2B-DF11-8AEB-00237DA13FB6.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/A6EBA5BD-082B-DF11-B1B7-0017A477102C.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/9E229FC2-082B-DF11-BA33-0017A4771020.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/7C5889F6-172B-DF11-AFD2-0017A4770418.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/5A051558-0E2B-DF11-9A77-0017A4770C10.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/4AE212AF-052B-DF11-9846-001E0B48D108.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/2A1B18B6-082B-DF11-A7AD-0017A4770034.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/2206BC35-1B2B-DF11-88FA-00237DA1ED1C.root',
	#'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Mar3rdSkim_v2/0000/129A56F7-172B-DF11-A506-0017A4770014.root',
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
                                           minimumNDOF = cms.uint32(4),
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )

# filter to remove scraping ("monster") events
process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.25)
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
    fileName = cms.untracked.string('onia2MuMuPAT_Data2009_Run124120.root'),
    outputCommands = cms.untracked.vstring('drop *',
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep patMuons_patMuons__SkimmingOnia2MuMuPAT',
        'keep patCompositeCandidates_*__SkimmingOnia2MuMuPAT'
    ),
    ## Uncomment to activate skimming!
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
)
process.e = cms.EndPath(process.out)

##This is real data:
from HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff import onia2MuMu_isNotMC
onia2MuMu_isNotMC(process)

## If this is Summer09 MC
#process.patTrigger.processName    = "HLT8E29" 

