import FWCore.ParameterSet.Config as cms
import copy

isData = 1
useMuonSkim = 0
hltType = "HLT"
#hltType = "REDIGI38X"

process = cms.Process('TTEffSkim')

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
    version = cms.untracked.string('$Revision: 1.16 $'),
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
#      '/store/data/Run2010A/MinimumBias/RAW/v1/000/144/114/9AB9A0B1-F1B3-DF11-BCBF-001D09F24FEC.root'
#	'/store/data/Run2010B/Jet/RAW/v1/000/149/181/326E0028-28E2-DF11-8EF5-001D09F2546F.root'
#	'rfio:/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/pickevents_Ztautau_MikeOct2010_Mu_Run2010A-v1_RAW.root'
#	'rfio:/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/pickevents_Mu_pflow_tau_Run2010AB_TTEffMuSkim_cmssw399.root'
#	'rfio:/castor/cern.ch/user/s/swanson/muTau_HLT-AOD.root'
	'file:/tmp/slehti/muTau_HLT-AOD.root'
    )
  )
else:
  process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#      '/store/mc/Fall10/MinBias_TuneZ2_7TeV-pythia6/GEN-SIM-RAW/START38_V12-v1/0003/FEE7EA26-DCC7-DF11-BCD1-0030486790FE.root'
            "file:/tmp/slehti/test_H120_100_1_08t_RAW_RECO.root"
    )
  )

#skiming 
#process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
#process.load("HLTrigger/HLTfilters/hltLevel1GTSeed_cfi")
#process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#if(isData):
#  process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')
#else:
#  process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
#Replacing bit 40 and 41 with good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),	
     maxd0 = cms.double(2)	
)


process.scrapping = cms.EDFilter("FilterOutScraping",
    	applyfilter = cms.untracked.bool(True),
    	debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
)

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffPFTau_cff")

process.PFTauSkimmed = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('TTEffPFTausSelected'),
  minNumber = cms.uint32(1)
)

process.TTEffSkimTauFilterCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimTauFilterCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.tauFilter = cms.Path(
	process.TTEffSkimTauFilterCounterAllEvents
)

if(isData):
####    process.tauFilter *= process.hltLevel1GTSeed
    process.tauFilter *= process.primaryVertexFilter
    process.tauFilter *= process.scrapping

process.tauFilter *= process.TTEffPFTau
process.tauFilter *= process.PFTauSkimmed
process.tauFilter *= process.TTEffSkimTauFilterCounterSavedEvents


process.TTEffSkimMuonFilterCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimMuonFilterCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTFilter_cff")
process.MuonHLTs.hltResults    = cms.InputTag("TriggerResults","",hltType)

process.muonFilter = cms.Path(
        process.TTEffSkimMuonFilterCounterAllEvents *
        process.MuonHLTs *
	process.TTEffSkimMuonFilterCounterSavedEvents
)

TTEffSkimSelection = cms.vstring('tauFilter')
if(useMuonSkim):
    TTEffSkimSelection = cms.vstring('muonFilter','tauFilter')

# Output definition
process.FEVTEventContent.outputCommands.append('drop *_*_*_TTEffSkim')
process.FEVTEventContent.outputCommands.append('drop *_*_*_RECO')
process.FEVTEventContent.outputCommands.append('keep edmHepMCProduct_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoGenParticles_*_*_*')
#process.FEVTEventContent.outputCommands.append('keep *_TTEffPFTausSelected_*_*')
#process.FEVTEventContent.outputCommands.append('keep *_TTEffPFTauDiscriminationByLeadingTrackFinding_*_*')
#process.FEVTEventContent.outputCommands.append('keep *_TTEffPFTauDiscriminationByIsolation_*_*')
#process.FEVTEventContent.outputCommands.append('keep *_TTEffPFTauDiscriminationAgainstMuon_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFTaus_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFTauDiscriminator_*_*_*')
process.FEVTEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
process.FEVTEventContent.outputCommands.append('keep *_elecpreid_*_*')
process.FEVTEventContent.outputCommands.append('keep *_particleFlow_*_*')
process.FEVTEventContent.outputCommands.append('keep *_ak5PFJets_*_*')
process.FEVTEventContent.outputCommands.append('keep *_generalTracks_*_*')
process.FEVTEventContent.outputCommands.append('keep *_hltHbhereco_*_*')
process.FEVTEventContent.outputCommands.append('keep recoMuons_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFBlocks_*_*_*')
process.FEVTEventContent.outputCommands.append('keep L1GctJetCands_*_*_*')
process.FEVTEventContent.outputCommands.append('keep HcalNoiseSummary_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoPFMETs_*_*_*')
process.FEVTEventContent.outputCommands.append('keep *_l1extraParticles_*_*')
process.FEVTEventContent.outputCommands.append('keep L1GlobalTriggerReadoutRecord_*_*_*')
process.FEVTEventContent.outputCommands.append('keep recoBeamSpot_*_*_*')
#process.FEVTEventContent.outputCommands.append('keep *')
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTEventContent.outputCommands,
    fileName = cms.untracked.string('TTEffSkim.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-RECO')
    ),
    SelectEvents = cms.untracked.PSet(SelectEvents = TTEffSkimSelection)
)

# Additional output definition

# Other statements
if(isData):
  #process.GlobalTag.globaltag = "GR10_P_V10::All" # propt reco 
  process.GlobalTag.globaltag = "GR_R_311_V4::All" # reprocessing  
else:
  process.GlobalTag.globaltag = 'START311_V2A::All'
  #process.GlobalTag.globaltag = 'MC_36Y_V9::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
#process.dqmoffline_step = cms.Path(process.DQMOffline)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.tauFilter,process.endjob_step,process.out_step)
if(useMuonSkim):
     process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.muonFilter,process.tauFilter,process.endjob_step,process.out_step)

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
    process.newSeedFromTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double(40.6)

    ### 2nd step of iterative tracking
    #---- new parameters ----
    process.secTriplets.RegionFactoryPSet.RegionPSet.nSigmaZ  = cms.double(4.47)  
    process.secTriplets.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double(44.7)

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


