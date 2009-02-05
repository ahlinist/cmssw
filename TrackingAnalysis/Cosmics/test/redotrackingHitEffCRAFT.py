import FWCore.ParameterSet.Config as cms

process = cms.Process("ReRec")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("CondCore.DBCommon.CondDBSetup_cfi")


process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #run 69912
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/02E4FEEC-B2B9-DD11-9110-0018F3D096DC.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/0A4A3CFB-B7B9-DD11-A305-0018F3D09604.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/0EC7E7B3-EBB9-DD11-961D-001731AF677F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/1C1CADD0-A1B9-DD11-AEC2-001731AF698F.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/3A24AB79-93B9-DD11-B6EE-003048754FB5.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/3EF62D01-B8B9-DD11-861F-003048767DC7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/485EC5AB-BDB9-DD11-8820-003048769E89.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/667A9CFF-B7B9-DD11-BDAD-0018F3D096E4.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/680B45A4-DCB9-DD11-AD85-0018F3D095EA.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/6A260BA3-BEB9-DD11-971C-001A92810ADE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/7A2E9FA3-DAB9-DD11-9C6B-00304875AAE7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/80D81601-6AB9-DD11-82CB-003048D15E96.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/9A830357-D0B9-DD11-A08E-00304875AAE7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/AACDFC1A-CDB9-DD11-8395-001A92810AEE.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/BCF0AAA0-E1B9-DD11-883A-001A92810AC6.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/FE7D1183-97B9-DD11-BD79-0017312A2519.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/FED6F8D4-B4B9-DD11-AE5F-00304875AAE7.root',
        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V3_CMSSW_2_2_0-RerecoTest-v4/0030/FEDF23E6-A1B9-DD11-A57B-0018F3D09664.root'
#Run 67541
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/0AB5E15F-27AD-DD11-B4AD-001A928116EA.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/1449A643-27AD-DD11-A45D-0030486790C0.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/32287063-27AD-DD11-83C9-003048678B0E.root',
#       '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/346E69BE-26AD-DD11-95FE-003048678E92.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/620561F7-26AD-DD11-BECC-001A92971B94.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/6C12F366-27AD-DD11-84B2-0030486792B2.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/A418B1AC-26AD-DD11-BF61-00304867C1BA.root',
#        '/store/data/Commissioning08/Cosmics/RECO/CRAFT_V3P_SuperPointing_v4/0041/A4B9475B-27AD-DD11-A760-0030486791AA.root'
    )
)


# output module#
process.load("Configuration.EventContent.EventContentCosmics_cff")

process.load("CalibTracker/SiStripESProducers/SiStripQualityConfigurableFakeESSource_cfi")
process.siStripQualityConfigurableFakeESSource.appendToDataLabel=""
#process.es_prefer = cms.ESPrefer("SiStripQualityConfigurableFakeESSource", "siStripQualityConfigurableFakeESSource")

process.siStripQualityConfigurableFakeESSource.BadComponentList = cms.untracked.VPSet(
    cms.PSet(
        #SubDet = cms.string('TIB'),  
        #layer = cms.uint32(1),        ## SELECTION: layer = 1..4, 0(ALL)          
        #bkw_frw = cms.uint32(0),      ## bkw_frw = 1(TIB-), 2(TIB+) 0(ALL)        
        #detid = cms.uint32(0),        ## int_ext = 1 (internal), 2(external), 0(ALL)  
        #ster = cms.uint32(0),         ## ster = 1(stereo), 2 (nonstereo), 0(ALL)      
        #string_ = cms.uint32(0),      ## string = 1..N, 0(ALL)                
        #int_ext = cms.uint32(0)       ## detid number = 0 (ALL),  specific number     

        #SubDet = cms.string('TID'),
        #wheel = cms.uint32(3),      ## SELECTION: side = 1(back, Z-), 2(front, Z+), 0(ALL)
        #detid = cms.uint32(0),      ## wheel = 1..3, 0(ALL)
        #ster = cms.uint32(0),       ## ring  = 1..3, 0(ALL)
        #ring = cms.uint32(0),       ## ster = 1(stereo), 2 (nonstereo), 0(ALL)
        #side = cms.uint32(0)            ## detid number = 0 (ALL),  specific number

        SubDet = cms.string('TOB'),
        layer = cms.uint32(1),    ## SELECTION: layer = 1..6, 0(ALL)
        bkw_frw = cms.uint32(0),  ## bkw_frw = 1(TOB-) 2(TOB+) 0(everything)
        rod = cms.uint32(0),      ## rod = 1..N, 0(ALL)
        detid = cms.uint32(0),       ## ster = 1(stereo), 2 (nonstereo), 0(ALL)
        ster = cms.uint32(0)         ## detid number = 0 (ALL),  specific number

        #SubDet = cms.string('TEC'),
        #wheel = cms.uint32(9),             ## SELECTION: side = 1(TEC-), 2(TEC+),  0(ALL)
        #petal = cms.uint32(0),             ## wheel = 1..9, 0(ALL)
        #detid = cms.uint32(0),             ## petal_bkw_frw = 1(backward) 2(forward) 0(all)
        #ster = cms.uint32(0),              ## petal = 1..8, 0(ALL)
        #petal_bkw_frw = cms.uint32(0),     ## ring = 1..7, 0(ALL)
        #ring = cms.uint32(0),              ## ster = 1(sterero), else(nonstereo), 0(ALL)
        #side = cms.uint32(0)                  ## detid number = 0 (ALL),  specific number
        )
    )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")

process.siStripQualityESProducer.ListOfRecordToMerge = cms.VPSet(
   cms.PSet( record = cms.string("SiStripDetCablingRcd"), tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadModuleRcd"),   tag    = cms.string("") )
   )

process.siStripQualityESProducerForClusterizer = process.siStripQualityESProducer.clone()
process.siStripQualityESProducerForClusterizer.ListOfRecordToMerge = cms.VPSet(
   cms.PSet( record = cms.string("SiStripDetCablingRcd"), tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
   cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") )
   )
process.siStripQualityESProducerForClusterizer.appendToDataLabel = cms.string('forCluster')


process.load("RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff")
process.siStripClusters.SiStripQualityLabel = cms.string('forCluster')

process.siStripMatchedRecHits.useSiStripQuality = cms.bool(True)
process.siStripMatchedRecHits.siStripQualityLabel = cms.string('')

process.load("RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi")


process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('RECO')),
    fileName = cms.untracked.string('promptrecoCosmics.root')
)

process.FEVT.outputCommands.append('keep CaloTowersSorted_calotoweroptmaker_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCALCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCALCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCComparatorDigiMuonDigiCollection_muonCSCDigis_MuonCSCComparatorDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCorrelatedLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCRPCDigiMuonDigiCollection_muonCSCDigis_MuonCSCRPCDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCWireDigiMuonDigiCollection_muonCSCDigis_MuonCSCWireDigi_*')
process.FEVT.outputCommands.append('keep cscL1TrackCSCDetIdCSCCorrelatedLCTDigiMuonDigiCollectionstdpairs_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep DTChamberIdDTLocalTriggerMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep DTLayerIdDTDigiMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep intL1CSCSPStatusDigisstdpair_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambPhContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambThContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTTrackContainer_dttfDigis_DATA_*')
process.FEVT.outputCommands.append('keep PixelDigiedmDetSetVector_siPixelDigis_*_*')
process.FEVT.outputCommands.append('keep recoCandidatesOwned_caloTowersOpt_*_*')
process.FEVT.outputCommands.append('keep RPCDetIdRPCDigiMuonDigiCollection_muonRPCDigis_*_*')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/TrackingAnalysis/Cosmics/test/recoHitEff_cfg.py,v $'),
    annotation = cms.untracked.string('CRUZET Prompt Reco with DQM with Mag field at 38T')
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) ## default is false


# Conditions (Global Tag is used here):
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff")
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRAFT_ALL_V8::All"
process.prefer("GlobalTag")

# Magnetic field: force mag field to be 0 tesla
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

# Real data raw to digi
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

# offline DQM
process.load("DQMOffline.Configuration.DQMOfflineCosmics_cff")
process.load("DQMServices.Components.MEtoEDMConverter_cff")

#L1 trigger validation
#process.load("L1Trigger.HardwareValidation.L1HardwareValidation_cff")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFConfigProducer_cfi")
process.load("L1TriggerConfig.CSCTFConfigProducers.L1MuCSCTFConfigurationRcdSrc_cfi")

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.ckfBaseTrajectoryFilterP5.filterPset.maxLostHits = 4
process.ckfBaseTrajectoryFilterP5.filterPset.maxConsecLostHits = 4
process.ckfBaseTrajectoryFilterP5.filterPset.minimumNumberOfHits =  8

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('hiteff_CRAFT.root')
)

process.anEff = cms.EDAnalyzer("HitEff",
                               Debug = cms.bool(False),
                               Layer = cms.int32(5),
                               rphirecHits = cms.InputTag("siStripMatchedRecHits:rphiRecHit"),
                               stereorecHits    = cms.InputTag("siStripMatchedRecHits:stereoRecHit"),
                               combinatorialTracks = cms.InputTag("ctfWithMaterialTracksP5"),
                               #combinatorialTracks = cms.InputTag("TrackRefitterP5"),
                               trajectories = cms.InputTag("ctfWithMaterialTracksP5"),
                               #trajectories   =   cms.InputTag("TrackRefitterP5"),
                               combinatorialSeeds = cms.InputTag("combinedP5SeedsForCTF")
                               )

#Paths
process.allPath = cms.Path( process.trackerCosmics * process.anEff )
#process.allPath = cms.Path( process.siStripMatchedRecHits * process.combinatorialcosmicseedfinderP5 * process.TrackRefitterP5 * process.anEff )

#process.outpath = cms.EndPath(process.FEVT)
