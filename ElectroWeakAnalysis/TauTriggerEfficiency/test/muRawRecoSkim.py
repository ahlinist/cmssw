import FWCore.ParameterSet.Config as cms
import copy

isData = 0

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

if(isData):
  process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
else:
  process.load('Configuration.StandardSequences.RawToDigi_cff')

process.MessageLogger.cerr.threshold = 'INFO'
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(

)
# Input source
if(isData):
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/data/Run2010A/MinimumBias/RAW/v1/000/144/114/9AB9A0B1-F1B3-DF11-BCBF-001D09F24FEC.root'
    )
  )
else:
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#      '/store/mc/Fall10/MinBias_TuneZ2_7TeV-pythia6/GEN-SIM-RAW/START38_V12-v1/0003/FEE7EA26-DCC7-DF11-BCD1-0030486790FE.root'
	"rfio:/castor/cern.ch/user/s/slehti/testData/Fall10_DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola_GEN-SIM-RAW_START38_V12-v1_0000_8037FA24-3EC8-DF11-9725-00215E221B48.root"
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

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")
process.MuonHLTs.hltResults    = cms.InputTag('TriggerResults::REDIGI38X')

process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.muonFilter = cms.Path(
        process.TTEffSkimCounterAllEvents *
        process.MuonHLTs *
        process.TTEffSkimCounterSavedEvents
)

process.tauFilter = cms.Path(
	process.hltLevel1GTSeed *
	process.scrapping *
	process.PFTausSelected *
	process.PFTauSkimmed
)
# Output definition
process.FEVTEventContent.outputCommands.append('keep edmHepMCProduct_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoGenParticles_*_*_*')
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTEventContent.outputCommands,
    fileName = cms.untracked.string('TTEffSkim.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-RECO'),
        #filterName = cms.untracked.string('tauFilter')
    ),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('muonFilter','tauFilter'))
)

# Additional output definition

# Other statements
if(isData):
  #process.GlobalTag.globaltag = "GR10_P_V10::All" # propt reco 
  process.GlobalTag.globaltag = "GR_R_38X_V13A::All" # reprocessing  
else:
  process.GlobalTag.globaltag = 'START38_V13::All'
  #process.GlobalTag.globaltag = 'MC_36Y_V9::All'

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(
	process.raw2digi_step,
	process.L1Reco_step,
	process.reconstruction_step,
	process.muonFilter,
	process.tauFilter,
	process.endjob_step,
	process.out_step)


# Automatic addition of the customisation function

def customiseCommon(process):
    
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
    process.fourthPLSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters=20000
    process.thTripletsA.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000
    process.thTripletsB.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000

    ###### FIXES TRIPLETS FOR LARGE BS DISPLACEMENT ######

    ### prevent bias in pixel vertex
    process.pixelVertices.useBeamConstraint = False
    
    ### pixelTracks
    #---- new parameters ----
    process.pixelTracks.RegionFactoryPSet.RegionPSet.nSigmaZ  = 4.06
    process.pixelTracks.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double(40.6)

    ### 0th step of iterative tracking
    #---- new parameters ----
    process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ   = cms.double(4.06)  
    process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 40.6

    ### 2nd step of iterative tracking
    #---- new parameters ----
    process.secTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ  = cms.double(4.47)  
    process.secTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = 44.7

    ## ECAL 
    process.ecalRecHit.ChannelStatusToBeExcluded = [ 1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 78, 142 ]

    ###
    ###  end of top level replacements
    ###
    ###############################################################################################

    return (process)


##############################################################################
def customisePPData(process):
    process= customiseCommon(process)

    ## particle flow HF cleaning
    process.particleFlowRecHitHCAL.LongShortFibre_Cut = 30.
    process.particleFlowRecHitHCAL.ApplyPulseDPG = True

    ## HF cleaning for data only
    process.hcalRecAlgos.SeverityLevels[3].RecHitFlags.remove("HFDigiTime")
    process.hcalRecAlgos.SeverityLevels[4].RecHitFlags.append("HFDigiTime")

    ##beam-halo-id for data only
    process.CSCHaloData.ExpectedBX = cms.int32(3)

    ## hcal hit flagging
    process.hfreco.PETstat.flagsToSkip  = 2
    process.hfreco.S8S1stat.flagsToSkip = 18
    process.hfreco.S9S1stat.flagsToSkip = 26
    
    return process


##############################################################################
def customisePPMC(process):
    process=customiseCommon(process)
    
    return process

##############################################################################
def customiseCosmicData(process):

    return process

##############################################################################
def customiseCosmicMC(process):
    
    return process
        

##############################################################################
def customiseExpress(process):
    process= customisePPData(process)

    import RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi
    process.offlineBeamSpot = RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi.onlineBeamSpotProducer.clone()
    
    return process

##############################################################################
def customisePrompt(process):
    process= customisePPData(process)

    import RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi
    process.offlineBeamSpot = RecoVertex.BeamSpotProducer.BeamSpotOnline_cfi.onlineBeamSpotProducer.clone()
    
    return process


# End of customisation function definition

process = customisePPMC(process)


