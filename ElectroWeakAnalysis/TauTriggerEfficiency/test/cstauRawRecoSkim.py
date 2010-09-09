# Auto generated configuration file
# using: 
# Revision: 1.172 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: reco -s RAW2DIGI,L1Reco,RECO,DQM --data --magField AutoFromDBCurrent --scenario pp --datatier RECO --eventcontent RECO --customise Configuration/GlobalRuns/customise_Collision_36X.py --no_exec --python_filename=rereco_Collision_36X.py --conditions GR_R_36X_V12B::All
import FWCore.ParameterSet.Config as cms
import copy

isData = 1

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.172 $'),
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2010A/MinimumBias/RAW/v1/000/144/114/9AB9A0B1-F1B3-DF11-BCBF-001D09F24FEC.root'
    )
)

#skiming 
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
process.load("HLTrigger/HLTfilters/hltLevel1GTSeed_cfi")
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
if(isData):
  process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')
else:
  process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

process.scrapping = cms.EDFilter("FilterOutScraping",
    	applyfilter = cms.untracked.bool(True),
    	debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
)

process.PFTausSelected = cms.EDFilter("PFTauSelector",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
	cms.PSet( discriminator=cms.InputTag("shrinkingConePFTauDiscriminationByIsolation"),
		   selectionCut=cms.double(0.5)
	),
    ),
    cut = cms.string('et > 15. && abs(eta) < 2.5') 
)

process.PFTauSkimmed = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('PFTausSelected'),
  minNumber = cms.uint32(1)
)


process.tauFilter = cms.Path(
	process.hltLevel1GTSeed *
	process.scrapping *
	process.PFTausSelected *
	process.PFTauSkimmed
)
# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTEventContent.outputCommands,
    fileName = cms.untracked.string('CSTauSkim.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-RECO'),
        filterName = cms.untracked.string('tauFilter')
    )
)

# Additional output definition

# Other statements
if(isData):
  process.GlobalTag.globaltag = "GR_R_37X_V6D::All"  
else:
  #process.GlobalTag.globaltag = 'START36_V10::All'
  process.GlobalTag.globaltag = 'MC_36Y_V9::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.out_step)


# Automatic addition of the customisation function

def customise(process):
    
    #####################################################################################################
    ####
    ####  Top level replaces for handling strange scenarios of early collisions
    ####

    ## TRACKING:
    ## Skip events with HV off
    process.newSeedFromTriplets.ClusterCheckPSet.MaxNumberOfPixelClusters=2000
    process.newSeedFromPairs.ClusterCheckPSet.MaxNumberOfCosmicClusters=20000
    process.secTriplets.ClusterCheckPSet.MaxNumberOfPixelClusters=2000
    process.fifthSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters = 20000
    process.fourthPLSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters= 20000
    process.thTripletsA.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000
    process.thTripletsB.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000

    ## local tracker strip reconstruction
    process.OutOfTime.TOBlateBP=0.071
    process.OutOfTime.TIBlateBP=0.036

    ###### FIXES TRIPLETS FOR LARGE BS DISPLACEMENT ######

    ### prevent bias in pixel vertex
    process.pixelVertices.useBeamConstraint = False
    
    ### pixelTracks
    #---- new parameters ----
    process.pixelTracks.RegionFactoryPSet.RegionPSet.nSigmaZ  = cms.double(4.06)
    process.pixelTracks.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double(40.6)
    
    ### 0th step of iterative tracking
    #---- new parameters ----
    process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ   = cms.double(4.06)
    process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 40.6

    ### 2nd step of iterative tracking
    #---- new parameters ----
    process.secTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ  = cms.double(4.47)
    process.secTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 44.7

    ## Primary Vertex
    process.offlinePrimaryVerticesWithBS.PVSelParameters.maxDistanceToBeam = 2
    process.offlinePrimaryVerticesWithBS.TkFilterParameters.maxNormalizedChi2 = 20
    process.offlinePrimaryVerticesWithBS.TkFilterParameters.maxD0Significance = 100
    process.offlinePrimaryVerticesWithBS.TkFilterParameters.minPixelLayersWithHits = 2
    process.offlinePrimaryVerticesWithBS.TkFilterParameters.minSiliconLayersWithHits = 5
    process.offlinePrimaryVerticesWithBS.TkClusParameters.TkGapClusParameters.zSeparation = 1
    process.offlinePrimaryVertices.PVSelParameters.maxDistanceToBeam = 2
    process.offlinePrimaryVertices.TkFilterParameters.maxNormalizedChi2 = 20
    process.offlinePrimaryVertices.TkFilterParameters.maxD0Significance = 100
    process.offlinePrimaryVertices.TkFilterParameters.minPixelLayersWithHits = 2
    process.offlinePrimaryVertices.TkFilterParameters.minSiliconLayersWithHits = 5
    process.offlinePrimaryVertices.TkClusParameters.TkGapClusParameters.zSeparation = 1

    ## ECAL 
    process.ecalRecHit.ChannelStatusToBeExcluded = [ 1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 78, 142 ]

    ##Preshower
    process.ecalPreshowerRecHit.ESBaseline = cms.int32(0)

    ## HCAL temporary fixes
    process.hfreco.firstSample  = 3
    process.hfreco.samplesToAdd = 4
    
    ## EGAMMA
    process.photons.minSCEtBarrel = 5.
    process.photons.minSCEtEndcap =5.
    process.photonCore.minSCEt = 5.
    process.conversionTrackCandidates.minSCEt =5.
    process.conversions.minSCEt =5.
    process.trackerOnlyConversions.rCut = 2.
    process.trackerOnlyConversions.vtxChi2 = 0.0005
    
    ###
    ###  end of top level replacements
    ###
    ###############################################################################################


    #add the DQM stream for this time only
    # DQMStream output definition
    #process.outputDQMStream = cms.OutputModule("PoolOutputModule",
    #                                           outputCommands = cms.untracked.vstring('drop *',
    #                                                                                  'keep *_MEtoEDMConverter_*_*'),
    #                                           fileName = cms.untracked.string('DQMStream.root'),
    #                                           dataset = cms.untracked.PSet(
    #    filterName = cms.untracked.string(''),
    #    dataTier = cms.untracked.string('DQM')
    #    )
    #)
    #process.outputDQMStreamOutPath = cms.EndPath(process.outputDQMStream)
    #process.schedule.append( process.outputDQMStreamOutPath )

    

    return (process)


# End of customisation function definition

process = customise(process)


