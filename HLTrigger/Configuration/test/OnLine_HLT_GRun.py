# /dev/CMSSW_3_6_2/GRun/V18 (CMSSW_3_6_2_HLT8)

import FWCore.ParameterSet.Config as cms

process = cms.Process( "HLT" )

process.HLTConfigVersion = cms.PSet(
  tableName = cms.string('/dev/CMSSW_3_6_2/GRun/V18')
)

process.options = cms.untracked.PSet(  Rethrow = cms.untracked.vstring( 'ProductNotFound',
  'TooManyProducts',
  'TooFewProducts' ) )
process.streams = cms.PSet( 
  OnlineErrors = cms.vstring( 'LogMonitor',
    'FEDMonitor' ),
  Offline = cms.vstring(  ),
  Calibration = cms.vstring( 'TestEnables' ),
  EcalCalibration = cms.vstring( 'EcalLaser' ),
  ALCAPHISYM = cms.vstring( 'AlCaPhiSymEcal' ),
  RPCMON = cms.vstring( 'RPCMonitor' ),
  ALCAP0 = cms.vstring( 'AlCaP0' ),
  DQM = cms.vstring(  ),
  HLTDQM = cms.vstring(  ),
  HLTMON = cms.vstring( 'OfflineMonitor' ),
  EventDisplay = cms.vstring(  ),
  Express = cms.vstring( 'ExpressPhysics' ),
  A = cms.vstring( 'MuMonitor',
    'Cosmics',
    'EG',
    'JetMETTau',
    'Mu',
    'MuOnia',
    'EGMonitor',
    'ZeroBias',
    'Commissioning',
    'JetMETTauMonitor',
    'RandomTriggers',
    'HcalHPDNoise',
    'HcalNZS',
    'MinimumBias' )
)
process.datasets = cms.PSet( 
  LogMonitor = cms.vstring( 'HLT_LogMonitor' ),
  FEDMonitor = cms.vstring( 'HLT_DTErrors' ),
  TestEnables = cms.vstring( 'HLT_Calibration' ),
  EcalLaser = cms.vstring( 'HLT_EcalCalibration' ),
  AlCaPhiSymEcal = cms.vstring( 'AlCa_EcalPhiSym' ),
  RPCMonitor = cms.vstring( 'AlCa_RPCMuonNormalisation',
    'AlCa_RPCMuonNoHits',
    'AlCa_RPCMuonNoTriggers' ),
  AlCaP0 = cms.vstring( 'AlCa_EcalEta_8E29',
    'AlCa_EcalPi0_8E29' ),
  OfflineMonitor = cms.vstring( 'HLT_L1Mu14_L1SingleEG10',
    'HLT_StoppedHSCP_8E29',
    'HLT_BTagIP_Jet50U',
    'HLT_DoubleLooseIsoTau15',
    'HLT_SingleLooseIsoTau20',
    'HLT_DoublePhoton10_L1R',
    'HLT_Photon30_L1R_8E29',
    'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
    'HLT_Photon15_TrackIso_Cleaned_L1R',
    'HLT_Photon10_Cleaned_L1R',
    'HLT_DoublePhoton5_Upsilon_L1R',
    'HLT_DoublePhoton5_Jpsi_L1R',
    'HLT_DoubleEle5_SW_L1R',
    'HLT_L1DoubleEG5',
    'HLT_Ele20_LW_L1R',
    'HLT_Ele15_SiStrip_L1R',
    'HLT_Ele15_SC10_LW_L1R',
    'HLT_Ele15_LW_L1R',
    'HLT_Ele10_LW_EleId_L1R',
    'HLT_Ele10_LW_L1R',
    'HLT_L1SingleEG8',
    'HLT_L1SingleEG5',
    'HLT_L1DoubleMuOpen',
    'HLT_L1Mu20',
    'HLT_L1Mu',
    'HLT_L1MuOpen',
    'HLT_HT100U',
    'HLT_MET100',
    'HLT_MET45',
    'HLT_L1_BptxXOR_BscMinBiasOR',
    'HLT_PixelTracks_Multiplicity85',
    'HLT_Jet15U_HcalNoiseFiltered',
    'HLT_L1MuOpen_DT',
    'HLT_Activity_Ecal_SC7',
    'HLT_Activity_Ecal_SC15',
    'HLT_Photon15_Cleaned_L1R',
    'HLT_Photon20_Cleaned_L1R',
    'HLT_Activity_CSC',
    'HLT_L1MET20',
    'HLT_QuadJet15U',
    'HLT_DiJetAve30U_8E29',
    'HLT_DiJetAve15U_8E29',
    'HLT_FwdJet20U',
    'HLT_Jet50U',
    'HLT_Jet30U',
    'HLT_Jet15U',
    'HLT_L1Jet10U',
    'HLT_Activity_DT',
    'HLT_Activity_PixelClusters',
    'HLT_Activity_L1A',
    'HLT_L2Mu0_NoVertex',
    'HLT_BTagMu_Jet10U',
    'HLT_DoubleMu3',
    'HLT_DoubleJet15U_ForwardBackward',
    'HLT_Ele10_SW_L1R',
    'HLT_L2Mu5',
    'HLT_L1Jet10U_NoBPTX',
    'HLT_L1SingleEG2',
    'HLT_Activity_DT_Tuned',
    'HLT_L1Jet6U',
    'HLT_L1Jet6U_NoBPTX',
    'HLT_L1SingleCenJet',
    'HLT_L1SingleForJet',
    'HLT_L1SingleTauJet',
    'HLT_Mu0_L1MuOpen',
    'HLT_Mu0_Track0_Jpsi',
    'HLT_Mu3_L1MuOpen',
    'HLT_Mu3_Track0_Jpsi',
    'HLT_Mu5_L1MuOpen',
    'HLT_Mu5_Track0_Jpsi',
    'HLT_SelectEcalSpikes_L1R',
    'HLT_SelectEcalSpikesHighEt_L1R',
    'HLT_DoublePhoton4_Jpsi_L1R',
    'HLT_DoublePhoton4_Upsilon_L1R',
    'HLT_DoublePhoton4_eeRes_L1R',
    'HLT_DoublePhoton5_L1R',
    'HLT_L2DoubleMu0',
    'HLT_L2Mu0',
    'HLT_L2Mu3',
    'HLT_Mu0_L2Mu0',
    'HLT_Mu3_L2Mu0',
    'HLT_Mu5_L2Mu0',
    'HLT_L1MuOpen_AntiBPTX',
    'HLT_DoubleMu0',
    'HLT_Mu9',
    'HLT_Mu5',
    'HLT_Mu3',
    'HLT_IsoMu3',
    'HLT_L2Mu11',
    'HLT_L2Mu9',
    'HLT_TechTrigHCALNoise',
    'HLT_GlobalRunHPDNoise',
    'HLT_L1Tech_HCAL_HF',
    'AlCa_EcalEta_8E29',
    'AlCa_EcalPi0_8E29',
    'AlCa_EcalPhiSym',
    'HLT_HcalNZS_8E29',
    'HLT_HcalPhiSym',
    'HLT_IsoTrackHB_8E29',
    'HLT_IsoTrackHE_8E29',
    'HLT_L1Tech_RPC_TTU_RBst1_collisions',
    'HLT_TrackerCosmics',
    'HLT_RPCBarrelCosmics',
    'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
    'HLT_L1Tech_BSC_halo',
    'HLT_L1Tech_BSC_HighMultiplicity',
    'HLT_CSCBeamHaloRing2or3',
    'HLT_CSCBeamHaloOverlapRing2',
    'HLT_CSCBeamHaloOverlapRing1',
    'HLT_CSCBeamHalo',
    'HLT_MinBiasPixel_DoubleIsoTrack5',
    'HLT_MinBiasPixel_DoubleTrack',
    'HLT_MinBiasPixel_SingleTrack',
    'HLT_ZeroBiasPixel_SingleTrack',
    'HLT_L1Tech_BSC_minBias',
    'HLT_L1Mu14_L1ETM30',
    'HLT_L1Mu14_L1SingleJet6U',
    'HLT_MultiVertex6',
    'HLT_MultiVertex8_L1ETT60',
    'HLT_PixelTracks_Multiplicity70',
    'HLT_L1Tech_BSC_halo_forPhysicsBackground',
    'HLT_SingleLooseIsoTau20_Trk5',
    'HLT_SingleLooseIsoTau25' ),
  ExpressPhysics = cms.vstring( 'HLT_MET100',
    'HLT_ZeroBias',
    'HLT_L1SingleEG2',
    'HLT_L1SingleEG5',
    'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
    'HLT_L1Jet10U',
    'HLT_L2DoubleMu0',
    'HLT_Mu3',
    'HLT_TrackerCosmics',
    'HLT_DoublePhoton10_L1R',
    'HLT_Ele15_LW_L1R',
    'HLT_Mu5',
    'HLT_Jet50U',
    'HLT_Photon20_Cleaned_L1R' ),
  MuMonitor = cms.vstring( 'HLT_L1Mu',
    'HLT_L1MuOpen',
    'HLT_L2Mu0',
    'HLT_L2Mu3',
    'HLT_L2Mu5',
    'HLT_L1MuOpen_DT' ),
  Cosmics = cms.vstring( 'HLT_CSCBeamHaloOverlapRing2',
    'HLT_CSCBeamHaloOverlapRing1',
    'HLT_CSCBeamHalo',
    'HLT_L1Tech_BSC_halo',
    'HLT_L1MuOpen_AntiBPTX',
    'HLT_L2Mu0_NoVertex',
    'HLT_TrackerCosmics',
    'HLT_RPCBarrelCosmics',
    'HLT_CSCBeamHaloRing2or3' ),
  EG = cms.vstring( 'HLT_DoublePhoton10_L1R',
    'HLT_Photon30_L1R_8E29',
    'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
    'HLT_Photon15_TrackIso_Cleaned_L1R',
    'HLT_Photon10_Cleaned_L1R',
    'HLT_DoublePhoton5_Upsilon_L1R',
    'HLT_DoublePhoton5_Jpsi_L1R',
    'HLT_DoubleEle5_SW_L1R',
    'HLT_Ele20_LW_L1R',
    'HLT_Ele15_SiStrip_L1R',
    'HLT_Ele15_SC10_LW_L1R',
    'HLT_Ele15_LW_L1R',
    'HLT_Ele10_LW_EleId_L1R',
    'HLT_Ele10_LW_L1R',
    'HLT_DoublePhoton4_Jpsi_L1R',
    'HLT_DoublePhoton4_Upsilon_L1R',
    'HLT_DoublePhoton4_eeRes_L1R',
    'HLT_DoublePhoton5_L1R',
    'HLT_Photon15_Cleaned_L1R',
    'HLT_Photon20_Cleaned_L1R',
    'HLT_Ele10_SW_L1R',
    'HLT_Ele15_SW_L1R',
    'HLT_Ele20_SW_L1R' ),
  JetMETTau = cms.vstring( 'HLT_BTagIP_Jet50U',
    'HLT_DoubleLooseIsoTau15',
    'HLT_SingleLooseIsoTau20',
    'HLT_HT100U',
    'HLT_MET100',
    'HLT_MET45',
    'HLT_QuadJet15U',
    'HLT_DiJetAve30U_8E29',
    'HLT_DiJetAve15U_8E29',
    'HLT_FwdJet20U',
    'HLT_Jet50U',
    'HLT_Jet30U',
    'HLT_Jet15U',
    'HLT_BTagMu_Jet10U',
    'HLT_DoubleJet15U_ForwardBackward',
    'HLT_Jet15U_HcalNoiseFiltered',
    'HLT_SingleLooseIsoTau20_Trk5',
    'HLT_SingleLooseIsoTau25',
    'HLT_DiJetAve50U_8E29' ),
  Mu = cms.vstring( 'HLT_L1Mu14_L1ETM30',
    'HLT_L1Mu14_L1SingleJet6U',
    'HLT_L1Mu14_L1SingleEG10',
    'HLT_L1Mu20',
    'HLT_DoubleMu3',
    'HLT_Mu3',
    'HLT_Mu5',
    'HLT_Mu9',
    'HLT_IsoMu3',
    'HLT_L2Mu9',
    'HLT_L2Mu11' ),
  MuOnia = cms.vstring( 'HLT_Mu0_L1MuOpen',
    'HLT_Mu0_Track0_Jpsi',
    'HLT_Mu3_L1MuOpen',
    'HLT_Mu3_Track0_Jpsi',
    'HLT_Mu5_L1MuOpen',
    'HLT_Mu5_Track0_Jpsi',
    'HLT_L1DoubleMuOpen',
    'HLT_Mu0_L2Mu0',
    'HLT_Mu3_L2Mu0',
    'HLT_Mu5_L2Mu0',
    'HLT_L2DoubleMu0',
    'HLT_DoubleMu0',
    'HLT_L1DoubleMuOpen_Tight' ),
  EGMonitor = cms.vstring( 'HLT_L1SingleEG2',
    'HLT_L1DoubleEG5',
    'HLT_L1SingleEG8',
    'HLT_L1SingleEG5',
    'HLT_SelectEcalSpikes_L1R',
    'HLT_SelectEcalSpikesHighEt_L1R',
    'HLT_Activity_Ecal_SC7',
    'HLT_Activity_Ecal_SC15' ),
  ZeroBias = cms.vstring( 'HLT_L1_BPTX_PlusOnly',
    'HLT_L1_BPTX_MinusOnly',
    'HLT_L1_BPTX',
    'HLT_ZeroBias' ),
  Commissioning = cms.vstring( 'HLT_Activity_DT',
    'HLT_Activity_DT_Tuned',
    'HLT_Activity_PixelClusters',
    'HLT_Activity_L1A',
    'HLT_Activity_CSC',
    'HLT_L1_BptxXOR_BscMinBiasOR' ),
  JetMETTauMonitor = cms.vstring( 'HLT_L1Jet10U_NoBPTX',
    'HLT_L1Jet6U',
    'HLT_L1Jet6U_NoBPTX',
    'HLT_L1SingleCenJet',
    'HLT_L1SingleForJet',
    'HLT_L1SingleTauJet',
    'HLT_L1MET20',
    'HLT_L1Jet10U' ),
  RandomTriggers = cms.vstring( 'HLT_Random' ),
  HcalHPDNoise = cms.vstring( 'HLT_TechTrigHCALNoise',
    'HLT_GlobalRunHPDNoise' ),
  HcalNZS = cms.vstring( 'HLT_HcalNZS_8E29',
    'HLT_HcalPhiSym' ),
  MinimumBias = cms.vstring( 'HLT_L1Tech_HCAL_HF',
    'HLT_IsoTrackHB_8E29',
    'HLT_IsoTrackHE_8E29',
    'HLT_L1Tech_RPC_TTU_RBst1_collisions',
    'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
    'HLT_L1Tech_BSC_HighMultiplicity',
    'HLT_MinBiasPixel_DoubleIsoTrack5',
    'HLT_MinBiasPixel_DoubleTrack',
    'HLT_MinBiasPixel_SingleTrack',
    'HLT_ZeroBiasPixel_SingleTrack',
    'HLT_L1Tech_BSC_minBias',
    'HLT_StoppedHSCP_8E29',
    'HLT_L1Tech_BSC_halo_forPhysicsBackground',
    'HLT_PixelTracks_Multiplicity85',
    'HLT_MultiVertex6',
    'HLT_MultiVertex8_L1ETT60',
    'HLT_PixelTracks_Multiplicity70' )
)

process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring( 'file:RelVal_DigiL1Raw_GRun.root' )
)

process.BTagRecord = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "JetTagComputerRecord" ),
    iovIsRunNotTime = cms.bool( True ),
    appendToDataLabel = cms.string( "" ),
    firstValid = cms.vuint32( 1 )
)
process.GlobalTag = cms.ESSource( "PoolDBESSource",
    appendToDataLabel = cms.string( "" ),
    timetype = cms.string( "runnumber" ),
    connect = cms.string( "frontier://(proxyurl=http://localhost:3128)(serverurl=http://localhost:8000/FrontierOnProd)(serverurl=http://localhost:8000/FrontierOnProd)(retrieve-ziplevel=0)/CMS_COND_31X_GLOBALTAG" ),
    DumpStat = cms.untracked.bool( False ),
    BlobStreamerName = cms.untracked.string( "TBufferBlobStreamingService" ),
    globaltag = cms.string( "GR10_H_V6A::All" ),
    DBParameters = cms.PSet( 
      authenticationPath = cms.untracked.string( "." ),
      connectionRetrialPeriod = cms.untracked.int32( 10 ),
      idleConnectionCleanupPeriod = cms.untracked.int32( 10 ),
      messageLevel = cms.untracked.int32( 0 ),
      enablePoolAutomaticCleanUp = cms.untracked.bool( False ),
      enableConnectionSharing = cms.untracked.bool( True ),
      enableReadOnlySessionOnUpdateConnection = cms.untracked.bool( False ),
      connectionTimeOut = cms.untracked.int32( 0 ),
      connectionRetrialTimeOut = cms.untracked.int32( 60 )
    ),
    toGet = cms.VPSet( 
    ),
    RefreshEachRun = cms.untracked.bool( True )
)
process.HepPDTESSource = cms.ESSource( "HepPDTESSource",
    pdtFileName = cms.FileInPath( "SimGeneral/HepPDTESSource/data/pythiaparticle.tbl" ),
    appendToDataLabel = cms.string( "" )
)
process.L2RelativeCorrectionService = cms.ESSource( "LXXXCorrectionService",
    appendToDataLabel = cms.string( "" ),
    level = cms.string( "L2Relative" ),
    algorithm = cms.string( "IC5Calo" ),
    section = cms.string( "" ),
    era = cms.string( "Summer09_7TeV_ReReco332" )
)
process.L3AbsoluteCorrectionService = cms.ESSource( "LXXXCorrectionService",
    appendToDataLabel = cms.string( "" ),
    level = cms.string( "L3Absolute" ),
    algorithm = cms.string( "IC5Calo" ),
    section = cms.string( "" ),
    era = cms.string( "Summer09_7TeV_ReReco332" )
)
process.MCJetCorrectorIcone5 = cms.ESSource( "JetCorrectionServiceChain",
    appendToDataLabel = cms.string( "" ),
    correctors = cms.vstring( 'L2RelativeCorrectionService',
      'L3AbsoluteCorrectionService' ),
    label = cms.string( "MCJetCorrectorIcone5" )
)
process.MCJetCorrectorIcone5HF07 = cms.ESSource( "LXXXCorrectionService",
    appendToDataLabel = cms.string( "" ),
    level = cms.string( "L2Relative" ),
    algorithm = cms.string( "" ),
    section = cms.string( "" ),
    era = cms.string( "HLT" )
)
process.MCJetCorrectorIcone5Unit = cms.ESSource( "LXXXCorrectionService",
    appendToDataLabel = cms.string( "" ),
    level = cms.string( "L2RelativeFlat" ),
    algorithm = cms.string( "" ),
    section = cms.string( "" ),
    era = cms.string( "HLT" )
)
process.eegeom = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "EcalMappingRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    appendToDataLabel = cms.string( "" ),
    firstValid = cms.vuint32( 1 )
)
process.es_hardcode = cms.ESSource( "HcalHardcodeCalibrations",
    toGet = cms.untracked.vstring( 'GainWidths' ),
    appendToDataLabel = cms.string( "" )
)
process.essourceSev = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "HcalSeverityLevelComputerRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    appendToDataLabel = cms.string( "" ),
    firstValid = cms.vuint32( 1 )
)
process.magfield = cms.ESSource( "XMLIdealGeometryESSource",
    rootNodeName = cms.string( "cmsMagneticField:MAGF" ),
    appendToDataLabel = cms.string( "" ),
    geomXMLFiles = cms.vstring( 'Geometry/CMSCommonData/data/normal/cmsextent.xml',
      'Geometry/CMSCommonData/data/cms.xml',
      'Geometry/CMSCommonData/data/cmsMagneticField.xml',
      'MagneticField/GeomBuilder/data/MagneticFieldVolumes_1103l.xml',
      'MagneticField/GeomBuilder/data/MagneticFieldParameters_07_2pi.xml' )
)

process.AnalyticalPropagator = cms.ESProducer( "AnalyticalPropagatorESProducer",
  ComponentName = cms.string( "AnalyticalPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  MaxDPhi = cms.double( 1.6 ),
  appendToDataLabel = cms.string( "" )
)
process.AnyDirectionAnalyticalPropagator = cms.ESProducer( "AnalyticalPropagatorESProducer",
  ComponentName = cms.string( "AnyDirectionAnalyticalPropagator" ),
  PropagationDirection = cms.string( "anyDirection" ),
  MaxDPhi = cms.double( 1.6 ),
  appendToDataLabel = cms.string( "" )
)
process.AutoMagneticFieldESProducer = cms.ESProducer( "AutoMagneticFieldESProducer",
  label = cms.untracked.string( "" ),
  valueOverride = cms.int32( -1 ),
  appendToDataLabel = cms.string( "" ),
  nominalCurrents = cms.untracked.vint32( -1, 0, 9558, 14416, 16819, 18268, 19262 ),
  mapLabels = cms.untracked.vstring( '090322_3_8t',
    '0t',
    '071212_2t',
    '071212_3t',
    '071212_3_5t',
    '090322_3_8t',
    '071212_4t' )
)
process.CSCGeometryESModule = cms.ESProducer( "CSCGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  useRealWireGeometry = cms.bool( True ),
  useOnlyWiresInME1a = cms.bool( False ),
  useGangedStripsInME1a = cms.bool( True ),
  useCentreTIOffsets = cms.bool( False ),
  useDDD = cms.bool( False ),
  applyAlignment = cms.bool( True )
)
process.CaloGeometryBuilder = cms.ESProducer( "CaloGeometryBuilder",
  appendToDataLabel = cms.string( "" ),
  SelectedCalos = cms.vstring( 'HCAL',
    'ZDC',
    'EcalBarrel',
    'EcalEndcap',
    'EcalPreshower',
    'TOWER' )
)
process.CaloTopologyBuilder = cms.ESProducer( "CaloTopologyBuilder",
  appendToDataLabel = cms.string( "" )
)
process.CaloTowerConstituentsMapBuilder = cms.ESProducer( "CaloTowerConstituentsMapBuilder",
  MapFile = cms.untracked.string( "Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz" ),
  appendToDataLabel = cms.string( "" )
)
process.CaloTowerGeometryFromDBEP = cms.ESProducer( "CaloTowerGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.Chi2EstimatorForRefit = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForRefit" ),
  MaxChi2 = cms.double( 100000.0 ),
  nSigma = cms.double( 3.0 ),
  appendToDataLabel = cms.string( "" )
)
process.Chi2MeasurementEstimator = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2" ),
  MaxChi2 = cms.double( 30.0 ),
  nSigma = cms.double( 3.0 ),
  appendToDataLabel = cms.string( "" )
)
process.CkfTrajectoryBuilder = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "CkfTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "ckfBaseTrajectoryFilter" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( True ),
  appendToDataLabel = cms.string( "" )
)
process.DTGeometryESModule = cms.ESProducer( "DTGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  fromDDD = cms.bool( False ),
  applyAlignment = cms.bool( True )
)
process.DummyDetLayerGeometry = cms.ESProducer( "DetLayerGeometryESProducer",
  ComponentName = cms.string( "DummyDetLayerGeometry" ),
  appendToDataLabel = cms.string( "" )
)
process.ESUnpackerWorkerESProducer = cms.ESProducer( "ESUnpackerWorkerESProducer",
  ComponentName = cms.string( "esRawToRecHit" ),
  appendToDataLabel = cms.string( "" ),
  DCCDataUnpacker = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) ),
  RHAlgo = cms.PSet( 
    Type = cms.string( "ESRecHitWorker" ),
    ESGain = cms.int32( 2 ),
    ESMIPkeV = cms.double( 81.08 ),
    ESMIPADC = cms.double( 55.0 ),
    ESBaseline = cms.int32( 0 ),
    ESRecoAlgo = cms.int32( 0 )
  )
)
process.EcalBarrelGeometryFromDBEP = cms.ESProducer( "EcalBarrelGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.EcalElectronicsMappingBuilder = cms.ESProducer( "EcalElectronicsMappingBuilder",
  appendToDataLabel = cms.string( "" )
)
process.EcalEndcapGeometryFromDBEP = cms.ESProducer( "EcalEndcapGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.EcalLaserCorrectionService = cms.ESProducer( "EcalLaserCorrectionService",
  appendToDataLabel = cms.string( "" )
)
process.EcalPreshowerGeometryFromDBEP = cms.ESProducer( "EcalPreshowerGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.EcalRegionCablingESProducer = cms.ESProducer( "EcalRegionCablingESProducer",
  appendToDataLabel = cms.string( "" ),
  esMapping = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) )
)
process.EcalTrigTowerConstituentsMapBuilder = cms.ESProducer( "EcalTrigTowerConstituentsMapBuilder",
  MapFile = cms.untracked.string( "Geometry/EcalMapping/data/EndCap_TTMap.txt" ),
  appendToDataLabel = cms.string( "" )
)
process.EcalUnpackerWorkerESProducer = cms.ESProducer( "EcalUnpackerWorkerESProducer",
  ComponentName = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  DCCDataUnpacker = cms.PSet( 
    orderedDCCIdList = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 ),
    tccUnpacking = cms.bool( True ),
    srpUnpacking = cms.bool( False ),
    syncCheck = cms.bool( False ),
    feIdCheck = cms.bool( True ),
    headerUnpacking = cms.bool( False ),
    orderedFedList = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    feUnpacking = cms.bool( True ),
    forceKeepFRData = cms.bool( False ),
    memUnpacking = cms.bool( False )
  ),
  ElectronicsMapper = cms.PSet( 
    numbXtalTSamples = cms.uint32( 10 ),
    numbTriggerTSamples = cms.uint32( 1 )
  ),
  UncalibRHAlgo = cms.PSet(  Type = cms.string( "EcalUncalibRecHitWorkerWeights" ) ),
  CalibRHAlgo = cms.PSet( 
    flagsMapDBReco = cms.vint32( 0, 0, 0, 0, 4, -1, -1, -1, 4, 4, 6, 6, 6, 7, 8 ),
    Type = cms.string( "EcalRecHitWorkerSimple" ),
    killDeadChannels = cms.bool( True ),
    ChannelStatusToBeExcluded = cms.vint32( 10, 11, 12, 13, 14, 78, 142 ),
    laserCorrection = cms.bool( False )
  )
)
process.FastSteppingHelixPropagatorAny = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "FastSteppingHelixPropagatorAny" ),
  PropagationDirection = cms.string( "anyDirection" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( True ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
process.FastSteppingHelixPropagatorOpposite = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "FastSteppingHelixPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( True ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
process.FitterRK = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "FitterRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.FittingSmootherRK = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "FittingSmootherRK" ),
  Fitter = cms.string( "FitterRK" ),
  Smoother = cms.string( "SmootherRK" ),
  EstimateCut = cms.double( -1.0 ),
  LogPixelProbabilityCut = cms.double( -16.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.GlobalTrackingGeometryESProducer = cms.ESProducer( "GlobalTrackingGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
process.HITTRHBuilderWithoutRefit = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "HITTRHBuilderWithoutRefit" ),
  StripCPE = cms.string( "Fake" ),
  PixelCPE = cms.string( "Fake" ),
  Matcher = cms.string( "Fake" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.HcalGeometryFromDBEP = cms.ESProducer( "HcalGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.HcalTopologyIdealEP = cms.ESProducer( "HcalTopologyIdealEP",
  appendToDataLabel = cms.string( "" )
)
process.KFFitterForRefitInsideOut = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFFitterForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.KFFitterSmootherForL2Muon = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "KFFitterSmootherForL2Muon" ),
  Fitter = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Smoother = cms.string( "KFTrajectorySmootherForL2Muon" ),
  EstimateCut = cms.double( -1.0 ),
  LogPixelProbabilityCut = cms.double( -16.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.KFSmootherForMuonTrackLoader = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForMuonTrackLoader" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 10.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.KFSmootherForRefitInsideOut = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.KFTrajectoryFitterForL2Muon = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.KFTrajectorySmootherForL2Muon = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFTrajectorySmootherForL2Muon" ),
  Propagator = cms.string( "FastSteppingHelixPropagatorOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.KFUpdatorESProducer = cms.ESProducer( "KFUpdatorESProducer",
  ComponentName = cms.string( "KFUpdator" ),
  appendToDataLabel = cms.string( "" )
)
process.L1GtTriggerMaskAlgoTrigTrivialProducer = cms.ESProducer( "L1GtTriggerMaskAlgoTrigTrivialProducer",
  appendToDataLabel = cms.string( "" ),
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.L1GtTriggerMaskTechTrigTrivialProducer = cms.ESProducer( "L1GtTriggerMaskTechTrigTrivialProducer",
  appendToDataLabel = cms.string( "" ),
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.L3MuKFFitter = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "L3MuKFFitter" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.MaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterial" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
process.MeasurementTracker = cms.ESProducer( "MeasurementTrackerESProducer",
  ComponentName = cms.string( "" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  HitMatcher = cms.string( "StandardMatcher" ),
  Regional = cms.bool( True ),
  OnDemand = cms.bool( True ),
  UsePixelModuleQualityDB = cms.bool( True ),
  DebugPixelModuleQualityDB = cms.untracked.bool( False ),
  UsePixelROCQualityDB = cms.bool( True ),
  DebugPixelROCQualityDB = cms.untracked.bool( False ),
  UseStripModuleQualityDB = cms.bool( True ),
  DebugStripModuleQualityDB = cms.untracked.bool( False ),
  UseStripAPVFiberQualityDB = cms.bool( True ),
  DebugStripAPVFiberQualityDB = cms.untracked.bool( False ),
  MaskBadAPVFibers = cms.bool( True ),
  UseStripStripQualityDB = cms.bool( True ),
  DebugStripStripQualityDB = cms.untracked.bool( False ),
  SiStripQualityLabel = cms.string( "" ),
  switchOffPixelsIfEmpty = cms.bool( True ),
  pixelClusterProducer = cms.string( "hltSiPixelClusters" ),
  stripClusterProducer = cms.string( "hltSiStripClusters" ),
  stripLazyGetterProducer = cms.string( "hltSiStripRawToClustersFacility" ),
  appendToDataLabel = cms.string( "" ),
  inactivePixelDetectorLabels = cms.VInputTag(  ),
  inactiveStripDetectorLabels = cms.VInputTag(  ),
  badStripCuts = cms.PSet( 
    TIB = cms.PSet( 
      maxBad = cms.uint32( 9999 ),
      maxConsecutiveBad = cms.uint32( 9999 )
    ),
    TOB = cms.PSet( 
      maxBad = cms.uint32( 9999 ),
      maxConsecutiveBad = cms.uint32( 9999 )
    ),
    TEC = cms.PSet( 
      maxBad = cms.uint32( 9999 ),
      maxConsecutiveBad = cms.uint32( 9999 )
    ),
    TID = cms.PSet( 
      maxBad = cms.uint32( 9999 ),
      maxConsecutiveBad = cms.uint32( 9999 )
    )
  )
)
process.MuonCkfTrajectoryBuilder = cms.ESProducer( "MuonCkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "muonCkfTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  propagatorProximity = cms.string( "SteppingHelixPropagatorAny" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "muonCkfTrajectoryFilter" ),
  useSeedLayer = cms.bool( False ),
  rescaleErrorIfFail = cms.double( 1.0 ),
  deltaEta = cms.double( 0.1 ),
  deltaPhi = cms.double( 0.1 ),
  appendToDataLabel = cms.string( "" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( False ),
  alwaysUseInvalidHits = cms.bool( True )
)
process.MuonDetLayerGeometryESProducer = cms.ESProducer( "MuonDetLayerGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
process.MuonNumberingInitialization = cms.ESProducer( "MuonNumberingInitialization",
  appendToDataLabel = cms.string( "" )
)
process.MuonTransientTrackingRecHitBuilderESProducer = cms.ESProducer( "MuonTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "MuonRecHitBuilder" ),
  appendToDataLabel = cms.string( "" )
)
process.OppositeMaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterialOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
process.PixelCPEGenericESProducer = cms.ESProducer( "PixelCPEGenericESProducer",
  ComponentName = cms.string( "PixelCPEGeneric" ),
  eff_charge_cut_lowX = cms.double( 0.0 ),
  eff_charge_cut_lowY = cms.double( 0.0 ),
  eff_charge_cut_highX = cms.double( 1.0 ),
  eff_charge_cut_highY = cms.double( 1.0 ),
  size_cutX = cms.double( 3.0 ),
  size_cutY = cms.double( 3.0 ),
  EdgeClusterErrorX = cms.double( 50.0 ),
  EdgeClusterErrorY = cms.double( 85.0 ),
  inflate_errors = cms.bool( False ),
  inflate_all_errors_no_trk_angle = cms.bool( False ),
  UseErrorsFromTemplates = cms.bool( True ),
  TruncatePixelCharge = cms.bool( True ),
  IrradiationBiasCorrection = cms.bool( False ),
  DoCosmics = cms.bool( False ),
  LoadTemplatesFromDB = cms.bool( True ),
  appendToDataLabel = cms.string( "" ),
  TanLorentzAnglePerTesla = cms.double( 0.106 ),
  PixelErrorParametrization = cms.string( "NOTcmsim" ),
  Alpha2Order = cms.bool( True ),
  ClusterProbComputationFlag = cms.int32( 0 )
)
process.RPCGeometryESModule = cms.ESProducer( "RPCGeometryESModule",
  useDDD = cms.untracked.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.RungeKuttaTrackerPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "RungeKuttaTrackerPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( True ),
  ptMin = cms.double( -1.0 ),
  appendToDataLabel = cms.string( "" )
)
process.SiPixelTemplateDBObjectESProducer = cms.ESProducer( "SiPixelTemplateDBObjectESProducer",
  appendToDataLabel = cms.string( "" )
)
process.SiStripGainESProducer = cms.ESProducer( "SiStripGainESProducer",
  AutomaticNormalization = cms.bool( False ),
  NormalizationFactor = cms.double( 1.0 ),
  printDebug = cms.untracked.bool( False ),
  APVGain = cms.VPSet( 
    cms.PSet(  Record = cms.string( "SiStripApvGainRcd" ),
      Label = cms.untracked.string( "" ),
      NormalizationFactor = cms.untracked.double( 1.0 )
    ),
    cms.PSet(  Record = cms.string( "SiStripApvGain2Rcd" ),
      Label = cms.untracked.string( "" ),
      NormalizationFactor = cms.untracked.double( 1.0 )
    )
  )
)
process.SiStripQualityESProducer = cms.ESProducer( "SiStripQualityESProducer",
  appendToDataLabel = cms.string( "" ),
  PrintDebugOutput = cms.bool( False ),
  ThresholdForReducedGranularity = cms.double( 0.3 ),
  UseEmptyRunInfo = cms.bool( False ),
  ReduceGranularity = cms.bool( False ),
  ListOfRecordToMerge = cms.VPSet( 
    cms.PSet(  record = cms.string( "SiStripDetVOffRcd" ),
      tag = cms.string( "" )
    ),
    cms.PSet(  record = cms.string( "SiStripDetCablingRcd" ),
      tag = cms.string( "" )
    ),
    cms.PSet(  record = cms.string( "SiStripBadChannelRcd" ),
      tag = cms.string( "" )
    ),
    cms.PSet(  record = cms.string( "SiStripBadFiberRcd" ),
      tag = cms.string( "" )
    ),
    cms.PSet(  record = cms.string( "SiStripBadModuleRcd" ),
      tag = cms.string( "" )
    ),
    cms.PSet(  record = cms.string( "RunInfoRcd" ),
      tag = cms.string( "" )
    )
  )
)
process.SiStripRecHitMatcherESProducer = cms.ESProducer( "SiStripRecHitMatcherESProducer",
  ComponentName = cms.string( "StandardMatcher" ),
  NSigmaInside = cms.double( 3.0 ),
  appendToDataLabel = cms.string( "" )
)
process.SiStripRegionConnectivity = cms.ESProducer( "SiStripRegionConnectivity",
  EtaDivisions = cms.untracked.uint32( 20 ),
  PhiDivisions = cms.untracked.uint32( 20 ),
  EtaMax = cms.untracked.double( 2.5 )
)
process.SlaveField0 = cms.ESProducer( "UniformMagneticFieldESProducer",
  ZFieldInTesla = cms.double( 0.0 ),
  label = cms.untracked.string( "slave_0" ),
  appendToDataLabel = cms.string( "" )
)
process.SlaveField20 = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "slave_20" ),
  version = cms.string( "OAE_1103l_071212" ),
  appendToDataLabel = cms.string( "" ),
  parameters = cms.PSet(  BValue = cms.string( "2_0T" ) )
)
process.SlaveField30 = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "slave_30" ),
  version = cms.string( "OAE_1103l_071212" ),
  appendToDataLabel = cms.string( "" ),
  parameters = cms.PSet(  BValue = cms.string( "3_0T" ) )
)
process.SlaveField35 = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "slave_35" ),
  version = cms.string( "OAE_1103l_071212" ),
  appendToDataLabel = cms.string( "" ),
  parameters = cms.PSet(  BValue = cms.string( "3_5T" ) )
)
process.SlaveField38 = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "slave_38" ),
  version = cms.string( "OAE_1103l_071212" ),
  appendToDataLabel = cms.string( "" ),
  parameters = cms.PSet(  BValue = cms.string( "3_8T" ) )
)
process.SlaveField40 = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "slave_40" ),
  version = cms.string( "OAE_1103l_071212" ),
  appendToDataLabel = cms.string( "" ),
  parameters = cms.PSet(  BValue = cms.string( "4_0T" ) )
)
process.SmartPropagator = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAlong" ),
  appendToDataLabel = cms.string( "" )
)
process.SmartPropagatorAny = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAny" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" ),
  appendToDataLabel = cms.string( "" )
)
process.SmartPropagatorAnyOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAnyOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" ),
  appendToDataLabel = cms.string( "" )
)
process.SmartPropagatorOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorOpposite" ),
  appendToDataLabel = cms.string( "" )
)
process.SmootherRK = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "SmootherRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.SteppingHelixPropagatorAlong = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAlong" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
process.SteppingHelixPropagatorAny = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAny" ),
  PropagationDirection = cms.string( "anyDirection" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
process.SteppingHelixPropagatorOpposite = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  useInTeslaFromMagField = cms.bool( False ),
  SetVBFPointer = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 ),
  appendToDataLabel = cms.string( "" )
)
process.StraightLinePropagator = cms.ESProducer( "StraightLinePropagatorESProducer",
  ComponentName = cms.string( "StraightLinePropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  appendToDataLabel = cms.string( "" )
)
process.StripCPEfromTrackAngleESProducer = cms.ESProducer( "StripCPEESProducer",
  ComponentName = cms.string( "StripCPEfromTrackAngle" ),
  TanDiffusionAngle = cms.double( 0.01 ),
  ThicknessRelativeUncertainty = cms.double( 0.02 ),
  NoiseThreshold = cms.double( 2.3 ),
  MaybeNoiseThreshold = cms.double( 3.5 ),
  UncertaintyScaling = cms.double( 1.42 ),
  MinimumUncertainty = cms.double( 0.01 ),
  APVpeakmode = cms.bool( False ),
  CouplingConstant = cms.double( 0.1 ),
  appendToDataLabel = cms.string( "" ),
  OutOfTime = cms.PSet( 
    TIBlateFP = cms.double( 0.0 ),
    TIDlateFP = cms.double( 0.0 ),
    TOBlateFP = cms.double( 0.0 ),
    TEClateFP = cms.double( 0.0 ),
    TOBlateBP = cms.double( 0.0 ),
    TEClateBP = cms.double( 0.0 ),
    TIBlateBP = cms.double( 0.0 ),
    TIDlateBP = cms.double( 0.0 )
  )
)
process.TTRHBuilderPixelOnly = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "TTRHBuilderPixelOnly" ),
  StripCPE = cms.string( "Fake" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.TrackerDigiGeometryESModule = cms.ESProducer( "TrackerDigiGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( True ),
  fromDDD = cms.bool( False )
)
process.TrackerGeometricDetESModule = cms.ESProducer( "TrackerGeometricDetESModule",
  fromDDD = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.TrackerRecoGeometryESProducer = cms.ESProducer( "TrackerRecoGeometryESProducer",
  appendToDataLabel = cms.string( "" )
)
process.TransientTrackBuilderESProducer = cms.ESProducer( "TransientTrackBuilderESProducer",
  ComponentName = cms.string( "TransientTrackBuilder" ),
  appendToDataLabel = cms.string( "" )
)
process.VBF0 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "0t" ),
  version = cms.string( "grid_1103l_071212_2t" ),
  overrideMasterSector = cms.bool( True ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_0" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32(  ),
  scalingFactors = cms.vdouble(  ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.VBF20 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "071212_2t" ),
  version = cms.string( "grid_1103l_071212_2t" ),
  overrideMasterSector = cms.bool( True ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_20" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32(  ),
  scalingFactors = cms.vdouble(  ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.VBF30 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "071212_3t" ),
  version = cms.string( "grid_1103l_071212_3t" ),
  overrideMasterSector = cms.bool( True ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_30" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32(  ),
  scalingFactors = cms.vdouble(  ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.VBF35 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "071212_3_5t" ),
  version = cms.string( "grid_1103l_071212_3_5t" ),
  overrideMasterSector = cms.bool( True ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_35" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32(  ),
  scalingFactors = cms.vdouble(  ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.VBF38 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "090322_3_8t" ),
  version = cms.string( "grid_1103l_090322_3_8t" ),
  overrideMasterSector = cms.bool( False ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_38" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32( 14100, 14200, 17600, 17800, 17900, 18100, 18300, 18400, 18600, 23100, 23300, 23400, 23600, 23800, 23900, 24100, 28600, 28800, 28900, 29100, 29300, 29400, 29600, 28609, 28809, 28909, 29109, 29309, 29409, 29609, 28610, 28810, 28910, 29110, 29310, 29410, 29610, 28611, 28811, 28911, 29111, 29311, 29411, 29611 ),
  scalingFactors = cms.vdouble( 1.0, 1.0, 0.994, 1.004, 1.004, 1.005, 1.004, 1.004, 0.994, 0.965, 0.958, 0.958, 0.953, 0.958, 0.958, 0.965, 0.918, 0.924, 0.924, 0.906, 0.924, 0.924, 0.918, 0.991, 0.998, 0.998, 0.978, 0.998, 0.998, 0.991, 0.991, 0.998, 0.998, 0.978, 0.998, 0.998, 0.991, 0.991, 0.998, 0.998, 0.978, 0.998, 0.998, 0.991 ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.VBF40 = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "071212_4t" ),
  version = cms.string( "grid_1103l_071212_4t" ),
  overrideMasterSector = cms.bool( True ),
  useParametrizedTrackerField = cms.bool( True ),
  paramLabel = cms.string( "slave_40" ),
  appendToDataLabel = cms.string( "" ),
  scalingVolumes = cms.vint32(  ),
  scalingFactors = cms.vdouble(  ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True )
)
process.WithTrackAngle = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "WithTrackAngle" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" ),
  ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.ZdcGeometryFromDBEP = cms.ESProducer( "ZdcGeometryFromDBEP",
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( False )
)
process.XMLFromDBSource = cms.ESProducer( "XMLIdealGeometryESProducer",
  rootDDName = cms.string( "cms:OCMS" ),
  label = cms.string( "Extended" ),
  appendToDataLabel = cms.string( "" )
)
process.bJetRegionalTrajectoryBuilder = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "bJetRegionalTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "bJetRegionalTrajectoryFilter" ),
  maxCand = cms.int32( 1 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.bJetRegionalTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "bJetRegionalTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 8 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 1.0 )
  )
)
process.ckfBaseTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "ckfBaseTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( -1 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
process.hcalRecAlgos = cms.ESProducer( "HcalRecAlgoESProducer",
  SeverityLevels = cms.VPSet( 
    cms.PSet(  RecHitFlags = cms.vstring(  ),
      ChannelStatus = cms.vstring(  ),
      Level = cms.int32( 0 )
    )
  ),
  DropChannelStatusBits = cms.vstring(  ),
  appendToDataLabel = cms.string( "" ),
  RecoveredRecHitBits = cms.vstring(  )
)
process.hcal_db_producer = cms.ESProducer( "HcalDbProducer",
  appendToDataLabel = cms.string( "" )
)
process.hltCkfTrajectoryBuilderMumu = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryBuilderMumu" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "hltCkfTrajectoryFilterMumu" ),
  maxCand = cms.int32( 3 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.hltCkfTrajectoryFilterMumu = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryFilterMumu" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 5 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 3.0 )
  )
)
process.hltKFFitter = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "hltKFFitter" ),
  Propagator = cms.string( "PropagatorWithMaterial" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.hltKFFittingSmoother = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "hltKFFittingSmoother" ),
  Fitter = cms.string( "hltKFFitter" ),
  Smoother = cms.string( "hltKFSmoother" ),
  EstimateCut = cms.double( -1.0 ),
  LogPixelProbabilityCut = cms.double( -16.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.hltKFSmoother = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "hltKFSmoother" ),
  Propagator = cms.string( "PropagatorWithMaterial" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  RecoGeometry = cms.string( "DummyDetLayerGeometry" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 ),
  appendToDataLabel = cms.string( "" )
)
process.hltMuTrackJpsiTrajectoryBuilder = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "hltMuTrackJpsiTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "hltMuTrackJpsiTrajectoryFilter" ),
  maxCand = cms.int32( 1 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.hltMuTrackJpsiTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "hltMuTrackJpsiTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 8 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 1.0 )
  )
)
process.mixedlayerpairs = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "MixedLayerPairs" ),
  layerList = cms.vstring( 'BPix1+BPix2',
    'BPix1+BPix3',
    'BPix2+BPix3',
    'BPix1+FPix1_pos',
    'BPix1+FPix1_neg',
    'BPix1+FPix2_pos',
    'BPix1+FPix2_neg',
    'BPix2+FPix1_pos',
    'BPix2+FPix1_neg',
    'BPix2+FPix2_pos',
    'BPix2+FPix2_neg',
    'FPix1_pos+FPix2_pos',
    'FPix1_neg+FPix2_neg',
    'FPix2_pos+TEC1_pos',
    'FPix2_pos+TEC2_pos',
    'TEC1_pos+TEC2_pos',
    'TEC2_pos+TEC3_pos',
    'FPix2_neg+TEC1_neg',
    'FPix2_neg+TEC2_neg',
    'TEC1_neg+TEC2_neg',
    'TEC2_neg+TEC3_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet( 
    useRingSlector = cms.bool( True ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    minRing = cms.int32( 1 ),
    maxRing = cms.int32( 1 )
  )
)
process.muonCkfTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "muonCkfTrajectoryFilter" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    chargeSignificance = cms.double( -1.0 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( -1 ),
    maxConsecLostHits = cms.int32( 1 ),
    minimumNumberOfHits = cms.int32( 5 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
process.navigationSchoolESProducer = cms.ESProducer( "NavigationSchoolESProducer",
  ComponentName = cms.string( "SimpleNavigationSchool" ),
  appendToDataLabel = cms.string( "" )
)
process.pixellayerpairs = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerPairs" ),
  layerList = cms.vstring( 'BPix1+BPix2',
    'BPix1+BPix3',
    'BPix2+BPix3',
    'BPix1+FPix1_pos',
    'BPix1+FPix1_neg',
    'BPix1+FPix2_pos',
    'BPix1+FPix2_neg',
    'BPix2+FPix1_pos',
    'BPix2+FPix1_neg',
    'BPix2+FPix2_pos',
    'BPix2+FPix2_neg',
    'FPix1_pos+FPix2_pos',
    'FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
process.pixellayertriplets = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTriplets" ),
  layerList = cms.vstring( 'BPix1+BPix2+BPix3',
    'BPix1+BPix2+FPix1_pos',
    'BPix1+BPix2+FPix1_neg',
    'BPix1+FPix1_pos+FPix2_pos',
    'BPix1+FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
process.pixellayertripletsHITHB = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTripletsHITHB" ),
  layerList = cms.vstring( 'BPix1+BPix2+BPix3' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
process.pixellayertripletsHITHE = cms.ESProducer( "SeedingLayersESProducer",
  appendToDataLabel = cms.string( "" ),
  ComponentName = cms.string( "PixelLayerTripletsHITHE" ),
  layerList = cms.vstring( 'BPix1+BPix2+FPix1_pos',
    'BPix1+BPix2+FPix1_neg',
    'BPix1+FPix1_pos+FPix2_pos',
    'BPix1+FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    useErrorsFromParam = cms.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    hitErrorRZ = cms.double( 0.0036 )
  ),
  TEC = cms.PSet(  )
)
process.sistripconn = cms.ESProducer( "SiStripConnectivity" )
process.softLeptonByDistance = cms.ESProducer( "LeptonTaggerByDistanceESProducer",
  appendToDataLabel = cms.string( "" ),
  distance = cms.double( 0.5 )
)
process.softLeptonByPt = cms.ESProducer( "LeptonTaggerByPtESProducer",
  appendToDataLabel = cms.string( "" ),
  ipSign = cms.string( "any" )
)
process.trackCounting3D2nd = cms.ESProducer( "TrackCountingESProducer",
  appendToDataLabel = cms.string( "" ),
  nthTrack = cms.int32( 2 ),
  impactParameterType = cms.int32( 0 ),
  deltaR = cms.double( -1.0 ),
  maximumDecayLength = cms.double( 5.0 ),
  maximumDistanceToJetAxis = cms.double( 0.07 ),
  trackQualityClass = cms.string( "any" )
)
process.trajBuilderL3 = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "trajBuilderL3" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "trajFilterL3" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False ),
  appendToDataLabel = cms.string( "" )
)
process.trajFilterL3 = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "trajFilterL3" ),
  appendToDataLabel = cms.string( "" ),
  filterPset = cms.PSet( 
    minimumNumberOfHits = cms.int32( 5 ),
    minHitsMinPt = cms.int32( 3 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxNumberOfHits = cms.int32( 7 ),
    maxConsecLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 )
  )
)
process.trajectoryCleanerBySharedHits = cms.ESProducer( "TrajectoryCleanerESProducer",
  ComponentName = cms.string( "TrajectoryCleanerBySharedHits" ),
  appendToDataLabel = cms.string( "" ),
  fractionShared = cms.double( 0.5 ),
  allowSharedFirstHit = cms.bool( False )
)

process.DQM = cms.Service( "DQM",
)
process.DQMStore = cms.Service( "DQMStore",
)
process.DTDataIntegrityTask = cms.Service( "DTDataIntegrityTask",
    getSCInfo = cms.untracked.bool( True ),
    processingMode = cms.untracked.string( "HLT" )
)
process.MessageLogger = cms.Service( "MessageLogger",
    destinations = cms.untracked.vstring( 'warnings',
      'errors',
      'infos',
      'debugs',
      'cout',
      'cerr' ),
    categories = cms.untracked.vstring( 'FwkJob',
      'FwkReport',
      'FwkSummary',
      'Root_NoDictionary' ),
    statistics = cms.untracked.vstring( 'cerr' ),
    cerr = cms.untracked.PSet( 
      INFO = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      noTimeStamps = cms.untracked.bool( False ),
      FwkReport = cms.untracked.PSet( 
        reportEvery = cms.untracked.int32( 1 ),
        limit = cms.untracked.int32( 0 )
      ),
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkSummary = cms.untracked.PSet( 
        reportEvery = cms.untracked.int32( 1 ),
        limit = cms.untracked.int32( 10000000 )
      ),
      threshold = cms.untracked.string( "INFO" ),
    ),
    cout = cms.untracked.PSet( 
      threshold = cms.untracked.string( "ERROR" ),
    ),
    errors = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
    ),
    warnings = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
    ),
    infos = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      placeholder = cms.untracked.bool( True ),
    ),
    debugs = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
    ),
    fwkJobReports = cms.untracked.vstring( 'FrameworkJobReport' ),
    FrameworkJobReport = cms.untracked.PSet( 
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) )
    ),
    suppressWarning = cms.untracked.vstring( 'hltOnlineBeamSpot',
      'hltPixelTracksForMinBias',
      'hltPixelTracksForHighMult',
      'hltHITPixelTracksHE',
      'hltHITPixelTracksHB',
      'hltSiPixelClusters',
      'hltPixelTracks' ),
    threshold = cms.untracked.string( "INFO" ),
)
process.MicroStateService = cms.Service( "MicroStateService",
)
process.ModuleWebRegistry = cms.Service( "ModuleWebRegistry",
)
process.PrescaleService = cms.Service( "PrescaleService",
    lvl1DefaultLabel = cms.untracked.string( "1.6E30" ),
    lvl1Labels = cms.vstring( '1.6E30',
      '8E29',
      '4E29',
      '2E29',
      'Cosmics' ),
    prescaleTable = cms.VPSet( 
      cms.PSet(  pathName = cms.string( "HLT_Activity_L1A" ),
        prescales = cms.vuint32( 40000, 32000, 20000, 12000, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_PixelClusters" ),
        prescales = cms.vuint32( 40000, 32000, 20000, 12000, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_CSC" ),
        prescales = cms.vuint32( 150, 50, 20, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_DT" ),
        prescales = cms.vuint32( 15, 5, 2, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_DT_Tuned" ),
        prescales = cms.vuint32( 5, 1, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_Ecal_SC7" ),
        prescales = cms.vuint32( 100, 50, 20, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Activity_Ecal_SC15" ),
        prescales = cms.vuint32( 5, 1, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_SelectEcalSpikes_L1R" ),
        prescales = cms.vuint32( 100, 80, 40, 20, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_SelectEcalSpikesHighEt_L1R" ),
        prescales = cms.vuint32( 50, 40, 20, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleForJet" ),
        prescales = cms.vuint32( 20000, 8000, 4000, 2000, 100 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleCenJet" ),
        prescales = cms.vuint32( 200, 100, 50, 25, 50 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleTauJet" ),
        prescales = cms.vuint32( 10000, 4000, 2000, 1000, 100 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Jet6U" ),
        prescales = cms.vuint32( 1000, 500, 200, 100, 100 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Jet6U_NoBPTX" ),
        prescales = cms.vuint32( 10000, 10000, 10000, 10000, 100 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Jet10U" ),
        prescales = cms.vuint32( 200, 100, 40, 20, 10 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Jet10U_NoBPTX" ),
        prescales = cms.vuint32( 2000, 1000, 1000, 1000, 10 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Jet15U" ),
        prescales = cms.vuint32( 60, 30, 10, 5, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Jet15U_HcalNoiseFiltered" ),
        prescales = cms.vuint32( 60, 30, 10, 5, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Jet30U" ),
        prescales = cms.vuint32( 6, 3, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Jet50U" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_FwdJet20U" ),
        prescales = cms.vuint32( 5, 2, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DiJetAve15U_8E29" ),
        prescales = cms.vuint32( 30, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DiJetAve30U_8E29" ),
        prescales = cms.vuint32( 3, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleJet15U_ForwardBackward" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_QuadJet15U" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1MET20" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MET45" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MET100" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_HT100U" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1MuOpen" ),
        prescales = cms.vuint32( 300, 150, 70, 30, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1MuOpen_DT" ),
        prescales = cms.vuint32( 10, 7, 3, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1MuOpen_AntiBPTX" ),
        prescales = cms.vuint32( 10, 10, 10, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Mu" ),
        prescales = cms.vuint32( 25, 10, 5, 2, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu0" ),
        prescales = cms.vuint32( 100, 50, 25, 10, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu3" ),
        prescales = cms.vuint32( 50, 25, 10, 5, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu5" ),
        prescales = cms.vuint32( 30, 20, 10, 5, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu9" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu11" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_IsoMu3" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu3" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu5" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu9" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2DoubleMu0" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleMu0" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleMu3" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu0_L1MuOpen" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu3_L1MuOpen" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu5_L1MuOpen" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu0_L2Mu0" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu3_L2Mu0" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu5_L2Mu0" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu0_Track0_Jpsi" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu3_Track0_Jpsi" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Mu5_Track0_Jpsi" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleEG2" ),
        prescales = cms.vuint32( 4000, 2000, 1000, 500, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleEG5" ),
        prescales = cms.vuint32( 300, 150, 80, 40, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1SingleEG8" ),
        prescales = cms.vuint32( 100, 50, 25, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1DoubleEG5" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele10_LW_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele10_LW_EleId_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele10_SW_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_LW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_SW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_SC10_LW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_SiStrip_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele20_LW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele20_SW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleEle5_SW_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon10_Cleaned_L1R" ),
        prescales = cms.vuint32( 80, 40, 20, 10, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon15_Cleaned_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon15_TrackIso_Cleaned_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon15_LooseEcalIso_Cleaned_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon20_Cleaned_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Photon30_L1R_8E29" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton4_eeRes_L1R" ),
        prescales = cms.vuint32( 100, 50, 20, 10, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton4_Jpsi_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton4_Upsilon_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton5_Jpsi_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton5_Upsilon_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton5_L1R" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoublePhoton10_L1R" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_SingleLooseIsoTau20" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_SingleLooseIsoTau20_Trk5" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_SingleLooseIsoTau25" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleLooseIsoTau15" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_BTagIP_Jet50U" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_BTagMu_Jet10U" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Mu14_L1SingleJet6U" ),
        prescales = cms.vuint32( 10, 5, 2, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_ZeroBias" ),
        prescales = cms.vuint32( 2000, 2000, 2000, 2000, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_ZeroBiasPixel_SingleTrack" ),
        prescales = cms.vuint32( 10000, 8000, 5000, 3000, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MinBiasPixel_SingleTrack" ),
        prescales = cms.vuint32( 10, 7, 4, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MinBiasPixel_DoubleTrack" ),
        prescales = cms.vuint32( 10, 7, 4, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MinBiasPixel_DoubleIsoTrack5" ),
        prescales = cms.vuint32( 10, 7, 4, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MultiVertex6" ),
        prescales = cms.vuint32( 2000, 70, 5, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_MultiVertex8_L1ETT60" ),
        prescales = cms.vuint32( 100, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_CSCBeamHalo" ),
        prescales = cms.vuint32( 4, 21, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1_BscMinBiasOR_BptxPlusORMinus" ),
        prescales = cms.vuint32( 10, 7, 4, 2, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_BSC_minBias" ),
        prescales = cms.vuint32( 40000, 32000, 20000, 12000, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_BSC_halo" ),
        prescales = cms.vuint32( 1600, 800, 400, 200, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_BSC_halo_forPhysicsBackground" ),
        prescales = cms.vuint32( 1600, 800, 400, 200, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_BSC_HighMultiplicity" ),
        prescales = cms.vuint32( 2000, 800, 400, 200, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_RPC_TTU_RBst1_collisions" ),
        prescales = cms.vuint32( 20, 5, 2, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L1Tech_HCAL_HF" ),
        prescales = cms.vuint32( 40000, 32000, 20000, 12000, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_RPCBarrelCosmics" ),
        prescales = cms.vuint32( 10, 10, 10, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_IsoTrackHE_8E29" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_IsoTrackHB_8E29" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_HcalPhiSym" ),
        prescales = cms.vuint32( 2, 1, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_HcalNZS_8E29" ),
        prescales = cms.vuint32( 5, 2, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_EcalPhiSym" ),
        prescales = cms.vuint32( 60, 40, 20, 10, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_EcalPi0_8E29" ),
        prescales = cms.vuint32( 5, 2, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_EcalEta_8E29" ),
        prescales = cms.vuint32( 2, 1, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_RPCMuonNoHits" ),
        prescales = cms.vuint32( 5, 2, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_RPCMuonNoTriggers" ),
        prescales = cms.vuint32( 5, 2, 1, 1, 1 )
      ),
      cms.PSet(  pathName = cms.string( "AlCa_RPCMuonNormalisation" ),
        prescales = cms.vuint32( 50, 20, 10, 10, 10 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Random" ),
        prescales = cms.vuint32( 600, 600, 600, 600, 1000 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_PixelTracks_Multiplicity70" ),
        prescales = cms.vuint32( 20, 10, 5, 2, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_PixelTracks_Multiplicity85" ),
        prescales = cms.vuint32( 1, 1, 1, 1, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_GlobalRunHPDNoise" ),
        prescales = cms.vuint32( 80, 80, 80, 80, 80 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_L2Mu0_NoVertex" ),
        prescales = cms.vuint32( 100, 100, 100, 100, 1 )
      ),
      cms.PSet(  pathName = cms.string( "DQM_FEDIntegrity" ),
        prescales = cms.vuint32( 2, 2, 2, 2, 2 )
      ),
      cms.PSet(  pathName = cms.string( "HLTMONOutput" ),
        prescales = cms.vuint32( 40, 40, 40, 40, 40 )
      )
    )
)
process.UpdaterService = cms.Service( "UpdaterService",
)

process.hltGetRaw = cms.EDAnalyzer( "HLTGetRaw",
    RawDataCollection = cms.InputTag( "rawDataCollector" )
)
process.hltTriggerType = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 1 )
)
process.hltGtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
    DaqGtInputTag = cms.InputTag( "rawDataCollector" ),
    DaqGtFedId = cms.untracked.int32( 813 ),
    ActiveBoardsMask = cms.uint32( 0xffff ),
    UnpackBxInEvent = cms.int32( 5 ),
    Verbosity = cms.untracked.int32( 0 )
)
process.hltGctDigis = cms.EDProducer( "GctRawToDigi",
    inputLabel = cms.InputTag( "rawDataCollector" ),
    gctFedId = cms.untracked.int32( 745 ),
    hltMode = cms.bool( True ),
    numberOfGctSamplesToUnpack = cms.uint32( 1 ),
    numberOfRctSamplesToUnpack = cms.uint32( 1 ),
    unpackSharedRegions = cms.bool( False ),
    unpackerVersion = cms.uint32( 0 )
)
process.hltL1GtObjectMap = cms.EDProducer( "L1GlobalTrigger",
    GmtInputTag = cms.InputTag( "hltGtDigis" ),
    GctInputTag = cms.InputTag( "hltGctDigis" ),
    CastorInputTag = cms.InputTag( "castorL1Digis" ),
    ProduceL1GtDaqRecord = cms.bool( False ),
    ProduceL1GtEvmRecord = cms.bool( False ),
    ProduceL1GtObjectMapRecord = cms.bool( True ),
    WritePsbL1GtDaqRecord = cms.bool( False ),
    ReadTechnicalTriggerRecords = cms.bool( True ),
    EmulateBxInEvent = cms.int32( 1 ),
    AlternativeNrBxBoardDaq = cms.uint32( 0 ),
    AlternativeNrBxBoardEvm = cms.uint32( 0 ),
    BstLengthBytes = cms.int32( -1 ),
    TechnicalTriggersInputTags = cms.VInputTag( 'simBscDigis' ),
    RecordLength = cms.vint32( 3, 0 )
)
process.hltL1extraParticles = cms.EDProducer( "L1ExtraParticlesProd",
    produceMuonParticles = cms.bool( True ),
    muonSource = cms.InputTag( "hltGtDigis" ),
    produceCaloParticles = cms.bool( True ),
    isolatedEmSource = cms.InputTag( 'hltGctDigis','isoEm' ),
    nonIsolatedEmSource = cms.InputTag( 'hltGctDigis','nonIsoEm' ),
    centralJetSource = cms.InputTag( 'hltGctDigis','cenJets' ),
    forwardJetSource = cms.InputTag( 'hltGctDigis','forJets' ),
    tauJetSource = cms.InputTag( 'hltGctDigis','tauJets' ),
    etTotalSource = cms.InputTag( "hltGctDigis" ),
    etHadSource = cms.InputTag( "hltGctDigis" ),
    etMissSource = cms.InputTag( "hltGctDigis" ),
    htMissSource = cms.InputTag( "hltGctDigis" ),
    hfRingEtSumsSource = cms.InputTag( "hltGctDigis" ),
    hfRingBitCountsSource = cms.InputTag( "hltGctDigis" ),
    centralBxOnly = cms.bool( True ),
    ignoreHtMiss = cms.bool( False )
)
process.hltBPTXCoincidence = cms.EDFilter( "HLTLevel1Activity",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    daqPartitions = cms.uint32( 1 ),
    ignoreL1Mask = cms.bool( True ),
    invert = cms.bool( False ),
    physicsLoBits = cms.uint64( 0x1 ),
    physicsHiBits = cms.uint64( 0x40000 ),
    technicalBits = cms.uint64( 0x0 ),
    bunchCrossings = cms.vint32( 0, -1, 1 )
)
process.hltScalersRawToDigi = cms.EDProducer( "ScalersRawToDigi",
    scalersInputTag = cms.InputTag( "rawDataCollector" )
)
process.hltOnlineBeamSpot = cms.EDProducer( "BeamSpotOnlineProducer",
    label = cms.InputTag( "hltScalersRawToDigi" ),
    changeToCMSCoordinates = cms.bool( False ),
    maxRadius = cms.double( 2.0 ),
    maxZ = cms.double( 40.0 ),
    setSigmaZ = cms.double( 10.0 )
)
process.hltOfflineBeamSpot = cms.EDProducer( "BeamSpotProducer" )
process.hltPreFirstPath = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltBoolFirstPath = cms.EDFilter( "HLTBool",
    result = cms.bool( False )
)
process.hltL1sZeroBias = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "4" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreActivityL1A = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltLevel1Activity = cms.EDFilter( "HLTLevel1Activity",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    daqPartitions = cms.uint32( 1 ),
    ignoreL1Mask = cms.bool( False ),
    invert = cms.bool( False ),
    physicsLoBits = cms.uint64( 0x7fdfdf03c03fbffc ),
    physicsHiBits = cms.uint64( 0x3f1bfddb01800bf6 ),
    technicalBits = cms.uint64( 0x70000fffff001f00 ),
    bunchCrossings = cms.vint32( 0, 1, -1 )
)
process.hltBoolEnd = cms.EDFilter( "HLTBool",
    result = cms.bool( True )
)
process.hltPreActivityPixelClusters = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSiPixelDigis = cms.EDProducer( "SiPixelRawToDigi",
    IncludeErrors = cms.bool( False ),
    InputLabel = cms.InputTag( "rawDataCollector" )
)
process.hltSiPixelClusters = cms.EDProducer( "SiPixelClusterProducer",
    src = cms.InputTag( "hltSiPixelDigis" ),
    maxNumberOfClusters = cms.int32( 10000 ),
    payloadType = cms.string( "HLT" ),
    ChannelThreshold = cms.int32( 1000 ),
    SeedThreshold = cms.int32( 1000 ),
    ClusterThreshold = cms.double( 4000.0 ),
    VCaltoElectronGain = cms.int32( 65 ),
    VCaltoElectronOffset = cms.int32( -414 ),
    MissCalibrate = cms.untracked.bool( True ),
    SplitClusters = cms.bool( False )
)
process.hltSiPixelRecHits = cms.EDProducer( "SiPixelRecHitConverter",
    src = cms.InputTag( "hltSiPixelClusters" ),
    CPE = cms.string( "PixelCPEGeneric" )
)
process.hltPixelActivityFilter = cms.EDFilter( "HLTPixelActivityFilter",
    inputTag = cms.InputTag( "hltSiPixelClusters" ),
    minClusters = cms.uint32( 3 )
)
process.hltPreActivityCSC = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMuonCSCDigis = cms.EDProducer( "CSCDCCUnpacker",
    InputObjects = cms.InputTag( "rawDataCollector" ),
    UseExaminer = cms.bool( True ),
    ExaminerMask = cms.uint32( 0x1febf3f6 ),
    UseSelectiveUnpacking = cms.bool( True ),
    ErrorMask = cms.uint32( 0x0 ),
    UnpackStatusDigis = cms.bool( False ),
    UseFormatStatus = cms.bool( True ),
    PrintEventNumber = cms.untracked.bool( False )
)
process.hltCSCActivityFilter = cms.EDFilter( "HLTCSCActivityFilter",
    cscStripDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCStripDigi' ),
    applyfilter = cms.bool( True ),
    skipStationRing = cms.bool( True ),
    skipRingNumber = cms.int32( 4 ),
    skipStationNumber = cms.int32( 1 )
)
process.hltPreActivityDT = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMuonDTDigis = cms.EDProducer( "DTUnpackingModule",
    dataType = cms.string( "DDU" ),
    fedbyType = cms.bool( False ),
    inputLabel = cms.InputTag( "rawDataCollector" ),
    useStandardFEDid = cms.bool( True ),
    dqmOnly = cms.bool( False ),
    rosParameters = cms.PSet( 
    ),
    readOutParameters = cms.PSet( 
      debug = cms.untracked.bool( False ),
      rosParameters = cms.PSet( 
        writeSC = cms.untracked.bool( True ),
        readingDDU = cms.untracked.bool( True ),
        performDataIntegrityMonitor = cms.untracked.bool( False ),
        readDDUIDfromDDU = cms.untracked.bool( True ),
        debug = cms.untracked.bool( False ),
        localDAQ = cms.untracked.bool( False )
      ),
      performDataIntegrityMonitor = cms.untracked.bool( False ),
      localDAQ = cms.untracked.bool( False )
    )
)
process.hltDTTFUnpacker = cms.EDProducer( "DTTFFEDReader",
    DTTF_FED_Source = cms.InputTag( "rawDataCollector" )
)
process.hltDTActivityFilter = cms.EDFilter( "HLTDTActivityFilter",
    inputDCC = cms.InputTag( "hltDTTFUnpacker" ),
    inputDDU = cms.InputTag( "hltMuonDTDigis" ),
    inputDigis = cms.InputTag( "hltMuonDTDigis" ),
    processDCC = cms.bool( True ),
    processDDU = cms.bool( True ),
    processDigis = cms.bool( True ),
    processingMode = cms.int32( 2 ),
    minQual = cms.int32( 2 ),
    maxStation = cms.int32( 3 ),
    minChamberLayers = cms.int32( 6 ),
    minDDUBX = cms.int32( 9 ),
    maxDDUBX = cms.int32( 14 ),
    minActiveChambs = cms.int32( 1 ),
    activeSectors = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 )
)
process.hltPreActivityDTTuned = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDTActivityFilterTuned = cms.EDFilter( "HLTDTActivityFilter",
    inputDCC = cms.InputTag( "hltDTTFUnpacker" ),
    inputDDU = cms.InputTag( "hltMuonDTDigis" ),
    inputDigis = cms.InputTag( "hltMuonDTDigis" ),
    processDCC = cms.bool( True ),
    processDDU = cms.bool( True ),
    processDigis = cms.bool( True ),
    processingMode = cms.int32( 2 ),
    minQual = cms.int32( 2 ),
    maxStation = cms.int32( 3 ),
    minChamberLayers = cms.int32( 6 ),
    minDDUBX = cms.int32( 9 ),
    maxDDUBX = cms.int32( 14 ),
    minActiveChambs = cms.int32( 1 ),
    activeSectors = cms.vint32( 1, 12 )
)
process.hltPreActivityEcalSC7 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
    sourceTag = cms.InputTag( "rawDataCollector" ),
    workerName = cms.string( "" )
)
process.hltESRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
    sourceTag = cms.InputTag( "rawDataCollector" ),
    workerName = cms.string( "esRawToRecHit" )
)
process.hltEcalRegionalRestFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "all" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltEcalRegionalESRestFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "all" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltEcalRecHitAll = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalRestFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
process.hltESRecHitAll = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalESRestFEDs','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
process.hltHybridSuperClustersActivity = cms.EDProducer( "HybridClusterProducer",
    debugLevel = cms.string( "ERROR" ),
    basicclusterCollection = cms.string( "hybridBarrelBasicClusters" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.string( "hltEcalRecHitAll" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 1.0 ),
    step = cms.int32( 17 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 0.0 ),
    dynamicEThresh = cms.bool( False ),
    eThreshA = cms.double( 0.0030 ),
    eThreshB = cms.double( 0.1 ),
    severityRecHitThreshold = cms.double( 4.0 ),
    severitySpikeId = cms.int32( 2 ),
    severitySpikeThreshold = cms.double( 0.95 ),
    excludeFlagged = cms.bool( False ),
    dynamicPhiRoad = cms.bool( False ),
    clustershapecollection = cms.string( "" ),
    shapeAssociation = cms.string( "hybridShapeAssoc" ),
    RecHitFlagToBeExcluded = cms.vint32(  ),
    RecHitSeverityToBeExcluded = cms.vint32( 999 ),
    bremRecoveryPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersActivity = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersActivity" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 0.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fBremVec = cms.vdouble( -0.04382, 0.1169, 0.9267, -9.413E-4, 1.419 ),
      brLinearHighThr = cms.double( 8.0 ),
      fEtEtaVec = cms.vdouble( 0.0, 1.00121, -0.63672, 0.0, 0.0, 0.0, 0.5655, 6.457, 0.5081, 8.0, 1.023, -0.00181 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
process.hltMulti5x5BasicClustersActivity = cms.EDProducer( "Multi5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    barrelHitProducer = cms.string( "hltEcalRecHitAll" ),
    endcapHitProducer = cms.string( "hltEcalRecHitAll" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    doEndcap = cms.bool( True ),
    doBarrel = cms.bool( False ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    IslandBarrelSeedThr = cms.double( 0.5 ),
    IslandEndcapSeedThr = cms.double( 0.18 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    clustershapecollectionEB = cms.string( "multi5x5BarrelShape" ),
    clustershapecollectionEE = cms.string( "multi5x5EndcapShape" ),
    barrelShapeAssociation = cms.string( "multi5x5BarrelShapeAssoc" ),
    endcapShapeAssociation = cms.string( "multi5x5EndcapShapeAssoc" ),
    RecHitFlagToBeExcluded = cms.vint32(  )
)
process.hltMulti5x5SuperClustersActivity = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersActivity" ),
    barrelClusterProducer = cms.string( "hltMulti5x5BasicClustersActivity" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 1.0 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet( 
        cryVec = cms.vint32( 16, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3 ),
        cryMin = cms.int32( 2 ),
        etVec = cms.vdouble( 5.0, 10.0, 15.0, 20.0, 30.0, 40.0, 45.0, 55.0, 135.0, 195.0, 225.0 )
      ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      )
    )
)
process.hltMulti5x5SuperClustersWithPreshowerActivity = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRecHitAll','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersActivity','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 0.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 8.11E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "ERROR" )
)
process.hltCorrectedMulti5x5SuperClustersWithPreshowerActivity = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5SuperClustersWithPreshowerActivity" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 0.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.9 ),
      fBremVec = cms.vdouble( -0.05228, 0.08738, 0.9508, 0.002677, 1.221 ),
      brLinearHighThr = cms.double( 6.0 ),
      fEtEtaVec = cms.vdouble( 1.0, -0.4386, -32.38, 0.6372, 15.67, -0.0928, -2.462, 1.138, 20.93 )
    )
)
process.hltRecoEcalSuperClusterActivityCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersActivity" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5SuperClustersWithPreshowerActivity" ),
    recoEcalCandidateCollection = cms.string( "" )
)
process.hltEcalActivitySuperClusterWrapper = cms.EDFilter( "HLTEgammaTriggerFilterObjectWrapper",
    candIsolatedTag = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    candNonIsolatedTag = cms.InputTag( "none" ),
    doIsolated = cms.bool( True )
)
process.hltEgammaSelectEcalSuperClustersActivityFilterSC7 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltEcalActivitySuperClusterWrapper" ),
    etcutEB = cms.double( 7.0 ),
    etcutEE = cms.double( 7.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "none" ),
    L1NonIsoCand = cms.InputTag( "none" )
)
process.hltEgammaEcalActivityR9Shape = cms.EDProducer( "EgammaHLTR9Producer",
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEE' ),
    useSwissCross = cms.bool( False )
)
process.hltEgammaEcalActivityR9ShapeFilterSC7 = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltEgammaSelectEcalSuperClustersActivityFilterSC7" ),
    isoTag = cms.InputTag( "hltEgammaEcalActivityR9Shape" ),
    nonIsoTag = cms.InputTag( "none" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 0.98 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( True ),
    L1IsoCand = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    L1NonIsoCand = cms.InputTag( "none" )
)
process.hltPreActivityEcalSC15 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEgammaSelectEcalSuperClustersActivityFilterSC15 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltEcalActivitySuperClusterWrapper" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "none" ),
    L1NonIsoCand = cms.InputTag( "none" )
)
process.hltEgammaEcalActivityR9ShapeFilterSC15 = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltEgammaSelectEcalSuperClustersActivityFilterSC15" ),
    isoTag = cms.InputTag( "hltEgammaEcalActivityR9Shape" ),
    nonIsoTag = cms.InputTag( "none" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 0.98 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( True ),
    L1IsoCand = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    L1NonIsoCand = cms.InputTag( "none" )
)
process.hltL1sL1SingleEG2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG2" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreSelectEcalSpikesL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalRegionalEgammaFEDsLowPt = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "egamma" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Isolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 3.0 ),
        regionEtaMargin = cms.double( 0.25 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 3.0 ),
        regionEtaMargin = cms.double( 0.25 )
      )
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltEcalRegionalEgammaRecHitLowPt = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalEgammaFEDsLowPt" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
process.hltESRegionalEgammaRecHitLowPt = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalEgammaFEDsLowPt','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
process.hltHybridSuperClustersL1IsolatedLowPt = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( True ),
    l1LowerThr = cms.double( 3.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 0.5 ),
    step = cms.int32( 17 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 0.0 ),
    dynamicEThresh = cms.bool( False ),
    eThreshA = cms.double( 0.0030 ),
    eThreshB = cms.double( 0.1 ),
    severityRecHitThreshold = cms.double( 4.0 ),
    severitySpikeId = cms.int32( 2 ),
    severitySpikeThreshold = cms.double( 0.95 ),
    excludeFlagged = cms.bool( False ),
    dynamicPhiRoad = cms.bool( False ),
    RecHitFlagToBeExcluded = cms.vint32(  ),
    RecHitSeverityToBeExcluded = cms.vint32( 999 ),
    bremRecoveryPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1IsolatedLowPt = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1IsolatedLowPt" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
process.hltMulti5x5BasicClustersL1IsolatedLowPt = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( True ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.18 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 3.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    RecHitFlagToBeExcluded = cms.vint32(  )
)
process.hltMulti5x5SuperClustersL1IsolatedLowPt = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1IsolatedLowPt" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 0.5 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
process.hltMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHitLowPt','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1IsolatedLowPt','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 3.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 8.11E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
process.hltHybridSuperClustersL1NonIsolatedLowPt = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( False ),
    l1LowerThr = cms.double( 3.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 0.5 ),
    step = cms.int32( 17 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 0.0 ),
    dynamicEThresh = cms.bool( False ),
    eThreshA = cms.double( 0.0030 ),
    eThreshB = cms.double( 0.1 ),
    severityRecHitThreshold = cms.double( 4.0 ),
    severitySpikeId = cms.int32( 2 ),
    severitySpikeThreshold = cms.double( 0.95 ),
    excludeFlagged = cms.bool( False ),
    dynamicPhiRoad = cms.bool( False ),
    RecHitFlagToBeExcluded = cms.vint32(  ),
    RecHitSeverityToBeExcluded = cms.vint32( 999 ),
    bremRecoveryPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1NonIsolatedTempLowPt = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1NonIsolatedLowPt" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1NonIsolatedLowPt = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolatedTempLowPt" ),
    L1IsoSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1IsolatedLowPt" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
process.hltMulti5x5BasicClustersL1NonIsolatedLowPt = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( False ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.5 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 3.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    RecHitFlagToBeExcluded = cms.vint32(  )
)
process.hltMulti5x5SuperClustersL1NonIsolatedLowPt = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1NonIsolatedLowPt" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 0.5 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
process.hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHitLowPt','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1NonIsolatedLowPt','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 3.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 8.11E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTempLowPt = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTempLowPt" ),
    L1IsoSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
process.hltL1IsoRecoEcalCandidateLowPt = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1IsolatedLowPt" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt" ),
    recoEcalCandidateCollection = cms.string( "" )
)
process.hltL1NonIsoRecoEcalCandidateLowPt = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolatedLowPt" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt" ),
    recoEcalCandidateCollection = cms.string( "" )
)
process.hltEgammaSelectEcalSpikesL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG2" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltEgammaSelectEcalSpikesEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltEgammaSelectEcalSpikesL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 999999.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1IsoR9shapeLowPt = cms.EDProducer( "EgammaHLTR9Producer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    useSwissCross = cms.bool( False )
)
process.hltL1NonIsoR9shapeLowPt = cms.EDProducer( "EgammaHLTR9Producer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    useSwissCross = cms.bool( False )
)
process.hltEgammaSelectEcalSpikesR9filter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltEgammaSelectEcalSpikesEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shapeLowPt" ),
    lessThan = cms.bool( False ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.85 ),
    thrRegularEE = cms.double( 999999.0 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1sL1SingleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreSelectEcalSpikesHighEtL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalRegionalEgammaFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "egamma" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Isolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 5.0 ),
        regionEtaMargin = cms.double( 0.25 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 5.0 ),
        regionEtaMargin = cms.double( 0.25 )
      )
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltEcalRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalEgammaFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
process.hltESRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalEgammaFEDs','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
process.hltHybridSuperClustersL1Isolated = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( True ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 1.5 ),
    step = cms.int32( 17 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 0.0 ),
    dynamicEThresh = cms.bool( False ),
    eThreshA = cms.double( 0.0030 ),
    eThreshB = cms.double( 0.1 ),
    severityRecHitThreshold = cms.double( 4.0 ),
    severitySpikeId = cms.int32( 2 ),
    severitySpikeThreshold = cms.double( 0.95 ),
    excludeFlagged = cms.bool( False ),
    dynamicPhiRoad = cms.bool( False ),
    RecHitFlagToBeExcluded = cms.vint32(  ),
    RecHitSeverityToBeExcluded = cms.vint32( 999 ),
    bremRecoveryPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1Isolated = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1Isolated" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
process.hltMulti5x5BasicClustersL1Isolated = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( True ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.18 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    RecHitFlagToBeExcluded = cms.vint32(  )
)
process.hltMulti5x5SuperClustersL1Isolated = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1Isolated" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 1.0 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
process.hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHit','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1Isolated','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 5.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 8.11E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
process.hltHybridSuperClustersL1NonIsolated = cms.EDProducer( "EgammaHLTHybridClusterProducer",
    debugLevel = cms.string( "INFO" ),
    basicclusterCollection = cms.string( "" ),
    superclusterCollection = cms.string( "" ),
    ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalhitcollection = cms.string( "EcalRecHitsEB" ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    doIsolated = cms.bool( False ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.14 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0 = cms.double( 7.4 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    HybridBarrelSeedThr = cms.double( 1.5 ),
    step = cms.int32( 17 ),
    ethresh = cms.double( 0.1 ),
    eseed = cms.double( 0.35 ),
    ewing = cms.double( 0.0 ),
    dynamicEThresh = cms.bool( False ),
    eThreshA = cms.double( 0.0030 ),
    eThreshB = cms.double( 0.1 ),
    severityRecHitThreshold = cms.double( 4.0 ),
    severitySpikeId = cms.int32( 2 ),
    severitySpikeThreshold = cms.double( 0.95 ),
    excludeFlagged = cms.bool( False ),
    dynamicPhiRoad = cms.bool( False ),
    RecHitFlagToBeExcluded = cms.vint32(  ),
    RecHitSeverityToBeExcluded = cms.vint32( 999 ),
    bremRecoveryPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1NonIsolatedTemp = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1NonIsolated" ),
    superClusterAlgo = cms.string( "Hybrid" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.03 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 1.1 ),
      fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 ),
      brLinearHighThr = cms.double( 8.0 ),
      fBremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 )
    ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet(  )
)
process.hltCorrectedHybridSuperClustersL1NonIsolated = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolatedTemp" ),
    L1IsoSC = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
process.hltMulti5x5BasicClustersL1NonIsolated = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    doIsolated = cms.bool( False ),
    barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    barrelHitCollection = cms.string( "EcalRecHitsEB" ),
    endcapHitCollection = cms.string( "EcalRecHitsEE" ),
    barrelClusterCollection = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    Multi5x5BarrelSeedThr = cms.double( 0.5 ),
    Multi5x5EndcapSeedThr = cms.double( 0.5 ),
    l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    l1LowerThr = cms.double( 5.0 ),
    l1UpperThr = cms.double( 999.0 ),
    l1LowerThrIgnoreIsolation = cms.double( 999.0 ),
    regionEtaMargin = cms.double( 0.3 ),
    regionPhiMargin = cms.double( 0.4 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    RecHitFlagToBeExcluded = cms.vint32(  )
)
process.hltMulti5x5SuperClustersL1NonIsolated = cms.EDProducer( "Multi5x5SuperClusterProducer",
    VerbosityLevel = cms.string( "ERROR" ),
    endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1NonIsolated" ),
    barrelClusterProducer = cms.string( "notused" ),
    endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
    barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
    endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
    barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
    doBarrel = cms.bool( False ),
    doEndcaps = cms.bool( True ),
    barrelEtaSearchRoad = cms.double( 0.06 ),
    barrelPhiSearchRoad = cms.double( 0.8 ),
    endcapEtaSearchRoad = cms.double( 0.14 ),
    endcapPhiSearchRoad = cms.double( 0.6 ),
    seedTransverseEnergyThreshold = cms.double( 1.0 ),
    dynamicPhiRoad = cms.bool( False ),
    bremRecoveryPset = cms.PSet( 
      barrel = cms.PSet(  ),
      endcap = cms.PSet( 
        a = cms.double( 47.85 ),
        c = cms.double( 0.1201 ),
        b = cms.double( 108.8 )
      ),
      doEndcaps = cms.bool( True ),
      doBarrel = cms.bool( False )
    )
)
process.hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer( "PreshowerClusterProducer",
    preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHit','EcalRecHitsES' ),
    endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1NonIsolated','multi5x5EndcapSuperClusters' ),
    preshClusterCollectionX = cms.string( "preshowerXClusters" ),
    preshClusterCollectionY = cms.string( "preshowerYClusters" ),
    preshNclust = cms.int32( 4 ),
    etThresh = cms.double( 5.0 ),
    preshCalibPlaneX = cms.double( 1.0 ),
    preshCalibPlaneY = cms.double( 0.7 ),
    preshCalibGamma = cms.double( 0.024 ),
    preshCalibMIP = cms.double( 8.11E-5 ),
    assocSClusterCollection = cms.string( "" ),
    preshStripEnergyCut = cms.double( 0.0 ),
    preshSeededNstrip = cms.int32( 15 ),
    preshClusterEnergyCut = cms.double( 0.0 ),
    debugLevel = cms.string( "" )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp = cms.EDProducer( "EgammaSCCorrectionMaker",
    VerbosityLevel = cms.string( "ERROR" ),
    recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    superClusterAlgo = cms.string( "Multi5x5" ),
    applyEnergyCorrection = cms.bool( True ),
    sigmaElectronicNoise = cms.double( 0.15 ),
    etThresh = cms.double( 1.0 ),
    corectedSuperClusterCollection = cms.string( "" ),
    hyb_fCorrPset = cms.PSet(  ),
    isl_fCorrPset = cms.PSet(  ),
    dyn_fCorrPset = cms.PSet(  ),
    fix_fCorrPset = cms.PSet( 
      brLinearLowThr = cms.double( 0.6 ),
      fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 ),
      brLinearHighThr = cms.double( 6.0 ),
      fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 )
    )
)
process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer( "EgammaHLTRemoveDuplicatedSC",
    L1NonIsoUskimmedSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp" ),
    L1IsoSC = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    L1NonIsoSkimmedCollection = cms.string( "" )
)
process.hltL1IsoRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    recoEcalCandidateCollection = cms.string( "" )
)
process.hltL1NonIsoRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
    scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    recoEcalCandidateCollection = cms.string( "" )
)
process.hltEgammaSelectEcalSpikesHighEtL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltEgammaSelectEcalSpikesHighEtEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltEgammaSelectEcalSpikesHighEtL1MatchFilterRegional" ),
    etcutEB = cms.double( 6.0 ),
    etcutEE = cms.double( 9999999.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoR9shape = cms.EDProducer( "EgammaHLTR9Producer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    useSwissCross = cms.bool( False )
)
process.hltL1NonIsoR9shape = cms.EDProducer( "EgammaHLTR9Producer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    useSwissCross = cms.bool( False )
)
process.hltEgammaSelectEcalSpikesHighEtR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltEgammaSelectEcalSpikesHighEtEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( False ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.85 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1sL1SingleForJet = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleForJet2U OR L1_SingleForJet4U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1SingleForJet_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1SingleCenJet = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleCenJet2U OR L1_SingleCenJet4U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1SingleCenJet_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1SingleTauJet = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleTauJet4U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1SingleTauJet_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1Jet6U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Jet6U_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreL1Jet6U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1Jet10U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet10U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Jet10U_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreL1Jet10U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sJet15U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreJet15U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltHcalDigis = cms.EDProducer( "HcalRawToDigi",
    InputLabel = cms.InputTag( "rawDataCollector" ),
    UnpackCalib = cms.untracked.bool( True ),
    UnpackZDC = cms.untracked.bool( True ),
    firstSample = cms.int32( 0 ),
    lastSample = cms.int32( 9 ),
    FilterDataQuality = cms.bool( True )
)
process.hltHbhereco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HBHE" ),
    firstSample = cms.int32( 4 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( True ),
    correctForPhaseContainment = cms.bool( True ),
    correctionPhaseNS = cms.double( 13.0 )
)
process.hltHfreco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HF" ),
    firstSample = cms.int32( 3 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( False ),
    correctForPhaseContainment = cms.bool( False ),
    correctionPhaseNS = cms.double( 0.0 )
)
process.hltHoreco = cms.EDProducer( "HcalSimpleReconstructor",
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    dropZSmarkedPassed = cms.bool( True ),
    Subdetector = cms.string( "HO" ),
    firstSample = cms.int32( 4 ),
    samplesToAdd = cms.int32( 4 ),
    correctForTimeslew = cms.bool( True ),
    correctForPhaseContainment = cms.bool( True ),
    correctionPhaseNS = cms.double( 13.0 )
)
process.hltTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.07 ),
    EEThreshold = cms.double( 0.3 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.7 ),
    HESThreshold = cms.double( 0.8 ),
    HEDThreshold = cms.double( 0.8 ),
    HOThreshold0 = cms.double( 3.5 ),
    HOThresholdPlus1 = cms.double( 3.5 ),
    HOThresholdMinus1 = cms.double( 3.5 ),
    HOThresholdPlus2 = cms.double( 3.5 ),
    HOThresholdMinus2 = cms.double( 3.5 ),
    HF1Threshold = cms.double( 0.5 ),
    HF2Threshold = cms.double( 0.85 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRecHitAll:EcalRecHitsEB','hltEcalRecHitAll:EcalRecHitsEE' )
)
process.hltIterativeCone5CaloJets = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.5 ),
    src = cms.InputTag( "hltTowerMakerForAll" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltMCJetCorJetIcone5HF07 = cms.EDProducer( "CaloJetCorrectionProducer",
    src = cms.InputTag( "hltIterativeCone5CaloJets" ),
    verbose = cms.untracked.bool( False ),
    alias = cms.untracked.string( "MCJetCorJetIcone5" ),
    correctors = cms.vstring( 'MCJetCorrectorIcone5HF07' )
)
process.hlt1jet15U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltPreJet15UHcalNoiseFiltered = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltHcalNoiseInfoProducer = cms.EDProducer( "HcalNoiseInfoProducer",
    pMinERatio = cms.double( 25.0 ),
    pMinEZeros = cms.double( 5.0 ),
    pMinEEMF = cms.double( 10.0 ),
    minERatio = cms.double( 50.0 ),
    minEZeros = cms.double( 10.0 ),
    minEEMF = cms.double( 50.0 ),
    pMinE = cms.double( 10.0 ),
    pMinRatio = cms.double( 0.75 ),
    pMaxRatio = cms.double( 0.85 ),
    pMinHPDHits = cms.int32( 10 ),
    pMinRBXHits = cms.int32( 20 ),
    pMinHPDNoOtherHits = cms.int32( 7 ),
    pMinZeros = cms.int32( 4 ),
    pMinLowEHitTime = cms.double( -6.0 ),
    pMaxLowEHitTime = cms.double( 6.0 ),
    pMinHighEHitTime = cms.double( -4.0 ),
    pMaxHighEHitTime = cms.double( 5.0 ),
    pMaxHPDEMF = cms.double( 0.02 ),
    pMaxRBXEMF = cms.double( 0.02 ),
    lMinRatio = cms.double( 0.7 ),
    lMaxRatio = cms.double( 0.96 ),
    lMinHPDHits = cms.int32( 17 ),
    lMinRBXHits = cms.int32( 999 ),
    lMinHPDNoOtherHits = cms.int32( 10 ),
    lMinZeros = cms.int32( 10 ),
    lMinLowEHitTime = cms.double( -9999.9 ),
    lMaxLowEHitTime = cms.double( 9999.0 ),
    lMinHighEHitTime = cms.double( -7.0 ),
    lMaxHighEHitTime = cms.double( 6.0 ),
    tMinRatio = cms.double( 0.73 ),
    tMaxRatio = cms.double( 0.92 ),
    tMinHPDHits = cms.int32( 16 ),
    tMinRBXHits = cms.int32( 50 ),
    tMinHPDNoOtherHits = cms.int32( 9 ),
    tMinZeros = cms.int32( 8 ),
    tMinLowEHitTime = cms.double( -9999.0 ),
    tMaxLowEHitTime = cms.double( 9999.0 ),
    tMinHighEHitTime = cms.double( -5.0 ),
    tMaxHighEHitTime = cms.double( 4.0 ),
    hlMaxHPDEMF = cms.double( -999.0 ),
    hlMaxRBXEMF = cms.double( 0.01 ),
    fillDigis = cms.bool( True ),
    fillRecHits = cms.bool( True ),
    fillCaloTowers = cms.bool( True ),
    fillTracks = cms.bool( False ),
    maxProblemRBXs = cms.int32( 20 ),
    maxCaloTowerIEta = cms.int32( 20 ),
    maxTrackEta = cms.double( 2.0 ),
    minTrackPt = cms.double( 1.0 ),
    digiCollName = cms.string( "hltHcalDigis" ),
    recHitCollName = cms.string( "hltHbhereco" ),
    caloTowerCollName = cms.string( "hltTowerMakerForAll" ),
    trackCollName = cms.string( "generalTracks" ),
    minRecHitE = cms.double( 1.5 ),
    minLowHitE = cms.double( 10.0 ),
    minHighHitE = cms.double( 25.0 )
)
process.hltHcalMETNoiseFilter = cms.EDFilter( "HLTHcalMETNoiseFilter",
    HcalNoiseRBXCollection = cms.InputTag( "hltHcalNoiseInfoProducer" ),
    severity = cms.int32( 1 ),
    maxNumRBXs = cms.int32( 2 ),
    numRBXsToConsider = cms.int32( 2 ),
    needEMFCoincidence = cms.bool( True ),
    minRBXEnergy = cms.double( 50.0 ),
    minRatio = cms.double( 0.65 ),
    maxRatio = cms.double( 0.98 ),
    minHPDHits = cms.int32( 17 ),
    minRBXHits = cms.int32( 999 ),
    minHPDNoOtherHits = cms.int32( 10 ),
    minZeros = cms.int32( 10 ),
    minHighEHitTime = cms.double( -9999.0 ),
    maxHighEHitTime = cms.double( 9999.0 ),
    maxRBXEMF = cms.double( 0.02 ),
    minRecHitE = cms.double( 1.5 ),
    minLowHitE = cms.double( 10.0 ),
    minHighHitE = cms.double( 25.0 )
)
process.hltL1sJet30U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreJet30U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hlt1jet30U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 30.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sJet50U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreJet50U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hlt1jet50U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 50.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sFwdJet20U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_IsoEG10_Jet6U_ForJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreFwdJet20U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltRapGap20U = cms.EDFilter( "HLTRapGapFilter",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    minEta = cms.double( 3.0 ),
    maxEta = cms.double( 5.0 ),
    caloThresh = cms.double( 20.0 )
)
process.hltL1sDiJetAve15U8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDiJetAve15U8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiJetAve15U = cms.EDFilter( "HLTDiJetAveFilter",
    inputJetTag = cms.InputTag( "hltIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    minPtAve = cms.double( 15.0 ),
    minPtJet3 = cms.double( 99999.0 ),
    minDphi = cms.double( -1.0 )
)
process.hltL1sDiJetAve30U8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDiJetAve30U8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiJetAve30U = cms.EDFilter( "HLTDiJetAveFilter",
    inputJetTag = cms.InputTag( "hltIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    minPtAve = cms.double( 30.0 ),
    minPtJet3 = cms.double( 99999.0 ),
    minDphi = cms.double( -1.0 )
)
process.hltL1sDiJetAve50U8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet50" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDiJetAve50U8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiJetAve50U = cms.EDFilter( "HLTDiJetAveFilter",
    inputJetTag = cms.InputTag( "hltIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    minPtAve = cms.double( 50.0 ),
    minPtJet3 = cms.double( 99999.0 ),
    minDphi = cms.double( -1.0 )
)
process.hltL1sDoubleJet15UForwardBackward = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleForJet10U_EtaOpp" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoubleJet15UForwardBackward = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalRegionalJetsFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "jet" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Central' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 14.0 ),
        regionEtaMargin = cms.double( 1.0 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Forward' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 20.0 ),
        regionEtaMargin = cms.double( 1.0 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Tau' ),
        regionPhiMargin = cms.double( 1.0 ),
        Ptmin = cms.double( 14.0 ),
        regionEtaMargin = cms.double( 1.0 )
      )
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltEcalRegionalJetsRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalJetsFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
process.hltTowerMakerForJets = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.07 ),
    EEThreshold = cms.double( 0.3 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.7 ),
    HESThreshold = cms.double( 0.8 ),
    HEDThreshold = cms.double( 0.8 ),
    HOThreshold0 = cms.double( 3.5 ),
    HOThresholdPlus1 = cms.double( 3.5 ),
    HOThresholdMinus1 = cms.double( 3.5 ),
    HOThresholdPlus2 = cms.double( 3.5 ),
    HOThresholdMinus2 = cms.double( 3.5 ),
    HF1Threshold = cms.double( 0.5 ),
    HF2Threshold = cms.double( 0.85 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRegionalJetsRecHit:EcalRecHitsEB','hltEcalRegionalJetsRecHit:EcalRecHitsEE' )
)
process.hltIterativeCone5CaloJetsRegional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.5 ),
    src = cms.InputTag( "hltTowerMakerForJets" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltMCJetCorJetIcone5Regional = cms.EDProducer( "CaloJetCorrectionProducer",
    src = cms.InputTag( "hltIterativeCone5CaloJetsRegional" ),
    verbose = cms.untracked.bool( False ),
    alias = cms.untracked.string( "corJetIcone5" ),
    correctors = cms.vstring( 'MCJetCorrectorIcone5' )
)
process.hltDoubleJet15UForwardBackward = cms.EDFilter( "HLTForwardBackwardJetsFilter",
    inputTag = cms.InputTag( "hltIterativeCone5CaloJetsRegional" ),
    saveTag = cms.untracked.bool( True ),
    minPt = cms.double( 15.0 ),
    minEta = cms.double( 3.0 ),
    maxEta = cms.double( 5.1 )
)
process.hltL1sQuadJet15U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_QuadJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreQuadJet15U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hlt4jet15U = cms.EDFilter( "HLT1CaloJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 4 )
)
process.hltL1sL1MET20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM20" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1MET20 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sMET45 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM30" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreMET45 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMet = cms.EDProducer( "METProducer",
    src = cms.InputTag( "hltTowerMakerForAll" ),
    InputType = cms.string( "CandidateCollection" ),
    METType = cms.string( "CaloMET" ),
    alias = cms.string( "RawCaloMET" ),
    globalThreshold = cms.double( 0.3 ),
    noHF = cms.bool( False ),
    calculateSignificance = cms.bool( False ),
    onlyFiducialParticles = cms.bool( False ),
    rf_type = cms.int32( 0 ),
    correctShowerTracks = cms.bool( False ),
    HO_EtResPar = cms.vdouble( 0.0, 1.3, 0.0050 ),
    HF_EtResPar = cms.vdouble( 0.0, 1.82, 0.09 ),
    HB_PhiResPar = cms.vdouble( 0.02511 ),
    HE_PhiResPar = cms.vdouble( 0.02511 ),
    EE_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    EB_PhiResPar = cms.vdouble( 0.00502 ),
    EE_PhiResPar = cms.vdouble( 0.02511 ),
    HB_EtResPar = cms.vdouble( 0.0, 1.22, 0.05 ),
    EB_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    HF_PhiResPar = cms.vdouble( 0.05022 ),
    HE_EtResPar = cms.vdouble( 0.0, 1.3, 0.05 ),
    HO_PhiResPar = cms.vdouble( 0.02511 )
)
process.hlt1MET45 = cms.EDFilter( "HLT1CaloMET",
    inputTag = cms.InputTag( "hltMet" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 45.0 ),
    MaxEta = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sMET100 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETM70" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreMET100 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hlt1MET100 = cms.EDFilter( "HLT1CaloMET",
    inputTag = cms.InputTag( "hltMet" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 100.0 ),
    MaxEta = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sHT100 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_HTT50" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreHT100 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltJet15UHt = cms.EDProducer( "METProducer",
    src = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    InputType = cms.string( "CaloJetCollection" ),
    METType = cms.string( "MET" ),
    alias = cms.string( "HTMET" ),
    globalThreshold = cms.double( 30.0 ),
    noHF = cms.bool( False ),
    calculateSignificance = cms.bool( False ),
    onlyFiducialParticles = cms.bool( False ),
    rf_type = cms.int32( 0 ),
    correctShowerTracks = cms.bool( False ),
    HO_EtResPar = cms.vdouble( 0.0, 1.3, 0.0050 ),
    HF_EtResPar = cms.vdouble( 0.0, 1.82, 0.09 ),
    HB_PhiResPar = cms.vdouble( 0.02511 ),
    HE_PhiResPar = cms.vdouble( 0.02511 ),
    EE_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    EB_PhiResPar = cms.vdouble( 0.00502 ),
    EE_PhiResPar = cms.vdouble( 0.02511 ),
    HB_EtResPar = cms.vdouble( 0.0, 1.22, 0.05 ),
    EB_EtResPar = cms.vdouble( 0.2, 0.03, 0.0050 ),
    HF_PhiResPar = cms.vdouble( 0.05022 ),
    HE_EtResPar = cms.vdouble( 0.0, 1.3, 0.05 ),
    HO_PhiResPar = cms.vdouble( 0.02511 )
)
process.hltHT100U = cms.EDFilter( "HLTGlobalSumsMET",
    inputTag = cms.InputTag( "hltJet15UHt" ),
    saveTag = cms.untracked.bool( True ),
    observable = cms.string( "sumEt" ),
    Min = cms.double( 100.0 ),
    Max = cms.double( -1.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sL1SingleMuOpenL1SingleMu0 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuOpen OR L1_SingleMu0" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1MuOpen_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1MuOpenL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltPreL1MuOpenDT = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1MuOpenL1FilteredDT = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0" ),
    MaxEta = cms.double( 1.25 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltBPTXAntiCoincidence = cms.EDFilter( "HLTLevel1Activity",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    daqPartitions = cms.uint32( 1 ),
    ignoreL1Mask = cms.bool( True ),
    invert = cms.bool( True ),
    physicsLoBits = cms.uint64( 0x1 ),
    physicsHiBits = cms.uint64( 0x0 ),
    technicalBits = cms.uint64( 0x0 ),
    bunchCrossings = cms.vint32( 0, 1, -1 )
)
process.hltPreL1MuOpen_AntiBPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1Mu = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu7 OR L1_DoubleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Mu = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1MuL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1Mu" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltL1sL1SingleMu20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu20" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Mu20 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1Mu20L1Filtered20 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu20" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 20.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL2Mu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1SingleMu0L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltDt1DRecHits = cms.EDProducer( "DTRecHitProducer",
    debug = cms.untracked.bool( False ),
    dtDigiLabel = cms.InputTag( "hltMuonDTDigis" ),
    recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
    recAlgoConfig = cms.PSet( 
      debug = cms.untracked.bool( False ),
      minTime = cms.double( -3.0 ),
      maxTime = cms.double( 420.0 ),
      tTrigModeConfig = cms.PSet( 
        vPropWire = cms.double( 24.4 ),
        doTOFCorrection = cms.bool( True ),
        tofCorrType = cms.int32( 1 ),
        wirePropCorrType = cms.int32( 1 ),
        doWirePropCorrection = cms.bool( True ),
        doT0Correction = cms.bool( True ),
        debug = cms.untracked.bool( False ),
        tTrigLabel = cms.string( "" )
      ),
      tTrigMode = cms.string( "DTTTrigSyncFromDB" )
    )
)
process.hltDt4DSegments = cms.EDProducer( "DTRecSegment4DProducer",
    debug = cms.untracked.bool( False ),
    recHits1DLabel = cms.InputTag( "hltDt1DRecHits" ),
    recHits2DLabel = cms.InputTag( "dt2DSegments" ),
    Reco4DAlgoName = cms.string( "DTCombinatorialPatternReco4D" ),
    Reco4DAlgoConfig = cms.PSet( 
      segmCleanerMode = cms.int32( 2 ),
      Reco2DAlgoName = cms.string( "DTCombinatorialPatternReco" ),
      recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
      nSharedHitsMax = cms.int32( 2 ),
      hit_afterT0_resolution = cms.double( 0.03 ),
      Reco2DAlgoConfig = cms.PSet( 
        segmCleanerMode = cms.int32( 2 ),
        recAlgo = cms.string( "DTLinearDriftFromDBAlgo" ),
        nSharedHitsMax = cms.int32( 2 ),
        AlphaMaxPhi = cms.double( 1.0 ),
        hit_afterT0_resolution = cms.double( 0.03 ),
        MaxAllowedHits = cms.uint32( 50 ),
        performT0_vdriftSegCorrection = cms.bool( False ),
        AlphaMaxTheta = cms.double( 0.9 ),
        debug = cms.untracked.bool( False ),
        recAlgoConfig = cms.PSet( 
          debug = cms.untracked.bool( False ),
          minTime = cms.double( -3.0 ),
          maxTime = cms.double( 420.0 ),
          tTrigModeConfig = cms.PSet( 
            vPropWire = cms.double( 24.4 ),
            doTOFCorrection = cms.bool( True ),
            tofCorrType = cms.int32( 1 ),
            wirePropCorrType = cms.int32( 1 ),
            doWirePropCorrection = cms.bool( True ),
            doT0Correction = cms.bool( True ),
            debug = cms.untracked.bool( False ),
            tTrigLabel = cms.string( "" )
          ),
          tTrigMode = cms.string( "DTTTrigSyncFromDB" )
        ),
        nUnSharedHitsMin = cms.int32( 2 ),
        performT0SegCorrection = cms.bool( False )
      ),
      performT0_vdriftSegCorrection = cms.bool( False ),
      debug = cms.untracked.bool( False ),
      recAlgoConfig = cms.PSet( 
        debug = cms.untracked.bool( False ),
        minTime = cms.double( -3.0 ),
        maxTime = cms.double( 420.0 ),
        tTrigModeConfig = cms.PSet( 
          vPropWire = cms.double( 24.4 ),
          doTOFCorrection = cms.bool( True ),
          tofCorrType = cms.int32( 1 ),
          wirePropCorrType = cms.int32( 1 ),
          doWirePropCorrection = cms.bool( True ),
          doT0Correction = cms.bool( True ),
          debug = cms.untracked.bool( False ),
          tTrigLabel = cms.string( "" )
        ),
        tTrigMode = cms.string( "DTTTrigSyncFromDB" )
      ),
      nUnSharedHitsMin = cms.int32( 2 ),
      AllDTRecHits = cms.bool( True ),
      performT0SegCorrection = cms.bool( False )
    )
)
process.hltCsc2DRecHits = cms.EDProducer( "CSCRecHitDProducer",
    CSCUseCalibrations = cms.bool( True ),
    CSCUseStaticPedestals = cms.bool( False ),
    CSCUseTimingCorrections = cms.bool( False ),
    stripDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCStripDigi' ),
    wireDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCWireDigi' ),
    CSCstripWireDeltaTime = cms.int32( 8 ),
    CSCNoOfTimeBinsForDynamicPedestal = cms.int32( 2 ),
    CSCStripPeakThreshold = cms.double( 10.0 ),
    CSCStripClusterChargeCut = cms.double( 25.0 ),
    CSCWireClusterDeltaT = cms.int32( 1 ),
    CSCStripxtalksOffset = cms.double( 0.03 ),
    NoiseLevel_ME1a = cms.double( 7.0 ),
    XTasymmetry_ME1a = cms.double( 0.0 ),
    ConstSyst_ME1a = cms.double( 0.022 ),
    NoiseLevel_ME1b = cms.double( 8.0 ),
    XTasymmetry_ME1b = cms.double( 0.0 ),
    ConstSyst_ME1b = cms.double( 0.0070 ),
    NoiseLevel_ME12 = cms.double( 9.0 ),
    XTasymmetry_ME12 = cms.double( 0.0 ),
    ConstSyst_ME12 = cms.double( 0.0 ),
    NoiseLevel_ME13 = cms.double( 8.0 ),
    XTasymmetry_ME13 = cms.double( 0.0 ),
    ConstSyst_ME13 = cms.double( 0.0 ),
    NoiseLevel_ME21 = cms.double( 9.0 ),
    XTasymmetry_ME21 = cms.double( 0.0 ),
    ConstSyst_ME21 = cms.double( 0.0 ),
    NoiseLevel_ME22 = cms.double( 9.0 ),
    XTasymmetry_ME22 = cms.double( 0.0 ),
    ConstSyst_ME22 = cms.double( 0.0 ),
    NoiseLevel_ME31 = cms.double( 9.0 ),
    XTasymmetry_ME31 = cms.double( 0.0 ),
    ConstSyst_ME31 = cms.double( 0.0 ),
    NoiseLevel_ME32 = cms.double( 9.0 ),
    XTasymmetry_ME32 = cms.double( 0.0 ),
    ConstSyst_ME32 = cms.double( 0.0 ),
    NoiseLevel_ME41 = cms.double( 9.0 ),
    XTasymmetry_ME41 = cms.double( 0.0 ),
    ConstSyst_ME41 = cms.double( 0.0 ),
    readBadChannels = cms.bool( True ),
    readBadChambers = cms.bool( True ),
    UseAverageTime = cms.bool( False ),
    UseParabolaFit = cms.bool( False ),
    UseFivePoleFit = cms.bool( True )
)
process.hltCscSegments = cms.EDProducer( "CSCSegmentProducer",
    inputObjects = cms.InputTag( "hltCsc2DRecHits" ),
    algo_type = cms.int32( 1 ),
    algo_psets = cms.VPSet( 
      cms.PSet(  chamber_types = cms.vstring( 'ME1/a',
  'ME1/b',
  'ME1/2',
  'ME1/3',
  'ME2/1',
  'ME2/2',
  'ME3/1',
  'ME3/2',
  'ME4/1',
  'ME4/2' ),
        algo_name = cms.string( "CSCSegAlgoST" ),
        algo_psets = cms.VPSet( 
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            yweightPenalty = cms.double( 1.5 ),
            maxRecHitsInCluster = cms.int32( 20 ),
            hitDropLimit6Hits = cms.double( 0.3333 ),
            BPMinImprovement = cms.double( 10000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            onlyBestSegment = cms.bool( False ),
            dRPhiFineMax = cms.double( 8.0 ),
            curvePenalty = cms.double( 2.0 ),
            dXclusBoxMax = cms.double( 4.0 ),
            BrutePruning = cms.bool( True ),
            tanThetaMax = cms.double( 1.2 ),
            hitDropLimit4Hits = cms.double( 0.6 ),
            useShowering = cms.bool( False ),
            CSCDebug = cms.untracked.bool( False ),
            curvePenaltyThreshold = cms.double( 0.85 ),
            minHitsPerSegment = cms.int32( 3 ),
            dPhiFineMax = cms.double( 0.025 ),
            yweightPenaltyThreshold = cms.double( 1.0 ),
            hitDropLimit5Hits = cms.double( 0.8 ),
            preClustering = cms.bool( True ),
            maxDPhi = cms.double( 999.0 ),
            maxDTheta = cms.double( 999.0 ),
            Pruning = cms.bool( True ),
            dYclusBoxMax = cms.double( 8.0 ),
            preClusteringUseChaining = cms.bool( True ),
            CorrectTheErrors = cms.bool( True ),
            NormChi2Cut2D = cms.double( 20.0 ),
            NormChi2Cut3D = cms.double( 10.0 ),
            prePrun = cms.bool( True ),
            prePrunLimit = cms.double( 3.17 ),
            SeedSmall = cms.double( 2.0E-4 ),
            SeedBig = cms.double( 0.0015 ),
            ForceCovariance = cms.bool( False ),
            ForceCovarianceAll = cms.bool( False ),
            Covariance = cms.double( 0.0 )
          ),
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            yweightPenalty = cms.double( 1.5 ),
            maxRecHitsInCluster = cms.int32( 24 ),
            hitDropLimit6Hits = cms.double( 0.3333 ),
            BPMinImprovement = cms.double( 10000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            onlyBestSegment = cms.bool( False ),
            dRPhiFineMax = cms.double( 8.0 ),
            curvePenalty = cms.double( 2.0 ),
            dXclusBoxMax = cms.double( 4.0 ),
            BrutePruning = cms.bool( True ),
            tanThetaMax = cms.double( 1.2 ),
            hitDropLimit4Hits = cms.double( 0.6 ),
            useShowering = cms.bool( False ),
            CSCDebug = cms.untracked.bool( False ),
            curvePenaltyThreshold = cms.double( 0.85 ),
            minHitsPerSegment = cms.int32( 3 ),
            dPhiFineMax = cms.double( 0.025 ),
            yweightPenaltyThreshold = cms.double( 1.0 ),
            hitDropLimit5Hits = cms.double( 0.8 ),
            preClustering = cms.bool( True ),
            maxDPhi = cms.double( 999.0 ),
            maxDTheta = cms.double( 999.0 ),
            Pruning = cms.bool( True ),
            dYclusBoxMax = cms.double( 8.0 ),
            preClusteringUseChaining = cms.bool( True ),
            CorrectTheErrors = cms.bool( True ),
            NormChi2Cut2D = cms.double( 20.0 ),
            NormChi2Cut3D = cms.double( 10.0 ),
            prePrun = cms.bool( True ),
            prePrunLimit = cms.double( 3.17 ),
            SeedSmall = cms.double( 2.0E-4 ),
            SeedBig = cms.double( 0.0015 ),
            ForceCovariance = cms.bool( False ),
            ForceCovarianceAll = cms.bool( False ),
            Covariance = cms.double( 0.0 )
          )
        ),
        parameters_per_chamber_type = cms.vint32( 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 )
      )
    )
)
process.hltMuonRPCDigis = cms.EDProducer( "RPCUnpackingModule",
    InputLabel = cms.InputTag( "rawDataCollector" ),
    doSynchro = cms.bool( False )
)
process.hltRpcRecHits = cms.EDProducer( "RPCRecHitProducer",
    rpcDigiLabel = cms.InputTag( "hltMuonRPCDigis" ),
    recAlgo = cms.string( "RPCRecHitStandardAlgo" ),
    maskSource = cms.string( "File" ),
    maskvecfile = cms.FileInPath( "RecoLocalMuon/RPCRecHit/data/RPCMaskVec.dat" ),
    deadSource = cms.string( "File" ),
    deadvecfile = cms.FileInPath( "RecoLocalMuon/RPCRecHit/data/RPCDeadVec.dat" ),
    recAlgoConfig = cms.PSet(  )
)
process.hltL2MuonSeeds = cms.EDProducer( "L2MuonSeedGenerator",
    InputObjects = cms.InputTag( "hltL1extraParticles" ),
    GMTReadoutCollection = cms.InputTag( "hltGtDigis" ),
    Propagator = cms.string( "SteppingHelixPropagatorAny" ),
    L1MinPt = cms.double( 0.0 ),
    L1MaxEta = cms.double( 2.5 ),
    L1MinQuality = cms.uint32( 1 ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    )
)
process.hltL2Muons = cms.EDProducer( "L2MuonProducer",
    InputObjects = cms.InputTag( "hltL2MuonSeeds" ),
    L2TrajBuilderParameters = cms.PSet( 
      DoRefit = cms.bool( False ),
      SeedPropagator = cms.string( "FastSteppingHelixPropagatorAny" ),
      FilterParameters = cms.PSet( 
        NumberOfSigma = cms.double( 3.0 ),
        FitDirection = cms.string( "insideOut" ),
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        MaxChi2 = cms.double( 1000.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 25.0 ),
          Granularity = cms.int32( 0 ),
          RescaleErrorFactor = cms.double( 100.0 ),
          UseInvalidHits = cms.bool( True ),
          RescaleError = cms.bool( False )
        ),
        EnableRPCMeasurement = cms.bool( True ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        EnableDTMeasurement = cms.bool( True ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        EnableCSCMeasurement = cms.bool( True )
      ),
      NavigationType = cms.string( "Standard" ),
      SeedTransformerParameters = cms.PSet( 
        Fitter = cms.string( "KFFitterSmootherForL2Muon" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        UseSubRecHits = cms.bool( False ),
        NMinRecHits = cms.uint32( 2 ),
        RescaleError = cms.double( 100.0 )
      ),
      DoBackwardFilter = cms.bool( True ),
      SeedPosition = cms.string( "in" ),
      BWFilterParameters = cms.PSet( 
        NumberOfSigma = cms.double( 3.0 ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        FitDirection = cms.string( "outsideIn" ),
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        MaxChi2 = cms.double( 100.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 25.0 ),
          Granularity = cms.int32( 2 ),
          RescaleErrorFactor = cms.double( 100.0 ),
          UseInvalidHits = cms.bool( True ),
          RescaleError = cms.bool( False )
        ),
        EnableRPCMeasurement = cms.bool( True ),
        BWSeedType = cms.string( "fromGenerator" ),
        EnableDTMeasurement = cms.bool( True ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        Propagator = cms.string( "FastSteppingHelixPropagatorAny" ),
        EnableCSCMeasurement = cms.bool( True )
      ),
      DoSeedRefit = cms.bool( False )
    ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'FastSteppingHelixPropagatorAny',
        'FastSteppingHelixPropagatorOpposite' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    TrackLoaderParameters = cms.PSet( 
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
      DoSmoothing = cms.bool( False ),
      MuonUpdatorAtVertexParameters = cms.PSet( 
        MaxChi2 = cms.double( 1000000.0 ),
        BeamSpotPosition = cms.vdouble( 0.0, 0.0, 0.0 ),
        Propagator = cms.string( "FastSteppingHelixPropagatorOpposite" ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 )
      ),
      VertexConstraint = cms.bool( True ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL2MuonCandidates = cms.EDProducer( "L2MuonCandidateProducer",
    InputObjects = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' )
)
process.hltL2Mu0L2Filtered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu0L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreL2Mu3 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSingleMu3L2Filtered3 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu0L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreL2Mu5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL2Mu5L2Filtered5 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu0L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltL1sL1SingleMu7 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu7" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL2Mu9 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1SingleMu7L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu7" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltL2Mu9L2Filtered9 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 9.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreL2Mu11 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL2Mu11L2Filtered11 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 11.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltL1sL1SingleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreIsoMu3 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSingleMuIsoL1Filtered3 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltSingleMuIsoL2PreFiltered3 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL1Filtered3" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltEcalRegionalMuonsFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "candidate" ),
    doES = cms.bool( False ),
    sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
    ),
    CandJobPSet = cms.VPSet( 
      cms.PSet(  bePrecise = cms.bool( False ),
        propagatorNameToBePrecise = cms.string( "" ),
        epsilon = cms.double( 0.01 ),
        regionPhiMargin = cms.double( 0.3 ),
        cType = cms.string( "chargedcandidate" ),
        Source = cms.InputTag( "hltL2MuonCandidates" ),
        Ptmin = cms.double( 0.0 ),
        regionEtaMargin = cms.double( 0.3 )
      )
    )
)
process.hltEcalRegionalMuonsRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalMuonsFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "NotNeededsplitOutputTrue" )
)
process.hltTowerMakerForMuons = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.07 ),
    EEThreshold = cms.double( 0.3 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.7 ),
    HESThreshold = cms.double( 0.8 ),
    HEDThreshold = cms.double( 0.8 ),
    HOThreshold0 = cms.double( 3.5 ),
    HOThresholdPlus1 = cms.double( 3.5 ),
    HOThresholdMinus1 = cms.double( 3.5 ),
    HOThresholdPlus2 = cms.double( 3.5 ),
    HOThresholdMinus2 = cms.double( 3.5 ),
    HF1Threshold = cms.double( 0.5 ),
    HF2Threshold = cms.double( 0.85 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "hltHoreco" ),
    hfInput = cms.InputTag( "hltHfreco" ),
    AllowMissingInputs = cms.bool( False ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag( 'hltEcalRegionalMuonsRecHit:EcalRecHitsEB','hltEcalRegionalMuonsRecHit:EcalRecHitsEE' )
)
process.hltL2MuonIsolations = cms.EDProducer( "L2MuonIsolationProducer",
    StandAloneCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    ExtractorPSet = cms.PSet( 
      DR_Veto_H = cms.double( 0.1 ),
      Vertex_Constraint_Z = cms.bool( False ),
      Threshold_H = cms.double( 0.5 ),
      ComponentName = cms.string( "CaloExtractor" ),
      Threshold_E = cms.double( 0.2 ),
      DR_Max = cms.double( 0.24 ),
      DR_Veto_E = cms.double( 0.07 ),
      Weight_E = cms.double( 1.5 ),
      Vertex_Constraint_XY = cms.bool( False ),
      DepositLabel = cms.untracked.string( "EcalPlusHcal" ),
      CaloTowerCollectionLabel = cms.InputTag( "hltTowerMakerForMuons" ),
      Weight_H = cms.double( 1.0 )
    ),
    IsolatorPSet = cms.PSet( 
      ConeSizes = cms.vdouble( 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24 ),
      ComponentName = cms.string( "SimpleCutsIsolator" ),
      Thresholds = cms.vdouble( 4.0, 3.7, 4.0, 3.5, 3.4, 3.4, 3.2, 3.4, 3.1, 2.9, 2.9, 2.7, 3.1, 3.0, 2.4, 2.1, 2.0, 2.3, 2.2, 2.4, 2.5, 2.5, 2.6, 2.9, 3.1, 2.9 ),
      EtaBounds = cms.vdouble( 0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 1.785, 1.88, 1.9865, 2.1075, 2.247, 2.411 )
    )
)
process.hltSingleMuIsoL2IsoFiltered3 = cms.EDFilter( "HLTMuonIsoFilter",
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL2PreFiltered3" ),
    MinN = cms.int32( 1 ),
    DepTag = cms.VInputTag( 'hltL2MuonIsolations' ),
    IsolatorPSet = cms.PSet(  )
)
process.hltSiStripRawToClustersFacility = cms.EDProducer( "SiStripRawToClusters",
    ProductLabel = cms.InputTag( "rawDataCollector" ),
    Clusterizer = cms.PSet( 
      ChannelThreshold = cms.double( 2.0 ),
      MaxSequentialBad = cms.uint32( 1 ),
      Algorithm = cms.string( "ThreeThresholdAlgorithm" ),
      MaxSequentialHoles = cms.uint32( 0 ),
      MaxAdjacentBad = cms.uint32( 0 ),
      QualityLabel = cms.string( "" ),
      SeedThreshold = cms.double( 3.0 ),
      ClusterThreshold = cms.double( 5.0 )
    ),
    Algorithms = cms.PSet( 
      SiStripFedZeroSuppressionMode = cms.uint32( 4 ),
      CommonModeNoiseSubtractionMode = cms.string( "Median" )
    )
)
process.hltSiStripClusters = cms.EDProducer( "MeasurementTrackerSiStripRefGetterProducer",
    InputModuleLabel = cms.InputTag( "hltSiStripRawToClustersFacility" ),
    measurementTrackerName = cms.string( "" )
)
process.hltL3TrajectorySeed = cms.EDProducer( "TSGFromL2Muon",
    PtCut = cms.double( 1.0 ),
    PCut = cms.double( 2.5 ),
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorOpposite',
        'SteppingHelixPropagatorAlong' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    MuonTrackingRegionBuilder = cms.PSet(  ),
    TkSeedGenerator = cms.PSet( 
      propagatorCompatibleName = cms.string( "SteppingHelixPropagatorOpposite" ),
      option = cms.uint32( 3 ),
      ComponentName = cms.string( "TSGForRoadSearch" ),
      errorMatrixPset = cms.PSet( 
        action = cms.string( "use" ),
        atIP = cms.bool( True ),
        errorMatrixValuesPSet = cms.PSet( 
          pf3_V12 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V13 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V11 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V14 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          yAxis = cms.vdouble( 0.0, 1.0, 1.4, 10.0 ),
          pf3_V15 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          zAxis = cms.vdouble( -3.14159, 3.14159 ),
          pf3_V33 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V45 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V44 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          xAxis = cms.vdouble( 0.0, 13.0, 30.0, 70.0, 1000.0 ),
          pf3_V23 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V22 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V55 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 3.0, 3.0, 3.0, 5.0, 4.0, 5.0, 10.0, 7.0, 10.0, 10.0, 10.0, 10.0 )
          ),
          pf3_V34 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V35 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V25 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          ),
          pf3_V24 = cms.PSet( 
            action = cms.string( "scale" ),
            values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
          )
        )
      ),
      propagatorName = cms.string( "SteppingHelixPropagatorAlong" ),
      manySeeds = cms.bool( False ),
      copyMuonRecHit = cms.bool( False ),
      maxChi2 = cms.double( 40.0 )
    ),
    TrackerSeedCleaner = cms.PSet(  ),
    TSGFromMixedPairs = cms.PSet(  ),
    TSGFromPixelTriplets = cms.PSet(  ),
    TSGFromPixelPairs = cms.PSet(  ),
    TSGForRoadSearchOI = cms.PSet(  ),
    TSGForRoadSearchIOpxl = cms.PSet(  ),
    TSGFromPropagation = cms.PSet(  ),
    TSGFromCombinedHits = cms.PSet(  )
)
process.hltL3TrackCandidateFromL2 = cms.EDProducer( "CkfTrajectoryMaker",
    trackCandidateAlso = cms.bool( True ),
    src = cms.InputTag( "hltL3TrajectorySeed" ),
    TrajectoryBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    doSeedingRegionRebuilding = cms.bool( False ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltL3TkTracksFromL2 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL3TrackCandidateFromL2" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltL3Muons = cms.EDProducer( "L3MuonProducer",
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    L3TrajBuilderParameters = cms.PSet( 
      ScaleTECxFactor = cms.double( -1.0 ),
      ScaleTECyFactor = cms.double( -1.0 ),
      TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
      MuonTrackingRegionBuilder = cms.PSet( 
        EtaR_UpperLimit_Par1 = cms.double( 0.25 ),
        Eta_fixed = cms.double( 0.2 ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        OnDemand = cms.double( -1.0 ),
        Rescale_Dz = cms.double( 3.0 ),
        Eta_min = cms.double( 0.05 ),
        Rescale_phi = cms.double( 3.0 ),
        PhiR_UpperLimit_Par1 = cms.double( 0.6 ),
        DeltaZ_Region = cms.double( 15.9 ),
        Phi_min = cms.double( 0.05 ),
        PhiR_UpperLimit_Par2 = cms.double( 0.2 ),
        vertexCollection = cms.InputTag( "pixelVertices" ),
        Phi_fixed = cms.double( 0.2 ),
        DeltaR = cms.double( 0.2 ),
        EtaR_UpperLimit_Par2 = cms.double( 0.15 ),
        UseFixedRegion = cms.bool( False ),
        Rescale_eta = cms.double( 3.0 ),
        UseVertex = cms.bool( False ),
        EscapePt = cms.double( 1.5 )
      ),
      TrackerPropagator = cms.string( "SteppingHelixPropagatorAny" ),
      GlobalMuonTrackMatcher = cms.PSet( 
        MinP = cms.double( 2.5 ),
        MinPt = cms.double( 1.0 ),
        Pt_threshold1 = cms.double( 0.0 ),
        Pt_threshold2 = cms.double( 9.99999999E8 ),
        Eta_threshold = cms.double( 1.2 ),
        Chi2Cut_1 = cms.double( 50.0 ),
        Chi2Cut_2 = cms.double( 50.0 ),
        Chi2Cut_3 = cms.double( 200.0 ),
        LocChi2Cut = cms.double( 0.0010 ),
        DeltaDCut_1 = cms.double( 40.0 ),
        DeltaDCut_2 = cms.double( 10.0 ),
        DeltaDCut_3 = cms.double( 15.0 ),
        DeltaRCut_1 = cms.double( 0.1 ),
        DeltaRCut_2 = cms.double( 0.2 ),
        DeltaRCut_3 = cms.double( 1.0 ),
        Quality_1 = cms.double( 20.0 ),
        Quality_2 = cms.double( 15.0 ),
        Quality_3 = cms.double( 7.0 ),
        Propagator = cms.string( "SmartPropagator" )
      ),
      tkTrajLabel = cms.InputTag( "hltL3TkTracksFromL2" ),
      MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
      RefitRPCHits = cms.bool( True ),
      TrackTransformer = cms.PSet( 
        DoPredictionsOnly = cms.bool( False ),
        Fitter = cms.string( "L3MuKFFitter" ),
        TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
        Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        RefitDirection = cms.string( "insideOut" ),
        RefitRPCHits = cms.bool( True ),
        Propagator = cms.string( "SmartPropagatorAny" )
      ),
      PtCut = cms.double( 1.0 ),
      GlbRefitterParameters = cms.PSet( 
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        MuonHitsOption = cms.int32( 1 ),
        Chi2CutCSC = cms.double( 150.0 ),
        Chi2CutDT = cms.double( 10.0 ),
        Chi2CutRPC = cms.double( 1.0 ),
        HitThreshold = cms.int32( 1 ),
        Fitter = cms.string( "L3MuKFFitter" ),
        Propagator = cms.string( "SmartPropagatorAny" ),
        TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        DoPredictionsOnly = cms.bool( False ),
        RefitDirection = cms.string( "insideOut" ),
        PropDirForCosmics = cms.bool( False ),
        RefitRPCHits = cms.bool( True ),
        SkipStation = cms.int32( -1 ),
        TrackerSkipSystem = cms.int32( -1 ),
        TrackerSkipSection = cms.int32( -1 )
      ),
      PCut = cms.double( 2.5 )
    ),
    ServiceParameters = cms.PSet( 
      Propagators = cms.untracked.vstring( 'SmartPropagatorAny',
        'SteppingHelixPropagatorAny',
        'SmartPropagator',
        'SteppingHelixPropagatorOpposite' ),
      RPCLayers = cms.bool( True ),
      UseMuonNavigation = cms.untracked.bool( True )
    ),
    TrackLoaderParameters = cms.PSet( 
      PutTkTrackIntoEvent = cms.untracked.bool( True ),
      VertexConstraint = cms.bool( False ),
      MuonSeededTracksInstance = cms.untracked.string( "L2Seeded" ),
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" ),
      MuonUpdatorAtVertexParameters = cms.PSet( 
        MaxChi2 = cms.double( 1000000.0 ),
        Propagator = cms.string( "SteppingHelixPropagatorOpposite" ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 )
      ),
      SmoothTkTrack = cms.untracked.bool( False ),
      DoSmoothing = cms.bool( True ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL3MuonCandidates = cms.EDProducer( "L3MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL3Muons" )
)
process.hltSingleMuIsoL3PreFiltered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL2IsoFiltered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltPixelTracks = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducer" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originHalfLength = cms.double( 15.9 ),
        originZPos = cms.double( 0.0 ),
        originYPos = cms.double( 0.0 ),
        ptMin = cms.double( 0.9 ),
        originXPos = cms.double( 0.0 ),
        originRadius = cms.double( 0.2 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.1 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
process.hltL3MuonIsolations = cms.EDProducer( "L3MuonIsolationProducer",
    inputMuonCollection = cms.InputTag( "hltL3Muons" ),
    OutputMuIsoDeposits = cms.bool( True ),
    TrackPt_Min = cms.double( -1.0 ),
    CutsPSet = cms.PSet( 
      ConeSizes = cms.vdouble( 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24, 0.24 ),
      ComponentName = cms.string( "SimpleCuts" ),
      Thresholds = cms.vdouble( 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.2, 1.1, 1.2, 1.0, 1.1, 1.0, 1.0, 1.1, 1.0, 1.0, 1.1, 0.9, 1.1, 0.9, 1.1, 1.0, 1.0, 0.9, 0.8, 0.1 ),
      maxNTracks = cms.int32( -1 ),
      EtaBounds = cms.vdouble( 0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 1.785, 1.88, 1.9865, 2.1075, 2.247, 2.411 ),
      applyCutsORmaxNTracks = cms.bool( False )
    ),
    ExtractorPSet = cms.PSet( 
      Chi2Prob_Min = cms.double( -1.0 ),
      Diff_z = cms.double( 0.2 ),
      inputTrackCollection = cms.InputTag( "hltPixelTracks" ),
      ReferenceRadius = cms.double( 6.0 ),
      BeamSpotLabel = cms.InputTag( "hltOfflineBeamSpot" ),
      ComponentName = cms.string( "PixelTrackExtractor" ),
      DR_Max = cms.double( 0.24 ),
      Diff_r = cms.double( 0.1 ),
      PropagateTracksToRadius = cms.bool( True ),
      DR_VetoPt = cms.double( 0.025 ),
      DR_Veto = cms.double( 0.01 ),
      NHits_Min = cms.uint32( 0 ),
      Chi2Ndof_Max = cms.double( 1.0E64 ),
      Pt_Min = cms.double( -1.0 ),
      DepositLabel = cms.untracked.string( "PXLS" ),
      BeamlineOption = cms.string( "BeamSpotFromEvent" ),
      VetoLeadingTrack = cms.bool( True ),
      PtVeto_Min = cms.double( 2.0 )
    )
)
process.hltSingleMuIsoL3IsoFiltered3 = cms.EDFilter( "HLTMuonIsoFilter",
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMuIsoL3PreFiltered3" ),
    MinN = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    DepTag = cms.VInputTag( 'hltL3MuonIsolations' ),
    IsolatorPSet = cms.PSet(  )
)
process.hltPreMu3 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSingleMu3L3Filtered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu3L2Filtered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1SingleMu3L1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltSingleMu5L2Filtered4 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu3L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 4.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltSingleMu5L3Filtered5 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu5L2Filtered4" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu9 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSingleMu9L2Filtered7 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltL1SingleMu7L1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 7.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltSingleMu9L3Filtered9 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu9L2Filtered7" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 9.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltL1sL1DoubleMuOpen = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleMuOpen" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1DoubleMuOpen = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDoubleMuLevel1PathL1OpenFiltered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltPreL1DoubleMuOpenTight = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "gtDigis" )
)
process.hltCscTfDigis = cms.EDProducer( "CSCTFUnpacker",
    MinBX = cms.int32( 3 ),
    MaxBX = cms.int32( 9 ),
    swapME1strips = cms.bool( False ),
    mappingFile = cms.string( "" ),
    slot2sector = cms.vint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.hltL1DoubleMuOpenTightL1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( True ),
    CSCTFtag = cms.InputTag( "hltCscTfDigis" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltPreL2DoubleMu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiMuonL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltDiMuonL2PreFiltered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreDoubleMu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiMuonL3PreFiltered0 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered0" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltL1sL1DoubleMu3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleMu3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoubleMu3 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDiMuonL1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltDiMuonL2PreFiltered = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL1Filtered" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltDiMuonL3PreFiltered = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu0L1MuOpen = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu0L1MuOpenL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltMu0L1MuOpenL2Filtered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu0L1MuOpenL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu0L1MuOpenL3Filtered0 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu0L1MuOpenL2Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu3L1MuOpen = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu3L1MuOpenL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltMu3L1MuOpenL2Filtered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu3L1MuOpenL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu3L1MuOpenL3Filtered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu3L1MuOpenL2Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu5L1MuOpen = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu5L1MuOpenL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1DoubleMuOpen" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 2 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltMu5L1MuOpenL2Filtered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu5L1MuOpenL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu5L1MuOpenL3Filtered5 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu5L1MuOpenL2Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu0L2Mu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu0L2Mu0L3Filtered0 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu3L2Mu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu3L2Mu0L3Filtered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu5L2Mu0 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu5L2Mu0L3Filtered5 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonL2PreFiltered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreMu0Track0Jpsi = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu0TrackJpsiL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltMu0TrackJpsiL2Filtered0 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu0TrackJpsiL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu0TrackJpsiL3Filtered0 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu0TrackJpsiL2Filtered0" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltMuTrackJpsiPixelTrackSelector = cms.EDProducer( "QuarkoniaTrackSelector",
    muonCandidates = cms.InputTag( "hltL3MuonCandidates" ),
    tracks = cms.InputTag( "hltPixelTracks" ),
    checkCharge = cms.bool( False ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MinMasses = cms.vdouble( 1.6 ),
    MaxMasses = cms.vdouble( 4.6 )
)
process.hltMuTrackJpsiPixelTrackCands = cms.EDProducer( "ConcreteChargedCandidateProducer",
    src = cms.InputTag( "hltMuTrackJpsiPixelTrackSelector" ),
    particleType = cms.string( "mu-" )
)
process.hltMu0TrackJpsiPixelMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiPixelTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu0TrackJpsiL3Filtered0" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( False ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 3 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 999.0 ),
    MinMasses = cms.vdouble( 1.6 ),
    MaxMasses = cms.vdouble( 4.6 )
)
process.hltMuTrackJpsiTrackSeeds = cms.EDProducer( "SeedGeneratorFromProtoTracksEDProducer",
    InputCollection = cms.InputTag( "hltMuTrackJpsiPixelTrackSelector" ),
    useProtoTrackKinematics = cms.bool( False ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
process.hltMuTrackJpsiCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltMuTrackJpsiTrackSeeds" ),
    TrajectoryBuilder = cms.string( "hltMuTrackJpsiTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" ),
      numberMeasurementsForFit = cms.int32( 4 )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltMuTrackJpsiCtfTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltMuTrackJpsiCtfTracks" ),
    Fitter = cms.string( "FittingSmootherRK" ),
    Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
    src = cms.InputTag( "hltMuTrackJpsiCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltMuTrackJpsiCtfTrackCands = cms.EDProducer( "ConcreteChargedCandidateProducer",
    src = cms.InputTag( "hltMuTrackJpsiCtfTracks" ),
    particleType = cms.string( "mu-" )
)
process.hltMu0TrackJpsiTrackMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiCtfTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu0TrackJpsiPixelMassFiltered" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( True ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 5 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 0.5 ),
    MinMasses = cms.vdouble( 2.7 ),
    MaxMasses = cms.vdouble( 3.5 )
)
process.hltPreMu3Track0Jpsi = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu3TrackJpsiL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltMu3TrackJpsiL2Filtered3 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu3TrackJpsiL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu3TrackJpsiL3Filtered3 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu3TrackJpsiL2Filtered3" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 3.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltMu3TrackJpsiPixelMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiPixelTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu3TrackJpsiL3Filtered3" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( False ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 3 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 999.0 ),
    MinMasses = cms.vdouble( 1.6 ),
    MaxMasses = cms.vdouble( 4.6 )
)
process.hltMu3TrackJpsiTrackMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiCtfTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu3TrackJpsiPixelMassFiltered" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( True ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 5 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 0.5 ),
    MinMasses = cms.vdouble( 2.7 ),
    MaxMasses = cms.vdouble( 3.5 )
)
process.hltPreMu5Track0Jpsi = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMu5TrackJpsiL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu3" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltMu5TrackJpsiL2Filtered4 = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu5TrackJpsiL1Filtered0" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 4.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltMu5TrackJpsiL3Filtered5 = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    PreviousCandTag = cms.InputTag( "hltMu5TrackJpsiL2Filtered4" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 5.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltMu5TrackJpsiPixelMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiPixelTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu5TrackJpsiL3Filtered5" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( False ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 3 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 999.0 ),
    MinMasses = cms.vdouble( 1.6 ),
    MaxMasses = cms.vdouble( 4.6 )
)
process.hltMu5TrackJpsiTrackMassFiltered = cms.EDFilter( "HLTMuonTrackMassFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    TrackTag = cms.InputTag( "hltMuTrackJpsiCtfTrackCands" ),
    PreviousCandTag = cms.InputTag( "hltMu5TrackJpsiPixelMassFiltered" ),
    SaveTag = cms.untracked.bool( True ),
    checkCharge = cms.bool( True ),
    MinTrackPt = cms.double( 0.0 ),
    MinTrackP = cms.double( 3.0 ),
    MaxTrackEta = cms.double( 999.0 ),
    MaxTrackDxy = cms.double( 999.0 ),
    MaxTrackDz = cms.double( 999.0 ),
    MinTrackHits = cms.int32( 5 ),
    MaxTrackNormChi2 = cms.double( 1.0E10 ),
    MaxDzMuonTrack = cms.double( 0.5 ),
    MinMasses = cms.vdouble( 2.7 ),
    MaxMasses = cms.vdouble( 3.5 )
)
process.hltPreL1SingleEG2 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreL1SingleEG5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1SingleEG8 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG8" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1SingleEG8 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1DoubleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1DoubleEG5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreEle10LWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsolatedElectronHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
process.hltL1NonIsolatedElectronHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoLargeWindowElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.3 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "PixelLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 ),
        maxElement = cms.uint32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.1 ),
      PhiMin2 = cms.double( -0.0080 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.05 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.2 ),
      rMinI = cms.double( -0.2 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.3 ),
      pPhiMin1 = cms.double( -0.05 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.1 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.2 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.2 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0080 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL1NonIsoLargeWindowElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.3 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "PixelLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 ),
        maxElement = cms.uint32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.1 ),
      PhiMin2 = cms.double( -0.0080 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.05 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.2 ),
      rMinI = cms.double( -0.2 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.3 ),
      pPhiMin1 = cms.double( -0.05 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.1 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.2 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.2 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0080 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle10LWEleIdL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    isIeta = cms.bool( True )
)
process.hltL1NonIsoHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    isIeta = cms.bool( True )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.015 ),
    thrRegularEE = cms.double( 0.04 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltCkfL1IsoLargeWindowTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltCtfL1IsoLargeWindowWithMaterialTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltCkfL1IsoLargeWindowTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltPixelMatchLargeWindowElectronsL1Iso = cms.EDProducer( "EgammaHLTPixelMatchElectronProducers",
    TrackProducer = cms.InputTag( "hltCtfL1IsoLargeWindowWithMaterialTracks" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
process.hltCkfL1NonIsoLargeWindowTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltCtfL1NonIsoLargeWindowWithMaterialTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltCkfL1NonIsoLargeWindowTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltPixelMatchLargeWindowElectronsL1NonIso = cms.EDProducer( "EgammaHLTPixelMatchElectronProducers",
    TrackProducer = cms.InputTag( "hltCtfL1NonIsoLargeWindowWithMaterialTracks" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter = cms.EDFilter( "HLTElectronOneOEMinusOneOPFilterRegional",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter" ),
    electronIsolatedProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    electronNonIsolatedProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" ),
    barrelcut = cms.double( 999.9 ),
    endcapcut = cms.double( 999.9 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False )
)
process.hltElectronL1IsoLargeWindowDetaDphi = cms.EDProducer( "EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    useTrackProjectionToEcal = cms.untracked.bool( True )
)
process.hltElectronL1NonIsoLargeWindowDetaDphi = cms.EDProducer( "EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    useTrackProjectionToEcal = cms.untracked.bool( True )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter = cms.EDFilter( "HLTElectronGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter" ),
    isoTag = cms.InputTag( 'hltElectronL1IsoLargeWindowDetaDphi','Deta' ),
    nonIsoTag = cms.InputTag( 'hltElectronL1NonIsoLargeWindowDetaDphi','Deta' ),
    lessThan = cms.bool( True ),
    thrRegularEB = cms.double( 0.0080 ),
    thrRegularEE = cms.double( 0.011 ),
    thrOverPtEB = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrTimesPtEB = cms.double( -1.0 ),
    thrTimesPtEE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter = cms.EDFilter( "HLTElectronGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter" ),
    isoTag = cms.InputTag( 'hltElectronL1IsoLargeWindowDetaDphi','Dphi' ),
    nonIsoTag = cms.InputTag( 'hltElectronL1NonIsoLargeWindowDetaDphi','Dphi' ),
    lessThan = cms.bool( True ),
    thrRegularEB = cms.double( 0.1 ),
    thrRegularEE = cms.double( 0.1 ),
    thrOverPtEB = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrTimesPtEB = cms.double( -1.0 ),
    thrTimesPtEE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1Iso" ),
    L1NonIsoCand = cms.InputTag( "hltPixelMatchLargeWindowElectronsL1NonIso" )
)
process.hltPreEle10SWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoStartUpElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.08 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 ),
        maxElement = cms.uint32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.08 ),
      PhiMin2 = cms.double( -0.0040 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.04 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.11 ),
      rMinI = cms.double( -0.11 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.08 ),
      pPhiMin1 = cms.double( -0.04 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.08 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.05 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.05 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0040 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL1NonIsoStartUpElectronPixelSeeds = cms.EDProducer( "ElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    SeedConfiguration = cms.PSet( 
      searchInTIDTEC = cms.bool( True ),
      HighPtThreshold = cms.double( 35.0 ),
      r2MinF = cms.double( -0.08 ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" ),
        useOnDemandTracker = cms.untracked.int32( 0 ),
        maxElement = cms.uint32( 0 )
      ),
      DeltaPhi1Low = cms.double( 0.23 ),
      DeltaPhi1High = cms.double( 0.08 ),
      ePhiMin1 = cms.double( -0.08 ),
      PhiMin2 = cms.double( -0.0040 ),
      LowPtThreshold = cms.double( 3.0 ),
      RegionPSet = cms.PSet( 
        deltaPhiRegion = cms.double( 0.4 ),
        originHalfLength = cms.double( 15.0 ),
        useZInVertex = cms.bool( True ),
        deltaEtaRegion = cms.double( 0.1 ),
        ptMin = cms.double( 1.5 ),
        originRadius = cms.double( 0.2 ),
        VertexProducer = cms.InputTag( "dummyVertices" )
      ),
      maxHOverE = cms.double( 999999.0 ),
      dynamicPhiRoad = cms.bool( False ),
      ePhiMax1 = cms.double( 0.04 ),
      DeltaPhi2 = cms.double( 0.0040 ),
      SizeWindowENeg = cms.double( 0.675 ),
      nSigmasDeltaZ1 = cms.double( 5.0 ),
      rMaxI = cms.double( 0.11 ),
      rMinI = cms.double( -0.11 ),
      preFilteredSeeds = cms.bool( True ),
      r2MaxF = cms.double( 0.08 ),
      pPhiMin1 = cms.double( -0.04 ),
      initialSeeds = cms.InputTag( "noSeedsHere" ),
      pPhiMax1 = cms.double( 0.08 ),
      hbheModule = cms.string( "hbhereco" ),
      SCEtCut = cms.double( 3.0 ),
      z2MaxB = cms.double( 0.05 ),
      fromTrackerSeeds = cms.bool( True ),
      hcalRecHits = cms.InputTag( "hltHbhereco" ),
      z2MinB = cms.double( -0.05 ),
      hbheInstance = cms.string( "" ),
      PhiMax2 = cms.double( 0.0040 ),
      hOverEConeSize = cms.double( 0.0 ),
      hOverEHBMinE = cms.double( 999999.0 ),
      applyHOverECut = cms.bool( False ),
      hOverEHFMinE = cms.double( 999999.0 ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" )
    )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoStartUpElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoStartUpElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle15LWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle15SWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt15R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt15R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoStartUpElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoStartUpElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle15SC10LWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper = cms.EDFilter( "HLTEgammaTriggerFilterObjectWrapper",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    doIsolated = cms.bool( False )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle15SiStripL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoSiStripElectronPixelSeeds = cms.EDProducer( "SiStripElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1Isolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated" ),
    SeedConfiguration = cms.PSet( 
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
      tibOriginZCut = cms.double( 20.0 ),
      tidOriginZCut = cms.double( 20.0 ),
      tecOriginZCut = cms.double( 20.0 ),
      monoOriginZCut = cms.double( 20.0 ),
      tibDeltaPsiCut = cms.double( 0.1 ),
      tidDeltaPsiCut = cms.double( 0.1 ),
      tecDeltaPsiCut = cms.double( 0.1 ),
      monoDeltaPsiCut = cms.double( 0.1 ),
      tibPhiMissHit2Cut = cms.double( 0.0060 ),
      tidPhiMissHit2Cut = cms.double( 0.0060 ),
      tecPhiMissHit2Cut = cms.double( 0.0070 ),
      monoPhiMissHit2Cut = cms.double( 0.02 ),
      tibZMissHit2Cut = cms.double( 0.35 ),
      tidRMissHit2Cut = cms.double( 0.3 ),
      tecRMissHit2Cut = cms.double( 0.3 ),
      tidEtaUsage = cms.double( 1.2 ),
      tidMaxHits = cms.int32( 4 ),
      tecMaxHits = cms.int32( 2 ),
      monoMaxHits = cms.int32( 4 ),
      maxSeeds = cms.int32( 5 )
    )
)
process.hltL1NonIsoSiStripElectronPixelSeeds = cms.EDProducer( "SiStripElectronSeedProducer",
    barrelSuperClusters = cms.InputTag( "hltCorrectedHybridSuperClustersL1NonIsolated" ),
    endcapSuperClusters = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated" ),
    SeedConfiguration = cms.PSet( 
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
      tibOriginZCut = cms.double( 20.0 ),
      tidOriginZCut = cms.double( 20.0 ),
      tecOriginZCut = cms.double( 20.0 ),
      monoOriginZCut = cms.double( 20.0 ),
      tibDeltaPsiCut = cms.double( 0.1 ),
      tidDeltaPsiCut = cms.double( 0.1 ),
      tecDeltaPsiCut = cms.double( 0.1 ),
      monoDeltaPsiCut = cms.double( 0.1 ),
      tibPhiMissHit2Cut = cms.double( 0.0060 ),
      tidPhiMissHit2Cut = cms.double( 0.0060 ),
      tecPhiMissHit2Cut = cms.double( 0.0070 ),
      monoPhiMissHit2Cut = cms.double( 0.02 ),
      tibZMissHit2Cut = cms.double( 0.35 ),
      tidRMissHit2Cut = cms.double( 0.3 ),
      tecRMissHit2Cut = cms.double( 0.3 ),
      tidEtaUsage = cms.double( 1.2 ),
      tidMaxHits = cms.int32( 4 ),
      tecMaxHits = cms.int32( 2 ),
      monoMaxHits = cms.int32( 4 ),
      maxSeeds = cms.int32( 5 )
    )
)
process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoSiStripElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoSiStripElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle20LWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt20L1MatchFilterRegional" ),
    etcutEB = cms.double( 20.0 ),
    etcutEE = cms.double( 20.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt20EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt20R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronLWEt20HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoLargeWindowElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoLargeWindowElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreEle20SWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional" ),
    etcutEB = cms.double( 20.0 ),
    etcutEE = cms.double( 20.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt20R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt20R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoStartUpElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoStartUpElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreDoubleEle5SWL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional" ),
    etcutEB = cms.double( 5.0 ),
    etcutEE = cms.double( 5.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoubleElectronEt5R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1IsoStartUpElectronPixelSeeds" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "hltL1NonIsoStartUpElectronPixelSeeds" ),
    npixelmatchcut = cms.double( 1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton10L1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt10R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsolatedPhotonHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.3 )
)
process.hltL1NonIsolatedPhotonHcalIsol = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.3 )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt10R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton15CleanedL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedL1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton15HTIL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoSinglePhotonEt15HTIEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoSinglePhotonEt15HTIR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsoEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    ptMin = cms.double( 1.5 ),
    vertexZ = cms.double( 0.0 ),
    originRadius = cms.double( 0.02 ),
    originHalfLength = cms.double( 15.0 ),
    deltaEtaRegion = cms.double( 0.3 ),
    deltaPhiRegion = cms.double( 0.3 ),
    candTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    UseZInVertex = cms.bool( False ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" ),
      maxElement = cms.uint32( 0 )
    )
)
process.hltL1IsoEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1IsoEgammaRegionalPixelSeedGenerator" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltL1IsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL1IsoEgammaRegionalCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltL1NonIsoEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    ptMin = cms.double( 1.5 ),
    vertexZ = cms.double( 0.0 ),
    originRadius = cms.double( 0.02 ),
    originHalfLength = cms.double( 15.0 ),
    deltaEtaRegion = cms.double( 0.3 ),
    deltaPhiRegion = cms.double( 0.3 ),
    candTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    UseZInVertex = cms.bool( False ),
    BSProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" ),
      maxElement = cms.uint32( 0 )
    )
)
process.hltL1NonIsoEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1NonIsoEgammaRegionalPixelSeedGenerator" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltL1NonIsoEgammaRegionalCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltL1IsoPhotonHollowTrackIsol = cms.EDProducer( "EgammaHLTPhotonTrackIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    trackProducer = cms.InputTag( "hltL1IsoEgammaRegionalCTFFinalFitWithMaterial" ),
    countTracks = cms.bool( False ),
    egTrkIsoPtMin = cms.double( 1.5 ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    egTrkIsoZSpan = cms.double( 999999.0 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSize = cms.double( 0.06 )
)
process.hltL1NonIsoPhotonHollowTrackIsol = cms.EDProducer( "EgammaHLTPhotonTrackIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    trackProducer = cms.InputTag( "hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial" ),
    countTracks = cms.bool( False ),
    egTrkIsoPtMin = cms.double( 1.5 ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    egTrkIsoZSpan = cms.double( 999999.0 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSize = cms.double( 0.06 )
)
process.hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsoPhotonHollowTrackIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoPhotonHollowTrackIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    thrRegularEB = cms.double( 4.0 ),
    thrRegularEE = cms.double( 4.0 ),
    thrOverEEB = cms.double( 0.05 ),
    thrOverEEE = cms.double( 0.05 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton15LEIL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoSinglePhotonEt15LEIEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoSinglePhotonEt15LEIR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1IsolatedPhotonEcalIsol = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
process.hltL1NonIsolatedPhotonEcalIsol = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
process.hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 3.0 ),
    thrRegularEE = cms.double( 3.0 ),
    thrOverEEB = cms.double( 0.1 ),
    thrOverEEE = cms.double( 0.1 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton20CleanedL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedL1MatchFilterRegional" ),
    etcutEB = cms.double( 20.0 ),
    etcutEE = cms.double( 20.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.98 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPrePhoton30L1R8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG5" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional" ),
    etcutEB = cms.double( 15.0 ),
    etcutEE = cms.double( 15.0 ),
    ncandcut = cms.int32( 1 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29 = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter" ),
    etcutEB = cms.double( 30.0 ),
    etcutEE = cms.double( 30.0 ),
    ncandcut = cms.int32( 1 ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1sL1DoubleEG2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleEG2" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoublePhoton4eeResL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoDoublePhotonEt4eeResL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG2" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoDoublePhotonEt4eeResEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4eeResL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt4eeResR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4eeResEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1IsoHLTClusterShapeLowPt = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    isIeta = cms.bool( True )
)
process.hltL1NonIsoHLTClusterShapeLowPt = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHitLowPt','EcalRecHitsEE' ),
    isIeta = cms.bool( True )
)
process.hltL1NonIsoDoublePhotonEt4eeResClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt4eeResR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1IsolatedPhotonEcalIsolLowPt = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
process.hltL1NonIsolatedPhotonEcalIsolLowPt = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHitLowPt" ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    etMinBarrel = cms.double( -9999.0 ),
    eMinBarrel = cms.double( 0.08 ),
    etMinEndcap = cms.double( -9999.0 ),
    eMinEndcap = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 0.045 ),
    intRadiusEndcap = cms.double( 0.07 ),
    extRadius = cms.double( 0.4 ),
    jurassicWidth = cms.double( 0.02 ),
    useIsolEt = cms.bool( True ),
    tryBoth = cms.bool( True ),
    subtract = cms.bool( False )
)
process.hltL1NonIsoDoublePhotonEt4eeResEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4eeResClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1IsolatedElectronHcalIsolLowPt = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
process.hltL1NonIsolatedElectronHcalIsolLowPt = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    hbRecHitProducer = cms.InputTag( "hltHbhereco" ),
    hfRecHitProducer = cms.InputTag( "hltHfreco" ),
    egHcalIsoPtMin = cms.double( 0.0 ),
    egHcalIsoConeSize = cms.double( 0.15 )
)
process.hltL1NonIsoDoublePhotonEt4eeResHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4eeResEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4eeResPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4eeResHcalIsolFilter" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    lowerMassCut = cms.double( 2.0 ),
    upperMassCut = cms.double( 999999.9 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltPreDoublePhoton4JpsiL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoDoublePhotonEt4JpsiL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG2" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoDoublePhotonEt4JpsiEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4JpsiL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt4JpsiR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4JpsiEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4JpsiClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt4JpsiR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4JpsiEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4JpsiClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4JpsiHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4JpsiEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4JpsiPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4JpsiHcalIsolFilter" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    lowerMassCut = cms.double( 2.0 ),
    upperMassCut = cms.double( 4.6 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltPreDoublePhoton4UpsL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoDoublePhotonEt4UpsL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG2" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoDoublePhotonEt4UpsEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4UpsL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt4UpsR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4UpsEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4UpsClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt4UpsR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShapeLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShapeLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4UpsEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4UpsClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4UpsHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4UpsEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsolLowPt" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsolLowPt" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1NonIsoDoublePhotonEt4UpsPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt4UpsHcalIsolFilter" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    lowerMassCut = cms.double( 7.0 ),
    upperMassCut = cms.double( 12.0 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidateLowPt" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidateLowPt" )
)
process.hltL1sL1SingleEG8orL1DoubleEG5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG8 OR L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoublePhoton5JpsiL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8orL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoDoublePhotonEt5JpsiEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5JpsiR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt5JpsiR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    lowerMassCut = cms.double( 2.0 ),
    upperMassCut = cms.double( 4.6 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreDoublePhoton5UpsL1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG8orL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoDoublePhotonEt5UpsEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional" ),
    etcutEB = cms.double( 4.0 ),
    etcutEE = cms.double( 4.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5UpsR9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsEtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt5UpsR9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsoHLTClusterShape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoHLTClusterShape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 0.016 ),
    thrRegularEE = cms.double( 0.042 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonEcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonEcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedElectronHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedElectronHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoDoublePhotonEt5UpsPMMassFilter = cms.EDFilter( "HLTPMMassFilter",
    candTag = cms.InputTag( "hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    lowerMassCut = cms.double( 8.0 ),
    upperMassCut = cms.double( 11.0 ),
    nZcandcut = cms.int32( 1 ),
    reqOppCharge = cms.untracked.bool( True ),
    isElectron1 = cms.untracked.bool( False ),
    isElectron2 = cms.untracked.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreDoublePhoton5_L1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt5L1MatchFilterRegional" ),
    etcutEB = cms.double( 5.0 ),
    etcutEE = cms.double( 5.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt5EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt5HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt5R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltPreDoublePhoton10L1R = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    candIsolatedTag = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candNonIsolatedTag = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG5" ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    region_eta_size = cms.double( 0.522 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    region_phi_size = cms.double( 1.044 ),
    barrel_end = cms.double( 1.4791 ),
    endcap_end = cms.double( 2.65 )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    inputTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional" ),
    etcutEB = cms.double( 10.0 ),
    etcutEE = cms.double( 10.0 ),
    ncandcut = cms.int32( 2 ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt10R9ShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter" ),
    isoTag = cms.InputTag( "hltL1IsoR9shape" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsoR9shape" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    candTag = cms.InputTag( "hltL1NonIsoHLTNonIsoDoublePhotonEt10R9ShapeFilter" ),
    isoTag = cms.InputTag( "hltL1IsolatedPhotonHcalIsol" ),
    nonIsoTag = cms.InputTag( "hltL1NonIsolatedPhotonHcalIsol" ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    thrRegularEB = cms.double( 999999.9 ),
    thrRegularEE = cms.double( 999999.9 ),
    thrOverEEB = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    thrOverE2EB = cms.double( -1.0 ),
    thrOverE2EE = cms.double( -1.0 ),
    ncandcut = cms.int32( 2 ),
    doIsolated = cms.bool( False ),
    SaveTag = cms.untracked.bool( True ),
    L1IsoCand = cms.InputTag( "hltL1IsoRecoEcalCandidate" ),
    L1NonIsoCand = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" )
)
process.hltL1sSingleLooseIsoTau20 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleTauJet20U OR L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreSingleLooseIsoTau20 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltCaloTowersTau1Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 0 )
)
process.hltIconeTau1Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau1Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersTau2Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 1 )
)
process.hltIconeTau2Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau2Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersTau3Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 2 )
)
process.hltIconeTau3Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau3Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersTau4Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Tau' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 3 )
)
process.hltIconeTau4Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersTau4Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersCentral1Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 0 )
)
process.hltIconeCentral1Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral1Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersCentral2Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 1 )
)
process.hltIconeCentral2Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral2Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersCentral3Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 2 )
)
process.hltIconeCentral3Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral3Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltCaloTowersCentral4Regional = cms.EDProducer( "CaloTowerCreatorForTauHLT",
    towers = cms.InputTag( "hltTowerMakerForJets" ),
    UseTowersInCone = cms.double( 0.8 ),
    TauTrigger = cms.InputTag( 'hltL1extraParticles','Central' ),
    minimumEt = cms.double( 0.5 ),
    minimumE = cms.double( 0.8 ),
    TauId = cms.int32( 3 )
)
process.hltIconeCentral4Regional = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.2 ),
    src = cms.InputTag( "hltCaloTowersCentral4Regional" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltL2TauJets = cms.EDProducer( "L2TauJetsMerger",
    EtMin = cms.double( 15.0 ),
    JetSrc = cms.VInputTag( 'hltIconeTau1Regional','hltIconeTau2Regional','hltIconeTau3Regional','hltIconeTau4Regional','hltIconeCentral1Regional','hltIconeCentral2Regional','hltIconeCentral3Regional','hltIconeCentral4Regional' )
)
process.hltFilterL2EtCutSingleLooseIsoTau20 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL2TauNarrowConeIsolationProducer = cms.EDProducer( "L2TauNarrowConeIsolationProducer",
    L2TauJetCollection = cms.InputTag( "hltL2TauJets" ),
    EBRecHits = cms.InputTag( 'hltEcalRegionalJetsRecHit','EcalRecHitsEB' ),
    EERecHits = cms.InputTag( 'hltEcalRegionalJetsRecHit','EcalRecHitsEE' ),
    CaloTowers = cms.InputTag( "hltTowerMakerForJets" ),
    associationRadius = cms.double( 0.5 ),
    crystalThresholdEE = cms.double( 0.45 ),
    crystalThresholdEB = cms.double( 0.15 ),
    towerThreshold = cms.double( 1.0 ),
    ECALIsolation = cms.PSet( 
      innerCone = cms.double( 0.15 ),
      runAlgorithm = cms.bool( True ),
      outerCone = cms.double( 0.5 )
    ),
    ECALClustering = cms.PSet( 
      runAlgorithm = cms.bool( True ),
      clusterRadius = cms.double( 0.08 )
    ),
    TowerIsolation = cms.PSet( 
      innerCone = cms.double( 0.2 ),
      runAlgorithm = cms.bool( True ),
      outerCone = cms.double( 0.5 )
    )
)
process.hltL2TauRelaxingIsolationSelector = cms.EDProducer( "L2TauRelaxingIsolationSelector",
    L2InfoAssociation = cms.InputTag( "hltL2TauNarrowConeIsolationProducer" ),
    MinJetEt = cms.double( 15.0 ),
    SeedTowerEt = cms.double( -10.0 ),
    EcalIsolationEt = cms.vdouble( 5.0, 0.025, 7.5E-4 ),
    TowerIsolationEt = cms.vdouble( 1000.0, 0.0, 0.0 ),
    NumberOfClusters = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterPhiRMS = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterEtaRMS = cms.vdouble( 1000.0, 0.0, 0.0 ),
    ClusterDRRMS = cms.vdouble( 1000.0, 0.0, 0.0 )
)
process.hltL1HLTSingleLooseIsoTau20JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    L1TauTrigger = cms.InputTag( "hltL1sSingleLooseIsoTau20" ),
    EtMin = cms.double( 20.0 )
)
process.hltFilterL2EcalIsolationSingleLooseIsoTau20 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTSingleLooseIsoTau20JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sSingleLooseIsoTau20Trk5 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleTauJet20U OR L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreSingleLooseIsoTau20Trk5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltFilterL2EtCutSingleLooseIsoTau20Trk5 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltFilterL2EcalIsolationSingleLooseIsoTau20Trk5 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltPixelVertices = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltPixelTracks" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    Method2 = cms.bool( True )
)
process.hltL25TauPixelSeeds = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "hltSiStripClusters" ),
      doClusterCheck = cms.bool( False ),
      PixelClusterCollectionLabel = cms.InputTag( "hltSiPixelClusters" ),
      MaxNumberOfPixelClusters = cms.uint32( 10000 )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "TauRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        deltaPhiRegion = cms.double( 0.2 ),
        originHalfLength = cms.double( 0.2 ),
        originRadius = cms.double( 0.2 ),
        deltaEtaRegion = cms.double( 0.2 ),
        vertexSrc = cms.InputTag( "hltPixelVertices" ),
        JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
        originZPos = cms.double( 0.0 ),
        ptMin = cms.double( 4.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" ),
      maxElement = cms.uint32( 0 )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
process.hltL25TauCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL25TauPixelSeeds" ),
    TrajectoryBuilder = cms.string( "trajBuilderL3" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltL25TauCtfWithMaterialTracks = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "FittingSmootherRK" ),
    Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
    src = cms.InputTag( "hltL25TauCkfTrackCandidates" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltL25TauJetTracksAssociator = cms.EDProducer( "JetTracksAssociatorAtVertex",
    jets = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    tracks = cms.InputTag( "hltL25TauCtfWithMaterialTracks" ),
    coneSize = cms.double( 0.5 )
)
process.hltL25TauConeIsolation = cms.EDProducer( "ConeIsolation",
    JetTrackSrc = cms.InputTag( "hltL25TauJetTracksAssociator" ),
    vertexSrc = cms.InputTag( "hltPixelVertices" ),
    useVertex = cms.bool( True ),
    useBeamSpot = cms.bool( True ),
    BeamSpotProducer = cms.InputTag( "hltOfflineBeamSpot" ),
    MinimumNumberOfPixelHits = cms.int32( 2 ),
    MinimumNumberOfHits = cms.int32( 5 ),
    MaximumTransverseImpactParameter = cms.double( 300.0 ),
    MinimumTransverseMomentum = cms.double( 1.0 ),
    MaximumChiSquared = cms.double( 100.0 ),
    DeltaZetTrackVertex = cms.double( 0.2 ),
    MatchingCone = cms.double( 0.2 ),
    SignalCone = cms.double( 0.15 ),
    IsolationCone = cms.double( 0.5 ),
    MinimumTransverseMomentumInIsolationRing = cms.double( 1.0 ),
    MinimumTransverseMomentumLeadingTrack = cms.double( 5.0 ),
    MaximumNumberOfTracksIsolationRing = cms.int32( 0 ),
    UseFixedSizeCone = cms.bool( True ),
    VariableConeParameter = cms.double( 3.5 ),
    VariableMaxCone = cms.double( 0.17 ),
    VariableMinCone = cms.double( 0.05 )
)
process.hltL25TauLeadingTrackPtCutSelector = cms.EDProducer( "IsolatedTauJetsSelector",
    MinimumTransverseMomentumLeadingTrack = cms.double( 5.0 ),
    UseIsolationDiscriminator = cms.bool( False ),
    UseInHLTOpen = cms.bool( False ),
    JetSrc = cms.VInputTag( 'hltL25TauConeIsolation' )
)
process.hltL1HLTSingleLooseIsoTau20Trk5JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( "hltL25TauLeadingTrackPtCutSelector" ),
    L1TauTrigger = cms.InputTag( "hltL1sSingleLooseIsoTau20Trk5" ),
    EtMin = cms.double( 20.0 )
)
process.hltFilterL25LeadingTrackPtCutSingleLooseIsoTau20Trk5 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTSingleLooseIsoTau20Trk5JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 20.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sSingleLooseIsoTau25 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleTauJet20U OR L1_SingleJet50U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreSingleLooseIsoTau25 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltFilterL2EtCutSingleLooseIsoTau25 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 25.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1HLTSingleLooseIsoTau25JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    L1TauTrigger = cms.InputTag( "hltL1sSingleLooseIsoTau25" ),
    EtMin = cms.double( 25.0 )
)
process.hltFilterL2EcalIsolationSingleLooseIsoTau25 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTSingleLooseIsoTau25JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 25.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sDoubleLooseIsoTau15 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleTauJet14U OR L1_DoubleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoubleLooseIsoTau15 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltFilterL2EtCutDoubleLooseIsoTau15 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL2TauJets" ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 2 )
)
process.hltL1HLTDoubleLooseIsoTau15JetsMatch = cms.EDProducer( "L1HLTJetsMatching",
    JetSrc = cms.InputTag( 'hltL2TauRelaxingIsolationSelector','Isolated' ),
    L1TauTrigger = cms.InputTag( "hltL1sDoubleLooseIsoTau15" ),
    EtMin = cms.double( 15.0 )
)
process.hltFilterL2EcalIsolationDoubleLooseIsoTau15 = cms.EDFilter( "HLT1Tau",
    inputTag = cms.InputTag( "hltL1HLTDoubleLooseIsoTau15JetsMatch" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 15.0 ),
    MaxEta = cms.double( 5.0 ),
    MinN = cms.int32( 2 )
)
process.hltL1sBTagIPJet50U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet30U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreBTagIPJet50U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltBJet50U = cms.EDFilter( "HLT1CaloBJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 50.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
process.hltSelector4JetsU = cms.EDFilter( "LargestEtCaloJetSelector",
    src = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    filter = cms.bool( False ),
    maxNumber = cms.uint32( 4 )
)
process.hltBLifetimeL25JetsStartupU = cms.EDFilter( "EtMinCaloJetSelector",
    src = cms.InputTag( "hltSelector4JetsU" ),
    filter = cms.bool( False ),
    etMin = cms.double( 15.0 )
)
process.hltBLifetimeL25AssociatorStartupU = cms.EDProducer( "JetTracksAssociatorAtVertex",
    jets = cms.InputTag( "hltBLifetimeL25JetsStartupU" ),
    tracks = cms.InputTag( "hltPixelTracks" ),
    coneSize = cms.double( 0.5 )
)
process.hltBLifetimeL25TagInfosStartupU = cms.EDProducer( "TrackIPProducer",
    jetTracks = cms.InputTag( "hltBLifetimeL25AssociatorStartupU" ),
    primaryVertex = cms.InputTag( "hltPixelVertices" ),
    computeProbabilities = cms.bool( False ),
    computeGhostTrack = cms.bool( False ),
    ghostTrackPriorDeltaR = cms.double( 0.03 ),
    minimumNumberOfPixelHits = cms.int32( 2 ),
    minimumNumberOfHits = cms.int32( 3 ),
    maximumTransverseImpactParameter = cms.double( 0.2 ),
    minimumTransverseMomentum = cms.double( 1.0 ),
    maximumChiSquared = cms.double( 5.0 ),
    maximumLongitudinalImpactParameter = cms.double( 17.0 ),
    jetDirectionUsingTracks = cms.bool( False ),
    jetDirectionUsingGhostTrack = cms.bool( False ),
    useTrackQuality = cms.bool( False )
)
process.hltBLifetimeL25BJetTagsStartupU = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "trackCounting3D2nd" ),
    tagInfos = cms.VInputTag( 'hltBLifetimeL25TagInfosStartupU' )
)
process.hltBLifetimeL25FilterStartupU = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBLifetimeL25BJetTagsStartupU" ),
    MinTag = cms.double( 2.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( False )
)
process.hltBLifetimeL3JetsStartupU = cms.EDProducer( "GetJetsFromHLTobject",
    jets = cms.InputTag( "hltBLifetimeL25FilterStartupU" )
)
process.hltBLifetimeRegionalPixelSeedGeneratorStartupU = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "hltSiStripClusters" ),
      doClusterCheck = cms.bool( False ),
      PixelClusterCollectionLabel = cms.InputTag( "hltSiPixelClusters" ),
      MaxNumberOfPixelClusters = cms.uint32( 10000 )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "TauRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        deltaPhiRegion = cms.double( 0.5 ),
        originHalfLength = cms.double( 0.2 ),
        originRadius = cms.double( 0.2 ),
        deltaEtaRegion = cms.double( 0.5 ),
        ptMin = cms.double( 1.0 ),
        JetSrc = cms.InputTag( "hltBLifetimeL3JetsStartupU" ),
        originZPos = cms.double( 0.0 ),
        vertexSrc = cms.InputTag( "hltPixelVertices" )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "PixelLayerPairs" ),
      maxElement = cms.uint32( 0 )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
process.hltBLifetimeRegionalCkfTrackCandidatesStartupU = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltBLifetimeRegionalPixelSeedGeneratorStartupU" ),
    TrajectoryBuilder = cms.string( "bJetRegionalTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltBLifetimeRegionalCtfWithMaterialTracksStartupU = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( True ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "ctfWithMaterialTracks" ),
    Fitter = cms.string( "FittingSmootherRK" ),
    Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
    src = cms.InputTag( "hltBLifetimeRegionalCkfTrackCandidatesStartupU" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltBLifetimeL3AssociatorStartupU = cms.EDProducer( "JetTracksAssociatorAtVertex",
    jets = cms.InputTag( "hltBLifetimeL3JetsStartupU" ),
    tracks = cms.InputTag( "hltBLifetimeRegionalCtfWithMaterialTracksStartupU" ),
    coneSize = cms.double( 0.5 )
)
process.hltBLifetimeL3TagInfosStartupU = cms.EDProducer( "TrackIPProducer",
    jetTracks = cms.InputTag( "hltBLifetimeL3AssociatorStartupU" ),
    primaryVertex = cms.InputTag( "hltPixelVertices" ),
    computeProbabilities = cms.bool( False ),
    computeGhostTrack = cms.bool( False ),
    ghostTrackPriorDeltaR = cms.double( 0.03 ),
    minimumNumberOfPixelHits = cms.int32( 2 ),
    minimumNumberOfHits = cms.int32( 8 ),
    maximumTransverseImpactParameter = cms.double( 0.2 ),
    minimumTransverseMomentum = cms.double( 1.0 ),
    maximumChiSquared = cms.double( 20.0 ),
    maximumLongitudinalImpactParameter = cms.double( 17.0 ),
    jetDirectionUsingTracks = cms.bool( False ),
    jetDirectionUsingGhostTrack = cms.bool( False ),
    useTrackQuality = cms.bool( False )
)
process.hltBLifetimeL3BJetTagsStartupU = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "trackCounting3D2nd" ),
    tagInfos = cms.VInputTag( 'hltBLifetimeL3TagInfosStartupU' )
)
process.hltBLifetimeL3FilterStartupU = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBLifetimeL3BJetTagsStartupU" ),
    MinTag = cms.double( 3.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( True )
)
process.hltL1sBTagMuJet10U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_Mu3_Jet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreBTagMuJet10U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltBJet10U = cms.EDFilter( "HLT1CaloBJet",
    inputTag = cms.InputTag( "hltMCJetCorJetIcone5HF07" ),
    saveTag = cms.untracked.bool( True ),
    MinPt = cms.double( 10.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
process.hltBSoftMuonL25JetsU = cms.EDFilter( "EtMinCaloJetSelector",
    src = cms.InputTag( "hltSelector4JetsU" ),
    filter = cms.bool( False ),
    etMin = cms.double( 10.0 )
)
process.hltBSoftMuonL25TagInfosU = cms.EDProducer( "SoftLepton",
    jets = cms.InputTag( "hltBSoftMuonL25JetsU" ),
    primaryVertex = cms.InputTag( "nominal" ),
    leptons = cms.InputTag( "hltL2Muons" ),
    leptonCands = cms.InputTag( "" ),
    leptonId = cms.InputTag( "" ),
    refineJetAxis = cms.uint32( 0 ),
    leptonDeltaRCut = cms.double( 0.4 ),
    leptonChi2Cut = cms.double( 0.0 ),
    muonSelection = cms.uint32( 0 )
)
process.hltBSoftMuonL25BJetTagsUByDR = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByDistance" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL25TagInfosU' )
)
process.hltBSoftMuonL25FilterUByDR = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBSoftMuonL25BJetTagsUByDR" ),
    MinTag = cms.double( 0.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( False )
)
process.hltBSoftMuonL3TagInfosU = cms.EDProducer( "SoftLepton",
    jets = cms.InputTag( "hltBSoftMuonL25JetsU" ),
    primaryVertex = cms.InputTag( "nominal" ),
    leptons = cms.InputTag( "hltL3Muons" ),
    leptonCands = cms.InputTag( "" ),
    leptonId = cms.InputTag( "" ),
    refineJetAxis = cms.uint32( 0 ),
    leptonDeltaRCut = cms.double( 0.4 ),
    leptonChi2Cut = cms.double( 0.0 ),
    muonSelection = cms.uint32( 0 )
)
process.hltBSoftMuonL3BJetTagsUByPt = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByPt" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL3TagInfosU' )
)
process.hltBSoftMuonL3BJetTagsUByDR = cms.EDProducer( "JetTagProducer",
    jetTagComputer = cms.string( "softLeptonByDistance" ),
    tagInfos = cms.VInputTag( 'hltBSoftMuonL3TagInfosU' )
)
process.hltBSoftMuonL3FilterUByDR = cms.EDFilter( "HLTJetTag",
    JetTag = cms.InputTag( "hltBSoftMuonL3BJetTagsUByDR" ),
    MinTag = cms.double( 0.5 ),
    MaxTag = cms.double( 99999.0 ),
    MinJets = cms.int32( 1 ),
    SaveTag = cms.bool( True )
)
process.hltL1sStoppedHSCP8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet10U_NotBptxC" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" ),
    saveTags = cms.untracked.bool( False )
)
process.hltPreStoppedHSCP8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltStoppedHSCPHpdFilter = cms.EDFilter( "HLTHPDFilter",
    inputTag = cms.InputTag( "hltHbhereco" ),
    energy = cms.double( -99.0 ),
    hpdSpikeEnergy = cms.double( 10.0 ),
    hpdSpikeIsolationEnergy = cms.double( 1.0 ),
    rbxSpikeEnergy = cms.double( 50.0 ),
    rbxSpikeUnbalance = cms.double( 0.2 )
)
process.hltStoppedHSCPTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
    EBThreshold = cms.double( 0.07 ),
    EEThreshold = cms.double( 0.3 ),
    UseEtEBTreshold = cms.bool( False ),
    UseEtEETreshold = cms.bool( False ),
    UseSymEBTreshold = cms.bool( False ),
    UseSymEETreshold = cms.bool( False ),
    HcalThreshold = cms.double( -1000.0 ),
    HBThreshold = cms.double( 0.7 ),
    HESThreshold = cms.double( 0.8 ),
    HEDThreshold = cms.double( 0.8 ),
    HOThreshold0 = cms.double( 3.5 ),
    HOThresholdPlus1 = cms.double( 3.5 ),
    HOThresholdMinus1 = cms.double( 3.5 ),
    HOThresholdPlus2 = cms.double( 3.5 ),
    HOThresholdMinus2 = cms.double( 3.5 ),
    HF1Threshold = cms.double( 0.5 ),
    HF2Threshold = cms.double( 0.85 ),
    EBWeight = cms.double( 1.0 ),
    EEWeight = cms.double( 1.0 ),
    HBWeight = cms.double( 1.0 ),
    HESWeight = cms.double( 1.0 ),
    HEDWeight = cms.double( 1.0 ),
    HOWeight = cms.double( 1.0E-99 ),
    HF1Weight = cms.double( 1.0 ),
    HF2Weight = cms.double( 1.0 ),
    EcutTower = cms.double( -1000.0 ),
    EBSumThreshold = cms.double( 0.2 ),
    EESumThreshold = cms.double( 0.45 ),
    UseHO = cms.bool( False ),
    MomConstrMethod = cms.int32( 1 ),
    MomHBDepth = cms.double( 0.2 ),
    MomHEDepth = cms.double( 0.4 ),
    MomEBDepth = cms.double( 0.3 ),
    MomEEDepth = cms.double( 0.0 ),
    hbheInput = cms.InputTag( "hltHbhereco" ),
    hoInput = cms.InputTag( "" ),
    hfInput = cms.InputTag( "" ),
    AllowMissingInputs = cms.bool( True ),
    HcalAcceptSeverityLevel = cms.uint32( 999 ),
    EcalAcceptSeverityLevel = cms.uint32( 1 ),
    UseHcalRecoveredHits = cms.bool( True ),
    UseEcalRecoveredHits = cms.bool( True ),
    EBGrid = cms.vdouble(  ),
    EBWeights = cms.vdouble(  ),
    EEGrid = cms.vdouble(  ),
    EEWeights = cms.vdouble(  ),
    HBGrid = cms.vdouble(  ),
    HBWeights = cms.vdouble(  ),
    HESGrid = cms.vdouble(  ),
    HESWeights = cms.vdouble(  ),
    HEDGrid = cms.vdouble(  ),
    HEDWeights = cms.vdouble(  ),
    HOGrid = cms.vdouble(  ),
    HOWeights = cms.vdouble(  ),
    HF1Grid = cms.vdouble(  ),
    HF1Weights = cms.vdouble(  ),
    HF2Grid = cms.vdouble(  ),
    HF2Weights = cms.vdouble(  ),
    ecalInputs = cms.VInputTag(  )
)
process.hltStoppedHSCPIterativeCone5CaloJets = cms.EDProducer( "FastjetJetProducer",
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    DxyTrVtxMax = cms.double( 0.0 ),
    jetAlgorithm = cms.string( "IterativeCone" ),
    rParam = cms.double( 0.5 ),
    src = cms.InputTag( "hltStoppedHSCPTowerMakerForAll" ),
    srcPVs = cms.InputTag( "offlinePrimaryVertices" ),
    jetType = cms.string( "CaloJet" ),
    jetPtMin = cms.double( 1.0 ),
    inputEtMin = cms.double( 0.3 ),
    inputEMin = cms.double( 0.0 ),
    doPVCorrection = cms.bool( False ),
    doPUOffsetCorr = cms.bool( False ),
    nSigmaPU = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    Active_Area_Repeats = cms.int32( 5 ),
    GhostArea = cms.double( 0.01 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doAreaFastjet = cms.bool( False ),
    doRhoFastjet = cms.bool( False )
)
process.hltStoppedHSCP1CaloJetEnergy = cms.EDFilter( "HLT1CaloJetEnergy",
    inputTag = cms.InputTag( "hltStoppedHSCPIterativeCone5CaloJets" ),
    saveTag = cms.untracked.bool( True ),
    MinE = cms.double( 20.0 ),
    MaxEta = cms.double( 3.0 ),
    MinN = cms.int32( 1 )
)
process.hltL1sL1Mu14L1SingleEG10 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_SingleEG10" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Mu14L1SingleEG10 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1Mu14L1SingleJet6U = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_SingleJet6U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Mu14L1SingleJet6U = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1Mu14L1ETM30 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu14 AND L1_ETM30" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1Mu14L1ETM30 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreZeroBias = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreZeroBiasPixelSingleTrack = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPixelTracksForMinBias = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducer" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originHalfLength = cms.double( 30.0 ),
        originRadius = cms.double( 0.5 ),
        originYPos = cms.double( 0.0 ),
        ptMin = cms.double( 0.2 ),
        originXPos = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      SeedingLayers = cms.string( "PixelLayerTriplets" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.1 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
process.hltPixelCandsForMinBias = cms.EDProducer( "ConcreteChargedCandidateProducer",
    src = cms.InputTag( "hltPixelTracksForMinBias" ),
    particleType = cms.string( "pi+" )
)
process.hltMinBiasPixelFilter1 = cms.EDFilter( "HLTPixlMBFilt",
    pixlTag = cms.InputTag( "hltPixelCandsForMinBias" ),
    MinPt = cms.double( 0.0 ),
    MinTrks = cms.uint32( 1 ),
    MinSep = cms.double( 1.0 )
)
process.hltL1sL1BscMinBiasORBptxPlusORMinus = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BscMinBiasOR_BptxPlusORMinus" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreMinBiasPixelSingleTrack = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreMinBiasPixelDoubleTrack = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltMinBiasPixelFilter2 = cms.EDFilter( "HLTPixlMBFilt",
    pixlTag = cms.InputTag( "hltPixelCandsForMinBias" ),
    MinPt = cms.double( 0.0 ),
    MinTrks = cms.uint32( 2 ),
    MinSep = cms.double( 1.0 )
)
process.hltPreMinBiasPixelDoubleIsoTrack5 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPixelMBForAlignment = cms.EDFilter( "HLTPixlMBForAlignmentFilter",
    pixlTag = cms.InputTag( "hltPixelCandsForMinBias" ),
    MinPt = cms.double( 5.0 ),
    MinTrks = cms.uint32( 2 ),
    MinSep = cms.double( 1.0 ),
    MinIsol = cms.double( 0.05 )
)
process.hltPreMultiVertex6 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPixelVerticesForMultiVertex = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.3 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 0.5 ),
    TrackCollection = cms.InputTag( "hltPixelTracks" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    Method2 = cms.bool( True )
)
process.hltVertexFilter6 = cms.EDFilter( "HLTVertexFilter",
    inputTag = cms.InputTag( "hltPixelVerticesForMultiVertex" ),
    minNDoF = cms.double( 0.0 ),
    maxChi2 = cms.double( 99999.0 ),
    maxD0 = cms.double( 1.0 ),
    maxZ = cms.double( 15.0 ),
    minVertices = cms.uint32( 6 )
)
process.hltL1sETT60 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_ETT60" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreMultiVertex8 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltVertexFilter8 = cms.EDFilter( "HLTVertexFilter",
    inputTag = cms.InputTag( "hltPixelVerticesForMultiVertex" ),
    minNDoF = cms.double( 0.0 ),
    maxChi2 = cms.double( 99999.0 ),
    maxD0 = cms.double( 1.0 ),
    maxZ = cms.double( 15.0 ),
    minVertices = cms.uint32( 8 )
)
process.hltL1sCSCBeamHalo = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreCSCBeamHalo = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltOverlapsHLTCSCBeamHaloOverlapRing1 = cms.EDFilter( "HLTCSCOverlapFilter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 1000.0 ),
    yWindow = cms.double( 1000.0 ),
    ring1 = cms.bool( True ),
    ring2 = cms.bool( False ),
    fillHists = cms.bool( False )
)
process.hltL1sCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltOverlapsHLTCSCBeamHaloOverlapRing2 = cms.EDFilter( "HLTCSCOverlapFilter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 1000.0 ),
    yWindow = cms.double( 1000.0 ),
    ring1 = cms.bool( False ),
    ring2 = cms.bool( True ),
    fillHists = cms.bool( False )
)
process.hltL1sCSCBeamHaloRing2or3 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuBeamHalo" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreCSCBeamHaloRing2or3 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltFilter23HLTCSCBeamHaloRing2or3 = cms.EDFilter( "HLTCSCRing2or3Filter",
    input = cms.InputTag( "hltCsc2DRecHits" ),
    minHits = cms.uint32( 4 ),
    xWindow = cms.double( 2.0 ),
    yWindow = cms.double( 2.0 )
)
process.hltPreL1BscMinBiasORBptxPlusORMinus = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1BptxXORBscMinBiasOR = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BptxXOR_BscMinBiasOR" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1BptxXORBscMinBiasOR = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1TechBSCminBias = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "4 AND (32 OR 33 OR 40 OR 41)" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1TechBSCminBias_BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1TechBSChalo = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "4 AND (36 OR 37 OR 38 OR 39)" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1TechBSChalo = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreL1TechBSChalo_forPhysicsBackground = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sHighMultiplicityBSC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "35" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreHighMultiplicityBSC = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1TechRPCTTURBst1collisions = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "31" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1TechRPCTTURBst1collisions = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1HFtech = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "4 AND (8 OR 9 OR 10)" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1HFTech = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sTrackerCosmics = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "25" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltTrackerCosmicsPattern = cms.EDFilter( "HLTLevel1Pattern",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    triggerBit = cms.string( "L1Tech_RPC_TTU_pointing_Cosmics.v0" ),
    daqPartitions = cms.uint32( 1 ),
    ignoreL1Mask = cms.bool( False ),
    invert = cms.bool( False ),
    throw = cms.bool( True ),
    bunchCrossings = cms.vint32( -2, -1, 0, 1, 2 ),
    triggerPattern = cms.vint32( 1, 1, 1, 0, 0 )
)
process.hltPreTrackerCosmics = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sRPCBarrelCosmics = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "24" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreRPCBarrelCosmics = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sIsoTrack8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreIsoTrackHE8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltHITPixelTracksHB = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducerFromBeamSpot" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originRadius = cms.double( 0.0015 ),
        nSigmaZ = cms.double( 3.0 ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        ptMin = cms.double( 0.7 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      ),
      SeedingLayers = cms.string( "PixelLayerTripletsHITHB" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByConformalMappingAndLine" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
      fixImpactParameter = cms.double( 0.0 )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.7 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
process.hltHITPixelTracksHE = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducerFromBeamSpot" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originRadius = cms.double( 0.0015 ),
        nSigmaZ = cms.double( 3.0 ),
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        ptMin = cms.double( 0.35 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      ),
      SeedingLayers = cms.string( "PixelLayerTripletsHITHE" )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByConformalMappingAndLine" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
      fixImpactParameter = cms.double( 0.0 )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.35 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
process.hltHITPixelVerticesHE = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltHITPixelTracksHE" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    Method2 = cms.bool( True )
)
process.hltIsolPixelTrackProdHE8E29 = cms.EDProducer( "IsolatedPixelTrackCandidateProducer",
    L1eTauJetsSource = cms.InputTag( 'hltL1extraParticles','Tau' ),
    tauAssociationCone = cms.double( 0.0 ),
    tauUnbiasCone = cms.double( 1.2 ),
    ExtrapolationConeSize = cms.double( 1.0 ),
    PixelIsolationConeSizeAtEC = cms.double( 40.0 ),
    L1GTSeedLabel = cms.InputTag( "hltL1sIsoTrack8E29" ),
    MaxVtxDXYSeed = cms.double( 101.0 ),
    MaxVtxDXYIsol = cms.double( 101.0 ),
    VertexLabel = cms.InputTag( "hltHITPixelVerticesHE" ),
    MagFieldRecordName = cms.string( "VolumeBasedMagneticField" ),
    minPTrack = cms.double( 5.0 ),
    maxPTrackForIsolation = cms.double( 3.0 ),
    EBEtaBoundary = cms.double( 1.479 ),
    PixelTracksSources = cms.VInputTag( 'hltHITPixelTracksHB','hltHITPixelTracksHE' )
)
process.hltIsolPixelTrackL2FilterHE8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltIsolPixelTrackProdHE8E29" ),
    MinPtTrack = cms.double( 3.5 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 2.0 ),
    MinEtaTrack = cms.double( 1.4 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 5 ),
    DropMultiL2Event = cms.bool( False )
)
process.hltHITPixelTripletSeedGeneratorHE8E29 = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "hltSiStripClusters" ),
      doClusterCheck = cms.bool( False ),
      PixelClusterCollectionLabel = cms.InputTag( "hltSiPixelClusters" ),
      MaxNumberOfPixelClusters = cms.uint32( 10000 )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "HITRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        useIsoTracks = cms.bool( True ),
        trackSrc = cms.InputTag( "hltHITPixelTracksHE" ),
        isoTrackSrc = cms.InputTag( "hltIsolPixelTrackL2FilterHE8E29" ),
        l1tjetSrc = cms.InputTag( 'hltl1extraParticles','Tau' ),
        originHalfLength = cms.double( 0.2 ),
        precise = cms.bool( True ),
        deltaEtaL1JetRegion = cms.double( 0.3 ),
        useTracks = cms.bool( False ),
        originRadius = cms.double( 0.2 ),
        useL1Jets = cms.bool( False ),
        deltaPhiTrackRegion = cms.double( 0.05 ),
        deltaPhiL1JetRegion = cms.double( 0.3 ),
        vertexSrc = cms.string( "hltHITPixelVerticesHE" ),
        fixedReg = cms.bool( False ),
        etaCenter = cms.double( 0.0 ),
        phiCenter = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 ),
        deltaEtaTrackRegion = cms.double( 0.05 ),
        ptMin = cms.double( 0.5 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
process.hltHITCkfTrackCandidatesHE8E29 = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltHITPixelTripletSeedGeneratorHE8E29" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltHITCtfWithMaterialTracksHE8E29 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltHITCtfWithMaterialTracksHE8E29" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltHITCkfTrackCandidatesHE8E29" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltHITIPTCorrectorHE8E29 = cms.EDProducer( "IPTCorrector",
    corTracksLabel = cms.InputTag( "hltHITCtfWithMaterialTracksHE8E29" ),
    filterLabel = cms.InputTag( "hltIsolPixelTrackL2FilterHE8E29" ),
    associationCone = cms.double( 0.2 )
)
process.hltIsolPixelTrackL3FilterHE8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltHITIPTCorrectorHE8E29" ),
    MinPtTrack = cms.double( 20.0 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 2.0 ),
    MinEtaTrack = cms.double( 1.4 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 999 ),
    DropMultiL2Event = cms.bool( False )
)
process.hltPreIsoTrackHB8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltHITPixelVerticesHB = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 1.0 ),
    TrackCollection = cms.InputTag( "hltHITPixelTracksHB" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    Method2 = cms.bool( True )
)
process.hltIsolPixelTrackProdHB8E29 = cms.EDProducer( "IsolatedPixelTrackCandidateProducer",
    L1eTauJetsSource = cms.InputTag( 'hltL1extraParticles','Tau' ),
    tauAssociationCone = cms.double( 0.0 ),
    tauUnbiasCone = cms.double( 1.2 ),
    ExtrapolationConeSize = cms.double( 1.0 ),
    PixelIsolationConeSizeAtEC = cms.double( 40.0 ),
    L1GTSeedLabel = cms.InputTag( "hltL1sIsoTrack8E29" ),
    MaxVtxDXYSeed = cms.double( 101.0 ),
    MaxVtxDXYIsol = cms.double( 101.0 ),
    VertexLabel = cms.InputTag( "hltHITPixelVerticesHB" ),
    MagFieldRecordName = cms.string( "VolumeBasedMagneticField" ),
    minPTrack = cms.double( 5.0 ),
    maxPTrackForIsolation = cms.double( 3.0 ),
    EBEtaBoundary = cms.double( 1.479 ),
    PixelTracksSources = cms.VInputTag( 'hltHITPixelTracksHB' )
)
process.hltIsolPixelTrackL2FilterHB8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltIsolPixelTrackProdHB8E29" ),
    MinPtTrack = cms.double( 3.5 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 1.45 ),
    MinEtaTrack = cms.double( 0.0 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 8.0 ),
    NMaxTrackCandidates = cms.int32( 10 ),
    DropMultiL2Event = cms.bool( False )
)
process.hltHITPixelTripletSeedGeneratorHB8E29 = cms.EDProducer( "SeedGeneratorFromRegionHitsEDProducer",
    ClusterCheckPSet = cms.PSet( 
      MaxNumberOfCosmicClusters = cms.uint32( 50000 ),
      ClusterCollectionLabel = cms.InputTag( "hltSiStripClusters" ),
      doClusterCheck = cms.bool( False ),
      PixelClusterCollectionLabel = cms.InputTag( "hltSiPixelClusters" ),
      MaxNumberOfPixelClusters = cms.uint32( 10000 )
    ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "HITRegionalPixelSeedGenerator" ),
      RegionPSet = cms.PSet( 
        useIsoTracks = cms.bool( True ),
        trackSrc = cms.InputTag( "hltHITPixelTracksHB" ),
        isoTrackSrc = cms.InputTag( "hltIsolPixelTrackL2FilterHB8E29" ),
        l1tjetSrc = cms.InputTag( 'hltl1extraParticles','Tau' ),
        originHalfLength = cms.double( 0.2 ),
        precise = cms.bool( True ),
        deltaEtaL1JetRegion = cms.double( 0.3 ),
        useTracks = cms.bool( False ),
        originRadius = cms.double( 0.2 ),
        useL1Jets = cms.bool( False ),
        deltaPhiTrackRegion = cms.double( 0.05 ),
        deltaPhiL1JetRegion = cms.double( 0.3 ),
        vertexSrc = cms.string( "hltHITPixelVerticesHB" ),
        fixedReg = cms.bool( False ),
        etaCenter = cms.double( 0.0 ),
        phiCenter = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 ),
        deltaEtaTrackRegion = cms.double( 0.05 ),
        ptMin = cms.double( 1.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      ),
      SeedingLayers = cms.string( "PixelLayerTriplets" )
    ),
    SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) ),
    SeedCreatorPSet = cms.PSet( 
      ComponentName = cms.string( "SeedFromConsecutiveHitsCreator" ),
      propagator = cms.string( "PropagatorWithMaterial" )
    ),
    TTRHBuilder = cms.string( "WithTrackAngle" )
)
process.hltHITCkfTrackCandidatesHB8E29 = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltHITPixelTripletSeedGeneratorHB8E29" ),
    TrajectoryBuilder = cms.string( "CkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    doSeedingRegionRebuilding = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 )
)
process.hltHITCtfWithMaterialTracksHB8E29 = cms.EDProducer( "TrackProducer",
    TrajectoryInEvent = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    clusterRemovalInfo = cms.InputTag( "" ),
    alias = cms.untracked.string( "hltHITCtfWithMaterialTracksHB8E29" ),
    Fitter = cms.string( "hltKFFittingSmoother" ),
    Propagator = cms.string( "PropagatorWithMaterial" ),
    src = cms.InputTag( "hltHITCkfTrackCandidatesHB8E29" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    NavigationSchool = cms.string( "" )
)
process.hltHITIPTCorrectorHB8E29 = cms.EDProducer( "IPTCorrector",
    corTracksLabel = cms.InputTag( "hltHITCtfWithMaterialTracksHB8E29" ),
    filterLabel = cms.InputTag( "hltIsolPixelTrackL2FilterHB8E29" ),
    associationCone = cms.double( 0.2 )
)
process.hltIsolPixelTrackL3FilterHB8E29 = cms.EDFilter( "HLTPixelIsolTrackFilter",
    candTag = cms.InputTag( "hltHITIPTCorrectorHB8E29" ),
    MinPtTrack = cms.double( 20.0 ),
    MaxPtNearby = cms.double( 2.0 ),
    MaxEtaTrack = cms.double( 1.45 ),
    MinEtaTrack = cms.double( 0.0 ),
    filterTrackEnergy = cms.bool( True ),
    MinEnergyTrack = cms.double( 20.0 ),
    NMaxTrackCandidates = cms.int32( 999 ),
    DropMultiL2Event = cms.bool( False )
)
process.hltL1EventNumberNZS = cms.EDFilter( "HLTL1NumberFilter",
    rawInput = cms.InputTag( "rawDataCollector" ),
    period = cms.uint32( 4096 ),
    invert = cms.bool( False )
)
process.hltPreHcalPhiSym = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sHcalNZS8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleJet6U OR L1_SingleJet10U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U OR L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3 OR L1_SingleMu5 OR L1_SingleMu7 OR L1_SingleMu10 OR L1_SingleMu14 OR L1_SingleMu20 OR L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreHcalNZS8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sAlCaEcalPhiSym = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BscMinBiasOR_BptxPlusORMinus OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_DoubleHfBitCountsRing2_P1N1" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreAlCaEcalPhiSym = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltAlCaPhiSymStream = cms.EDFilter( "HLTEcalPhiSymFilter",
    barrelHitCollection = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEB' ),
    endcapHitCollection = cms.InputTag( 'hltEcalRecHitAll','EcalRecHitsEE' ),
    phiSymBarrelHitCollection = cms.string( "phiSymEcalRecHitsEB" ),
    phiSymEndcapHitCollection = cms.string( "phiSymEcalRecHitsEE" ),
    eCut_barrel = cms.double( 0.15 ),
    eCut_endcap = cms.double( 0.75 ),
    eCut_barrel_high = cms.double( 999999.0 ),
    eCut_endcap_high = cms.double( 999999.0 ),
    statusThreshold = cms.uint32( 3 ),
    useRecoFlag = cms.bool( False )
)
process.hltL1sAlCaEcalPi0Eta8E29 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleJet6U OR L1_SingleJet10U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_DoubleJet30U OR L1_DoubleEG5" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreAlCaEcalPi08E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalRegionalPi0EtaFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    type = cms.string( "egamma" ),
    doES = cms.bool( True ),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    MuJobPSet = cms.PSet(  ),
    JetJobPSet = cms.VPSet( 
    ),
    EmJobPSet = cms.VPSet( 
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','Isolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 2.0 ),
        regionEtaMargin = cms.double( 0.25 )
      ),
      cms.PSet(  Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
        regionPhiMargin = cms.double( 0.4 ),
        Ptmin = cms.double( 2.0 ),
        regionEtaMargin = cms.double( 0.25 )
      )
    ),
    CandJobPSet = cms.VPSet( 
    )
)
process.hltESRegionalPi0EtaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalPi0EtaFEDs','es' ),
    splitOutput = cms.bool( False ),
    EBrechitCollection = cms.string( "" ),
    EErechitCollection = cms.string( "" ),
    rechitCollection = cms.string( "EcalRecHitsES" )
)
process.hltEcalRegionalPi0EtaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
    lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    sourceTag = cms.InputTag( "hltEcalRegionalPi0EtaFEDs" ),
    splitOutput = cms.bool( True ),
    EBrechitCollection = cms.string( "EcalRecHitsEB" ),
    EErechitCollection = cms.string( "EcalRecHitsEE" ),
    rechitCollection = cms.string( "" )
)
process.hltSimple3x3Clusters = cms.EDProducer( "EgammaHLTNxNClusterProducer",
    doBarrel = cms.bool( True ),
    doEndcaps = cms.bool( True ),
    barrelHitProducer = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEB' ),
    endcapHitProducer = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEE' ),
    clusEtaSize = cms.int32( 3 ),
    clusPhiSize = cms.int32( 3 ),
    barrelClusterCollection = cms.string( "Simple3x3ClustersBarrel" ),
    endcapClusterCollection = cms.string( "Simple3x3ClustersEndcap" ),
    clusSeedThr = cms.double( 0.5 ),
    clusSeedThrEndCap = cms.double( 1.0 ),
    useRecoFlag = cms.bool( False ),
    flagLevelRecHitsToUse = cms.int32( 1 ),
    useDBStatus = cms.bool( True ),
    statusLevelRecHitsToUse = cms.int32( 1 ),
    posCalc_logweight = cms.bool( True ),
    posCalc_t0_barl = cms.double( 7.4 ),
    posCalc_t0_endc = cms.double( 3.1 ),
    posCalc_t0_endcPresh = cms.double( 1.2 ),
    posCalc_w0 = cms.double( 4.2 ),
    posCalc_x0 = cms.double( 0.89 ),
    maxNumberofSeeds = cms.int32( 200 ),
    maxNumberofClusters = cms.int32( 30 ),
    debugLevel = cms.int32( 0 )
)
process.hltAlCaPi0RecHitsFilter = cms.EDFilter( "HLTEcalResonanceFilter",
    barrelHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEB' ),
    barrelClusters = cms.InputTag( 'hltSimple3x3Clusters','Simple3x3ClustersBarrel' ),
    endcapHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEE' ),
    endcapClusters = cms.InputTag( 'hltSimple3x3Clusters','Simple3x3ClustersEndcap' ),
    doSelBarrel = cms.bool( True ),
    doSelEndcap = cms.bool( True ),
    useRecoFlag = cms.bool( False ),
    flagLevelRecHitsToUse = cms.int32( 1 ),
    useDBStatus = cms.bool( True ),
    statusLevelRecHitsToUse = cms.int32( 1 ),
    preshRecHitProducer = cms.InputTag( 'hltESRegionalPi0EtaRecHit','EcalRecHitsES' ),
    storeRecHitES = cms.bool( True ),
    debugLevel = cms.int32( 0 ),
    barrelSelection = cms.PSet( 
      selePtGamma = cms.double( 0.8 ),
      selePtPair = cms.double( 1.6 ),
      seleS4S9Gamma = cms.double( 0.83 ),
      seleS9S25Gamma = cms.double( 0.0 ),
      seleMinvMaxBarrel = cms.double( 0.26 ),
      seleMinvMinBarrel = cms.double( 0.04 ),
      ptMinForIsolation = cms.double( 0.5 ),
      removePi0CandidatesForEta = cms.bool( False ),
      massLowPi0Cand = cms.double( 0.084 ),
      massHighPi0Cand = cms.double( 0.156 ),
      seleIso = cms.double( 0.5 ),
      seleBeltDR = cms.double( 0.2 ),
      seleBeltDeta = cms.double( 0.05 ),
      store5x5RecHitEB = cms.bool( False ),
      barrelHitCollection = cms.string( "pi0EcalRecHitsEB" )
    ),
    endcapSelection = cms.PSet( 
      seleMinvMaxEndCap = cms.double( 0.3 ),
      seleMinvMinEndCap = cms.double( 0.05 ),
      region1_EndCap = cms.double( 2.0 ),
      selePtGammaEndCap_region1 = cms.double( 0.5 ),
      selePtPairEndCap_region1 = cms.double( 2.0 ),
      region2_EndCap = cms.double( 2.5 ),
      selePtGammaEndCap_region2 = cms.double( 0.5 ),
      selePtPairEndCap_region2 = cms.double( 2.0 ),
      selePtGammaEndCap_region3 = cms.double( 0.3 ),
      selePtPairEndCap_region3 = cms.double( 1.2 ),
      seleS4S9GammaEndCap = cms.double( 0.9 ),
      seleS9S25GammaEndCap = cms.double( 0.0 ),
      ptMinForIsolationEndCap = cms.double( 0.5 ),
      seleBeltDREndCap = cms.double( 0.2 ),
      seleBeltDetaEndCap = cms.double( 0.05 ),
      seleIsoEndCap = cms.double( 0.5 ),
      store5x5RecHitEE = cms.bool( False ),
      endcapHitCollection = cms.string( "pi0EcalRecHitsEE" )
    ),
    preshowerSelection = cms.PSet( 
      preshNclust = cms.int32( 4 ),
      preshClusterEnergyCut = cms.double( 0.0 ),
      preshStripEnergyCut = cms.double( 0.0 ),
      preshSeededNstrip = cms.int32( 15 ),
      preshCalibPlaneX = cms.double( 1.0 ),
      preshCalibPlaneY = cms.double( 0.7 ),
      preshCalibGamma = cms.double( 0.024 ),
      preshCalibMIP = cms.double( 9.0E-5 ),
      debugLevelES = cms.string( "" ),
      ESCollection = cms.string( "pi0EcalRecHitsES" )
    )
)
process.hltPreAlCaEcalEta8E29 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltAlCaEtaRecHitsFilter = cms.EDFilter( "HLTEcalResonanceFilter",
    barrelHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEB' ),
    barrelClusters = cms.InputTag( 'hltSimple3x3Clusters','Simple3x3ClustersBarrel' ),
    endcapHits = cms.InputTag( 'hltEcalRegionalPi0EtaRecHit','EcalRecHitsEE' ),
    endcapClusters = cms.InputTag( 'hltSimple3x3Clusters','Simple3x3ClustersEndcap' ),
    doSelBarrel = cms.bool( True ),
    doSelEndcap = cms.bool( True ),
    useRecoFlag = cms.bool( False ),
    flagLevelRecHitsToUse = cms.int32( 1 ),
    useDBStatus = cms.bool( True ),
    statusLevelRecHitsToUse = cms.int32( 1 ),
    preshRecHitProducer = cms.InputTag( 'hltESRegionalPi0EtaRecHit','EcalRecHitsES' ),
    storeRecHitES = cms.bool( True ),
    debugLevel = cms.int32( 0 ),
    barrelSelection = cms.PSet( 
      selePtGamma = cms.double( 0.8 ),
      selePtPair = cms.double( 3.0 ),
      seleS4S9Gamma = cms.double( 0.87 ),
      seleS9S25Gamma = cms.double( 0.8 ),
      seleMinvMaxBarrel = cms.double( 0.9 ),
      seleMinvMinBarrel = cms.double( 0.2 ),
      ptMinForIsolation = cms.double( 0.5 ),
      removePi0CandidatesForEta = cms.bool( True ),
      massLowPi0Cand = cms.double( 0.084 ),
      massHighPi0Cand = cms.double( 0.156 ),
      seleIso = cms.double( 0.5 ),
      seleBeltDR = cms.double( 0.3 ),
      seleBeltDeta = cms.double( 0.1 ),
      store5x5RecHitEB = cms.bool( True ),
      barrelHitCollection = cms.string( "etaEcalRecHitsEB" )
    ),
    endcapSelection = cms.PSet( 
      seleMinvMaxEndCap = cms.double( 0.9 ),
      seleMinvMinEndCap = cms.double( 0.2 ),
      region1_EndCap = cms.double( 2.0 ),
      selePtGammaEndCap_region1 = cms.double( 1.0 ),
      selePtPairEndCap_region1 = cms.double( 3.0 ),
      region2_EndCap = cms.double( 2.5 ),
      selePtGammaEndCap_region2 = cms.double( 1.0 ),
      selePtPairEndCap_region2 = cms.double( 3.0 ),
      selePtGammaEndCap_region3 = cms.double( 0.7 ),
      selePtPairEndCap_region3 = cms.double( 3.0 ),
      seleS4S9GammaEndCap = cms.double( 0.9 ),
      seleS9S25GammaEndCap = cms.double( 0.85 ),
      ptMinForIsolationEndCap = cms.double( 0.5 ),
      seleBeltDREndCap = cms.double( 0.3 ),
      seleBeltDetaEndCap = cms.double( 0.1 ),
      seleIsoEndCap = cms.double( 0.5 ),
      store5x5RecHitEE = cms.bool( True ),
      endcapHitCollection = cms.string( "etaEcalRecHitsEE" )
    ),
    preshowerSelection = cms.PSet( 
      preshNclust = cms.int32( 4 ),
      preshClusterEnergyCut = cms.double( 0.0 ),
      preshStripEnergyCut = cms.double( 0.0 ),
      preshSeededNstrip = cms.int32( 15 ),
      preshCalibPlaneX = cms.double( 1.0 ),
      preshCalibPlaneY = cms.double( 0.7 ),
      preshCalibGamma = cms.double( 0.024 ),
      preshCalibMIP = cms.double( 9.0E-5 ),
      debugLevelES = cms.string( "" ),
      ESCollection = cms.string( "etaEcalRecHitsES" )
    )
)
process.hltL1sAlCaRPC = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3 OR L1_SingleMu7" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreRPCMuonNoHits = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltRPCPointProducer = cms.EDProducer( "RPCPointProducer",
    cscSegments = cms.InputTag( "hltCscSegments" ),
    dt4DSegments = cms.InputTag( "hltDt4DSegments" )
)
process.hltRPCFilter = cms.EDFilter( "HLTRPCFilter",
    rpcRecHits = cms.InputTag( "hltRpcRecHits" ),
    rpcDTPoints = cms.InputTag( 'hltRPCPointProducer','RPCDTExtrapolatedPoints' ),
    rpcCSCPoints = cms.InputTag( 'hltRPCPointProducer','RPCCSCExtrapolatedPoints' )
)
process.hltPreRPCMuonNoTriggers = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltRPCMuonNoTriggersL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sAlCaRPC" ),
    MaxEta = cms.double( 1.6 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32( 6 )
)
process.hltPreRPCMuonNorma = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltRPCMuonNormaL1Filtered0 = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sAlCaRPC" ),
    MaxEta = cms.double( 1.6 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SaveTag = cms.untracked.bool( True ),
    SelectQualities = cms.vint32(  )
)
process.hltPreAlCaDTErrors = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDTROMonitorFilter = cms.EDFilter( "HLTDTROMonitorFilter",
    inputLabel = cms.InputTag( "rawDataCollector" )
)
process.hltDynAlCaDTErrors = cms.EDFilter( "HLTDynamicPrescaler" )
process.hltCalibrationEventsFilter = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 2 )
)
process.hltPreCalibration = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreEcalCalibration = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltEcalCalibrationRaw = cms.EDProducer( "SubdetFEDSelector",
    getECAL = cms.bool( True ),
    getSiStrip = cms.bool( False ),
    getSiPixel = cms.bool( False ),
    getHCAL = cms.bool( False ),
    getMuon = cms.bool( False ),
    getTrigger = cms.bool( False ),
    rawInputLabel = cms.InputTag( "rawDataCollector" )
)
process.hltHcalCalibTypeFilter = cms.EDFilter( "HLTHcalCalibTypeFilter",
    InputTag = cms.InputTag( "rawDataCollector" ),
    CalibTypes = cms.vint32( 1, 2, 3, 4, 5, 6 )
)
process.hltPreHcalCalibration = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltRandomEventsFilter = cms.EDFilter( "HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32( 3 )
)
process.hltPreRandom = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPrePixelTracksMultiplicity70 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPixelTracksForHighMult = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "GlobalRegionProducer" ),
      RegionPSet = cms.PSet( 
        precise = cms.bool( True ),
        originHalfLength = cms.double( 30.0 ),
        originRadius = cms.double( 0.5 ),
        originYPos = cms.double( 0.0 ),
        ptMin = cms.double( 0.4 ),
        originXPos = cms.double( 0.0 ),
        originZPos = cms.double( 0.0 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet( 
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      SeedingLayers = cms.string( "PixelLayerTriplets" ),
      GeneratorPSet = cms.PSet( 
        useBending = cms.bool( True ),
        useFixedPreFiltering = cms.bool( False ),
        phiPreFiltering = cms.double( 0.3 ),
        extraHitRPhitolerance = cms.double( 0.06 ),
        useMultScattering = cms.bool( True ),
        ComponentName = cms.string( "PixelTripletHLTGenerator" ),
        extraHitRZtolerance = cms.double( 0.06 ),
        maxElement = cms.uint32( 10000 )
      )
    ),
    FitterPSet = cms.PSet( 
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet( 
      chi2 = cms.double( 1000.0 ),
      nSigmaTipMaxTolerance = cms.double( 0.0 ),
      ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
      nSigmaInvPtTolerance = cms.double( 0.0 ),
      ptMin = cms.double( 0.1 ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)
process.hltPixelVerticesForHighMult = cms.EDProducer( "PixelVertexProducer",
    Verbosity = cms.int32( 0 ),
    Finder = cms.string( "DivisiveVertexFinder" ),
    UseError = cms.bool( True ),
    WtAverage = cms.bool( True ),
    ZOffset = cms.double( 5.0 ),
    ZSeparation = cms.double( 0.05 ),
    NTrkMin = cms.int32( 2 ),
    PtMin = cms.double( 0.2 ),
    TrackCollection = cms.InputTag( "hltPixelTracksForHighMult" ),
    beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
    Method2 = cms.bool( True )
)
process.hltPixelCandsForHighMult = cms.EDProducer( "ConcreteChargedCandidateProducer",
    src = cms.InputTag( "hltPixelTracksForHighMult" ),
    particleType = cms.string( "pi+" )
)
process.hlt1HighMult70 = cms.EDFilter( "HLTSingleVertexPixelTrackFilter",
    vertexCollection = cms.InputTag( "hltPixelVerticesForHighMult" ),
    trackCollection = cms.InputTag( "hltPixelCandsForHighMult" ),
    MinPt = cms.double( 0.2 ),
    MaxPt = cms.double( 10000.0 ),
    MaxEta = cms.double( 2.0 ),
    MaxVz = cms.double( 10.0 ),
    MinTrks = cms.int32( 70 ),
    MinSep = cms.double( 0.12 )
)
process.hltPrePixelTracksMultiplicity85 = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hlt1HighMult85 = cms.EDFilter( "HLTSingleVertexPixelTrackFilter",
    vertexCollection = cms.InputTag( "hltPixelVerticesForHighMult" ),
    trackCollection = cms.InputTag( "hltPixelCandsForHighMult" ),
    MinPt = cms.double( 0.2 ),
    MaxPt = cms.double( 10000.0 ),
    MaxEta = cms.double( 2.0 ),
    MaxVz = cms.double( 10.0 ),
    MinTrks = cms.int32( 85 ),
    MinSep = cms.double( 0.12 )
)
process.hltL1sGlobalRunHPDNoise = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleJet10U_NotBptxC" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreGlobalRunHPDNoise = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sTechTrigHCALNoise = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "11 OR 12" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltL1sNotBptxPlusOrMinus = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "NOT L1_BptxPlusORMinus" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreTechTrigHCALNoise = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1BPTX = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "3" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1BPTX = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1BPTXMinusOnly = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BptxMinus AND NOT L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1BPTXMinusOnly = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1BPTXPlusOnly = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_BptxPlus AND NOT L1_ZeroBias" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL1BPTXPlusOnly = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltL1sL1SingleMu0 = cms.EDFilter( "HLTLevel1GTSeed",
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1TechTriggerSeeding = cms.bool( False ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_SingleMu0" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreL2Mu0NoVertex = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltSingleMu0L1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sL1SingleMu0" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinN = cms.int32( 1 ),
    ExcludeSingleSegmentCSC = cms.bool( False ),
    CSCTFtag = cms.InputTag( "unused" ),
    SelectQualities = cms.vint32(  )
)
process.hltL2MuonCandidatesNoVtx = cms.EDProducer( "L2MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL2Muons" )
)
process.hltSingleL2Mu0L2PreFilteredNoVtx = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidatesNoVtx" ),
    PreviousCandTag = cms.InputTag( "hltSingleMu0L1Filtered" ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),
    MinN = cms.int32( 1 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltPreLogMonitor = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltLogMonitorFilter = cms.EDFilter( "HLTLogMonitorFilter",
    default_threshold = cms.uint32( 10 ),
    categories = cms.VPSet( 
    )
)
process.hltPreFEDIntegrity = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltDTDQMEvF = cms.EDProducer( "DTUnpackingModule",
    dataType = cms.string( "DDU" ),
    fedbyType = cms.bool( False ),
    inputLabel = cms.InputTag( "rawDataCollector" ),
    useStandardFEDid = cms.bool( True ),
    dqmOnly = cms.bool( False ),
    rosParameters = cms.PSet( 
    ),
    readOutParameters = cms.PSet( 
      debug = cms.untracked.bool( False ),
      rosParameters = cms.PSet( 
        writeSC = cms.untracked.bool( True ),
        readingDDU = cms.untracked.bool( True ),
        performDataIntegrityMonitor = cms.untracked.bool( True ),
        readDDUIDfromDDU = cms.untracked.bool( True ),
        debug = cms.untracked.bool( False ),
        localDAQ = cms.untracked.bool( False )
      ),
      performDataIntegrityMonitor = cms.untracked.bool( True ),
      localDAQ = cms.untracked.bool( False )
    )
)
process.hltEcalDigis = cms.EDProducer( "EcalRawToDigi",
    numbXtalTSamples = cms.int32( 10 ),
    numbTriggerTSamples = cms.int32( 1 ),
    headerUnpacking = cms.bool( True ),
    srpUnpacking = cms.bool( False ),
    tccUnpacking = cms.bool( False ),
    feUnpacking = cms.bool( True ),
    memUnpacking = cms.bool( True ),
    syncCheck = cms.bool( False ),
    feIdCheck = cms.bool( True ),
    forceToKeepFRData = cms.bool( False ),
    eventPut = cms.bool( True ),
    InputLabel = cms.string( "rawDataCollector" ),
    DoRegional = cms.bool( False ),
    FedLabel = cms.InputTag( "listfeds" ),
    silentMode = cms.untracked.bool( True ),
    FEDs = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    orderedFedList = cms.vint32( 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654 ),
    orderedDCCIdList = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 )
)
process.hltEBHltTask = cms.EDAnalyzer( "EBHltTask",
    prefixME = cms.untracked.string( "EcalBarrel" ),
    EBDetIdCollection0 = cms.InputTag( 'hltEcalDigis','EcalIntegrityDCCSizeErrors' ),
    EBDetIdCollection1 = cms.InputTag( 'hltEcalDigis','EcalIntegrityGainErrors' ),
    EBDetIdCollection2 = cms.InputTag( 'hltEcalDigis','EcalIntegrityChIdErrors' ),
    EBDetIdCollection3 = cms.InputTag( 'hltEcalDigis','EcalIntegrityGainSwitchErrors' ),
    EcalElectronicsIdCollection1 = cms.InputTag( 'hltEcalDigis','EcalIntegrityTTIdErrors' ),
    EcalElectronicsIdCollection2 = cms.InputTag( 'hltEcalDigis','EcalIntegrityBlockSizeErrors' ),
    EcalElectronicsIdCollection3 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemTtIdErrors' ),
    EcalElectronicsIdCollection4 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemBlockSizeErrors' ),
    EcalElectronicsIdCollection5 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemChIdErrors' ),
    EcalElectronicsIdCollection6 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemGainErrors' ),
    FEDRawDataCollection = cms.InputTag( "rawDataCollector" )
)
process.hltEEHltTask = cms.EDAnalyzer( "EEHltTask",
    prefixME = cms.untracked.string( "EcalEndcap" ),
    EEDetIdCollection0 = cms.InputTag( 'hltEcalDigis','EcalIntegrityDCCSizeErrors' ),
    EEDetIdCollection1 = cms.InputTag( 'hltEcalDigis','EcalIntegrityGainErrors' ),
    EEDetIdCollection2 = cms.InputTag( 'hltEcalDigis','EcalIntegrityChIdErrors' ),
    EEDetIdCollection3 = cms.InputTag( 'hltEcalDigis','EcalIntegrityGainSwitchErrors' ),
    EcalElectronicsIdCollection1 = cms.InputTag( 'hltEcalDigis','EcalIntegrityTTIdErrors' ),
    EcalElectronicsIdCollection2 = cms.InputTag( 'hltEcalDigis','EcalIntegrityBlockSizeErrors' ),
    EcalElectronicsIdCollection3 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemTtIdErrors' ),
    EcalElectronicsIdCollection4 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemBlockSizeErrors' ),
    EcalElectronicsIdCollection5 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemChIdErrors' ),
    EcalElectronicsIdCollection6 = cms.InputTag( 'hltEcalDigis','EcalIntegrityMemGainErrors' ),
    FEDRawDataCollection = cms.InputTag( "rawDataCollector" )
)
process.hltL1tfed = cms.EDAnalyzer( "L1TFED",
    rawTag = cms.InputTag( "rawDataCollector" ),
    DQMStore = cms.untracked.bool( True ),
    disableROOToutput = cms.untracked.bool( True ),
    FEDDirName = cms.untracked.string( "FEDIntegrity" ),
    L1FEDS = cms.vint32( 745, 760, 780, 812, 813 )
)
process.hltSiPixelDigisWithErrors = cms.EDProducer( "SiPixelRawToDigi",
    IncludeErrors = cms.bool( False ),
    InputLabel = cms.InputTag( "rawDataCollector" )
)
process.hltSiPixelHLTSource = cms.EDAnalyzer( "SiPixelHLTSource",
    RawInput = cms.InputTag( "rawDataCollector" ),
    ErrorInput = cms.InputTag( "hltSiPixelDigisWithErrors" ),
    DirName = cms.untracked.string( "Pixel/FEDIntegrity" ),
    outputFile = cms.string( "Pixel_DQM_HLT.root" )
)
process.hltSiStripFEDCheck = cms.EDAnalyzer( "SiStripFEDCheckPlugin",
    RawDataTag = cms.InputTag( "rawDataCollector" ),
    DirName = cms.untracked.string( "SiStrip/FEDIntegrity" ),
    HistogramUpdateFrequency = cms.untracked.uint32( 1000 ),
    DoPayloadChecks = cms.untracked.bool( False ),
    CheckChannelLengths = cms.untracked.bool( False ),
    CheckChannelPacketCodes = cms.untracked.bool( False ),
    CheckFELengths = cms.untracked.bool( False ),
    CheckChannelStatus = cms.untracked.bool( False )
)
process.hltRPCFEDIntegrity = cms.EDAnalyzer( "RPCFEDIntegrity",
    RPCRawCountsInputTag = cms.untracked.InputTag( "hltMuonRPCDigis" )
)
process.hltBoolDQMPath = cms.EDFilter( "HLTBool",
    result = cms.bool( False )
)
process.hltTriggerSummaryAOD = cms.EDProducer( "TriggerSummaryProducerAOD",
    processName = cms.string( "@" )
)
process.hltPreTriggerSummaryRAW = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltTriggerSummaryRAW = cms.EDProducer( "TriggerSummaryProducerRAW",
    processName = cms.string( "@" )
)
process.hltBoolFinalPath = cms.EDFilter( "HLTBool",
    result = cms.bool( False )
)
process.hltL1GtTrigReport = cms.EDAnalyzer( "L1GtTrigReport",
    UseL1GlobalTriggerRecord = cms.bool( False ),
    L1GtRecordInputTag = cms.InputTag( "hltGtDigis" )
)
process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)
process.hltPreEventDisplay = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreEventDisplaySmart = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'HLT_L1Jet10U / 4',
      'HLT_Jet50U',
      'HLT_MET100',
      'HLT_L1SingleEG2 / 4',
      'HLT_L1SingleEG5 / 4',
      'HLT_Ele15_LW_L1R',
      'HLT_Photon20_Cleaned_L1R',
      'HLT_DoublePhoton10_L1R',
      'HLT_Mu3 / 3',
      'HLT_Mu5',
      'HLT_L2DoubleMu0',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus / 3',
      'HLT_ZeroBias / 10',
      'HLT_TrackerCosmics / 50' ),
    hltResults = cms.InputTag( "TriggerResults" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)
process.hltPreExpress = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreExpressSmart = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'HLT_L1Jet10U / 4',
      'HLT_Jet50U',
      'HLT_MET100',
      'HLT_L1SingleEG2 / 4',
      'HLT_L1SingleEG5 / 4',
      'HLT_Ele15_LW_L1R',
      'HLT_Photon20_Cleaned_L1R',
      'HLT_DoublePhoton10_L1R',
      'HLT_Mu3 / 3',
      'HLT_Mu5',
      'HLT_L2DoubleMu0',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus / 3',
      'HLT_ZeroBias / 10',
      'HLT_TrackerCosmics / 50' ),
    hltResults = cms.InputTag( "TriggerResults" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)
process.hltDQML1Scalers = cms.EDAnalyzer( "L1Scalers",
    l1GtData = cms.InputTag( "hltGtDigis" ),
    fedRawData = cms.InputTag( "rawDataCollector" ),
    HFRecHitCollection = cms.InputTag( "hltHfreco" ),
    maskedChannels = cms.untracked.vint32( 8137, 8141, 8146, 8149, 8150, 8153 )
)
process.hltDQML1SeedLogicScalers = cms.EDAnalyzer( "HLTSeedL1LogicScalers",
    l1BeforeMask = cms.bool( False ),
    processname = cms.string( "HLT" ),
    L1GtDaqReadoutRecordInputTag = cms.InputTag( "hltGtDigis" ),
    L1GtRecordInputTag = cms.InputTag( "unused" ),
    DQMFolder = cms.untracked.string( "HLT/HLTSeedL1LogicScalers_EvF" ),
    monitorPaths = cms.untracked.vstring( 'HLT_L1MuOpen',
      'HLT_L1Mu',
      'HLT_Mu3',
      'HLT_L1SingleForJet',
      'HLT_SingleLooseIsoTau20',
      'HLT_MinBiasEcal' )
)
process.hltDQMHLTScalers = cms.EDAnalyzer( "HLTScalers",
    triggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)
process.hltPreDQM = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreDQMSmart = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'HLT_Activity_CSC / 10',
      'HLT_Activity_DT / 10',
      'HLT_Activity_DT_Tuned / 10',
      'HLT_Activity_Ecal_SC15 / 10',
      'HLT_Activity_Ecal_SC7 / 10',
      'HLT_Activity_L1A / 10',
      'HLT_Activity_PixelClusters',
      'HLT_BTagIP_Jet50U',
      'HLT_BTagMu_Jet10U',
      'HLT_Calibration / 10',
      'HLT_CSCBeamHalo',
      'HLT_CSCBeamHaloOverlapRing1',
      'HLT_CSCBeamHaloOverlapRing2',
      'HLT_CSCBeamHaloRing2or3',
      'HLT_DiJetAve15U_8E29',
      'HLT_DiJetAve30U_8E29',
      'HLT_DoubleEle5_SW_L1R',
      'HLT_DoubleJet15U_ForwardBackward',
      'HLT_DoubleLooseIsoTau15',
      'HLT_DoubleMu0',
      'HLT_DoubleMu3',
      'HLT_DoublePhoton10_L1R',
      'HLT_DoublePhoton4_eeRes_L1R',
      'HLT_DoublePhoton4_Jpsi_L1R',
      'HLT_DoublePhoton4_Upsilon_L1R',
      'HLT_DoublePhoton5_Jpsi_L1R',
      'HLT_DoublePhoton5_L1R',
      'HLT_DoublePhoton5_Upsilon_L1R',
      'HLT_Ele10_LW_EleId_L1R',
      'HLT_Ele10_LW_L1R',
      'HLT_Ele10_SW_L1R',
      'HLT_Ele15_LW_L1R',
      'HLT_Ele15_SC10_LW_L1R',
      'HLT_Ele15_SiStrip_L1R',
      'HLT_Ele20_LW_L1R',
      'HLT_FwdJet20U',
      'HLT_GlobalRunHPDNoise',
      'HLT_HcalCalibration',
      'HLT_HcalNZS_8E29',
      'HLT_HcalPhiSym',
      'HLT_HT100U',
      'HLT_IsoMu3',
      'HLT_IsoTrackHB_8E29',
      'HLT_IsoTrackHE_8E29',
      'HLT_Jet15U',
      'HLT_Jet15U_HcalNoiseFiltered',
      'HLT_Jet30U',
      'HLT_Jet50U',
      'HLT_L1_BPTX / 10',
      'HLT_L1_BPTX_MinusOnly / 10',
      'HLT_L1_BPTX_PlusOnly / 10',
      'HLT_L1_BptxXOR_BscMinBiasOR',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
      'HLT_L1DoubleEG5',
      'HLT_L1DoubleMuOpen',
      'HLT_L1Jet10U',
      'HLT_L1Jet10U_NoBPTX',
      'HLT_L1Jet6U',
      'HLT_L1Jet6U_NoBPTX',
      'HLT_L1MET20',
      'HLT_L1Mu',
      'HLT_L1Mu14_L1ETM30',
      'HLT_L1Mu14_L1SingleEG10',
      'HLT_L1Mu14_L1SingleJet6U',
      'HLT_L1Mu20',
      'HLT_L1MuOpen',
      'HLT_L1MuOpen_AntiBPTX',
      'HLT_L1MuOpen_DT',
      'HLT_L1MuOpen_NoBPTX',
      'HLT_L1SingleCenJet',
      'HLT_L1SingleCenJet_NoBPTX',
      'HLT_L1SingleEG2',
      'HLT_L1SingleEG5',
      'HLT_L1SingleEG8',
      'HLT_L1SingleForJet',
      'HLT_L1SingleForJet_NoBPTX',
      'HLT_L1SingleTauJet',
      'HLT_L1SingleTauJet_NoBPTX',
      'HLT_L1Tech_BSC_halo',
      'HLT_L1Tech_BSC_halo_forPhysicsBackground',
      'HLT_L1Tech_BSC_HighMultiplicity',
      'HLT_L1Tech_BSC_minBias',
      'HLT_L1Tech_HCAL_HF',
      'HLT_L1Tech_RPC_TTU_RBst1_collisions',
      'HLT_L2DoubleMu0',
      'HLT_L2Mu0',
      'HLT_L2Mu0_NoVertex',
      'HLT_L2Mu11',
      'HLT_L2Mu3',
      'HLT_L2Mu5',
      'HLT_L2Mu9',
      'HLT_MET45',
      'HLT_MET100',
      'HLT_MinBiasPixel_DoubleIsoTrack5',
      'HLT_MinBiasPixel_DoubleTrack',
      'HLT_MinBiasPixel_SingleTrack',
      'HLT_Mu0_L1MuOpen',
      'HLT_Mu0_L2Mu0',
      'HLT_Mu0_Track0_Jpsi',
      'HLT_Mu3',
      'HLT_Mu3_L1MuOpen',
      'HLT_Mu3_L2Mu0',
      'HLT_Mu3_Track0_Jpsi',
      'HLT_Mu5',
      'HLT_Mu5_L1MuOpen',
      'HLT_Mu5_L2Mu0',
      'HLT_Mu5_Track0_Jpsi',
      'HLT_Mu9',
      'HLT_MultiVertex6',
      'HLT_MultiVertex8_L1ETT60',
      'HLT_Photon10_Cleaned_L1R',
      'HLT_Photon15_Cleaned_L1R',
      'HLT_Photon15_L1R',
      'HLT_Photon15_LooseEcalIso_L1R',
      'HLT_Photon15_TrackIso_L1R',
      'HLT_Photon20_Cleaned_L1R',
      'HLT_Photon20_L1R',
      'HLT_Photon30_L1R_8E29',
      'HLT_PixelTracks_Multiplicity70',
      'HLT_PixelTracks_Multiplicity85',
      'HLT_QuadJet15U',
      'HLT_Random',
      'HLT_RPCBarrelCosmics',
      'HLT_SelectEcalSpikesHighEt_L1R',
      'HLT_SelectEcalSpikes_L1R',
      'HLT_SingleLooseIsoTau20',
      'HLT_SingleLooseIsoTau20_Trk5',
      'HLT_SingleLooseIsoTau25',
      'HLT_StoppedHSCP_8E29',
      'HLT_TechTrigHCALNoise',
      'HLT_TkMu3_NoVertex',
      'HLT_TrackerCosmics',
      'HLT_ZeroBias / 40',
      'HLT_ZeroBiasPixel_SingleTrack' ),
    hltResults = cms.InputTag( "TriggerResults" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)
process.hltPreHLTDQM = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreHLTDQMSmart = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'AlCa_EcalEta_8E29',
      'AlCa_EcalPhiSym',
      'AlCa_EcalPi0_8E29',
      'HLT_Activity_CSC / 10',
      'HLT_Activity_DT / 10',
      'HLT_Activity_DT_Tuned / 10',
      'HLT_Activity_Ecal_SC15 / 10',
      'HLT_Activity_Ecal_SC7 / 10',
      'HLT_Activity_L1A / 10',
      'HLT_Activity_PixelClusters',
      'HLT_BTagIP_Jet50U',
      'HLT_BTagMu_Jet10U',
      'HLT_Calibration / 10',
      'HLT_CSCBeamHalo',
      'HLT_CSCBeamHaloOverlapRing1',
      'HLT_CSCBeamHaloOverlapRing2',
      'HLT_CSCBeamHaloRing2or3',
      'HLT_DiJetAve15U_8E29',
      'HLT_DiJetAve30U_8E29',
      'HLT_DoubleEle5_SW_L1R',
      'HLT_DoubleJet15U_ForwardBackward',
      'HLT_DoubleLooseIsoTau15',
      'HLT_DoubleMu0',
      'HLT_DoubleMu3',
      'HLT_DoublePhoton10_L1R',
      'HLT_DoublePhoton4_eeRes_L1R',
      'HLT_DoublePhoton4_Jpsi_L1R',
      'HLT_DoublePhoton4_Upsilon_L1R',
      'HLT_DoublePhoton5_Jpsi_L1R',
      'HLT_DoublePhoton5_L1R',
      'HLT_DoublePhoton5_Upsilon_L1R',
      'HLT_Ele10_LW_EleId_L1R',
      'HLT_Ele10_LW_L1R',
      'HLT_Ele10_SW_L1R',
      'HLT_Ele15_LW_L1R',
      'HLT_Ele15_SC10_LW_L1R',
      'HLT_Ele15_SiStrip_L1R',
      'HLT_Ele20_LW_L1R',
      'HLT_FwdJet20U',
      'HLT_GlobalRunHPDNoise',
      'HLT_HcalNZS_8E29',
      'HLT_HcalPhiSym',
      'HLT_HT100U',
      'HLT_IsoMu3',
      'HLT_IsoTrackHB_8E29',
      'HLT_IsoTrackHE_8E29',
      'HLT_Jet15U',
      'HLT_Jet15U_HcalNoiseFiltered',
      'HLT_Jet30U',
      'HLT_Jet50U',
      'HLT_L1_BPTX / 10',
      'HLT_L1_BPTX_MinusOnly / 10',
      'HLT_L1_BPTX_PlusOnly / 10',
      'HLT_L1_BptxXOR_BscMinBiasOR',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
      'HLT_L1DoubleEG5',
      'HLT_L1DoubleMuOpen',
      'HLT_L1Jet10U',
      'HLT_L1Jet10U_NoBPTX',
      'HLT_L1Jet6U',
      'HLT_L1Jet6U_NoBPTX',
      'HLT_L1MET20',
      'HLT_L1Mu',
      'HLT_L1Mu14_L1ETM30',
      'HLT_L1Mu14_L1SingleEG10',
      'HLT_L1Mu14_L1SingleJet6U',
      'HLT_L1Mu20',
      'HLT_L1MuOpen',
      'HLT_L1MuOpen_AntiBPTX',
      'HLT_L1MuOpen_DT',
      'HLT_L1MuOpen_NoBPTX',
      'HLT_L1SingleCenJet',
      'HLT_L1SingleCenJet_NoBPTX',
      'HLT_L1SingleEG2',
      'HLT_L1SingleEG5',
      'HLT_L1SingleEG8',
      'HLT_L1SingleForJet',
      'HLT_L1SingleForJet_NoBPTX',
      'HLT_L1SingleTauJet',
      'HLT_L1SingleTauJet_NoBPTX',
      'HLT_L1Tech_BSC_halo',
      'HLT_L1Tech_BSC_halo_forPhysicsBackground',
      'HLT_L1Tech_BSC_HighMultiplicity',
      'HLT_L1Tech_BSC_minBias',
      'HLT_L1Tech_HCAL_HF',
      'HLT_L1Tech_RPC_TTU_RBst1_collisions',
      'HLT_L2DoubleMu0',
      'HLT_L2Mu0',
      'HLT_L2Mu0_NoVertex',
      'HLT_L2Mu11',
      'HLT_L2Mu3',
      'HLT_L2Mu5',
      'HLT_L2Mu9',
      'HLT_MET100',
      'HLT_MET45',
      'HLT_MinBiasPixel_DoubleIsoTrack5',
      'HLT_MinBiasPixel_DoubleTrack',
      'HLT_MinBiasPixel_SingleTrack',
      'HLT_Mu0_L1MuOpen',
      'HLT_Mu0_L2Mu0',
      'HLT_Mu0_Track0_Jpsi',
      'HLT_Mu3',
      'HLT_Mu3_L1MuOpen',
      'HLT_Mu3_L2Mu0',
      'HLT_Mu3_Track0_Jpsi',
      'HLT_Mu5',
      'HLT_Mu5_L1MuOpen',
      'HLT_Mu5_L2Mu0',
      'HLT_Mu5_Track0_Jpsi',
      'HLT_Mu9',
      'HLT_MultiVertex6',
      'HLT_MultiVertex8_L1ETT60',
      'HLT_Photon10_Cleaned_L1R',
      'HLT_Photon15_Cleaned_L1R',
      'HLT_Photon15_L1R',
      'HLT_Photon15_LooseEcalIso_L1R',
      'HLT_Photon15_TrackIso_L1R',
      'HLT_Photon20_Cleaned_L1R',
      'HLT_Photon20_L1R',
      'HLT_Photon30_L1R_8E29',
      'HLT_PixelTracks_Multiplicity70',
      'HLT_PixelTracks_Multiplicity85',
      'HLT_QuadJet15U',
      'HLT_Random',
      'HLT_RPCBarrelCosmics',
      'HLT_SelectEcalSpikesHighEt_L1R',
      'HLT_SelectEcalSpikes_L1R',
      'HLT_SingleLooseIsoTau20',
      'HLT_SingleLooseIsoTau20_Trk5',
      'HLT_SingleLooseIsoTau25',
      'HLT_StoppedHSCP_8E29',
      'HLT_TechTrigHCALNoise',
      'HLT_TkMu3_NoVertex',
      'HLT_TrackerCosmics',
      'HLT_ZeroBias / 40',
      'HLT_ZeroBiasPixel_SingleTrack' ),
    hltResults = cms.InputTag( "TriggerResults" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)
process.hltPreHLTMON = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" )
)
process.hltPreHLTMONSmart = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'AlCa_EcalEta_8E29 / 250',
      'AlCa_EcalPhiSym / 250',
      'AlCa_EcalPi0_8E29 / 250',
      'HLT_Activity_CSC',
      'HLT_Activity_DT',
      'HLT_Activity_DT_Tuned',
      'HLT_Activity_Ecal_SC15',
      'HLT_Activity_Ecal_SC7',
      'HLT_Activity_L1A',
      'HLT_Activity_PixelClusters',
      'HLT_BTagIP_Jet50U',
      'HLT_BTagMu_Jet10U',
      'HLT_CSCBeamHalo',
      'HLT_CSCBeamHaloOverlapRing1',
      'HLT_CSCBeamHaloOverlapRing2',
      'HLT_CSCBeamHaloRing2or3',
      'HLT_DiJetAve15U_8E29',
      'HLT_DiJetAve30U_8E29',
      'HLT_DoubleEle5_SW_L1R',
      'HLT_DoubleJet15U_ForwardBackward',
      'HLT_DoubleLooseIsoTau15',
      'HLT_DoubleMu0',
      'HLT_DoubleMu3',
      'HLT_DoublePhoton10_L1R',
      'HLT_DoublePhoton4_eeRes_L1R',
      'HLT_DoublePhoton4_Jpsi_L1R',
      'HLT_DoublePhoton4_Upsilon_L1R',
      'HLT_DoublePhoton5_Jpsi_L1R',
      'HLT_DoublePhoton5_L1R',
      'HLT_DoublePhoton5_Upsilon_L1R',
      'HLT_Ele10_LW_EleId_L1R',
      'HLT_Ele10_LW_L1R',
      'HLT_Ele10_SW_L1R',
      'HLT_Ele15_LW_L1R',
      'HLT_Ele15_SC10_LW_L1R',
      'HLT_Ele15_SiStrip_L1R',
      'HLT_Ele20_LW_L1R',
      'HLT_FwdJet20U',
      'HLT_GlobalRunHPDNoise',
      'HLT_HcalNZS_8E29',
      'HLT_HcalPhiSym',
      'HLT_HT100U',
      'HLT_IsoMu3',
      'HLT_IsoTrackHB_8E29',
      'HLT_IsoTrackHE_8E29',
      'HLT_Jet15U',
      'HLT_Jet15U_HcalNoiseFiltered',
      'HLT_Jet30U',
      'HLT_Jet50U',
      'HLT_L1_BptxXOR_BscMinBiasOR',
      'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
      'HLT_L1DoubleEG5',
      'HLT_L1DoubleMuOpen',
      'HLT_L1Jet10U',
      'HLT_L1Jet10U_NoBPTX',
      'HLT_L1Jet6U',
      'HLT_L1Jet6U_NoBPTX',
      'HLT_L1MET20',
      'HLT_L1Mu / 10',
      'HLT_L1Mu14_L1ETM30',
      'HLT_L1Mu14_L1SingleEG10',
      'HLT_L1Mu14_L1SingleJet6U',
      'HLT_L1Mu20',
      'HLT_L1MuOpen / 100',
      'HLT_L1MuOpen_AntiBPTX / 100',
      'HLT_L1MuOpen_DT / 100',
      'HLT_L1MuOpen_NoBPTX / 100',
      'HLT_L1SingleCenJet',
      'HLT_L1SingleCenJet_NoBPTX',
      'HLT_L1SingleEG2',
      'HLT_L1SingleEG5',
      'HLT_L1SingleEG8',
      'HLT_L1SingleForJet',
      'HLT_L1SingleForJet_NoBPTX',
      'HLT_L1SingleTauJet',
      'HLT_L1SingleTauJet_NoBPTX',
      'HLT_L1Tech_BSC_halo',
      'HLT_L1Tech_BSC_halo_forPhysicsBackground',
      'HLT_L1Tech_BSC_HighMultiplicity',
      'HLT_L1Tech_BSC_minBias',
      'HLT_L1Tech_HCAL_HF',
      'HLT_L1Tech_RPC_TTU_RBst1_collisions',
      'HLT_L2DoubleMu0',
      'HLT_L2Mu0',
      'HLT_L2Mu0_NoVertex',
      'HLT_L2Mu11',
      'HLT_L2Mu3',
      'HLT_L2Mu5',
      'HLT_L2Mu9',
      'HLT_MET100',
      'HLT_MET45',
      'HLT_MinBiasPixel_DoubleIsoTrack5',
      'HLT_MinBiasPixel_DoubleTrack',
      'HLT_MinBiasPixel_SingleTrack',
      'HLT_Mu0_L1MuOpen',
      'HLT_Mu0_L2Mu0',
      'HLT_Mu0_Track0_Jpsi',
      'HLT_Mu3',
      'HLT_Mu3_L1MuOpen',
      'HLT_Mu3_L2Mu0',
      'HLT_Mu3_Track0_Jpsi',
      'HLT_Mu5',
      'HLT_Mu5_L1MuOpen',
      'HLT_Mu5_L2Mu0',
      'HLT_Mu5_Track0_Jpsi',
      'HLT_Mu9',
      'HLT_MultiVertex6',
      'HLT_MultiVertex8_L1ETT60',
      'HLT_Photon10_Cleaned_L1R',
      'HLT_Photon15_Cleaned_L1R',
      'HLT_Photon15_L1R',
      'HLT_Photon15_LooseEcalIso_L1R',
      'HLT_Photon15_TrackIso_L1R',
      'HLT_Photon20_Cleaned_L1R',
      'HLT_Photon20_L1R',
      'HLT_Photon30_L1R_8E29',
      'HLT_PixelTracks_Multiplicity70',
      'HLT_PixelTracks_Multiplicity85',
      'HLT_QuadJet15U',
      'HLT_RPCBarrelCosmics',
      'HLT_SelectEcalSpikesHighEt_L1R',
      'HLT_SelectEcalSpikes_L1R',
      'HLT_SingleLooseIsoTau20',
      'HLT_SingleLooseIsoTau20_Trk5',
      'HLT_SingleLooseIsoTau25',
      'HLT_StoppedHSCP_8E29',
      'HLT_TechTrigHCALNoise',
      'HLT_TkMu3_NoVertex',
      'HLT_TrackerCosmics',
      'HLT_ZeroBiasPixel_SingleTrack' ),
    hltResults = cms.InputTag( "TriggerResults" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)

process.hltOutputA = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputA.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_CSCBeamHaloOverlapRing2',
  'HLT_CSCBeamHaloOverlapRing1',
  'HLT_CSCBeamHalo',
  'HLT_Activity_DT',
  'HLT_Random',
  'HLT_L1Tech_HCAL_HF',
  'HLT_IsoTrackHB_8E29',
  'HLT_IsoTrackHE_8E29',
  'HLT_L1Tech_RPC_TTU_RBst1_collisions',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1Tech_BSC_halo',
  'HLT_L1Tech_BSC_HighMultiplicity',
  'HLT_MinBiasPixel_DoubleIsoTrack5',
  'HLT_MinBiasPixel_DoubleTrack',
  'HLT_MinBiasPixel_SingleTrack',
  'HLT_ZeroBiasPixel_SingleTrack',
  'HLT_L1Tech_BSC_minBias',
  'HLT_L1Mu14_L1ETM30',
  'HLT_L1Mu14_L1SingleJet6U',
  'HLT_L1Mu14_L1SingleEG10',
  'HLT_StoppedHSCP_8E29',
  'HLT_BTagIP_Jet50U',
  'HLT_L1Jet10U_NoBPTX',
  'HLT_L1SingleEG2',
  'HLT_Activity_DT_Tuned',
  'HLT_L1Jet6U',
  'HLT_L1Jet6U_NoBPTX',
  'HLT_L1SingleCenJet',
  'HLT_L1SingleForJet',
  'HLT_L1SingleTauJet',
  'HLT_Mu0_L1MuOpen',
  'HLT_Mu0_Track0_Jpsi',
  'HLT_Mu3_L1MuOpen',
  'HLT_Mu3_Track0_Jpsi',
  'HLT_Mu5_L1MuOpen',
  'HLT_Mu5_Track0_Jpsi',
  'HLT_DoubleLooseIsoTau15',
  'HLT_SingleLooseIsoTau20',
  'HLT_DoublePhoton10_L1R',
  'HLT_Photon30_L1R_8E29',
  'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
  'HLT_Photon15_TrackIso_Cleaned_L1R',
  'HLT_Photon10_Cleaned_L1R',
  'HLT_DoublePhoton5_Upsilon_L1R',
  'HLT_DoublePhoton5_Jpsi_L1R',
  'HLT_DoubleEle5_SW_L1R',
  'HLT_L1DoubleEG5',
  'HLT_Ele20_LW_L1R',
  'HLT_Ele15_SiStrip_L1R',
  'HLT_Ele15_SC10_LW_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Ele10_LW_EleId_L1R',
  'HLT_Ele10_LW_L1R',
  'HLT_L1SingleEG8',
  'HLT_L1SingleEG5',
  'HLT_L1DoubleMuOpen',
  'HLT_L1Mu20',
  'HLT_L1Mu',
  'HLT_L1MuOpen',
  'HLT_HT100U',
  'HLT_MET100',
  'HLT_MET45',
  'HLT_L1MET20',
  'HLT_SelectEcalSpikes_L1R',
  'HLT_SelectEcalSpikesHighEt_L1R',
  'HLT_DoublePhoton4_Jpsi_L1R',
  'HLT_DoublePhoton4_Upsilon_L1R',
  'HLT_DoublePhoton4_eeRes_L1R',
  'HLT_DoublePhoton5_L1R',
  'HLT_Mu0_L2Mu0',
  'HLT_Mu3_L2Mu0',
  'HLT_Mu5_L2Mu0',
  'HLT_L2DoubleMu0',
  'HLT_L2Mu0',
  'HLT_L2Mu3',
  'HLT_L1MuOpen_AntiBPTX',
  'HLT_QuadJet15U',
  'HLT_DiJetAve30U_8E29',
  'HLT_DiJetAve15U_8E29',
  'HLT_FwdJet20U',
  'HLT_Jet50U',
  'HLT_Jet30U',
  'HLT_Jet15U',
  'HLT_L1Jet10U',
  'HLT_Activity_PixelClusters',
  'HLT_Activity_L1A',
  'HLT_L1_BPTX_PlusOnly',
  'HLT_L1_BPTX_MinusOnly',
  'HLT_L1_BPTX',
  'HLT_ZeroBias',
  'HLT_HcalNZS_8E29',
  'HLT_HcalPhiSym',
  'HLT_L2Mu0_NoVertex',
  'HLT_BTagMu_Jet10U',
  'HLT_DoubleMu0',
  'HLT_DoubleMu3',
  'HLT_Mu3',
  'HLT_Mu5',
  'HLT_Mu9',
  'HLT_IsoMu3',
  'HLT_L2Mu9',
  'HLT_L2Mu11',
  'HLT_Photon15_Cleaned_L1R',
  'HLT_Photon20_Cleaned_L1R',
  'HLT_Activity_CSC',
  'HLT_Activity_Ecal_SC7',
  'HLT_Activity_Ecal_SC15',
  'HLT_DoubleJet15U_ForwardBackward',
  'HLT_Ele10_SW_L1R',
  'HLT_Ele15_SW_L1R',
  'HLT_Ele20_SW_L1R',
  'HLT_L1Tech_BSC_halo_forPhysicsBackground',
  'HLT_L2Mu5',
  'HLT_L1_BptxXOR_BscMinBiasOR',
  'HLT_PixelTracks_Multiplicity85',
  'HLT_Jet15U_HcalNoiseFiltered',
  'HLT_L1MuOpen_DT',
  'HLT_TechTrigHCALNoise',
  'HLT_GlobalRunHPDNoise',
  'HLT_TrackerCosmics',
  'HLT_RPCBarrelCosmics',
  'HLT_CSCBeamHaloRing2or3',
  'HLT_MultiVertex6',
  'HLT_MultiVertex8_L1ETT60',
  'HLT_PixelTracks_Multiplicity70',
  'HLT_SingleLooseIsoTau20_Trk5',
  'HLT_SingleLooseIsoTau25',
  'HLT_L1DoubleMuOpen_Tight',
  'HLT_DiJetAve50U_8E29' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep *_hltL1GtObjectMap_*_*' )
)
process.hltOutputEventDisplay = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputEventDisplay.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1SingleEG2',
  'HLT_L1SingleEG5',
  'HLT_MET100',
  'HLT_ZeroBias',
  'HLT_L1Jet10U',
  'HLT_L2DoubleMu0',
  'HLT_Mu3',
  'HLT_TrackerCosmics',
  'HLT_DoublePhoton10_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Mu5',
  'HLT_Jet50U',
  'HLT_Photon20_Cleaned_L1R' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep *_hltL1GtObjectMap_*_*' )
)
process.hltOutputExpress = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputExpress.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_MET100',
  'HLT_ZeroBias',
  'HLT_L1SingleEG2',
  'HLT_L1SingleEG5',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1Jet10U',
  'HLT_L2DoubleMu0',
  'HLT_Mu3',
  'HLT_TrackerCosmics',
  'HLT_DoublePhoton10_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Mu5',
  'HLT_Jet50U',
  'HLT_Photon20_Cleaned_L1R' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep *_hltL1GtObjectMap_*_*' )
)
process.hltOutputCalibration = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputCalibration.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_Calibration' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*' )
)
process.hltOutputEcalCalibration = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputEcalCalibration.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_EcalCalibration' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep *_hltEcalCalibrationRaw_*_*' )
)
process.hltOutputALCAP0 = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputALCAP0.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'AlCa_EcalEta_8E29',
  'AlCa_EcalPi0_8E29' ) ),
    outputCommands = cms.untracked.vstring( 'drop *',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*',
      'keep *_hltAlCaEtaRecHitsFilter_*_*',
      'keep *_hltAlCaPi0RecHitsFilter_*_*' )
)
process.hltOutputALCAPHISYM = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputALCAPHISYM.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'AlCa_EcalPhiSym' ) ),
    outputCommands = cms.untracked.vstring( 'drop *',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep L1GlobalTriggerReadoutRecord_hltGtDigis_*_*',
      'keep *_hltAlCaPhiSymStream_*_*' )
)
process.hltOutputRPCMON = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputRPCMON.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'AlCa_RPCMuonNormalisation',
  'AlCa_RPCMuonNoHits',
  'AlCa_RPCMuonNoTriggers' ) ),
    outputCommands = cms.untracked.vstring( 'drop *',
      'keep edmTriggerResults_*_*_*',
      'keep *_hltRpcRecHits_*_*',
      'keep *_hltMuonDTDigis_*_*',
      'keep *_hltDt4DSegments_*_*',
      'keep *_hltMuonRPCDigis_*_*',
      'keep L1MuGMTCands_hltGtDigis_*_*',
      'keep L1MuGMTReadoutCollection_hltGtDigis_*_*',
      'keep *_hltCscSegments_*_*' )
)
process.hltOutputOnlineErrors = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputOnlineErrors.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_LogMonitor',
  'HLT_DTErrors' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*' )
)
process.hltOutputDQM = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputDQM.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_PixelTracks_Multiplicity85',
  'HLT_L1Tech_HCAL_HF',
  'HLT_L2Mu0_NoVertex',
  'HLT_L2Mu11',
  'HLT_L2Mu9',
  'HLT_MET100',
  'HLT_MET45',
  'HLT_L1Tech_BSC_minBias',
  'HLT_MinBiasPixel_DoubleIsoTrack5',
  'HLT_MinBiasPixel_DoubleTrack',
  'HLT_MinBiasPixel_SingleTrack',
  'HLT_Mu3',
  'HLT_Mu5',
  'HLT_Mu9',
  'HLT_Photon10_Cleaned_L1R',
  'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
  'HLT_Photon15_TrackIso_Cleaned_L1R',
  'HLT_L1Jet10U_NoBPTX',
  'HLT_L1SingleEG2',
  'HLT_Activity_DT_Tuned',
  'HLT_L1Jet6U',
  'HLT_L1Jet6U_NoBPTX',
  'HLT_L1SingleCenJet',
  'HLT_L1SingleForJet',
  'HLT_L1SingleTauJet',
  'HLT_Mu0_L1MuOpen',
  'HLT_Mu0_Track0_Jpsi',
  'HLT_Mu3_Track0_Jpsi',
  'HLT_Mu3_L1MuOpen',
  'HLT_Mu5_L1MuOpen',
  'HLT_Mu5_Track0_Jpsi',
  'HLT_Photon30_L1R_8E29',
  'HLT_QuadJet15U',
  'HLT_RPCBarrelCosmics',
  'HLT_Random',
  'HLT_SingleLooseIsoTau20',
  'HLT_StoppedHSCP_8E29',
  'HLT_TechTrigHCALNoise',
  'HLT_TrackerCosmics',
  'HLT_ZeroBias',
  'HLT_ZeroBiasPixel_SingleTrack',
  'HLT_Activity_DT',
  'HLT_Activity_L1A',
  'HLT_Activity_PixelClusters',
  'HLT_BTagIP_Jet50U',
  'HLT_BTagMu_Jet10U',
  'HLT_CSCBeamHalo',
  'HLT_CSCBeamHaloOverlapRing1',
  'HLT_CSCBeamHaloOverlapRing2',
  'HLT_CSCBeamHaloRing2or3',
  'HLT_Calibration',
  'HLT_DiJetAve15U_8E29',
  'HLT_DiJetAve30U_8E29',
  'HLT_DoubleEle5_SW_L1R',
  'HLT_DoubleJet15U_ForwardBackward',
  'HLT_DoubleLooseIsoTau15',
  'HLT_DoubleMu0',
  'HLT_DoubleMu3',
  'HLT_SelectEcalSpikes_L1R',
  'HLT_SelectEcalSpikesHighEt_L1R',
  'HLT_L2Mu0',
  'HLT_L2Mu3',
  'HLT_L2DoubleMu0',
  'HLT_DoublePhoton5_L1R',
  'HLT_DoublePhoton4_eeRes_L1R',
  'HLT_DoublePhoton4_Jpsi_L1R',
  'HLT_DoublePhoton4_Upsilon_L1R',
  'HLT_Mu0_L2Mu0',
  'HLT_Mu3_L2Mu0',
  'HLT_Mu5_L2Mu0',
  'HLT_L1MuOpen_AntiBPTX',
  'HLT_L2Mu5',
  'HLT_DoublePhoton10_L1R',
  'HLT_DoublePhoton5_Jpsi_L1R',
  'HLT_DoublePhoton5_Upsilon_L1R',
  'HLT_Ele10_LW_EleId_L1R',
  'HLT_Ele10_LW_L1R',
  'HLT_L1MuOpen_DT',
  'HLT_Activity_Ecal_SC7',
  'HLT_Activity_Ecal_SC15',
  'HLT_Photon15_Cleaned_L1R',
  'HLT_Photon20_Cleaned_L1R',
  'HLT_Activity_CSC',
  'HLT_PixelTracks_Multiplicity70',
  'HLT_MultiVertex6',
  'HLT_MultiVertex8_L1ETT60',
  'HLT_L1Tech_BSC_halo_forPhysicsBackground',
  'HLT_SingleLooseIsoTau25',
  'HLT_SingleLooseIsoTau20_Trk5',
  'HLT_Ele10_SW_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Ele15_SC10_LW_L1R',
  'HLT_Jet15U_HcalNoiseFiltered',
  'HLT_Ele15_SW_L1R',
  'HLT_Ele15_SiStrip_L1R',
  'HLT_Ele20_LW_L1R',
  'HLT_Ele20_SW_L1R',
  'HLT_FwdJet20U',
  'HLT_GlobalRunHPDNoise',
  'HLT_HT100U',
  'HLT_HcalCalibration',
  'HLT_HcalNZS_8E29',
  'HLT_HcalPhiSym',
  'HLT_L1Tech_BSC_HighMultiplicity',
  'HLT_IsoMu3',
  'HLT_IsoTrackHB_8E29',
  'HLT_IsoTrackHE_8E29',
  'HLT_Jet15U',
  'HLT_Jet30U',
  'HLT_Jet50U',
  'HLT_L1DoubleEG5',
  'HLT_L1DoubleMuOpen',
  'HLT_L1Jet10U',
  'HLT_L1MET20',
  'HLT_L1Mu',
  'HLT_L1Mu14_L1ETM30',
  'HLT_L1Mu14_L1SingleEG10',
  'HLT_L1Mu14_L1SingleJet6U',
  'HLT_L1Mu20',
  'HLT_L1MuOpen',
  'HLT_L1SingleEG5',
  'HLT_L1SingleEG8',
  'HLT_L1Tech_RPC_TTU_RBst1_collisions',
  'HLT_L1_BPTX',
  'HLT_L1_BPTX_MinusOnly',
  'HLT_L1_BPTX_PlusOnly',
  'HLT_L1Tech_BSC_halo',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1_BptxXOR_BscMinBiasOR' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep triggerTriggerEventWithRefs_*_*_*',
      'keep *_hltL1GtObjectMap_*_*',
      'keep *_hltDt4DSegments_*_*' )
)
process.hltOutputHLTDQM = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputHLTDQM.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_IsoTrackHB_8E29',
  'HLT_IsoTrackHE_8E29',
  'HLT_Jet15U',
  'HLT_Jet30U',
  'HLT_Jet50U',
  'HLT_L1DoubleEG5',
  'HLT_L1DoubleMuOpen',
  'HLT_L1Jet10U',
  'HLT_L1MET20',
  'HLT_L1Mu',
  'HLT_L1Mu14_L1ETM30',
  'HLT_L1Mu14_L1SingleEG10',
  'HLT_L1Mu14_L1SingleJet6U',
  'HLT_L1Mu20',
  'HLT_L1MuOpen',
  'HLT_L1SingleEG5',
  'HLT_L1SingleEG8',
  'HLT_L1Tech_RPC_TTU_RBst1_collisions',
  'HLT_L1_BPTX',
  'HLT_L1_BPTX_MinusOnly',
  'HLT_L1_BPTX_PlusOnly',
  'HLT_L1Tech_BSC_halo',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1Tech_HCAL_HF',
  'HLT_L2Mu0_NoVertex',
  'HLT_L2Mu11',
  'HLT_L1_BptxXOR_BscMinBiasOR',
  'HLT_PixelTracks_Multiplicity85',
  'HLT_Jet15U_HcalNoiseFiltered',
  'HLT_L1MuOpen_DT',
  'HLT_Activity_Ecal_SC7',
  'HLT_Activity_Ecal_SC15',
  'HLT_L2Mu9',
  'HLT_MET100',
  'HLT_MET45',
  'HLT_L1Tech_BSC_minBias',
  'HLT_MinBiasPixel_DoubleIsoTrack5',
  'HLT_MinBiasPixel_DoubleTrack',
  'HLT_MinBiasPixel_SingleTrack',
  'HLT_Mu3',
  'HLT_Mu5',
  'HLT_Mu9',
  'HLT_Photon10_Cleaned_L1R',
  'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
  'HLT_Photon15_TrackIso_Cleaned_L1R',
  'HLT_Photon30_L1R_8E29',
  'HLT_QuadJet15U',
  'HLT_RPCBarrelCosmics',
  'HLT_Random',
  'HLT_SingleLooseIsoTau20',
  'HLT_StoppedHSCP_8E29',
  'HLT_TechTrigHCALNoise',
  'HLT_TrackerCosmics',
  'HLT_ZeroBias',
  'HLT_ZeroBiasPixel_SingleTrack',
  'AlCa_EcalEta_8E29',
  'AlCa_EcalPhiSym',
  'AlCa_EcalPi0_8E29',
  'HLT_Activity_DT',
  'HLT_Activity_L1A',
  'HLT_Activity_PixelClusters',
  'HLT_BTagIP_Jet50U',
  'HLT_BTagMu_Jet10U',
  'HLT_CSCBeamHalo',
  'HLT_CSCBeamHaloOverlapRing1',
  'HLT_CSCBeamHaloOverlapRing2',
  'HLT_CSCBeamHaloRing2or3',
  'HLT_Photon15_Cleaned_L1R',
  'HLT_Photon20_Cleaned_L1R',
  'HLT_Activity_CSC',
  'HLT_Calibration',
  'HLT_DiJetAve15U_8E29',
  'HLT_DiJetAve30U_8E29',
  'HLT_DoubleEle5_SW_L1R',
  'HLT_DoubleJet15U_ForwardBackward',
  'HLT_DoubleLooseIsoTau15',
  'HLT_DoubleMu0',
  'HLT_DoubleMu3',
  'HLT_DoublePhoton10_L1R',
  'HLT_Mu0_L1MuOpen',
  'HLT_Mu0_Track0_Jpsi',
  'HLT_Mu3_L1MuOpen',
  'HLT_Mu3_Track0_Jpsi',
  'HLT_Mu5_L1MuOpen',
  'HLT_Mu5_Track0_Jpsi',
  'HLT_SelectEcalSpikes_L1R',
  'HLT_SelectEcalSpikesHighEt_L1R',
  'HLT_DoublePhoton5_L1R',
  'HLT_DoublePhoton4_eeRes_L1R',
  'HLT_DoublePhoton4_Jpsi_L1R',
  'HLT_DoublePhoton4_Upsilon_L1R',
  'HLT_L2Mu0',
  'HLT_L2Mu3',
  'HLT_L2DoubleMu0',
  'HLT_Mu0_L2Mu0',
  'HLT_Mu3_L2Mu0',
  'HLT_Mu5_L2Mu0',
  'HLT_DoublePhoton5_Jpsi_L1R',
  'HLT_DoublePhoton5_Upsilon_L1R',
  'HLT_Ele10_LW_EleId_L1R',
  'HLT_Ele10_LW_L1R',
  'HLT_Ele10_SW_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Ele15_SC10_LW_L1R',
  'HLT_Ele15_SW_L1R',
  'HLT_Ele15_SiStrip_L1R',
  'HLT_Ele20_LW_L1R',
  'HLT_Ele20_SW_L1R',
  'HLT_L1SingleEG2',
  'HLT_L1Jet10U_NoBPTX',
  'HLT_Activity_DT_Tuned',
  'HLT_L1Jet6U',
  'HLT_L1Jet6U_NoBPTX',
  'HLT_L1SingleCenJet',
  'HLT_L1SingleForJet',
  'HLT_L1SingleTauJet',
  'HLT_FwdJet20U',
  'HLT_GlobalRunHPDNoise',
  'HLT_HT100U',
  'HLT_L1MuOpen_AntiBPTX',
  'HLT_L2Mu5',
  'HLT_HcalNZS_8E29',
  'HLT_HcalPhiSym',
  'HLT_L1Tech_BSC_HighMultiplicity',
  'HLT_IsoMu3',
  'HLT_L1Tech_BSC_halo_forPhysicsBackground',
  'HLT_MultiVertex6',
  'HLT_MultiVertex8_L1ETT60',
  'HLT_PixelTracks_Multiplicity70',
  'HLT_SingleLooseIsoTau25',
  'HLT_SingleLooseIsoTau20_Trk5' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep triggerTriggerEventWithRefs_*_*_*',
      'keep *_hltFilterL2EcalIsolationDoubleLooseIsoTau15_*_*',
      'keep *_hltL1sSingleLooseIsoTau20_*_*',
      'keep *_hltFilterL2EcalIsolationSingleLooseIsoTau20_*_*',
      'keep *_hltIconeTau4Regional_*_*',
      'keep *_hltL1NonIsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1NonIso_*_*',
      'keep *_hltRpcRecHits_*_*',
      'keep *_hltIconeCentral1Regional_*_*',
      'keep *_hltMuonRPCDigis_*_*',
      'keep *_hltIterativeCone5CaloJets_*_*',
      'keep *_hltBSoftMuonL25BJetTagsUByDR_*_*',
      'keep *_hltBSoftMuonL25TagInfosU_*_*',
      'keep *_hltFilterL2EtCutDoubleLooseIsoTau15_*_*',
      'keep *_hltMCJetCorJetIcone5HF07_*_*',
      'keep *_hltL3MuonCandidates_*_*',
      'keep *_hltTowerMakerForMuons_*_*',
      'keep *_hltGtDigis_*_*',
      'keep *_hltBSoftMuonL25JetsU_*_*',
      'keep *_hltL1NonIsolatedElectronHcalIsol_*_*',
      'keep *_hltL1sDoubleLooseIsoTau15_*_*',
      'keep *_hltL1NonIsoSiStripElectronPixelSeeds_*_*',
      'keep *_hltBLifetimeL25JetsStartupU_*_*',
      'keep *_hltSiStripRawToClustersFacility_*_*',
      'keep *_hltBLifetimeRegionalCtfWithMaterialTracksStartupU_*_*',
      'keep *_hltSiPixelClusters_*_*',
      'keep *_hltL3Muons_*_*',
      'keep *_hltBLifetimeL3JetsStartupU_*_*',
      'keep *_hltIconeCentral3Regional_*_*',
      'keep *_hltL1IsoRecoEcalCandidate_*_*',
      'keep *_hltL1extraParticles_*_*',
      'keep *_hltMuonCSCDigis_*_*',
      'keep *_hltL3TrajectorySeed_*_*',
      'keep *_hltAlCaPhiSymStream_*_*',
      'keep *_hltL1IsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1Iso_*_*',
      'keep *_hltL2TauRelaxingIsolationSelector_*_*',
      'keep *_hltMet_*_*',
      'keep *_hltCscSegments_*_*',
      'keep *_hltBLifetimeL3AssociatorStartupU_*_*',
      'keep *_hltBLifetimeL25TagInfosStartupU_*_*',
      'keep *_hltCsc2DRecHits_*_*',
      'keep *_hltBSoftMuonL3BJetTagsUByDR_*_*',
      'keep *_hltIconeCentral4Regional_*_*',
      'keep *_hltL2MuonSeeds_*_*',
      'keep *_hltL2Muons_*_*',
      'keep *_hltBLifetimeL3BJetTagsStartupU_*_*',
      'keep *_hltBLifetimeL3TagInfosStartupU_*_*',
      'keep *_hltFilterL2EtCutSingleLooseIsoTau20_*_*',
      'keep *_hltPixelTracks_*_*',
      'keep *_hltL2TauNarrowConeIsolationProducer_*_*',
      'keep *_hltL3MuonIsolations_*_*',
      'keep *_hltL2MuonIsolations_*_*',
      'keep *_hltL1IsolatedElectronHcalIsol_*_*',
      'keep *_hltBLifetimeL25BJetTagsStartupU_*_*',
      'keep *_hltIconeCentral2Regional_*_*',
      'keep *_hltL1IsoSiStripElectronPixelSeeds_*_*',
      'keep *_hltL2MuonCandidates_*_*',
      'keep *_hltBSoftMuonL3TagInfosU_*_*',
      'keep *_hltOfflineBeamSpot_*_*',
      'keep *_hltL1NonIsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltIconeTau2Regional_*_*',
      'keep *_hltBLifetimeL25AssociatorStartupU_*_*',
      'keep *_hltL1NonIsoRecoEcalCandidate_*_*',
      'keep *_hltIconeTau1Regional_*_*',
      'keep *_hltL1IsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltL2TauJets_*_*',
      'keep *_hltDt4DSegments_*_*',
      'keep *_hltIconeTau3Regional_*_*',
      'keep *_hltHITCtfWithMaterialTracksHB8E29_*_*',
      'keep *_hltHITCtfWithMaterialTracksHE8E29_*_*',
      'keep *_hltHITIPTCorrectorHB8E29_*_*',
      'keep *_hltHITIPTCorrectorHE8E29_*_*',
      'keep *_hltHcalDigis_*_*',
      'keep *_hltIsolPixelTrackProdHB8E29_*_*',
      'keep *_hltIsolPixelTrackProdHE8E29_*_*',
      'keep *_hltAlCaEtaRecHitsFilter_*_*',
      'keep *_hltAlCaPi0RecHitsFilter_*_*',
      'keep *_hltMuTrackJpsiPixelTrackCands_*_*',
      'keep *_hltMuTrackJpsiCtfTrackCands_*_*' )
)
process.hltOutputHLTMON = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputHLTMON.root" ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring( 'HLT_Ele20_SW_L1R',
  'HLT_L1Mu14_L1SingleEG10',
  'HLT_StoppedHSCP_8E29',
  'HLT_BTagIP_Jet50U',
  'HLT_DoubleLooseIsoTau15',
  'HLT_SingleLooseIsoTau20',
  'HLT_DoublePhoton10_L1R',
  'HLT_Photon30_L1R_8E29',
  'HLT_Photon15_LooseEcalIso_Cleaned_L1R',
  'HLT_Photon15_TrackIso_Cleaned_L1R',
  'HLT_Photon10_Cleaned_L1R',
  'HLT_DoublePhoton5_Upsilon_L1R',
  'HLT_DoublePhoton5_Jpsi_L1R',
  'HLT_DoubleEle5_SW_L1R',
  'HLT_L1DoubleEG5',
  'HLT_Ele20_LW_L1R',
  'HLT_Ele15_SiStrip_L1R',
  'HLT_Ele15_SC10_LW_L1R',
  'HLT_Ele15_LW_L1R',
  'HLT_Ele10_LW_EleId_L1R',
  'HLT_Ele10_LW_L1R',
  'HLT_L1SingleEG8',
  'HLT_L1SingleEG5',
  'HLT_L1DoubleMuOpen',
  'HLT_L1Mu20',
  'HLT_L1Mu',
  'HLT_L1MuOpen',
  'HLT_HT100U',
  'HLT_MET100',
  'HLT_MET45',
  'HLT_L1_BptxXOR_BscMinBiasOR',
  'HLT_PixelTracks_Multiplicity85',
  'HLT_Jet15U_HcalNoiseFiltered',
  'HLT_L1MuOpen_DT',
  'HLT_Activity_Ecal_SC7',
  'HLT_Activity_Ecal_SC15',
  'HLT_Photon15_Cleaned_L1R',
  'HLT_Photon20_Cleaned_L1R',
  'HLT_Activity_CSC',
  'HLT_L1MET20',
  'HLT_QuadJet15U',
  'HLT_DiJetAve30U_8E29',
  'HLT_DiJetAve15U_8E29',
  'HLT_FwdJet20U',
  'HLT_Jet50U',
  'HLT_Jet30U',
  'HLT_Jet15U',
  'HLT_L1Jet10U',
  'HLT_Activity_DT',
  'HLT_Activity_PixelClusters',
  'HLT_Activity_L1A',
  'HLT_L2Mu0_NoVertex',
  'HLT_BTagMu_Jet10U',
  'HLT_DoubleMu3',
  'HLT_DoubleJet15U_ForwardBackward',
  'HLT_Ele10_SW_L1R',
  'HLT_Ele15_SW_L1R',
  'HLT_L2Mu5',
  'HLT_L1Jet10U_NoBPTX',
  'HLT_L1SingleEG2',
  'HLT_Activity_DT_Tuned',
  'HLT_L1Jet6U',
  'HLT_L1Jet6U_NoBPTX',
  'HLT_L1SingleCenJet',
  'HLT_L1SingleForJet',
  'HLT_L1SingleTauJet',
  'HLT_Mu0_L1MuOpen',
  'HLT_Mu0_Track0_Jpsi',
  'HLT_Mu3_L1MuOpen',
  'HLT_Mu3_Track0_Jpsi',
  'HLT_Mu5_L1MuOpen',
  'HLT_Mu5_Track0_Jpsi',
  'HLT_SelectEcalSpikes_L1R',
  'HLT_SelectEcalSpikesHighEt_L1R',
  'HLT_DoublePhoton4_Jpsi_L1R',
  'HLT_DoublePhoton4_Upsilon_L1R',
  'HLT_DoublePhoton4_eeRes_L1R',
  'HLT_DoublePhoton5_L1R',
  'HLT_L2DoubleMu0',
  'HLT_L2Mu0',
  'HLT_L2Mu3',
  'HLT_Mu0_L2Mu0',
  'HLT_Mu3_L2Mu0',
  'HLT_Mu5_L2Mu0',
  'HLT_L1MuOpen_AntiBPTX',
  'HLT_DoubleMu0',
  'HLT_Mu9',
  'HLT_Mu5',
  'HLT_Mu3',
  'HLT_IsoMu3',
  'HLT_L2Mu11',
  'HLT_L2Mu9',
  'HLT_TechTrigHCALNoise',
  'HLT_GlobalRunHPDNoise',
  'HLT_L1Tech_HCAL_HF',
  'AlCa_EcalEta_8E29',
  'AlCa_EcalPi0_8E29',
  'AlCa_EcalPhiSym',
  'HLT_HcalNZS_8E29',
  'HLT_HcalPhiSym',
  'HLT_IsoTrackHB_8E29',
  'HLT_IsoTrackHE_8E29',
  'HLT_L1Tech_RPC_TTU_RBst1_collisions',
  'HLT_TrackerCosmics',
  'HLT_RPCBarrelCosmics',
  'HLT_L1_BscMinBiasOR_BptxPlusORMinus',
  'HLT_L1Tech_BSC_halo',
  'HLT_L1Tech_BSC_HighMultiplicity',
  'HLT_CSCBeamHaloRing2or3',
  'HLT_CSCBeamHaloOverlapRing2',
  'HLT_CSCBeamHaloOverlapRing1',
  'HLT_CSCBeamHalo',
  'HLT_MinBiasPixel_DoubleIsoTrack5',
  'HLT_MinBiasPixel_DoubleTrack',
  'HLT_MinBiasPixel_SingleTrack',
  'HLT_ZeroBiasPixel_SingleTrack',
  'HLT_L1Tech_BSC_minBias',
  'HLT_L1Mu14_L1ETM30',
  'HLT_L1Mu14_L1SingleJet6U',
  'HLT_MultiVertex6',
  'HLT_MultiVertex8_L1ETT60',
  'HLT_PixelTracks_Multiplicity70',
  'HLT_L1Tech_BSC_halo_forPhysicsBackground',
  'HLT_SingleLooseIsoTau20_Trk5',
  'HLT_SingleLooseIsoTau25' ) ),
    outputCommands = cms.untracked.vstring( 'drop *_hlt*_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep triggerTriggerEventWithRefs_*_*_*',
      'keep *_hltL1IsoEgammaRegionalCkfTrackCandidates_*_*',
      'keep *_hltTowerMakerForAll_*_*',
      'keep *_hltL1NonIsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltL1NonIsolatedPhotonEcalIsol_*_*',
      'keep *_hltRpcRecHits_*_*',
      'keep *_hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated_*_*',
      'keep *_hltL1NonIsoEgammaRegionalPixelSeedGenerator_*_*',
      'keep *_hltBSoftMuonL3BJetTagsUByPt_*_*',
      'keep *_hltPixelVertices_*_*',
      'keep *_hltL1NonIsoEgammaRegionalCkfTrackCandidates_*_*',
      'keep *_hltBSoftMuonL25BJetTagsUByDR_*_*',
      'keep *_hltL3MuonCandidates_*_*',
      'keep *_hltBSoftMuonL25JetsU_*_*',
      'keep *_hltL3TrackCandidateFromL2_*_*',
      'keep *_hltSiStripRawToClustersFacility_*_*',
      'keep *_hltCkfL1NonIsoLargeWindowTrackCandidates_*_*',
      'keep *_hltSiStripClusters_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1Iso_*_*',
      'keep *_hltL2TauRelaxingIsolationSelector_*_*',
      'keep *_hltMet_*_*',
      'keep *_hltCorrectedHybridSuperClustersL1NonIsolated_*_*',
      'keep *_hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated_*_*',
      'keep *_hltBSoftMuonL3BJetTagsUByDR_*_*',
      'keep *_hltL2MuonSeeds_*_*',
      'keep *_hltL2MuonCandidatesNoVtx_*_*',
      'keep *_hltL2TauNarrowConeIsolationProducer_*_*',
      'keep *_hltL3MuonIsolations_*_*',
      'keep *_hltCtfL1IsoLargeWindowWithMaterialTracks_*_*',
      'keep *_hltGctDigis_*_*',
      'keep *_hltBLifetimeL25BJetTagsStartupU_*_*',
      'keep *_hltOfflineBeamSpot_*_*',
      'keep *_hltL1NonIsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltBLifetimeL25AssociatorStartupU_*_*',
      'keep *_hltL1IsoStartUpElectronPixelSeeds_*_*',
      'keep *_hltDt4DSegments_*_*',
      'keep *_hltPixelMatchLargeWindowElectronsL1NonIso_*_*',
      'keep *_hltMuonRPCDigis_*_*',
      'keep *_hltSiPixelRecHits_*_*',
      'keep *_hltBSoftMuonL25TagInfosU_*_*',
      'keep *_hltHoreco_*_*',
      'keep *_hltL3TrajectorySeedNoVtx_*_*',
      'keep *_hltDt1DRecHits_*_*',
      'keep *_hltL3MuonsNoVtx_*_*',
      'keep *_hltTowerMakerForMuons_*_*',
      'keep *_hltCorrectedHybridSuperClustersL1Isolated_*_*',
      'keep *_hltGtDigis_*_*',
      'keep *_hltL1NonIsoHLTClusterShape_*_*',
      'keep *_hltBLifetimeL25JetsStartupU_*_*',
      'keep *_hltBLifetimeRegionalCtfWithMaterialTracksStartupU_*_*',
      'keep *_hltL3Muons_*_*',
      'keep *_hltBLifetimeL3JetsStartupU_*_*',
      'keep *_hltL1IsoEgammaRegionalPixelSeedGenerator_*_*',
      'keep *_hltL1NonIsolatedPhotonHcalIsol_*_*',
      'keep *_hltL1extraParticles_*_*',
      'keep *_hltL3TrajectorySeed_*_*',
      'keep *_hltL1IsoLargeWindowElectronPixelSeeds_*_*',
      'keep *_hltL1NonIsoPhotonHollowTrackIsol_*_*',
      'keep *_hltL1IsolatedPhotonEcalIsol_*_*',
      'keep *_hltCscSegments_*_*',
      'keep *_hltBLifetimeL3AssociatorStartupU_*_*',
      'keep *_hltL1IsoEgammaRegionalCTFFinalFitWithMaterial_*_*',
      'keep *_hltBLifetimeL25TagInfosStartupU_*_*',
      'keep *_hltL1GtObjectMap_*_*',
      'keep *_hltL3TkTracksFromL2_*_*',
      'keep *_hltL2Muons_*_*',
      'keep *_hltBLifetimeL3BJetTagsStartupU_*_*',
      'keep *_hltL1IsoHLTClusterShape_*_*',
      'keep *_hltBLifetimeL3TagInfosStartupU_*_*',
      'keep *_hltPixelTracks_*_*',
      'keep *_hltL3MuonCandidatesNoVtx_*_*',
      'keep *_hltL2MuonIsolations_*_*',
      'keep *_hltCkfL1IsoLargeWindowTrackCandidates_*_*',
      'keep *_hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial_*_*',
      'keep *_hltL2MuonCandidates_*_*',
      'keep *_hltL1IsoPhotonHollowTrackIsol_*_*',
      'keep *_hltBSoftMuonL3TagInfosU_*_*',
      'keep *_hltL1IsolatedPhotonHcalIsol_*_*',
      'keep *_hltCtfL1NonIsoLargeWindowWithMaterialTracks_*_*',
      'keep *_hltL2TauJets_*_*',
      'keep *_hltFilterL2EtCutDoubleLooseIsoTau15_*_*',
      'keep *_hltFilterL2EcalIsolationDoubleLooseIsoTau15_*_*',
      'keep *_hltFilterL2EtCutSingleLooseIsoTau20_*_*',
      'keep *_hltL1sSingleLooseIsoTau20_*_*',
      'keep *_hltFilterL2EcalIsolationSingleLooseIsoTau20_*_*',
      'keep *_hltL1sDoubleLooseIsoTau15_*_*',
      'keep *_hltIconeTau1Regional_*_*',
      'keep *_hltIconeTau2Regional_*_*',
      'keep *_hltIconeTau3Regional_*_*',
      'keep *_hltIconeTau4Regional_*_*',
      'keep *_hltIconeCentral1Regional_*_*',
      'keep *_hltIconeCentral2Regional_*_*',
      'keep *_hltIconeCentral3Regional_*_*',
      'keep *_hltIconeCentral4Regional_*_*',
      'keep *_hltAlCaEtaRecHitsFilter_*_*',
      'keep *_hltAlCaPi0RecHitsFilter_*_*',
      'keep *_hltMuTrackJpsiCtfTracks_*_*',
      'keep *_hltMuTrackJpsiCtfTrackCands_*_*',
      'keep *_hltMuTrackJpsiTrackSeeds_*_*',
      'keep *_hltMuTrackJpsiPixelTrackCands_*_*',
      'keep *_hltMuTrackJpsiPixelTrackSelector_*_*',
      'keep SiPixelClusteredmNewDetSetVector_hltSiPixelClusters_*_*' )
)

process.HLTL1UnpackerSequence = cms.Sequence( process.hltGtDigis + process.hltGctDigis + process.hltL1GtObjectMap + process.hltL1extraParticles )
process.HLTBeamSpot = cms.Sequence( process.hltScalersRawToDigi + process.hltOnlineBeamSpot + process.hltOfflineBeamSpot )
process.HLTBeginSequenceBPTX = cms.Sequence( process.hltTriggerType + process.HLTL1UnpackerSequence + process.hltBPTXCoincidence + process.HLTBeamSpot )
process.HLTEndSequence = cms.Sequence( process.hltBoolEnd )
process.HLTDoLocalPixelSequence = cms.Sequence( process.hltSiPixelDigis + process.hltSiPixelClusters + process.hltSiPixelRecHits )
process.HLTEcalActivitySequence = cms.Sequence( process.hltEcalRawToRecHitFacility + process.hltESRawToRecHitFacility + process.hltEcalRegionalRestFEDs + process.hltEcalRegionalESRestFEDs + process.hltEcalRecHitAll + process.hltESRecHitAll + process.hltHybridSuperClustersActivity + process.hltCorrectedHybridSuperClustersActivity + process.hltMulti5x5BasicClustersActivity + process.hltMulti5x5SuperClustersActivity + process.hltMulti5x5SuperClustersWithPreshowerActivity + process.hltCorrectedMulti5x5SuperClustersWithPreshowerActivity + process.hltRecoEcalSuperClusterActivityCandidate + process.hltEcalActivitySuperClusterWrapper )
process.HLTBeginSequence = cms.Sequence( process.hltTriggerType + process.HLTL1UnpackerSequence + process.HLTBeamSpot )
process.HLTDoRegionalEgammaEcalSequenceLowPt = cms.Sequence( process.hltESRawToRecHitFacility + process.hltEcalRawToRecHitFacility + process.hltEcalRegionalEgammaFEDsLowPt + process.hltEcalRegionalEgammaRecHitLowPt + process.hltESRegionalEgammaRecHitLowPt )
process.HLTMulti5x5SuperClusterL1IsolatedLowPt = cms.Sequence( process.hltMulti5x5BasicClustersL1IsolatedLowPt + process.hltMulti5x5SuperClustersL1IsolatedLowPt + process.hltMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1IsolatedLowPt )
process.HLTL1IsolatedEcalClustersSequenceLowPt = cms.Sequence( process.hltHybridSuperClustersL1IsolatedLowPt + process.hltCorrectedHybridSuperClustersL1IsolatedLowPt + process.HLTMulti5x5SuperClusterL1IsolatedLowPt )
process.HLTMulti5x5SuperClusterL1NonIsolatedLowPt = cms.Sequence( process.hltMulti5x5BasicClustersL1NonIsolatedLowPt + process.hltMulti5x5SuperClustersL1NonIsolatedLowPt + process.hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTempLowPt + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedLowPt )
process.HLTL1NonIsolatedEcalClustersSequenceLowPt = cms.Sequence( process.hltHybridSuperClustersL1NonIsolatedLowPt + process.hltCorrectedHybridSuperClustersL1NonIsolatedTempLowPt + process.hltCorrectedHybridSuperClustersL1NonIsolatedLowPt + process.HLTMulti5x5SuperClusterL1NonIsolatedLowPt )
process.HLTEgammaR9ShapeSequenceLowPt = cms.Sequence( process.hltL1IsoR9shapeLowPt + process.hltL1NonIsoR9shapeLowPt )
process.HLTEgammaSelectEcalSpikesSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequenceLowPt + process.HLTL1IsolatedEcalClustersSequenceLowPt + process.HLTL1NonIsolatedEcalClustersSequenceLowPt + process.hltL1IsoRecoEcalCandidateLowPt + process.hltL1NonIsoRecoEcalCandidateLowPt + process.hltEgammaSelectEcalSpikesL1MatchFilterRegional + process.hltEgammaSelectEcalSpikesEtFilter + process.HLTEgammaR9ShapeSequenceLowPt + process.hltEgammaSelectEcalSpikesR9filter )
process.HLTDoRegionalEgammaEcalSequence = cms.Sequence( process.hltESRawToRecHitFacility + process.hltEcalRawToRecHitFacility + process.hltEcalRegionalEgammaFEDs + process.hltEcalRegionalEgammaRecHit + process.hltESRegionalEgammaRecHit )
process.HLTMulti5x5SuperClusterL1Isolated = cms.Sequence( process.hltMulti5x5BasicClustersL1Isolated + process.hltMulti5x5SuperClustersL1Isolated + process.hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated )
process.HLTL1IsolatedEcalClustersSequence = cms.Sequence( process.hltHybridSuperClustersL1Isolated + process.hltCorrectedHybridSuperClustersL1Isolated + process.HLTMulti5x5SuperClusterL1Isolated )
process.HLTMulti5x5SuperClusterL1NonIsolated = cms.Sequence( process.hltMulti5x5BasicClustersL1NonIsolated + process.hltMulti5x5SuperClustersL1NonIsolated + process.hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp + process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated )
process.HLTL1NonIsolatedEcalClustersSequence = cms.Sequence( process.hltHybridSuperClustersL1NonIsolated + process.hltCorrectedHybridSuperClustersL1NonIsolatedTemp + process.hltCorrectedHybridSuperClustersL1NonIsolated + process.HLTMulti5x5SuperClusterL1NonIsolated )
process.HLTEgammaSelectEcalSpikesHighEtSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltEgammaSelectEcalSpikesHighEtL1MatchFilterRegional + process.hltEgammaSelectEcalSpikesHighEtEtFilter + process.hltL1IsoR9shape + process.hltL1NonIsoR9shape + process.hltEgammaSelectEcalSpikesHighEtR9ShapeFilter )
process.HLTDoLocalHcalSequence = cms.Sequence( process.hltHcalDigis + process.hltHbhereco + process.hltHfreco + process.hltHoreco )
process.HLTDoCaloSequence = cms.Sequence( process.hltEcalRawToRecHitFacility + process.hltEcalRegionalRestFEDs + process.hltEcalRecHitAll + process.HLTDoLocalHcalSequence + process.hltTowerMakerForAll )
process.HLTRecoJetSequenceU = cms.Sequence( process.HLTDoCaloSequence + process.hltIterativeCone5CaloJets + process.hltMCJetCorJetIcone5HF07 )
process.HLTHcalNoiseSequence = cms.Sequence( process.hltHcalNoiseInfoProducer + process.hltHcalMETNoiseFilter )
process.HLTDoRegionalJetEcalSequence = cms.Sequence( process.hltEcalRawToRecHitFacility + process.hltEcalRegionalJetsFEDs + process.hltEcalRegionalJetsRecHit )
process.HLTRecoJetRegionalSequence = cms.Sequence( process.HLTDoRegionalJetEcalSequence + process.HLTDoLocalHcalSequence + process.hltTowerMakerForJets + process.hltIterativeCone5CaloJetsRegional + process.hltMCJetCorJetIcone5Regional )
process.HLTRecoMETSequence = cms.Sequence( process.HLTDoCaloSequence + process.hltMet )
process.HLTDoJet15UHTRecoSequence = cms.Sequence( process.hltJet15UHt )
process.HLTBeginSequenceAntiBPTX = cms.Sequence( process.hltTriggerType + process.HLTL1UnpackerSequence + process.hltBPTXAntiCoincidence + process.HLTBeamSpot )
process.HLTmuonlocalrecoSequence = cms.Sequence( process.hltMuonDTDigis + process.hltDt1DRecHits + process.hltDt4DSegments + process.hltMuonCSCDigis + process.hltCsc2DRecHits + process.hltCscSegments + process.hltMuonRPCDigis + process.hltRpcRecHits )
process.HLTL2muonrecoNocandSequence = cms.Sequence( process.HLTmuonlocalrecoSequence + process.hltL2MuonSeeds + process.hltL2Muons )
process.HLTL2muonrecoSequence = cms.Sequence( process.HLTL2muonrecoNocandSequence + process.hltL2MuonCandidates )
process.HLTL2muonisorecoSequence = cms.Sequence( process.hltEcalRawToRecHitFacility + process.hltEcalRegionalMuonsFEDs + process.hltEcalRegionalMuonsRecHit + process.HLTDoLocalHcalSequence + process.hltTowerMakerForMuons + process.hltL2MuonIsolations )
process.HLTDoLocalStripSequence = cms.Sequence( process.hltSiStripRawToClustersFacility + process.hltSiStripClusters )
process.HLTL3muonTkCandidateSequence = cms.Sequence( process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL3TrajectorySeed + process.hltL3TrackCandidateFromL2 )
process.HLTL3muonrecoNocandSequence = cms.Sequence( process.HLTL3muonTkCandidateSequence + process.hltL3TkTracksFromL2 + process.hltL3Muons )
process.HLTL3muonrecoSequence = cms.Sequence( process.HLTL3muonrecoNocandSequence + process.hltL3MuonCandidates )
process.HLTL3muonisorecoSequence = cms.Sequence( process.hltPixelTracks + process.hltL3MuonIsolations )
process.HLTMuTrackJpsiPixelRecoSequence = cms.Sequence( process.HLTDoLocalPixelSequence + process.hltPixelTracks + process.hltMuTrackJpsiPixelTrackSelector + process.hltMuTrackJpsiPixelTrackCands )
process.HLTMuTrackJpsiTrackRecoSequence = cms.Sequence( process.HLTDoLocalStripSequence + process.hltMuTrackJpsiTrackSeeds + process.hltMuTrackJpsiCkfTrackCandidates + process.hltMuTrackJpsiCtfTracks + process.hltMuTrackJpsiCtfTrackCands )
process.HLTEgammaR9ShapeSequence = cms.Sequence( process.hltL1IsoR9shape + process.hltL1NonIsoR9shape )
process.HLTDoLocalHcalWithoutHOSequence = cms.Sequence( process.hltHcalDigis + process.hltHbhereco + process.hltHfreco )
process.HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter )
process.HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence = cms.Sequence( process.hltCkfL1IsoLargeWindowTrackCandidates + process.hltCtfL1IsoLargeWindowWithMaterialTracks + process.hltPixelMatchLargeWindowElectronsL1Iso )
process.HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence = cms.Sequence( process.hltCkfL1NonIsoLargeWindowTrackCandidates + process.hltCtfL1NonIsoLargeWindowWithMaterialTracks + process.hltPixelMatchLargeWindowElectronsL1NonIso )
process.HLTSingleElectronEt10L1NonIsoHLTEleIdSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdR9ShapeFilter + process.hltL1IsoHLTClusterShape + process.hltL1NonIsoHLTClusterShape + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter + process.HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence + process.HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter + process.hltElectronL1IsoLargeWindowDetaDphi + process.hltElectronL1NonIsoLargeWindowDetaDphi + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter )
process.HLTSingleElectronEt10L1NonIsoHLTnonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoStartUpElectronPixelSeeds + process.hltL1NonIsoStartUpElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter )
process.HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter )
process.HLTSingleElectronEt15L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronEt15R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoStartUpElectronPixelSeeds + process.hltL1NonIsoStartUpElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter )
process.HLTSingleElectronLWEt20L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoLargeWindowElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt20PixelMatchFilter )
process.HLTSingleElectronEt20L1NonIsoHLTnonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronEt20R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoStartUpElectronPixelSeeds + process.hltL1NonIsoStartUpElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter )
process.HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoDoubleElectronEt5R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoStartUpElectronPixelSeeds + process.hltL1NonIsoStartUpElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter )
process.HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter + process.hltL1IsoR9shape + process.hltL1NonIsoR9shape + process.hltL1NonIsoHLTNonIsoSinglePhotonEt10R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter )
process.HLTSinglePhoton15CleanL1NonIsolatedHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedL1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedEtFilter + process.hltL1IsoR9shape + process.hltL1NonIsoR9shape + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedR9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15CleanedHcalIsolFilter )
process.HLTDoLocalTrackerSequence = cms.Sequence( process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence )
process.HLTL1IsoEgammaRegionalRecoTrackerSequence = cms.Sequence( process.hltL1IsoEgammaRegionalPixelSeedGenerator + process.hltL1IsoEgammaRegionalCkfTrackCandidates + process.hltL1IsoEgammaRegionalCTFFinalFitWithMaterial )
process.HLTL1NonIsoEgammaRegionalRecoTrackerSequence = cms.Sequence( process.hltL1NonIsoEgammaRegionalPixelSeedGenerator + process.hltL1NonIsoEgammaRegionalCkfTrackCandidates + process.hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial )
process.HLTSinglePhoton15L1NonIsolatedHLTHTISequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional + process.hltL1NonIsoSinglePhotonEt15HTIEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoSinglePhotonEt15HTIR9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter + process.HLTDoLocalTrackerSequence + process.HLTL1IsoEgammaRegionalRecoTrackerSequence + process.HLTL1NonIsoEgammaRegionalRecoTrackerSequence + process.hltL1IsoPhotonHollowTrackIsol + process.hltL1NonIsoPhotonHollowTrackIsol + process.hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter )
process.HLTSinglePhoton15L1NonIsolatedHLTLEISequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional + process.hltL1NonIsoSinglePhotonEt15LEIEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoSinglePhotonEt15LEIR9ShapeFilter + process.hltL1IsolatedPhotonEcalIsol + process.hltL1NonIsolatedPhotonEcalIsol + process.hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter )
process.HLTSinglePhoton20CleanL1NonIsolatedHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedL1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedR9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoSinglePhotonEt20CleanedHcalIsolFilter )
process.HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter + process.hltL1IsoR9shape + process.hltL1NonIsoR9shape + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter )
process.HLTDoublePhotonEt4eeResSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequenceLowPt + process.HLTL1IsolatedEcalClustersSequenceLowPt + process.HLTL1NonIsolatedEcalClustersSequenceLowPt + process.hltL1IsoRecoEcalCandidateLowPt + process.hltL1NonIsoRecoEcalCandidateLowPt + process.hltL1NonIsoDoublePhotonEt4eeResL1MatchFilterRegional + process.hltL1NonIsoDoublePhotonEt4eeResEtFilter + process.HLTEgammaR9ShapeSequenceLowPt + process.hltL1NonIsoHLTNonIsoDoublePhotonEt4eeResR9ShapeFilter + process.hltL1IsoHLTClusterShapeLowPt + process.hltL1NonIsoHLTClusterShapeLowPt + process.hltL1NonIsoDoublePhotonEt4eeResClusterShapeFilter + process.hltL1IsolatedPhotonEcalIsolLowPt + process.hltL1NonIsolatedPhotonEcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4eeResEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsolLowPt + process.hltL1NonIsolatedElectronHcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4eeResHcalIsolFilter + process.hltL1NonIsoDoublePhotonEt4eeResPMMassFilter )
process.HLTDoublePhotonEt4JpsiSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequenceLowPt + process.HLTL1IsolatedEcalClustersSequenceLowPt + process.HLTL1NonIsolatedEcalClustersSequenceLowPt + process.hltL1IsoRecoEcalCandidateLowPt + process.hltL1NonIsoRecoEcalCandidateLowPt + process.hltL1NonIsoDoublePhotonEt4JpsiL1MatchFilterRegional + process.hltL1NonIsoDoublePhotonEt4JpsiEtFilter + process.HLTEgammaR9ShapeSequenceLowPt + process.hltL1NonIsoHLTNonIsoDoublePhotonEt4JpsiR9ShapeFilter + process.hltL1IsoHLTClusterShapeLowPt + process.hltL1NonIsoHLTClusterShapeLowPt + process.hltL1NonIsoDoublePhotonEt4JpsiClusterShapeFilter + process.hltL1IsolatedPhotonEcalIsolLowPt + process.hltL1NonIsolatedPhotonEcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4JpsiEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsolLowPt + process.hltL1NonIsolatedElectronHcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4JpsiHcalIsolFilter + process.hltL1NonIsoDoublePhotonEt4JpsiPMMassFilter )
process.HLTDoublePhotonEt4UpsSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequenceLowPt + process.HLTL1IsolatedEcalClustersSequenceLowPt + process.HLTL1NonIsolatedEcalClustersSequenceLowPt + process.hltL1IsoRecoEcalCandidateLowPt + process.hltL1NonIsoRecoEcalCandidateLowPt + process.hltL1NonIsoDoublePhotonEt4UpsL1MatchFilterRegional + process.hltL1NonIsoDoublePhotonEt4UpsEtFilter + process.HLTEgammaR9ShapeSequenceLowPt + process.hltL1NonIsoHLTNonIsoDoublePhotonEt4UpsR9ShapeFilter + process.hltL1IsoHLTClusterShapeLowPt + process.hltL1NonIsoHLTClusterShapeLowPt + process.hltL1NonIsoDoublePhotonEt4UpsClusterShapeFilter + process.hltL1IsolatedPhotonEcalIsolLowPt + process.hltL1NonIsolatedPhotonEcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4UpsEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsolLowPt + process.hltL1NonIsolatedElectronHcalIsolLowPt + process.hltL1NonIsoDoublePhotonEt4UpsHcalIsolFilter + process.hltL1NonIsoDoublePhotonEt4UpsPMMassFilter )
process.HLTDoublePhotonEt5JpsiSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional + process.hltL1NonIsoDoublePhotonEt5JpsiEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5JpsiR9ShapeFilter + process.hltL1IsoHLTClusterShape + process.hltL1NonIsoHLTClusterShape + process.hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter + process.hltL1IsolatedPhotonEcalIsol + process.hltL1NonIsolatedPhotonEcalIsol + process.hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter + process.hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter )
process.HLTDoublePhotonEt5UpsSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional + process.hltL1NonIsoDoublePhotonEt5UpsEtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5UpsR9ShapeFilter + process.hltL1IsoHLTClusterShape + process.hltL1NonIsoHLTClusterShape + process.hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter + process.hltL1IsolatedPhotonEcalIsol + process.hltL1NonIsolatedPhotonEcalIsol + process.hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter + process.hltL1NonIsoDoublePhotonEt5UpsPMMassFilter )
process.HLTDoublePhotonEt5L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoDoublePhotonEt5HcalIsolFilter )
process.HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoDoublePhotonEt10R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedPhotonHcalIsol + process.hltL1NonIsolatedPhotonHcalIsol + process.hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter )
process.HLTCaloTausCreatorRegionalSequence = cms.Sequence( process.HLTDoRegionalJetEcalSequence + process.HLTDoLocalHcalSequence + process.hltTowerMakerForJets + process.hltCaloTowersTau1Regional + process.hltIconeTau1Regional + process.hltCaloTowersTau2Regional + process.hltIconeTau2Regional + process.hltCaloTowersTau3Regional + process.hltIconeTau3Regional + process.hltCaloTowersTau4Regional + process.hltIconeTau4Regional + process.hltCaloTowersCentral1Regional + process.hltIconeCentral1Regional + process.hltCaloTowersCentral2Regional + process.hltIconeCentral2Regional + process.hltCaloTowersCentral3Regional + process.hltIconeCentral3Regional + process.hltCaloTowersCentral4Regional + process.hltIconeCentral4Regional )
process.HLTL2TauJetsSequence = cms.Sequence( process.HLTCaloTausCreatorRegionalSequence + process.hltL2TauJets )
process.HLTL2TauEcalIsolationSequence = cms.Sequence( process.hltL2TauNarrowConeIsolationProducer + process.hltL2TauRelaxingIsolationSelector )
process.HLTRecopixelvertexingSequence = cms.Sequence( process.hltPixelTracks + process.hltPixelVertices )
process.HLTL25TauTrackReconstructionSequence = cms.Sequence( process.HLTDoLocalStripSequence + process.hltL25TauPixelSeeds + process.hltL25TauCkfTrackCandidates + process.hltL25TauCtfWithMaterialTracks )
process.HLTL25TauTrackIsolationSequence = cms.Sequence( process.hltL25TauJetTracksAssociator + process.hltL25TauConeIsolation + process.hltL25TauLeadingTrackPtCutSelector )
process.HLTBTagIPSequenceL25StartupU = cms.Sequence( process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingSequence + process.hltSelector4JetsU + process.hltBLifetimeL25JetsStartupU + process.hltBLifetimeL25AssociatorStartupU + process.hltBLifetimeL25TagInfosStartupU + process.hltBLifetimeL25BJetTagsStartupU )
process.HLTBTagIPSequenceL3StartupU = cms.Sequence( process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltBLifetimeL3JetsStartupU + process.hltBLifetimeRegionalPixelSeedGeneratorStartupU + process.hltBLifetimeRegionalCkfTrackCandidatesStartupU + process.hltBLifetimeRegionalCtfWithMaterialTracksStartupU + process.hltBLifetimeL3AssociatorStartupU + process.hltBLifetimeL3TagInfosStartupU + process.hltBLifetimeL3BJetTagsStartupU )
process.HLTBTagMuSequenceL25U = cms.Sequence( process.HLTL2muonrecoNocandSequence + process.hltSelector4JetsU + process.hltBSoftMuonL25JetsU + process.hltBSoftMuonL25TagInfosU + process.hltBSoftMuonL25BJetTagsUByDR )
process.HLTBTagMuSequenceL3U = cms.Sequence( process.HLTL3muonrecoNocandSequence + process.hltBSoftMuonL3TagInfosU + process.hltBSoftMuonL3BJetTagsUByPt + process.hltBSoftMuonL3BJetTagsUByDR )
process.HLTPixelTrackingForMinBiasSequence = cms.Sequence( process.hltPixelTracksForMinBias )
process.HLTRecopixelvertexingForMultiVertexSequence = cms.Sequence( process.hltPixelTracks + process.hltPixelVerticesForMultiVertex )
process.HLTL2HcalIsolTrackSequenceHE = cms.Sequence( process.HLTDoLocalPixelSequence + process.hltHITPixelTracksHB + process.hltHITPixelTracksHE + process.hltHITPixelVerticesHE )
process.HLTL2HcalIsolTrackSequenceHB = cms.Sequence( process.HLTDoLocalPixelSequence + process.hltHITPixelTracksHB + process.hltHITPixelVerticesHB )
process.HLTBeginSequenceNZS = cms.Sequence( process.hltTriggerType + process.hltL1EventNumberNZS + process.HLTL1UnpackerSequence + process.hltBPTXCoincidence + process.HLTBeamSpot )
process.HLTDoRegionalPi0EtaSequence = cms.Sequence( process.hltESRawToRecHitFacility + process.hltEcalRawToRecHitFacility + process.hltEcalRegionalPi0EtaFEDs + process.hltESRegionalPi0EtaRecHit + process.hltEcalRegionalPi0EtaRecHit )
process.HLTRecopixelvertexingForHighMultSequence = cms.Sequence( process.hltPixelTracksForHighMult + process.hltPixelVerticesForHighMult )
process.HLTL2muonrecoSequenceNoVtx = cms.Sequence( process.HLTL2muonrecoNocandSequence + process.hltL2MuonCandidatesNoVtx )

process.HLTriggerFirstPath = cms.Path( process.hltGetRaw + process.HLTBeginSequenceBPTX + process.hltPreFirstPath + process.hltBoolFirstPath )
process.HLT_Activity_L1A = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityL1A + process.hltLevel1Activity + process.HLTEndSequence )
process.HLT_Activity_PixelClusters = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityPixelClusters + process.HLTDoLocalPixelSequence + process.hltPixelActivityFilter + process.HLTEndSequence )
process.HLT_Activity_CSC = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityCSC + process.hltMuonCSCDigis + process.hltCSCActivityFilter + process.HLTEndSequence )
process.HLT_Activity_DT = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityDT + process.hltMuonDTDigis + process.hltDTTFUnpacker + process.hltDTActivityFilter + process.HLTEndSequence )
process.HLT_Activity_DT_Tuned = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityDTTuned + process.hltMuonDTDigis + process.hltDTTFUnpacker + process.hltDTActivityFilterTuned + process.HLTEndSequence )
process.HLT_Activity_Ecal_SC7 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityEcalSC7 + process.HLTEcalActivitySequence + process.hltEgammaSelectEcalSuperClustersActivityFilterSC7 + process.hltEgammaEcalActivityR9Shape + process.hltEgammaEcalActivityR9ShapeFilterSC7 + process.HLTEndSequence )
process.HLT_Activity_Ecal_SC15 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreActivityEcalSC15 + process.HLTEcalActivitySequence + process.hltEgammaSelectEcalSuperClustersActivityFilterSC15 + process.hltEgammaEcalActivityR9Shape + process.hltEgammaEcalActivityR9ShapeFilterSC15 + process.HLTEndSequence )
process.HLT_SelectEcalSpikes_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG2 + process.hltPreSelectEcalSpikesL1R + process.HLTEgammaSelectEcalSpikesSequence + process.HLTEndSequence )
process.HLT_SelectEcalSpikesHighEt_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreSelectEcalSpikesHighEtL1R + process.HLTEgammaSelectEcalSpikesHighEtSequence + process.HLTEndSequence )
process.HLT_L1SingleForJet = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleForJet + process.hltPreL1SingleForJet_BPTX + process.HLTEndSequence )
process.HLT_L1SingleCenJet = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleCenJet + process.hltPreL1SingleCenJet_BPTX + process.HLTEndSequence )
process.HLT_L1SingleTauJet = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleTauJet + process.hltPreL1SingleTauJet_BPTX + process.HLTEndSequence )
process.HLT_L1Jet6U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Jet6U + process.hltPreL1Jet6U_BPTX + process.HLTEndSequence )
process.HLT_L1Jet6U_NoBPTX = cms.Path( process.HLTBeginSequence + process.hltL1sL1Jet6U + process.hltPreL1Jet6U + process.HLTEndSequence )
process.HLT_L1Jet10U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Jet10U + process.hltPreL1Jet10U_BPTX + process.HLTEndSequence )
process.HLT_L1Jet10U_NoBPTX = cms.Path( process.HLTBeginSequence + process.hltL1sL1Jet10U + process.hltPreL1Jet10U + process.HLTEndSequence )
process.HLT_Jet15U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sJet15U + process.hltPreJet15U + process.HLTRecoJetSequenceU + process.hlt1jet15U + process.HLTEndSequence )
process.HLT_Jet15U_HcalNoiseFiltered = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sJet15U + process.hltPreJet15UHcalNoiseFiltered + process.HLTRecoJetSequenceU + process.hlt1jet15U + process.HLTHcalNoiseSequence + process.HLTEndSequence )
process.HLT_Jet30U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sJet30U + process.hltPreJet30U + process.HLTRecoJetSequenceU + process.hlt1jet30U + process.HLTEndSequence )
process.HLT_Jet50U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sJet50U + process.hltPreJet50U + process.HLTRecoJetSequenceU + process.hlt1jet50U + process.HLTEndSequence )
process.HLT_FwdJet20U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sFwdJet20U + process.hltPreFwdJet20U + process.HLTRecoJetSequenceU + process.hltRapGap20U + process.HLTEndSequence )
process.HLT_DiJetAve15U_8E29 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sDiJetAve15U8E29 + process.hltPreDiJetAve15U8E29 + process.HLTDoCaloSequence + process.hltIterativeCone5CaloJets + process.hltDiJetAve15U + process.HLTEndSequence )
process.HLT_DiJetAve30U_8E29 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sDiJetAve30U8E29 + process.hltPreDiJetAve30U8E29 + process.HLTDoCaloSequence + process.hltIterativeCone5CaloJets + process.hltDiJetAve30U + process.HLTEndSequence )
process.HLT_DiJetAve50U_8E29 = cms.Path( process.HLTBeginSequence + process.hltL1sDiJetAve50U8E29 + process.hltPreDiJetAve50U8E29 + process.HLTDoCaloSequence + process.hltIterativeCone5CaloJets + process.hltDiJetAve50U + process.HLTEndSequence )
process.HLT_DoubleJet15U_ForwardBackward = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sDoubleJet15UForwardBackward + process.hltPreDoubleJet15UForwardBackward + process.HLTRecoJetRegionalSequence + process.hltDoubleJet15UForwardBackward + process.HLTEndSequence )
process.HLT_QuadJet15U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sQuadJet15U + process.hltPreQuadJet15U + process.HLTRecoJetSequenceU + process.hlt4jet15U + process.HLTEndSequence )
process.HLT_L1MET20 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1MET20 + process.hltPreL1MET20 + process.HLTEndSequence )
process.HLT_MET45 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sMET45 + process.hltPreMET45 + process.HLTRecoMETSequence + process.hlt1MET45 + process.HLTEndSequence )
process.HLT_MET100 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sMET100 + process.hltPreMET100 + process.HLTRecoMETSequence + process.hlt1MET100 + process.HLTEndSequence )
process.HLT_HT100U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sHT100 + process.hltPreHT100 + process.HLTRecoJetSequenceU + process.HLTDoJet15UHTRecoSequence + process.hltHT100U + process.HLTEndSequence )
process.HLT_L1MuOpen = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0 + process.hltPreL1MuOpen_BPTX + process.hltL1MuOpenL1Filtered0 + process.HLTEndSequence )
process.HLT_L1MuOpen_DT = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0 + process.hltPreL1MuOpenDT + process.hltL1MuOpenL1FilteredDT + process.HLTEndSequence )
process.HLT_L1MuOpen_AntiBPTX = cms.Path( process.HLTBeginSequenceAntiBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0 + process.hltPreL1MuOpen_AntiBPTX + process.hltL1MuOpenL1Filtered0 + process.HLTEndSequence )
process.HLT_L1Mu = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Mu + process.hltPreL1Mu + process.hltL1MuL1Filtered0 + process.HLTEndSequence )
process.HLT_L1Mu20 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu20 + process.hltPreL1Mu20 + process.hltL1Mu20L1Filtered20 + process.HLTEndSequence )
process.HLT_L2Mu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreL2Mu0 + process.hltL1SingleMu0L1Filtered0 + process.HLTL2muonrecoSequence + process.hltL2Mu0L2Filtered0 + process.HLTEndSequence )
process.HLT_L2Mu3 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreL2Mu3 + process.hltL1SingleMu0L1Filtered0 + process.HLTL2muonrecoSequence + process.hltSingleMu3L2Filtered3 + process.HLTEndSequence )
process.HLT_L2Mu5 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreL2Mu5 + process.hltL1SingleMu0L1Filtered0 + process.HLTL2muonrecoSequence + process.hltL2Mu5L2Filtered5 + process.HLTEndSequence )
process.HLT_L2Mu9 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu7 + process.hltPreL2Mu9 + process.hltL1SingleMu7L1Filtered0 + process.HLTL2muonrecoSequence + process.hltL2Mu9L2Filtered9 + process.HLTEndSequence )
process.HLT_L2Mu11 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu7 + process.hltPreL2Mu11 + process.hltL1SingleMu7L1Filtered0 + process.HLTL2muonrecoSequence + process.hltL2Mu11L2Filtered11 + process.HLTEndSequence )
process.HLT_IsoMu3 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu3 + process.hltPreIsoMu3 + process.hltSingleMuIsoL1Filtered3 + process.HLTL2muonrecoSequence + process.hltSingleMuIsoL2PreFiltered3 + process.HLTL2muonisorecoSequence + process.hltSingleMuIsoL2IsoFiltered3 + process.HLTL3muonrecoSequence + process.hltSingleMuIsoL3PreFiltered3 + process.HLTL3muonisorecoSequence + process.hltSingleMuIsoL3IsoFiltered3 + process.HLTEndSequence )
process.HLT_Mu3 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreMu3 + process.hltL1SingleMu0L1Filtered0 + process.HLTL2muonrecoSequence + process.hltSingleMu3L2Filtered3 + process.HLTL3muonrecoSequence + process.hltSingleMu3L3Filtered3 + process.HLTEndSequence )
process.HLT_Mu5 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu3 + process.hltPreMu5 + process.hltL1SingleMu3L1Filtered0 + process.HLTL2muonrecoSequence + process.hltSingleMu5L2Filtered4 + process.HLTL3muonrecoSequence + process.hltSingleMu5L3Filtered5 + process.HLTEndSequence )
process.HLT_Mu9 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu7 + process.hltPreMu9 + process.hltL1SingleMu7L1Filtered0 + process.HLTL2muonrecoSequence + process.hltSingleMu9L2Filtered7 + process.HLTL3muonrecoSequence + process.hltSingleMu9L3Filtered9 + process.HLTEndSequence )
process.HLT_L1DoubleMuOpen = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreL1DoubleMuOpen + process.hltDoubleMuLevel1PathL1OpenFiltered + process.HLTEndSequence )
process.HLT_L1DoubleMuOpen_Tight = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreL1DoubleMuOpenTight + process.hltCscTfDigis + process.hltL1DoubleMuOpenTightL1Filtered + process.HLTEndSequence )
process.HLT_L2DoubleMu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreL2DoubleMu0 + process.hltDiMuonL1Filtered0 + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered0 + process.HLTEndSequence )
process.HLT_DoubleMu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreDoubleMu0 + process.hltDiMuonL1Filtered0 + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered0 + process.HLTL3muonrecoSequence + process.hltDiMuonL3PreFiltered0 + process.HLTEndSequence )
process.HLT_DoubleMu3 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMu3 + process.hltPreDoubleMu3 + process.hltDiMuonL1Filtered + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered + process.HLTL3muonrecoSequence + process.hltDiMuonL3PreFiltered + process.HLTEndSequence )
process.HLT_Mu0_L1MuOpen = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu0L1MuOpen + process.hltMu0L1MuOpenL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu0L1MuOpenL2Filtered0 + process.HLTL3muonrecoSequence + process.hltMu0L1MuOpenL3Filtered0 + process.HLTEndSequence )
process.HLT_Mu3_L1MuOpen = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu3L1MuOpen + process.hltMu3L1MuOpenL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu3L1MuOpenL2Filtered0 + process.HLTL3muonrecoSequence + process.hltMu3L1MuOpenL3Filtered3 + process.HLTEndSequence )
process.HLT_Mu5_L1MuOpen = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu5L1MuOpen + process.hltMu5L1MuOpenL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu5L1MuOpenL2Filtered0 + process.HLTL3muonrecoSequence + process.hltMu5L1MuOpenL3Filtered5 + process.HLTEndSequence )
process.HLT_Mu0_L2Mu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu0L2Mu0 + process.hltDiMuonL1Filtered0 + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered0 + process.HLTL3muonrecoSequence + process.hltMu0L2Mu0L3Filtered0 + process.HLTEndSequence )
process.HLT_Mu3_L2Mu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu3L2Mu0 + process.hltDiMuonL1Filtered0 + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered0 + process.HLTL3muonrecoSequence + process.hltMu3L2Mu0L3Filtered3 + process.HLTEndSequence )
process.HLT_Mu5_L2Mu0 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1DoubleMuOpen + process.hltPreMu5L2Mu0 + process.hltDiMuonL1Filtered0 + process.HLTL2muonrecoSequence + process.hltDiMuonL2PreFiltered0 + process.HLTL3muonrecoSequence + process.hltMu5L2Mu0L3Filtered5 + process.HLTEndSequence )
process.HLT_Mu0_Track0_Jpsi = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreMu0Track0Jpsi + process.hltMu0TrackJpsiL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu0TrackJpsiL2Filtered0 + process.HLTL3muonrecoSequence + process.hltMu0TrackJpsiL3Filtered0 + process.HLTMuTrackJpsiPixelRecoSequence + process.hltMu0TrackJpsiPixelMassFiltered + process.HLTMuTrackJpsiTrackRecoSequence + process.hltMu0TrackJpsiTrackMassFiltered + process.HLTEndSequence )
process.HLT_Mu3_Track0_Jpsi = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 + process.hltPreMu3Track0Jpsi + process.hltMu3TrackJpsiL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu3TrackJpsiL2Filtered3 + process.HLTL3muonrecoSequence + process.hltMu3TrackJpsiL3Filtered3 + process.HLTMuTrackJpsiPixelRecoSequence + process.hltMu3TrackJpsiPixelMassFiltered + process.HLTMuTrackJpsiTrackRecoSequence + process.hltMu3TrackJpsiTrackMassFiltered + process.HLTEndSequence )
process.HLT_Mu5_Track0_Jpsi = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1SingleMu3 + process.hltPreMu5Track0Jpsi + process.hltMu5TrackJpsiL1Filtered0 + process.HLTL2muonrecoSequence + process.hltMu5TrackJpsiL2Filtered4 + process.HLTL3muonrecoSequence + process.hltMu5TrackJpsiL3Filtered5 + process.HLTMuTrackJpsiPixelRecoSequence + process.hltMu5TrackJpsiPixelMassFiltered + process.HLTMuTrackJpsiTrackRecoSequence + process.hltMu5TrackJpsiTrackMassFiltered + process.HLTEndSequence )
process.HLT_L1SingleEG2 = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG2 + process.hltPreL1SingleEG2 + process.HLTEndSequence )
process.HLT_L1SingleEG5 = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreL1SingleEG5 + process.HLTEndSequence )
process.HLT_L1SingleEG8 = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8 + process.hltPreL1SingleEG8 + process.HLTEndSequence )
process.HLT_L1DoubleEG5 = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG5 + process.hltPreL1DoubleEG5 + process.HLTEndSequence )
process.HLT_Ele10_LW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle10LWL1R + process.HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Ele10_LW_EleId_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle10LWEleIdL1R + process.HLTSingleElectronEt10L1NonIsoHLTEleIdSequence + process.HLTEndSequence )
process.HLT_Ele10_SW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle10SWL1R + process.HLTSingleElectronEt10L1NonIsoHLTnonIsoSequence + process.HLTEndSequence )
process.HLT_Ele15_LW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle15LWL1R + process.HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Ele15_SW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle15SWL1R + process.HLTSingleElectronEt15L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Ele15_SC10_LW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle15SC10LWL1R + process.HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10 + process.hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10R9ShapeFilter + process.HLTEndSequence )
process.HLT_Ele15_SiStrip_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPreEle15SiStripL1R + process.HLTDoRegionalEgammaEcalSequence + process.HLTL1IsolatedEcalClustersSequence + process.HLTL1NonIsolatedEcalClustersSequence + process.hltL1IsoRecoEcalCandidate + process.hltL1NonIsoRecoEcalCandidate + process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional + process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter + process.HLTEgammaR9ShapeSequence + process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15R9ShapeFilter + process.HLTDoLocalHcalWithoutHOSequence + process.hltL1IsolatedElectronHcalIsol + process.hltL1NonIsolatedElectronHcalIsol + process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter + process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL1IsoSiStripElectronPixelSeeds + process.hltL1NonIsoSiStripElectronPixelSeeds + process.hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter + process.HLTEndSequence )
process.HLT_Ele20_LW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8 + process.hltPreEle20LWL1R + process.HLTSingleElectronLWEt20L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Ele20_SW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8 + process.hltPreEle20SWL1R + process.HLTSingleElectronEt20L1NonIsoHLTnonIsoSequence + process.HLTEndSequence )
process.HLT_DoubleEle5_SW_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG5 + process.hltPreDoubleEle5SWL1R + process.HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence + process.HLTEndSequence )
process.HLT_Photon10_Cleaned_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPrePhoton10L1R + process.HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Photon15_Cleaned_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPrePhoton15CleanedL1R + process.HLTSinglePhoton15CleanL1NonIsolatedHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Photon15_TrackIso_Cleaned_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPrePhoton15HTIL1R + process.HLTSinglePhoton15L1NonIsolatedHLTHTISequence + process.HLTEndSequence )
process.HLT_Photon15_LooseEcalIso_Cleaned_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPrePhoton15LEIL1R + process.HLTSinglePhoton15L1NonIsolatedHLTLEISequence + process.HLTEndSequence )
process.HLT_Photon20_Cleaned_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8 + process.hltPrePhoton20CleanedL1R + process.HLTSinglePhoton20CleanL1NonIsolatedHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_Photon30_L1R_8E29 = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG5 + process.hltPrePhoton30L1R8E29 + process.HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence + process.hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29 + process.HLTEndSequence )
process.HLT_DoublePhoton4_eeRes_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG2 + process.hltPreDoublePhoton4eeResL1R + process.HLTDoublePhotonEt4eeResSequence + process.HLTEndSequence )
process.HLT_DoublePhoton4_Jpsi_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG2 + process.hltPreDoublePhoton4JpsiL1R + process.HLTDoublePhotonEt4JpsiSequence + process.HLTEndSequence )
process.HLT_DoublePhoton4_Upsilon_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG2 + process.hltPreDoublePhoton4UpsL1R + process.HLTDoublePhotonEt4UpsSequence + process.HLTEndSequence )
process.HLT_DoublePhoton5_Jpsi_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8orL1DoubleEG5 + process.hltPreDoublePhoton5JpsiL1R + process.HLTDoublePhotonEt5JpsiSequence + process.HLTEndSequence )
process.HLT_DoublePhoton5_Upsilon_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleEG8orL1DoubleEG5 + process.hltPreDoublePhoton5UpsL1R + process.HLTDoublePhotonEt5UpsSequence + process.HLTEndSequence )
process.HLT_DoublePhoton5_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG5 + process.hltPreDoublePhoton5_L1R + process.HLTDoublePhotonEt5L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_DoublePhoton10_L1R = cms.Path( process.HLTBeginSequence + process.hltL1sL1DoubleEG5 + process.hltPreDoublePhoton10L1R + process.HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence + process.HLTEndSequence )
process.HLT_SingleLooseIsoTau20 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sSingleLooseIsoTau20 + process.hltPreSingleLooseIsoTau20 + process.HLTL2TauJetsSequence + process.hltFilterL2EtCutSingleLooseIsoTau20 + process.HLTL2TauEcalIsolationSequence + process.hltL1HLTSingleLooseIsoTau20JetsMatch + process.hltFilterL2EcalIsolationSingleLooseIsoTau20 + process.HLTEndSequence )
process.HLT_SingleLooseIsoTau20_Trk5 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sSingleLooseIsoTau20Trk5 + process.hltPreSingleLooseIsoTau20Trk5 + process.HLTL2TauJetsSequence + process.hltFilterL2EtCutSingleLooseIsoTau20Trk5 + process.HLTL2TauEcalIsolationSequence + process.hltFilterL2EcalIsolationSingleLooseIsoTau20Trk5 + process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingSequence + process.HLTL25TauTrackReconstructionSequence + process.HLTL25TauTrackIsolationSequence + process.hltL1HLTSingleLooseIsoTau20Trk5JetsMatch + process.hltFilterL25LeadingTrackPtCutSingleLooseIsoTau20Trk5 + process.HLTEndSequence )
process.HLT_SingleLooseIsoTau25 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sSingleLooseIsoTau25 + process.hltPreSingleLooseIsoTau25 + process.HLTL2TauJetsSequence + process.hltFilterL2EtCutSingleLooseIsoTau25 + process.HLTL2TauEcalIsolationSequence + process.hltL1HLTSingleLooseIsoTau25JetsMatch + process.hltFilterL2EcalIsolationSingleLooseIsoTau25 + process.HLTEndSequence )
process.HLT_DoubleLooseIsoTau15 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sDoubleLooseIsoTau15 + process.hltPreDoubleLooseIsoTau15 + process.HLTL2TauJetsSequence + process.hltFilterL2EtCutDoubleLooseIsoTau15 + process.HLTL2TauEcalIsolationSequence + process.hltL1HLTDoubleLooseIsoTau15JetsMatch + process.hltFilterL2EcalIsolationDoubleLooseIsoTau15 + process.HLTEndSequence )
process.HLT_BTagIP_Jet50U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sBTagIPJet50U + process.hltPreBTagIPJet50U + process.HLTRecoJetSequenceU + process.hltBJet50U + process.HLTBTagIPSequenceL25StartupU + process.hltBLifetimeL25FilterStartupU + process.HLTBTagIPSequenceL3StartupU + process.hltBLifetimeL3FilterStartupU + process.HLTEndSequence )
process.HLT_BTagMu_Jet10U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sBTagMuJet10U + process.hltPreBTagMuJet10U + process.HLTRecoJetSequenceU + process.hltBJet10U + process.HLTBTagMuSequenceL25U + process.hltBSoftMuonL25FilterUByDR + process.HLTBTagMuSequenceL3U + process.hltBSoftMuonL3FilterUByDR + process.HLTEndSequence )
process.HLT_StoppedHSCP_8E29 = cms.Path( process.HLTBeginSequence + process.hltL1sStoppedHSCP8E29 + process.hltPreStoppedHSCP8E29 + process.hltHcalDigis + process.hltHbhereco + process.hltStoppedHSCPHpdFilter + process.hltStoppedHSCPTowerMakerForAll + process.hltStoppedHSCPIterativeCone5CaloJets + process.hltStoppedHSCP1CaloJetEnergy + process.HLTEndSequence )
process.HLT_L1Mu14_L1SingleEG10 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Mu14L1SingleEG10 + process.hltPreL1Mu14L1SingleEG10 + process.HLTEndSequence )
process.HLT_L1Mu14_L1SingleJet6U = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Mu14L1SingleJet6U + process.hltPreL1Mu14L1SingleJet6U + process.HLTEndSequence )
process.HLT_L1Mu14_L1ETM30 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1Mu14L1ETM30 + process.hltPreL1Mu14L1ETM30 + process.HLTEndSequence )
process.HLT_ZeroBias = cms.Path( process.HLTBeginSequence + process.hltL1sZeroBias + process.hltPreZeroBias + process.HLTEndSequence )
process.HLT_ZeroBiasPixel_SingleTrack = cms.Path( process.HLTBeginSequence + process.hltL1sZeroBias + process.hltPreZeroBiasPixelSingleTrack + process.HLTDoLocalPixelSequence + process.HLTPixelTrackingForMinBiasSequence + process.hltPixelCandsForMinBias + process.hltMinBiasPixelFilter1 + process.HLTEndSequence )
process.HLT_MinBiasPixel_SingleTrack = cms.Path( process.HLTBeginSequence + process.hltL1sL1BscMinBiasORBptxPlusORMinus + process.hltPreMinBiasPixelSingleTrack + process.HLTDoLocalPixelSequence + process.HLTPixelTrackingForMinBiasSequence + process.hltPixelCandsForMinBias + process.hltMinBiasPixelFilter1 + process.HLTEndSequence )
process.HLT_MinBiasPixel_DoubleTrack = cms.Path( process.HLTBeginSequence + process.hltL1sL1BscMinBiasORBptxPlusORMinus + process.hltPreMinBiasPixelDoubleTrack + process.HLTDoLocalPixelSequence + process.HLTPixelTrackingForMinBiasSequence + process.hltPixelCandsForMinBias + process.hltMinBiasPixelFilter2 + process.HLTEndSequence )
process.HLT_MinBiasPixel_DoubleIsoTrack5 = cms.Path( process.HLTBeginSequence + process.hltL1sL1BscMinBiasORBptxPlusORMinus + process.hltPreMinBiasPixelDoubleIsoTrack5 + process.HLTDoLocalPixelSequence + process.HLTPixelTrackingForMinBiasSequence + process.hltPixelCandsForMinBias + process.hltPixelMBForAlignment + process.HLTEndSequence )
process.HLT_MultiVertex6 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltPreMultiVertex6 + process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingForMultiVertexSequence + process.hltVertexFilter6 + process.HLTEndSequence )
process.HLT_MultiVertex8_L1ETT60 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sETT60 + process.hltPreMultiVertex8 + process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingForMultiVertexSequence + process.hltVertexFilter8 + process.HLTEndSequence )
process.HLT_CSCBeamHalo = cms.Path( process.HLTBeginSequence + process.hltL1sCSCBeamHalo + process.hltPreCSCBeamHalo + process.HLTEndSequence )
process.HLT_CSCBeamHaloOverlapRing1 = cms.Path( process.HLTBeginSequence + process.hltL1sCSCBeamHaloOverlapRing1 + process.hltPreCSCBeamHaloOverlapRing1 + process.hltMuonCSCDigis + process.hltCsc2DRecHits + process.hltOverlapsHLTCSCBeamHaloOverlapRing1 + process.HLTEndSequence )
process.HLT_CSCBeamHaloOverlapRing2 = cms.Path( process.HLTBeginSequence + process.hltL1sCSCBeamHaloOverlapRing2 + process.hltPreCSCBeamHaloOverlapRing2 + process.hltMuonCSCDigis + process.hltCsc2DRecHits + process.hltOverlapsHLTCSCBeamHaloOverlapRing2 + process.HLTEndSequence )
process.HLT_CSCBeamHaloRing2or3 = cms.Path( process.HLTBeginSequence + process.hltL1sCSCBeamHaloRing2or3 + process.hltPreCSCBeamHaloRing2or3 + process.hltMuonCSCDigis + process.hltCsc2DRecHits + process.hltFilter23HLTCSCBeamHaloRing2or3 + process.HLTEndSequence )
process.HLT_L1_BscMinBiasOR_BptxPlusORMinus = cms.Path( process.HLTBeginSequence + process.hltL1sL1BscMinBiasORBptxPlusORMinus + process.hltPreL1BscMinBiasORBptxPlusORMinus + process.HLTEndSequence )
process.HLT_L1_BptxXOR_BscMinBiasOR = cms.Path( process.HLTBeginSequence + process.hltL1sL1BptxXORBscMinBiasOR + process.hltPreL1BptxXORBscMinBiasOR + process.HLTEndSequence )
process.HLT_L1Tech_BSC_minBias = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1TechBSCminBias + process.hltPreL1TechBSCminBias_BPTX + process.HLTEndSequence )
process.HLT_L1Tech_BSC_halo = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1TechBSChalo + process.hltPreL1TechBSChalo + process.HLTEndSequence )
process.HLT_L1Tech_BSC_halo_forPhysicsBackground = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1TechBSChalo + process.hltPreL1TechBSChalo_forPhysicsBackground + process.HLTEndSequence )
process.HLT_L1Tech_BSC_HighMultiplicity = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sHighMultiplicityBSC + process.hltPreHighMultiplicityBSC + process.HLTEndSequence )
process.HLT_L1Tech_RPC_TTU_RBst1_collisions = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1TechRPCTTURBst1collisions + process.hltPreL1TechRPCTTURBst1collisions + process.HLTEndSequence )
process.HLT_L1Tech_HCAL_HF = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sL1HFtech + process.hltPreL1HFTech + process.HLTEndSequence )
process.HLT_TrackerCosmics = cms.Path( process.HLTBeginSequence + process.hltL1sTrackerCosmics + process.hltTrackerCosmicsPattern + process.hltPreTrackerCosmics + process.HLTEndSequence )
process.HLT_RPCBarrelCosmics = cms.Path( process.HLTBeginSequence + process.hltL1sRPCBarrelCosmics + process.hltPreRPCBarrelCosmics + process.HLTEndSequence )
process.HLT_IsoTrackHE_8E29 = cms.Path( process.HLTBeginSequence + process.hltL1sIsoTrack8E29 + process.hltPreIsoTrackHE8E29 + process.HLTL2HcalIsolTrackSequenceHE + process.hltIsolPixelTrackProdHE8E29 + process.hltIsolPixelTrackL2FilterHE8E29 + process.HLTDoLocalStripSequence + process.hltHITPixelTripletSeedGeneratorHE8E29 + process.hltHITCkfTrackCandidatesHE8E29 + process.hltHITCtfWithMaterialTracksHE8E29 + process.hltHITIPTCorrectorHE8E29 + process.hltIsolPixelTrackL3FilterHE8E29 + process.HLTEndSequence )
process.HLT_IsoTrackHB_8E29 = cms.Path( process.HLTBeginSequence + process.hltL1sIsoTrack8E29 + process.hltPreIsoTrackHB8E29 + process.HLTL2HcalIsolTrackSequenceHB + process.hltIsolPixelTrackProdHB8E29 + process.hltIsolPixelTrackL2FilterHB8E29 + process.HLTDoLocalStripSequence + process.hltHITPixelTripletSeedGeneratorHB8E29 + process.hltHITCkfTrackCandidatesHB8E29 + process.hltHITCtfWithMaterialTracksHB8E29 + process.hltHITIPTCorrectorHB8E29 + process.hltIsolPixelTrackL3FilterHB8E29 + process.HLTEndSequence )
process.HLT_HcalPhiSym = cms.Path( process.HLTBeginSequenceNZS + process.hltLevel1Activity + process.hltPreHcalPhiSym + process.HLTEndSequence )
process.HLT_HcalNZS_8E29 = cms.Path( process.HLTBeginSequenceNZS + process.hltL1sHcalNZS8E29 + process.hltPreHcalNZS8E29 + process.HLTEndSequence )
process.AlCa_EcalPhiSym = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sZeroBias + process.hltL1sAlCaEcalPhiSym + process.hltPreAlCaEcalPhiSym + process.hltEcalRawToRecHitFacility + process.hltESRawToRecHitFacility + process.hltEcalRegionalRestFEDs + process.hltEcalRecHitAll + process.hltAlCaPhiSymStream + process.HLTEndSequence )
process.AlCa_EcalPi0_8E29 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sAlCaEcalPi0Eta8E29 + process.hltPreAlCaEcalPi08E29 + process.HLTDoRegionalPi0EtaSequence + process.hltSimple3x3Clusters + process.hltAlCaPi0RecHitsFilter + process.HLTEndSequence )
process.AlCa_EcalEta_8E29 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sAlCaEcalPi0Eta8E29 + process.hltPreAlCaEcalEta8E29 + process.HLTDoRegionalPi0EtaSequence + process.hltSimple3x3Clusters + process.hltAlCaEtaRecHitsFilter + process.HLTEndSequence )
process.AlCa_RPCMuonNoHits = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sAlCaRPC + process.hltPreRPCMuonNoHits + process.HLTmuonlocalrecoSequence + process.hltRPCPointProducer + process.hltRPCFilter + process.HLTEndSequence )
process.AlCa_RPCMuonNoTriggers = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sAlCaRPC + process.hltPreRPCMuonNoTriggers + process.hltRPCMuonNoTriggersL1Filtered0 + process.HLTmuonlocalrecoSequence + process.HLTEndSequence )
process.AlCa_RPCMuonNormalisation = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sAlCaRPC + process.hltPreRPCMuonNorma + process.hltRPCMuonNormaL1Filtered0 + process.HLTmuonlocalrecoSequence + process.HLTEndSequence )
process.HLT_DTErrors = cms.Path( process.hltPreAlCaDTErrors + process.hltDTROMonitorFilter + process.hltDynAlCaDTErrors + process.HLTEndSequence )
process.HLT_Calibration = cms.Path( process.hltCalibrationEventsFilter + process.hltPreCalibration + process.HLTEndSequence )
process.HLT_EcalCalibration = cms.Path( process.hltCalibrationEventsFilter + process.hltPreEcalCalibration + process.hltEcalCalibrationRaw + process.HLTEndSequence )
process.HLT_HcalCalibration = cms.Path( process.hltCalibrationEventsFilter + process.hltHcalCalibTypeFilter + process.hltPreHcalCalibration + process.HLTEndSequence )
process.HLT_Random = cms.Path( process.hltRandomEventsFilter + process.hltPreRandom + process.HLTEndSequence )
process.HLT_PixelTracks_Multiplicity70 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sETT60 + process.hltPrePixelTracksMultiplicity70 + process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingForHighMultSequence + process.hltPixelCandsForHighMult + process.hlt1HighMult70 + process.HLTEndSequence )
process.HLT_PixelTracks_Multiplicity85 = cms.Path( process.HLTBeginSequenceBPTX + process.hltL1sETT60 + process.hltPrePixelTracksMultiplicity85 + process.HLTDoLocalPixelSequence + process.HLTRecopixelvertexingForHighMultSequence + process.hltPixelCandsForHighMult + process.hlt1HighMult85 + process.HLTEndSequence )
process.HLT_GlobalRunHPDNoise = cms.Path( process.HLTBeginSequence + process.hltL1sGlobalRunHPDNoise + process.hltPreGlobalRunHPDNoise + process.HLTEndSequence )
process.HLT_TechTrigHCALNoise = cms.Path( process.HLTBeginSequence + process.hltL1sTechTrigHCALNoise + process.hltL1sNotBptxPlusOrMinus + process.hltPreTechTrigHCALNoise + process.HLTEndSequence )
process.HLT_L1_BPTX = cms.Path( process.HLTBeginSequence + process.hltL1sL1BPTX + process.hltPreL1BPTX + process.HLTEndSequence )
process.HLT_L1_BPTX_MinusOnly = cms.Path( process.HLTBeginSequence + process.hltL1sL1BPTX + process.hltL1sL1BPTXMinusOnly + process.hltPreL1BPTXMinusOnly + process.HLTEndSequence )
process.HLT_L1_BPTX_PlusOnly = cms.Path( process.HLTBeginSequence + process.hltL1sL1BPTX + process.hltL1sL1BPTXPlusOnly + process.hltPreL1BPTXPlusOnly + process.HLTEndSequence )
process.HLT_L2Mu0_NoVertex = cms.Path( process.HLTBeginSequence + process.hltL1sL1SingleMu0 + process.hltPreL2Mu0NoVertex + process.hltSingleMu0L1Filtered + process.HLTL2muonrecoSequenceNoVtx + process.hltSingleL2Mu0L2PreFilteredNoVtx + process.HLTEndSequence )
process.HLT_LogMonitor = cms.Path( process.hltPreLogMonitor + process.hltLogMonitorFilter + process.HLTEndSequence )
process.DQM_FEDIntegrity = cms.Path( process.HLTBeginSequence + process.hltPreFEDIntegrity + process.hltDTDQMEvF + process.hltEcalDigis + process.hltEBHltTask + process.hltEEHltTask + process.hltL1tfed + process.hltSiPixelDigisWithErrors + process.hltSiPixelHLTSource + process.hltSiStripFEDCheck + process.hltMuonRPCDigis + process.hltRPCFEDIntegrity + process.hltBoolDQMPath )
process.HLTriggerFinalPath = cms.Path( process.hltTriggerSummaryAOD + process.hltPreTriggerSummaryRAW + process.hltTriggerSummaryRAW + process.hltBoolFinalPath )
process.HLTAnalyzerEndpath = cms.EndPath( process.hltL1GtTrigReport + process.hltTrigReport )
process.HLTOutput = cms.EndPath( process.hltOutputA )
process.EventDisplayOutput = cms.EndPath( process.hltPreEventDisplay + process.hltPreEventDisplaySmart + process.hltOutputEventDisplay )
process.ExpressOutput = cms.EndPath( process.hltPreExpress + process.hltPreExpressSmart + process.hltOutputExpress )
process.AlCaOutput = cms.EndPath( process.hltOutputCalibration + process.hltOutputEcalCalibration + process.hltOutputALCAP0 + process.hltOutputALCAPHISYM + process.hltOutputRPCMON + process.hltOutputOnlineErrors )
process.DQMOutput = cms.EndPath( process.hltDQML1Scalers + process.hltDQML1SeedLogicScalers + process.hltDQMHLTScalers + process.hltPreDQM + process.hltPreDQMSmart + process.hltOutputDQM )
process.HLTDQMOutput = cms.EndPath( process.hltPreHLTDQM + process.hltPreHLTDQMSmart + process.hltOutputHLTDQM )
process.HLTMONOutput = cms.EndPath( process.hltPreHLTMON + process.hltPreHLTMONSmart + process.hltOutputHLTMON )

# override the preshower baseline setting for MC
if 'ESUnpackerWorkerESProducer' in process.__dict__:
    process.ESUnpackerWorkerESProducer.RHAlgo.ESBaseline = 1000

process.setName_('HLT')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( 100 )
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool( True )
)

if 'GlobalTag' in process.__dict__:
    process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
    from Configuration.PyReleaseValidation.autoCond import autoCond
    process.GlobalTag.globaltag = autoCond['startup']

if 'Level1MenuOverride' in process.__dict__:
    process.Level1MenuOverride.connect   = 'frontier://FrontierProd/CMS_COND_31X_L1T'

if 'hltTrigReport' in process.__dict__:
    process.hltTrigReport.HLTriggerResults       = cms.InputTag( 'TriggerResults','',process.name_() )

if 'hltDQMHLTScalers' in process.__dict__:
    process.hltDQMHLTScalers.triggerResults      = cms.InputTag( 'TriggerResults','',process.name_() )

if 'hltPreExpressSmart' in process.__dict__:
    process.hltPreExpressSmart.TriggerResultsTag = cms.InputTag( 'TriggerResults','',process.name_() )

if 'hltPreHLTMONSmart' in process.__dict__:
    process.hltPreHLTMONSmart.TriggerResultsTag  = cms.InputTag( 'TriggerResults','',process.name_() )

if 'hltPreDQMSmart' in process.__dict__:
    process.hltPreDQMSmart.TriggerResultsTag     = cms.InputTag( 'TriggerResults','',process.name_() )

if 'hltDQML1SeedLogicScalers' in process.__dict__:
    process.hltDQML1SeedLogicScalers.processname = process.name_()

process.options.wantSummary = cms.untracked.bool(True)
process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
process.MessageLogger.categories.append('L1GtTrigReport')
process.MessageLogger.categories.append('HLTrigReport')
