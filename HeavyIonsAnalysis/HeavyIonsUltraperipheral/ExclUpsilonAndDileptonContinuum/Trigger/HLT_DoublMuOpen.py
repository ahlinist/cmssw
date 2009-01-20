# /dev/CMSSW_2_1_9/HLT/V10 (CMSSW_2_1_X_2008-10-09-0000)

import FWCore.ParameterSet.Config as cms

process = cms.Process( "HLT" )

process.HLTConfigVersion = cms.PSet(
  tableName = cms.string('/dev/CMSSW_2_1_9/HLT/V10')
)

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(  wantSummary = cms.untracked.bool( True ) )

process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring( 
                     #'file:HepMc_Pure_Raw.root' 
                     'rfio:/castor/cern.ch/user/k/kumarv/Trigger/Up1s_Pure_Raw_n_1.root'
    )
)

process.L1MuTriggerPtScaleRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1MuTriggerPtScaleRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.SiStripQualityFakeESSource = cms.ESSource( "SiStripQualityFakeESSource" )
process.GlobalTag = cms.ESSource( "PoolDBESSource",
    BlobStreamerName = cms.untracked.string( "TBufferBlobStreamingService" ),
    connect = cms.string( "frontier://FrontierProd/CMS_COND_21X_GLOBALTAG" ),
    globaltag = cms.string( "STARTUP_V4::All" ),
    DBParameters = cms.PSet( 
      authenticationPath = cms.untracked.string( "." ),
      messageLevel = cms.untracked.int32( 0 ),
      connectionTimeOut = cms.untracked.int32( 0 ),
      connectionRetrialPeriod = cms.untracked.int32( 10 ),
      connectionRetrialTimeOut = cms.untracked.int32( 60 ),
      enableConnectionSharing = cms.untracked.bool( True ),
      enableReadOnlySessionOnUpdateConnection = cms.untracked.bool( False ),
      enablePoolAutomaticCleanUp = cms.untracked.bool( False ),
      idleConnectionCleanupPeriod = cms.untracked.int32( 10 )
    ),
    toGet = cms.VPSet( 
    ),
    timetype = cms.string( "runnumber" )
)
process.BTagRecord = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "JetTagComputerRecord" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.HepPDTESSource = cms.ESSource( "HepPDTESSource",
    pdtFileName = cms.FileInPath( "SimGeneral/HepPDTESSource/data/pythiaparticle.tbl" )
)
process.L1GtBoardMapsRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtBoardMapsRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtParametersRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtParametersRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtPrescaleFactorsAlgoTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtPrescaleFactorsAlgoTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtPrescaleFactorsTechTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtPrescaleFactorsTechTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtStableParametersRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtStableParametersRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtTriggerMaskAlgoTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtTriggerMaskAlgoTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtTriggerMaskTechTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtTriggerMaskTechTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtTriggerMaskVetoAlgoTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtTriggerMaskVetoAlgoTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1GtTriggerMaskVetoTechTrigRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1GtTriggerMaskVetoTechTrigRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.L1MuTriggerScalesRcdSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1MuTriggerScalesRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.MCJetCorrectorIcone5 = cms.ESSource( "MCJetCorrectionService",
    tagName = cms.string( "CMSSW_152_iterativeCone5" ),
    label = cms.string( "MCJetCorrectorIcone5" )
)
process.XMLIdealGeometryESSource = cms.ESSource( "XMLIdealGeometryESSource",
    rootNodeName = cms.string( "cms:OCMS" ),
    geomXMLFiles = cms.vstring( 'Geometry/CMSCommonData/data/materials.xml',
      'Geometry/CMSCommonData/data/rotations.xml',
      'Geometry/CMSCommonData/data/normal/cmsextent.xml',
      'Geometry/CMSCommonData/data/cms.xml',
      'Geometry/CMSCommonData/data/cmsMother.xml',
      'Geometry/CMSCommonData/data/cmsTracker.xml',
      'Geometry/CMSCommonData/data/caloBase.xml',
      'Geometry/CMSCommonData/data/cmsCalo.xml',
      'Geometry/CMSCommonData/data/muonBase.xml',
      'Geometry/CMSCommonData/data/cmsMuon.xml',
      'Geometry/CMSCommonData/data/mgnt.xml',
      'Geometry/CMSCommonData/data/beampipe.xml',
      'Geometry/CMSCommonData/data/cmsBeam.xml',
      'Geometry/CMSCommonData/data/muonMB.xml',
      'Geometry/CMSCommonData/data/muonMagnet.xml',
      'Geometry/TrackerCommonData/data/pixfwdMaterials.xml',
      'Geometry/TrackerCommonData/data/pixfwdCommon.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq1x2.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq1x5.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq2x3.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq2x4.xml',
      'Geometry/TrackerCommonData/data/pixfwdPlaq2x5.xml',
      'Geometry/TrackerCommonData/data/pixfwdPanelBase.xml',
      'Geometry/TrackerCommonData/data/pixfwdPanel.xml',
      'Geometry/TrackerCommonData/data/pixfwdBlade.xml',
      'Geometry/TrackerCommonData/data/pixfwdNipple.xml',
      'Geometry/TrackerCommonData/data/pixfwdDisk.xml',
      'Geometry/TrackerCommonData/data/pixfwdCylinder.xml',
      'Geometry/TrackerCommonData/data/pixfwd.xml',
      'Geometry/TrackerCommonData/data/pixbarmaterial.xml',
      'Geometry/TrackerCommonData/data/pixbarladder.xml',
      'Geometry/TrackerCommonData/data/pixbarladderfull.xml',
      'Geometry/TrackerCommonData/data/pixbarladderhalf.xml',
      'Geometry/TrackerCommonData/data/pixbarlayer.xml',
      'Geometry/TrackerCommonData/data/pixbarlayer0.xml',
      'Geometry/TrackerCommonData/data/pixbarlayer1.xml',
      'Geometry/TrackerCommonData/data/pixbarlayer2.xml',
      'Geometry/TrackerCommonData/data/pixbar.xml',
      'Geometry/TrackerCommonData/data/tibtidcommonmaterial.xml',
      'Geometry/TrackerCommonData/data/tibmaterial.xml',
      'Geometry/TrackerCommonData/data/tibmodpar.xml',
      'Geometry/TrackerCommonData/data/tibmodule0.xml',
      'Geometry/TrackerCommonData/data/tibmodule0a.xml',
      'Geometry/TrackerCommonData/data/tibmodule0b.xml',
      'Geometry/TrackerCommonData/data/tibmodule2.xml',
      'Geometry/TrackerCommonData/data/tibstringpar.xml',
      'Geometry/TrackerCommonData/data/tibstring0ll.xml',
      'Geometry/TrackerCommonData/data/tibstring0lr.xml',
      'Geometry/TrackerCommonData/data/tibstring0ul.xml',
      'Geometry/TrackerCommonData/data/tibstring0ur.xml',
      'Geometry/TrackerCommonData/data/tibstring0.xml',
      'Geometry/TrackerCommonData/data/tibstring1ll.xml',
      'Geometry/TrackerCommonData/data/tibstring1lr.xml',
      'Geometry/TrackerCommonData/data/tibstring1ul.xml',
      'Geometry/TrackerCommonData/data/tibstring1ur.xml',
      'Geometry/TrackerCommonData/data/tibstring1.xml',
      'Geometry/TrackerCommonData/data/tibstring2ll.xml',
      'Geometry/TrackerCommonData/data/tibstring2lr.xml',
      'Geometry/TrackerCommonData/data/tibstring2ul.xml',
      'Geometry/TrackerCommonData/data/tibstring2ur.xml',
      'Geometry/TrackerCommonData/data/tibstring2.xml',
      'Geometry/TrackerCommonData/data/tibstring3ll.xml',
      'Geometry/TrackerCommonData/data/tibstring3lr.xml',
      'Geometry/TrackerCommonData/data/tibstring3ul.xml',
      'Geometry/TrackerCommonData/data/tibstring3ur.xml',
      'Geometry/TrackerCommonData/data/tibstring3.xml',
      'Geometry/TrackerCommonData/data/tiblayerpar.xml',
      'Geometry/TrackerCommonData/data/tiblayer0.xml',
      'Geometry/TrackerCommonData/data/tiblayer1.xml',
      'Geometry/TrackerCommonData/data/tiblayer2.xml',
      'Geometry/TrackerCommonData/data/tiblayer3.xml',
      'Geometry/TrackerCommonData/data/tib.xml',
      'Geometry/TrackerCommonData/data/tidmaterial.xml',
      'Geometry/TrackerCommonData/data/tidmodpar.xml',
      'Geometry/TrackerCommonData/data/tidmodule0.xml',
      'Geometry/TrackerCommonData/data/tidmodule0r.xml',
      'Geometry/TrackerCommonData/data/tidmodule0l.xml',
      'Geometry/TrackerCommonData/data/tidmodule1.xml',
      'Geometry/TrackerCommonData/data/tidmodule1r.xml',
      'Geometry/TrackerCommonData/data/tidmodule1l.xml',
      'Geometry/TrackerCommonData/data/tidmodule2.xml',
      'Geometry/TrackerCommonData/data/tidringpar.xml',
      'Geometry/TrackerCommonData/data/tidring0.xml',
      'Geometry/TrackerCommonData/data/tidring0f.xml',
      'Geometry/TrackerCommonData/data/tidring0b.xml',
      'Geometry/TrackerCommonData/data/tidring1.xml',
      'Geometry/TrackerCommonData/data/tidring1f.xml',
      'Geometry/TrackerCommonData/data/tidring1b.xml',
      'Geometry/TrackerCommonData/data/tidring2.xml',
      'Geometry/TrackerCommonData/data/tid.xml',
      'Geometry/TrackerCommonData/data/tidf.xml',
      'Geometry/TrackerCommonData/data/tidb.xml',
      'Geometry/TrackerCommonData/data/tibtidservices.xml',
      'Geometry/TrackerCommonData/data/tibtidservicesf.xml',
      'Geometry/TrackerCommonData/data/tibtidservicesb.xml',
      'Geometry/TrackerCommonData/data/tobmaterial.xml',
      'Geometry/TrackerCommonData/data/tobmodpar.xml',
      'Geometry/TrackerCommonData/data/tobmodule0.xml',
      'Geometry/TrackerCommonData/data/tobmodule2.xml',
      'Geometry/TrackerCommonData/data/tobmodule4.xml',
      'Geometry/TrackerCommonData/data/tobrodpar.xml',
      'Geometry/TrackerCommonData/data/tobrod0c.xml',
      'Geometry/TrackerCommonData/data/tobrod0l.xml',
      'Geometry/TrackerCommonData/data/tobrod0h.xml',
      'Geometry/TrackerCommonData/data/tobrod0.xml',
      'Geometry/TrackerCommonData/data/tobrod1l.xml',
      'Geometry/TrackerCommonData/data/tobrod1h.xml',
      'Geometry/TrackerCommonData/data/tobrod1.xml',
      'Geometry/TrackerCommonData/data/tobrod2c.xml',
      'Geometry/TrackerCommonData/data/tobrod2l.xml',
      'Geometry/TrackerCommonData/data/tobrod2h.xml',
      'Geometry/TrackerCommonData/data/tobrod2.xml',
      'Geometry/TrackerCommonData/data/tobrod3l.xml',
      'Geometry/TrackerCommonData/data/tobrod3h.xml',
      'Geometry/TrackerCommonData/data/tobrod3.xml',
      'Geometry/TrackerCommonData/data/tobrod4c.xml',
      'Geometry/TrackerCommonData/data/tobrod4l.xml',
      'Geometry/TrackerCommonData/data/tobrod4h.xml',
      'Geometry/TrackerCommonData/data/tobrod4.xml',
      'Geometry/TrackerCommonData/data/tobrod5l.xml',
      'Geometry/TrackerCommonData/data/tobrod5h.xml',
      'Geometry/TrackerCommonData/data/tobrod5.xml',
      'Geometry/TrackerCommonData/data/tob.xml',
      'Geometry/TrackerCommonData/data/tecmaterial.xml',
      'Geometry/TrackerCommonData/data/tecmodpar.xml',
      'Geometry/TrackerCommonData/data/tecmodule0.xml',
      'Geometry/TrackerCommonData/data/tecmodule0r.xml',
      'Geometry/TrackerCommonData/data/tecmodule0s.xml',
      'Geometry/TrackerCommonData/data/tecmodule1.xml',
      'Geometry/TrackerCommonData/data/tecmodule1r.xml',
      'Geometry/TrackerCommonData/data/tecmodule1s.xml',
      'Geometry/TrackerCommonData/data/tecmodule2.xml',
      'Geometry/TrackerCommonData/data/tecmodule3.xml',
      'Geometry/TrackerCommonData/data/tecmodule4.xml',
      'Geometry/TrackerCommonData/data/tecmodule4r.xml',
      'Geometry/TrackerCommonData/data/tecmodule4s.xml',
      'Geometry/TrackerCommonData/data/tecmodule5.xml',
      'Geometry/TrackerCommonData/data/tecmodule6.xml',
      'Geometry/TrackerCommonData/data/tecpetpar.xml',
      'Geometry/TrackerCommonData/data/tecring0.xml',
      'Geometry/TrackerCommonData/data/tecring1.xml',
      'Geometry/TrackerCommonData/data/tecring2.xml',
      'Geometry/TrackerCommonData/data/tecring3.xml',
      'Geometry/TrackerCommonData/data/tecring4.xml',
      'Geometry/TrackerCommonData/data/tecring5.xml',
      'Geometry/TrackerCommonData/data/tecring6.xml',
      'Geometry/TrackerCommonData/data/tecring0f.xml',
      'Geometry/TrackerCommonData/data/tecring1f.xml',
      'Geometry/TrackerCommonData/data/tecring2f.xml',
      'Geometry/TrackerCommonData/data/tecring3f.xml',
      'Geometry/TrackerCommonData/data/tecring4f.xml',
      'Geometry/TrackerCommonData/data/tecring5f.xml',
      'Geometry/TrackerCommonData/data/tecring6f.xml',
      'Geometry/TrackerCommonData/data/tecring0b.xml',
      'Geometry/TrackerCommonData/data/tecring1b.xml',
      'Geometry/TrackerCommonData/data/tecring2b.xml',
      'Geometry/TrackerCommonData/data/tecring3b.xml',
      'Geometry/TrackerCommonData/data/tecring4b.xml',
      'Geometry/TrackerCommonData/data/tecring5b.xml',
      'Geometry/TrackerCommonData/data/tecring6b.xml',
      'Geometry/TrackerCommonData/data/tecpetalf.xml',
      'Geometry/TrackerCommonData/data/tecpetalb.xml',
      'Geometry/TrackerCommonData/data/tecpetal0.xml',
      'Geometry/TrackerCommonData/data/tecpetal0f.xml',
      'Geometry/TrackerCommonData/data/tecpetal0b.xml',
      'Geometry/TrackerCommonData/data/tecpetal3.xml',
      'Geometry/TrackerCommonData/data/tecpetal3f.xml',
      'Geometry/TrackerCommonData/data/tecpetal3b.xml',
      'Geometry/TrackerCommonData/data/tecpetal6f.xml',
      'Geometry/TrackerCommonData/data/tecpetal6b.xml',
      'Geometry/TrackerCommonData/data/tecpetal8f.xml',
      'Geometry/TrackerCommonData/data/tecpetal8b.xml',
      'Geometry/TrackerCommonData/data/tecwheel.xml',
      'Geometry/TrackerCommonData/data/tecwheela.xml',
      'Geometry/TrackerCommonData/data/tecwheelb.xml',
      'Geometry/TrackerCommonData/data/tecwheelc.xml',
      'Geometry/TrackerCommonData/data/tecwheeld.xml',
      'Geometry/TrackerCommonData/data/tecwheel6.xml',
      'Geometry/TrackerCommonData/data/tecservices.xml',
      'Geometry/TrackerCommonData/data/tecbackplate.xml',
      'Geometry/TrackerCommonData/data/tec.xml',
      'Geometry/TrackerCommonData/data/trackermaterial.xml',
      'Geometry/TrackerCommonData/data/tracker.xml',
      'Geometry/TrackerCommonData/data/trackerpixbar.xml',
      'Geometry/TrackerCommonData/data/trackerpixfwd.xml',
      'Geometry/TrackerCommonData/data/trackertibtidservices.xml',
      'Geometry/TrackerCommonData/data/trackertib.xml',
      'Geometry/TrackerCommonData/data/trackertid.xml',
      'Geometry/TrackerCommonData/data/trackertob.xml',
      'Geometry/TrackerCommonData/data/trackertec.xml',
      'Geometry/TrackerCommonData/data/trackerbulkhead.xml',
      'Geometry/TrackerCommonData/data/trackerother.xml',
      'Geometry/EcalCommonData/data/eregalgo.xml',
      'Geometry/EcalCommonData/data/ebalgo.xml',
      'Geometry/EcalCommonData/data/ebcon.xml',
      'Geometry/EcalCommonData/data/ebrot.xml',
      'Geometry/EcalCommonData/data/eecon.xml',
      'Geometry/EcalCommonData/data/eefixed.xml',
      'Geometry/EcalCommonData/data/eehier.xml',
      'Geometry/EcalCommonData/data/eealgo.xml',
      'Geometry/EcalCommonData/data/escon.xml',
      'Geometry/EcalCommonData/data/eeF.xml',
      'Geometry/EcalCommonData/data/eeB.xml',
      'Geometry/HcalCommonData/data/hcalrotations.xml',
      'Geometry/HcalCommonData/data/hcalalgo.xml',
      'Geometry/HcalCommonData/data/hcalbarrelalgo.xml',
      'Geometry/HcalCommonData/data/hcalendcapalgo.xml',
      'Geometry/HcalCommonData/data/hcalouteralgo.xml',
      'Geometry/HcalCommonData/data/hcalforwardalgo.xml',
      'Geometry/HcalCommonData/data/hcalforwardfibre.xml',
      'Geometry/HcalCommonData/data/hcalforwardmaterial.xml',
      'Geometry/MuonCommonData/data/mbCommon.xml',
      'Geometry/MuonCommonData/data/mb1.xml',
      'Geometry/MuonCommonData/data/mb2.xml',
      'Geometry/MuonCommonData/data/mb3.xml',
      'Geometry/MuonCommonData/data/mb4.xml',
      'Geometry/MuonCommonData/data/muonYoke.xml',
      'Geometry/MuonCommonData/data/mf.xml',
      'Geometry/ForwardCommonData/data/forward.xml',
      'Geometry/ForwardCommonData/data/forwardshield.xml',
      'Geometry/ForwardCommonData/data/brmrotations.xml',
      'Geometry/ForwardCommonData/data/brm.xml',
      'Geometry/ForwardCommonData/data/totemMaterials.xml',
      'Geometry/ForwardCommonData/data/totemRotations.xml',
      'Geometry/ForwardCommonData/data/totemt1.xml',
      'Geometry/ForwardCommonData/data/totemt2.xml',
      'Geometry/ForwardCommonData/data/ionpump.xml',
      'Geometry/MuonCommonData/data/muonNumbering.xml',
      'Geometry/TrackerCommonData/data/trackerStructureTopology.xml',
      'Geometry/TrackerSimData/data/trackersens.xml',
      'Geometry/TrackerRecoData/data/trackerRecoMaterial.xml',
      'Geometry/EcalSimData/data/ebsens.xml',
      'Geometry/EcalSimData/data/eesens.xml',
      'Geometry/HcalCommonData/data/hcalsens.xml',
      'Geometry/HcalSimData/data/CaloUtil.xml',
      'Geometry/MuonSimData/data/muonSens.xml',
      'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml',
      'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml',
      'Geometry/CSCGeometryBuilder/data/cscSpecs.xml',
      'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml',
      'Geometry/ForwardCommonData/data/brmsens.xml',
      'Geometry/HcalSimData/data/HcalProdCuts.xml',
      'Geometry/EcalSimData/data/EBProdCuts.xml',
      'Geometry/TrackerSimData/data/trackerProdCuts.xml',
      'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml',
      'Geometry/MuonSimData/data/muonProdCuts.xml',
      'Geometry/CMSCommonData/data/FieldParameters.xml' )
)
process.eegeom = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "EcalMappingRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.emrcdsrc = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1EmEtScaleRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.es_hardcode = cms.ESSource( "HcalHardcodeCalibrations",
    toGet = cms.untracked.vstring( 'GainWidths',
      'channelQuality',
      'ZSThresholds' )
)
process.jetrcdsrc = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1JetEtScaleRcd" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.l1CaloGeomRecordSource = cms.ESSource( "EmptyESSource",
    recordName = cms.string( "L1CaloGeometryRecord" ),
    iovIsRunNotTime = cms.bool( True ),
    firstValid = cms.vuint32( 1 )
)
process.magfield = cms.ESSource( "XMLIdealGeometryESSource",
    rootNodeName = cms.string( "cmsMagneticField:MAGF" ),
    geomXMLFiles = cms.vstring( 'Geometry/CMSCommonData/data/normal/cmsextent.xml',
      'Geometry/CMSCommonData/data/cms.xml',
      'Geometry/CMSCommonData/data/cmsMagneticField.xml',
      'MagneticField/GeomBuilder/data/MagneticFieldVolumes_1103l.xml' )
)

process.AnyDirectionAnalyticalPropagator = cms.ESProducer( "AnalyticalPropagatorESProducer",
  ComponentName = cms.string( "AnyDirectionAnalyticalPropagator" ),
  PropagationDirection = cms.string( "anyDirection" ),
  MaxDPhi = cms.double( 1.6 )
)
process.ParametrizedMagneticFieldProducer = cms.ESProducer( "ParametrizedMagneticFieldProducer",
  label = cms.untracked.string( "parametrizedField" ),
  version = cms.string( "OAE_1103l_071212" ),
  parameters = cms.PSet(  BValue = cms.string( "3_8T" ) )
)
process.L1MuTriggerPtScale = cms.ESProducer( "L1MuTriggerPtScaleProducer",
  nbitPackingPt = cms.int32( 5 ),
  signedPackingPt = cms.bool( False ),
  nbinsPt = cms.int32( 32 ),
  scalePt = cms.vdouble( -1.0, 0.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 7.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 120.0, 140.0, 1000000.0 )
)
process.CaloTopologyBuilder = cms.ESProducer( "CaloTopologyBuilder" )
process.Chi2EstimatorForL2Refit = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForL2Refit" ),
  MaxChi2 = cms.double( 1000.0 ),
  nSigma = cms.double( 3.0 )
)
process.KFTrajectoryFitterForL2Muon = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Propagator = cms.string( "SteppingHelixPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForL2Refit" ),
  minHits = cms.int32( 3 )
)
process.KFTrajectorySmootherForL2Muon = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFTrajectorySmootherForL2Muon" ),
  Propagator = cms.string( "SteppingHelixPropagatorOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForL2Refit" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 )
)
process.KFFitterSmootherForL2Muon = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "KFFitterSmootherForL2Muon" ),
  Fitter = cms.string( "KFTrajectoryFitterForL2Muon" ),
  Smoother = cms.string( "KFTrajectorySmootherForL2Muon" ),
  EstimateCut = cms.double( -1.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False )
)
process.CSCGeometryESModule = cms.ESProducer( "CSCGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  useRealWireGeometry = cms.bool( True ),
  useOnlyWiresInME1a = cms.bool( False ),
  useGangedStripsInME1a = cms.bool( True ),
  useCentreTIOffsets = cms.bool( False ),
  applyAlignment = cms.bool( True )
)
process.CaloGeometryBuilder = cms.ESProducer( "CaloGeometryBuilder",
  SelectedCalos = cms.vstring( 'HCAL',
    'ZDC',
    'EcalBarrel',
    'EcalEndcap',
    'TOWER' )
)
process.CaloTowerConstituentsMapBuilder = cms.ESProducer( "CaloTowerConstituentsMapBuilder",
  MapFile = cms.untracked.string( "Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz" )
)
process.CaloTowerHardcodeGeometryEP = cms.ESProducer( "CaloTowerHardcodeGeometryEP" )
process.Chi2EstimatorForL3Refit = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForL3Refit" ),
  MaxChi2 = cms.double( 100000.0 ),
  nSigma = cms.double( 3.0 )
)
process.Chi2EstimatorForMuonTrackLoader = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForMuonTrackLoader" ),
  MaxChi2 = cms.double( 100000.0 ),
  nSigma = cms.double( 3.0 )
)
process.Chi2EstimatorForRefit = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2EstimatorForRefit" ),
  MaxChi2 = cms.double( 100000.0 ),
  nSigma = cms.double( 3.0 )
)
process.Chi2MeasurementEstimator = cms.ESProducer( "Chi2MeasurementEstimatorESProducer",
  ComponentName = cms.string( "Chi2" ),
  MaxChi2 = cms.double( 30.0 ),
  nSigma = cms.double( 3.0 )
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
  alwaysUseInvalidHits = cms.bool( True )
)
process.DTGeometryESModule = cms.ESProducer( "DTGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( True )
)
process.EcalBarrelGeometryEP = cms.ESProducer( "EcalBarrelGeometryEP",
  applyAlignment = cms.untracked.bool( False )
)
process.EcalElectronicsMappingBuilder = cms.ESProducer( "EcalElectronicsMappingBuilder",
  MapFile = cms.untracked.string( "Geometry/EcalMapping/data/EEMap.txt" )
)
process.EcalEndcapGeometryEP = cms.ESProducer( "EcalEndcapGeometryEP",
  applyAlignment = cms.untracked.bool( False )
)
process.EcalLaserCorrectionService = cms.ESProducer( "EcalLaserCorrectionService" )
process.EcalPreshowerGeometryEP = cms.ESProducer( "EcalPreshowerGeometryEP",
  applyAlignment = cms.untracked.bool( False )
)
process.FitterRK = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "FitterRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  minHits = cms.int32( 3 )
)
process.FittingSmootherRK = cms.ESProducer( "KFFittingSmootherESProducer",
  ComponentName = cms.string( "FittingSmootherRK" ),
  Fitter = cms.string( "FitterRK" ),
  Smoother = cms.string( "SmootherRK" ),
  EstimateCut = cms.double( -1.0 ),
  MinNumberOfHits = cms.int32( 5 ),
  RejectTracks = cms.bool( True ),
  BreakTrajWith2ConsecutiveMissing = cms.bool( False ),
  NoInvalidHitsBeginEnd = cms.bool( False )
)
process.GlobalTrackingGeometryESProducer = cms.ESProducer( "GlobalTrackingGeometryESProducer" )
process.GroupedCkfTrajectoryBuilder = cms.ESProducer( "GroupedCkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "GroupedCkfTrajectoryBuilder" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "ckfBaseTrajectoryFilter" ),
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  foundHitBonus = cms.double( 5.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( True ),
  lockHits = cms.bool( True ),
  bestHitOnly = cms.bool( True ),
  requireSeedHitsInRebuild = cms.bool( True ),
  minNrOfHitsForRebuild = cms.int32( 5 )
)
process.HcalHardcodeGeometryEP = cms.ESProducer( "HcalHardcodeGeometryEP"
)
process.HcalTopologyIdealEP = cms.ESProducer( "HcalTopologyIdealEP"
)
process.KFFitterForRefitInsideOut = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "KFFitterForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  minHits = cms.int32( 3 )
)
process.KFSmootherForMuonTrackLoader = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForMuonTrackLoader" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForMuonTrackLoader" ),
  errorRescaling = cms.double( 10.0 ),
  minHits = cms.int32( 3 )
)
process.KFSmootherForRefitInsideOut = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "KFSmootherForRefitInsideOut" ),
  Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForRefit" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 )
)
process.KFUpdatorESProducer = cms.ESProducer( "KFUpdatorESProducer",
  ComponentName = cms.string( "KFUpdator" )
)
process.L1GctConfigProducers = cms.ESProducer( "L1GctConfigProducers",
  JetFinderCentralJetSeed = cms.uint32( 1 ),
  JetFinderForwardJetSeed = cms.uint32( 1 ),
  L1CaloHtScaleLsbInGeV = cms.double( 1.0 ),
  L1CaloJetZeroSuppressionThresholdInGeV = cms.double( 5.0 ),
  CalibrationStyle = cms.string( "ORCAStyle" ),
  PowerSeriesCoefficients = cms.PSet( 
    nonTauJetCalib0 = cms.vdouble(  ),
    nonTauJetCalib1 = cms.vdouble(  ),
    nonTauJetCalib2 = cms.vdouble(  ),
    nonTauJetCalib3 = cms.vdouble(  ),
    nonTauJetCalib4 = cms.vdouble(  ),
    nonTauJetCalib5 = cms.vdouble(  ),
    nonTauJetCalib6 = cms.vdouble(  ),
    nonTauJetCalib7 = cms.vdouble(  ),
    nonTauJetCalib8 = cms.vdouble(  ),
    nonTauJetCalib9 = cms.vdouble(  ),
    nonTauJetCalib10 = cms.vdouble(  ),
    tauJetCalib0 = cms.vdouble(  ),
    tauJetCalib1 = cms.vdouble(  ),
    tauJetCalib2 = cms.vdouble(  ),
    tauJetCalib3 = cms.vdouble(  ),
    tauJetCalib4 = cms.vdouble(  ),
    tauJetCalib5 = cms.vdouble(  ),
    tauJetCalib6 = cms.vdouble(  )
  ),
  OrcaStyleCoefficients = cms.PSet( 
    nonTauJetCalib0 = cms.vdouble( 47.4, -20.7, 0.7922, 9.53E-5 ),
    nonTauJetCalib1 = cms.vdouble( 49.4, -22.5, 0.7867, 9.6E-5 ),
    nonTauJetCalib2 = cms.vdouble( 47.1, -22.2, 0.7645, 1.209E-4 ),
    nonTauJetCalib3 = cms.vdouble( 49.3, -22.9, 0.7331, 1.221E-4 ),
    nonTauJetCalib4 = cms.vdouble( 48.2, -24.5, 0.7706, 1.28E-4 ),
    nonTauJetCalib5 = cms.vdouble( 42.0, -23.9, 0.7945, 1.458E-4 ),
    nonTauJetCalib6 = cms.vdouble( 33.8, -22.1, 0.8202, 1.403E-4 ),
    nonTauJetCalib7 = cms.vdouble( 17.1, -6.6, 0.6958, 6.88E-5 ),
    nonTauJetCalib8 = cms.vdouble( 13.1, -4.5, 0.7071, 7.26E-5 ),
    nonTauJetCalib9 = cms.vdouble( 12.4, -3.8, 0.6558, 4.89E-4 ),
    nonTauJetCalib10 = cms.vdouble( 9.3, 1.3, 0.2719, 0.003418 ),
    tauJetCalib0 = cms.vdouble( 47.4, -20.7, 0.7922, 9.53E-5 ),
    tauJetCalib1 = cms.vdouble( 49.4, -22.5, 0.7867, 9.6E-5 ),
    tauJetCalib2 = cms.vdouble( 47.1, -22.2, 0.7645, 1.209E-4 ),
    tauJetCalib3 = cms.vdouble( 49.3, -22.9, 0.7331, 1.221E-4 ),
    tauJetCalib4 = cms.vdouble( 48.2, -24.5, 0.7706, 1.28E-4 ),
    tauJetCalib5 = cms.vdouble( 42.0, -23.9, 0.7945, 1.458E-4 ),
    tauJetCalib6 = cms.vdouble( 33.8, -22.1, 0.8202, 1.403E-4 )
  ),
  PiecewiseCubicCoefficients = cms.PSet( 
    nonTauJetCalib0 = cms.vdouble( 500.0, 100.0, 17.7409, 0.351901, -7.01462E-4, 5.77204E-7, 5.0, 0.720604, 1.25179, -0.0150777, 7.13711E-5 ),
    nonTauJetCalib1 = cms.vdouble( 500.0, 100.0, 20.0549, 0.321867, -6.4901E-4, 5.50042E-7, 5.0, 1.30465, 1.2774, -0.0159193, 7.64496E-5 ),
    nonTauJetCalib2 = cms.vdouble( 500.0, 100.0, 24.3454, 0.257989, -4.50184E-4, 3.09951E-7, 5.0, 2.1034, 1.32441, -0.0173659, 8.50669E-5 ),
    nonTauJetCalib3 = cms.vdouble( 500.0, 100.0, 27.7822, 0.155986, -2.66441E-4, 6.69814E-8, 5.0, 2.64613, 1.30745, -0.0180964, 8.83567E-5 ),
    nonTauJetCalib4 = cms.vdouble( 500.0, 100.0, 26.6384, 0.0567369, -4.16292E-4, 2.60929E-7, 5.0, 2.63299, 1.16558, -0.0170351, 7.95703E-5 ),
    nonTauJetCalib5 = cms.vdouble( 500.0, 100.0, 29.5396, 0.001137, -1.45232E-4, 6.91445E-8, 5.0, 4.16752, 1.08477, -0.016134, 7.69652E-5 ),
    nonTauJetCalib6 = cms.vdouble( 500.0, 100.0, 30.1405, -0.14281, 5.55849E-4, -7.52446E-7, 5.0, 4.79283, 0.672125, -0.00879174, 3.65776E-5 ),
    nonTauJetCalib7 = cms.vdouble( 300.0, 80.0, 30.2715, -0.539688, 0.00499898, -1.2204E-5, 5.0, 1.97284, 0.0610729, 0.00671548, -7.22583E-5 ),
    nonTauJetCalib8 = cms.vdouble( 250.0, 150.0, 1.38861, 0.0362661, 0.0, 0.0, 5.0, 1.87993, 0.0329907, 0.0, 0.0 ),
    nonTauJetCalib9 = cms.vdouble( 200.0, 80.0, 35.0095, -0.669677, 0.00208498, -1.50554E-6, 5.0, 3.16074, -0.114404, 0.0, 0.0 ),
    nonTauJetCalib10 = cms.vdouble( 150.0, 80.0, 1.70475, -0.142171, 0.00104963, -1.62214E-5, 5.0, 1.70475, -0.142171, 0.00104963, -1.62214E-5 ),
    tauJetCalib0 = cms.vdouble( 500.0, 100.0, 17.7409, 0.351901, -7.01462E-4, 5.77204E-7, 5.0, 0.720604, 1.25179, -0.0150777, 7.13711E-5 ),
    tauJetCalib1 = cms.vdouble( 500.0, 100.0, 20.0549, 0.321867, -6.4901E-4, 5.50042E-7, 5.0, 1.30465, 1.2774, -0.0159193, 7.64496E-5 ),
    tauJetCalib2 = cms.vdouble( 500.0, 100.0, 24.3454, 0.257989, -4.50184E-4, 3.09951E-7, 5.0, 2.1034, 1.32441, -0.0173659, 8.50669E-5 ),
    tauJetCalib3 = cms.vdouble( 500.0, 100.0, 27.7822, 0.155986, -2.66441E-4, 6.69814E-8, 5.0, 2.64613, 1.30745, -0.0180964, 8.83567E-5 ),
    tauJetCalib4 = cms.vdouble( 500.0, 100.0, 26.6384, 0.0567369, -4.16292E-4, 2.60929E-7, 5.0, 2.63299, 1.16558, -0.0170351, 7.95703E-5 ),
    tauJetCalib5 = cms.vdouble( 500.0, 100.0, 29.5396, 0.001137, -1.45232E-4, 6.91445E-8, 5.0, 4.16752, 1.08477, -0.016134, 7.69652E-5 ),
    tauJetCalib6 = cms.vdouble( 500.0, 100.0, 30.1405, -0.14281, 5.55849E-4, -7.52446E-7, 5.0, 4.79283, 0.672125, -0.00879174, 3.65776E-5 )
  ),
  jetCounterSetup = cms.PSet( 
    jetCountersNegativeWheel = cms.VPSet( 
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_1' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_1',
  'JC_centralEta_6' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_11' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_11',
  'JC_centralEta_6' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_19' )      )
    ),
    jetCountersPositiveWheel = cms.VPSet( 
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_1' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_1',
  'JC_centralEta_6' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_11' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_11',
  'JC_centralEta_6' )      ),
      cms.PSet(  cutDescriptionList = cms.vstring( 'JC_minRank_19' )      )
    )
  ),
  ConvertEtValuesToEnergy = cms.bool( False ),
  HfLutEtSumThresholds = cms.vuint32( 8, 12, 16, 24, 200, 800, 2000 ),
  HfLutBitCountThresholds = cms.vuint32( 1, 2, 3, 4, 5, 6, 7 )
)
process.L1MuTriggerScales = cms.ESProducer( "L1MuTriggerScalesProducer",
  nbitPackingDTEta = cms.int32( 6 ),
  signedPackingDTEta = cms.bool( True ),
  nbinsDTEta = cms.int32( 64 ),
  minDTEta = cms.double( -1.2 ),
  maxDTEta = cms.double( 1.2 ),
  offsetDTEta = cms.int32( 32 ),
  nbitPackingCSCEta = cms.int32( 6 ),
  nbinsCSCEta = cms.int32( 32 ),
  minCSCEta = cms.double( 0.9 ),
  maxCSCEta = cms.double( 2.5 ),
  nbitPackingBrlRPCEta = cms.int32( 6 ),
  signedPackingBrlRPCEta = cms.bool( True ),
  nbinsBrlRPCEta = cms.int32( 33 ),
  offsetBrlRPCEta = cms.int32( 16 ),
  nbitPackingFwdRPCEta = cms.int32( 6 ),
  signedPackingFwdRPCEta = cms.bool( True ),
  nbinsFwdRPCEta = cms.int32( 33 ),
  offsetFwdRPCEta = cms.int32( 16 ),
  nbitPackingGMTEta = cms.int32( 6 ),
  nbinsGMTEta = cms.int32( 31 ),
  nbitPackingPhi = cms.int32( 8 ),
  signedPackingPhi = cms.bool( False ),
  nbinsPhi = cms.int32( 144 ),
  minPhi = cms.double( 0.0 ),
  maxPhi = cms.double( 6.2831853 ),
  scaleRPCEta = cms.vdouble( -2.1, -1.97, -1.85, -1.73, -1.61, -1.48, -1.36, -1.24, -1.14, -1.04, -0.93, -0.83, -0.72, -0.58, -0.44, -0.27, -0.07, 0.07, 0.27, 0.44, 0.58, 0.72, 0.83, 0.93, 1.04, 1.14, 1.24, 1.36, 1.48, 1.61, 1.73, 1.85, 1.97, 2.1 ),
  scaleGMTEta = cms.vdouble( 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95, 2.0, 2.05, 2.1, 2.15, 2.2, 2.25, 2.3, 2.35, 2.4 )
)
process.L3MuKFFitter = cms.ESProducer( "KFTrajectoryFitterESProducer",
  ComponentName = cms.string( "L3MuKFFitter" ),
  Propagator = cms.string( "SmartPropagatorAny" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2EstimatorForL3Refit" ),
  minHits = cms.int32( 3 )
)
process.MaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterial" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False )
)
process.MeasurementTracker = cms.ESProducer( "MeasurementTrackerESProducer",
  ComponentName = cms.string( "" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  HitMatcher = cms.string( "StandardMatcher" ),
  Regional = cms.bool( True ),
  OnDemand = cms.bool( True ),
  UseStripModuleQualityDB = cms.bool( False ),
  DebugStripModuleQualityDB = cms.untracked.bool( False ),
  UseStripAPVFiberQualityDB = cms.bool( False ),
  DebugStripAPVFiberQualityDB = cms.untracked.bool( False ),
  MaskBadAPVFibers = cms.bool( False ),
  UseStripStripQualityDB = cms.bool( False ),
  DebugStripStripQualityDB = cms.untracked.bool( False ),
  switchOffPixelsIfEmpty = cms.bool( True ),
  pixelClusterProducer = cms.string( "hltSiPixelClusters" ),
  stripClusterProducer = cms.string( "hltSiStripClusters" ),
  stripLazyGetterProducer = cms.string( "hltSiStripRawToClustersFacility" )
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
  maxCand = cms.int32( 5 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( False ),
  alwaysUseInvalidHits = cms.bool( True )
)
process.MuonDetLayerGeometryESProducer = cms.ESProducer( "MuonDetLayerGeometryESProducer" )
process.MuonNumberingInitialization = cms.ESProducer( "MuonNumberingInitialization" )
process.MuonTransientTrackingRecHitBuilderESProducer = cms.ESProducer( "MuonTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "MuonRecHitBuilder" )
)
process.OppositeMaterialPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "PropagatorWithMaterialOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( False )
)
process.PixelCPEGenericESProducer = cms.ESProducer( "PixelCPEGenericESProducer",
  ComponentName = cms.string( "PixelCPEGeneric" ),
  eff_charge_cut_lowX = cms.untracked.double( 0.0 ),
  eff_charge_cut_lowY = cms.untracked.double( 0.0 ),
  eff_charge_cut_highX = cms.untracked.double( 1.0 ),
  eff_charge_cut_highY = cms.untracked.double( 1.0 ),
  size_cutX = cms.untracked.double( 3.0 ),
  size_cutY = cms.untracked.double( 3.0 ),
  TanLorentzAnglePerTesla = cms.double( 0.106 ),
  PixelErrorParametrization = cms.string( "NOTcmsim" ),
  Alpha2Order = cms.bool( True ),
  ClusterProbComputationFlag = cms.int32( 0 )
)
process.RKTrackerPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "RKTrackerPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( True )
)
process.RPCGeometryESModule = cms.ESProducer( "RPCGeometryESModule"
)
process.RungeKuttaTrackerPropagator = cms.ESProducer( "PropagatorWithMaterialESProducer",
  ComponentName = cms.string( "RungeKuttaTrackerPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Mass = cms.double( 0.105 ),
  MaxDPhi = cms.double( 1.6 ),
  useRungeKutta = cms.bool( True )
)
process.SiStripGainESProducer = cms.ESProducer( "SiStripGainESProducer",
  AutomaticNormalization = cms.bool( False ),
  NormalizationFactor = cms.double( 1.0 ),
  printDebug = cms.untracked.bool( False ),
  APVGain = cms.string( "" )
)
process.SiStripRecHitMatcherESProducer = cms.ESProducer( "SiStripRecHitMatcherESProducer",
  ComponentName = cms.string( "StandardMatcher" ),
  NSigmaInside = cms.double( 3.0 )
)
process.SiStripRegionConnectivity = cms.ESProducer( "SiStripRegionConnectivity",
  EtaDivisions = cms.untracked.uint32( 20 ),
  PhiDivisions = cms.untracked.uint32( 20 ),
  EtaMax = cms.untracked.double( 2.5 )
)
process.SmartPropagator = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagator" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAlong" )
)
process.SmartPropagatorAny = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAny" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterial" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" )
)
process.SmartPropagatorAnyOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAnyOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" )
)
process.SmartPropagatorAnyRK = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorAnyRK" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "RKTrackerPropagator" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAny" )
)
process.SmartPropagatorOpposite = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "PropagatorWithMaterialOpposite" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorOpposite" )
)
process.SmartPropagatorRK = cms.ESProducer( "SmartPropagatorESProducer",
  ComponentName = cms.string( "SmartPropagatorRK" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  Epsilon = cms.double( 5.0 ),
  TrackerPropagator = cms.string( "RKTrackerPropagator" ),
  MuonPropagator = cms.string( "SteppingHelixPropagatorAlong" )
)
process.SmootherRK = cms.ESProducer( "KFTrajectorySmootherESProducer",
  ComponentName = cms.string( "SmootherRK" ),
  Propagator = cms.string( "RungeKuttaTrackerPropagator" ),
  Updator = cms.string( "KFUpdator" ),
  Estimator = cms.string( "Chi2" ),
  errorRescaling = cms.double( 100.0 ),
  minHits = cms.int32( 3 )
)
process.SteppingHelixPropagatorAlong = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAlong" ),
  PropagationDirection = cms.string( "alongMomentum" ),
  SetVBFPointer = cms.bool( False ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  useInTeslaFromMagField = cms.bool( False ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 )
)
process.SteppingHelixPropagatorAny = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorAny" ),
  PropagationDirection = cms.string( "anyDirection" ),
  SetVBFPointer = cms.bool( False ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  useInTeslaFromMagField = cms.bool( False ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 )
)
process.SteppingHelixPropagatorOpposite = cms.ESProducer( "SteppingHelixPropagatorESProducer",
  ComponentName = cms.string( "SteppingHelixPropagatorOpposite" ),
  PropagationDirection = cms.string( "oppositeToMomentum" ),
  SetVBFPointer = cms.bool( False ),
  VBFName = cms.string( "VolumeBasedMagneticField" ),
  useInTeslaFromMagField = cms.bool( False ),
  ApplyRadX0Correction = cms.bool( True ),
  AssumeNoMaterial = cms.bool( False ),
  NoErrorPropagation = cms.bool( False ),
  debug = cms.bool( False ),
  useMagVolumes = cms.bool( True ),
  useMatVolumes = cms.bool( True ),
  useIsYokeFlag = cms.bool( True ),
  returnTangentPlane = cms.bool( True ),
  sendLogWarning = cms.bool( False ),
  useTuningForL2Speed = cms.bool( False ),
  useEndcapShiftsInZ = cms.bool( False ),
  endcapShiftInZPos = cms.double( 0.0 ),
  endcapShiftInZNeg = cms.double( 0.0 )
)
process.StripCPEfromTrackAngleESProducer = cms.ESProducer( "StripCPEfromTrackAngleESProducer",
  ComponentName = cms.string( "StripCPEfromTrackAngle" )
)
process.TrackerDigiGeometryESModule = cms.ESProducer( "TrackerDigiGeometryESModule",
  alignmentsLabel = cms.string( "" ),
  appendToDataLabel = cms.string( "" ),
  applyAlignment = cms.bool( True ),
  fromDDD = cms.bool( True )
)
process.TrackerGeometricDetESModule = cms.ESProducer( "TrackerGeometricDetESModule",
  fromDDD = cms.bool( True )
)
process.TrackerRecoGeometryESProducer = cms.ESProducer( "TrackerRecoGeometryESProducer"
)
process.TransientTrackBuilderESProducer = cms.ESProducer( "TransientTrackBuilderESProducer",
  ComponentName = cms.string( "TransientTrackBuilder" )
)
process.VolumeBasedMagneticFieldESProducer = cms.ESProducer( "VolumeBasedMagneticFieldESProducer",
  label = cms.untracked.string( "" ),
  version = cms.string( "grid_1103l_071212_3_8t" ),
  useParametrizedTrackerField = cms.bool( True ),
  findVolumeTolerance = cms.double( 0.0 ),
  cacheLastVolume = cms.untracked.bool( True ),
  timerOn = cms.untracked.bool( False )
)
process.ZdcHardcodeGeometryEP = cms.ESProducer( "ZdcHardcodeGeometryEP"
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
  alwaysUseInvalidHits = cms.bool( False )
)
process.bJetRegionalTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "bJetRegionalTrajectoryFilter" ),
  filterPset = cms.PSet( 
    minHitsMinPt = cms.int32( 3 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 1.0 ),
    minimumNumberOfHits = cms.int32( 5 ),
    maxNumberOfHits = cms.int32( 8 ),
    maxConsecLostHits = cms.int32( 1 ),
    maxLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" )
  )
)
process.ckfBaseTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "ckfBaseTrajectoryFilter" ),
  filterPset = cms.PSet( 
    minHitsMinPt = cms.int32( 3 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 ),
    minimumNumberOfHits = cms.int32( 5 ),
    maxNumberOfHits = cms.int32( -1 ),
    maxConsecLostHits = cms.int32( 1 ),
    maxLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" )
  )
)
process.hcal_db_producer = cms.ESProducer( "HcalDbProducer",
  dump = cms.untracked.vstring( '' )
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
  alwaysUseInvalidHits = cms.bool( False )
)
process.hltCkfTrajectoryBuilderMumuk = cms.ESProducer( "CkfTrajectoryBuilderESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryBuilderMumuk" ),
  updator = cms.string( "KFUpdator" ),
  propagatorAlong = cms.string( "PropagatorWithMaterial" ),
  propagatorOpposite = cms.string( "PropagatorWithMaterialOpposite" ),
  estimator = cms.string( "Chi2" ),
  TTRHBuilder = cms.string( "WithTrackAngle" ),
  MeasurementTrackerName = cms.string( "" ),
  trajectoryFilterName = cms.string( "hltCkfTrajectoryFilterMumuk" ),
  maxCand = cms.int32( 3 ),
  lostHitPenalty = cms.double( 30.0 ),
  intermediateCleaning = cms.bool( True ),
  alwaysUseInvalidHits = cms.bool( False )
)
process.hltCkfTrajectoryFilterMumu = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryFilterMumu" ),
  filterPset = cms.PSet( 
    minHitsMinPt = cms.int32( 3 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 3.0 ),
    minimumNumberOfHits = cms.int32( 5 ),
    maxNumberOfHits = cms.int32( 5 ),
    maxConsecLostHits = cms.int32( 1 ),
    maxLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" )
  )
)
process.hltCkfTrajectoryFilterMumuk = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "hltCkfTrajectoryFilterMumuk" ),
  filterPset = cms.PSet( 
    minHitsMinPt = cms.int32( 3 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 3.0 ),
    minimumNumberOfHits = cms.int32( 5 ),
    maxNumberOfHits = cms.int32( 5 ),
    maxConsecLostHits = cms.int32( 1 ),
    maxLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" )
  )
)
process.l1CaloGeometry = cms.ESProducer( "L1CaloGeometryProd",
  numberGctEmJetPhiBins = cms.uint32( 18 ),
  gctEmJetPhiBinOffset = cms.double( -0.5 ),
  numberGctEtSumPhiBins = cms.uint32( 72 ),
  gctEtSumPhiBinOffset = cms.double( 0.0 ),
  numberGctCentralEtaBinsPerHalf = cms.uint32( 7 ),
  numberGctForwardEtaBinsPerHalf = cms.uint32( 4 ),
  etaSignBitOffset = cms.uint32( 8 ),
  gctEtaBinBoundaries = cms.vdouble( 0.0, 0.348, 0.695, 1.044, 1.392, 1.74, 2.172, 3.0, 3.5, 4.0, 4.5, 5.0 )
)
process.l1CaloScales = cms.ESProducer( "L1ScalesTrivialProducer",
  L1CaloEmEtScaleLSB = cms.double( 0.5 ),
  L1CaloRegionEtScaleLSB = cms.double( 0.5 ),
  L1CaloEmThresholds = cms.vdouble( 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0 ),
  L1CaloJetThresholds = cms.vdouble( 0.0, 10.0, 12.0, 14.0, 15.0, 18.0, 20.0, 22.0, 24.0, 25.0, 28.0, 30.0, 32.0, 35.0, 37.0, 40.0, 45.0, 50.0, 55.0, 60.0, 65.0, 70.0, 75.0, 80.0, 85.0, 90.0, 100.0, 110.0, 120.0, 125.0, 130.0, 140.0, 150.0, 160.0, 170.0, 175.0, 180.0, 190.0, 200.0, 215.0, 225.0, 235.0, 250.0, 275.0, 300.0, 325.0, 350.0, 375.0, 400.0, 425.0, 450.0, 475.0, 500.0, 525.0, 550.0, 575.0, 600.0, 625.0, 650.0, 675.0, 700.0, 725.0, 750.0, 775.0 )
)
process.l1GtBoardMaps = cms.ESProducer( "L1GtBoardMapsTrivialProducer",
  BoardList = cms.vstring( 'GTFE',
    'FDL',
    'PSB',
    'PSB',
    'PSB',
    'PSB',
    'PSB',
    'PSB',
    'PSB',
    'GMT',
    'TCS',
    'TIM' ),
  BoardIndex = cms.vint32( 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0 ),
  BoardPositionDaqRecord = cms.vint32( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1, -1 ),
  BoardPositionEvmRecord = cms.vint32( 1, 3, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1 ),
  ActiveBoardsDaqRecord = cms.vint32( -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1 ),
  ActiveBoardsEvmRecord = cms.vint32( -1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1 ),
  BoardSlotMap = cms.vint32( 17, 10, 9, 13, 14, 15, 19, 20, 21, 18, 7, 16 ),
  BoardHexNameMap = cms.vint32( 0, 253, 187, 187, 187, 187, 187, 187, 187, 221, 204, 173 ),
  CableList = cms.vstring( 'TechTr',
    'Free',
    'Free',
    'Free',
    'IsoEGQ',
    'NoIsoEGQ',
    'CenJetQ',
    'ForJetQ',
    'TauJetQ',
    'ESumsQ',
    'JetCountsQ',
    'Free',
    'Free',
    'Free',
    'Free',
    'Free',
    'MQB1',
    'MQB2',
    'MQF3',
    'MQF4',
    'MQB5',
    'MQB6',
    'MQF7',
    'MQF8',
    'MQB9',
    'MQB10',
    'MQF11',
    'MQF12' ),
  CableToPsbMap = cms.vint32( 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6 )
)
process.l1GtParameters = cms.ESProducer( "L1GtParametersTrivialProducer",
  TotalBxInEvent = cms.int32( 1 ),
  DaqActiveBoards = cms.uint32( 0xffff ),
  EvmActiveBoards = cms.uint32( 0xffff ),
  BstLengthBytes = cms.uint32( 30 )
)
process.l1GtPrescaleFactorsAlgoTrig = cms.ESProducer( "L1GtPrescaleFactorsAlgoTrigTrivialProducer",
  PrescaleFactorsSet = cms.VPSet( 
    cms.PSet(  PrescaleFactors = cms.vint32( 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 )    )
  )
)
process.l1GtPrescaleFactorsTechTrig = cms.ESProducer( "L1GtPrescaleFactorsTechTrigTrivialProducer",
  PrescaleFactorsSet = cms.VPSet( 
    cms.PSet(  PrescaleFactors = cms.vint32( 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 )    )
  )
)
process.l1GtStableParameters = cms.ESProducer( "L1GtStableParametersTrivialProducer",
  NumberPhysTriggers = cms.uint32( 128 ),
  NumberPhysTriggersExtended = cms.uint32( 64 ),
  NumberTechnicalTriggers = cms.uint32( 64 ),
  NumberL1Mu = cms.uint32( 4 ),
  NumberL1NoIsoEG = cms.uint32( 4 ),
  NumberL1IsoEG = cms.uint32( 4 ),
  NumberL1CenJet = cms.uint32( 4 ),
  NumberL1ForJet = cms.uint32( 4 ),
  NumberL1TauJet = cms.uint32( 4 ),
  NumberL1JetCounts = cms.uint32( 12 ),
  NumberConditionChips = cms.uint32( 2 ),
  PinsOnConditionChip = cms.uint32( 96 ),
  NumberPsbBoards = cms.int32( 7 ),
  IfCaloEtaNumberBits = cms.uint32( 4 ),
  IfMuEtaNumberBits = cms.uint32( 6 ),
  WordLength = cms.int32( 64 ),
  UnitLength = cms.int32( 8 ),
  OrderConditionChip = cms.vint32( 2, 1 )
)
process.l1GtTriggerMaskAlgoTrig = cms.ESProducer( "L1GtTriggerMaskAlgoTrigTrivialProducer",
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.l1GtTriggerMaskTechTrig = cms.ESProducer( "L1GtTriggerMaskTechTrigTrivialProducer",
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.l1GtTriggerMaskVetoAlgoTrig = cms.ESProducer( "L1GtTriggerMaskVetoAlgoTrigTrivialProducer",
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
process.l1GtTriggerMaskVetoTechTrig = cms.ESProducer( "L1GtTriggerMaskVetoTechTrigTrivialProducer",
  TriggerMask = cms.vuint32( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
)
#process.l1GtTriggerMenuXml = cms.ESProducer( "L1GtTriggerMenuXmlProducer",
#  TriggerMenuLuminosity = cms.string( "lumi1030" ),
#  DefXmlFile = cms.string( "L1Menu2008_2E30.xml" ),
#  VmeXmlFile = cms.string( "" )
#)
process.l1GtTriggerMenuXml = cms.ESProducer( "L1GtTriggerMenuXmlProducer",
  TriggerMenuLuminosity = cms.string( "startup" ),
  DefXmlFile = cms.string( "L1Menu_startup2_v2.xml" ),
  #DefXmlFile = cms.string( "L1Menu_startup_v3.xml" ),
  VmeXmlFile = cms.string( "" )
)

process.muonCkfTrajectoryFilter = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "muonCkfTrajectoryFilter" ),
  filterPset = cms.PSet( 
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" ),
    maxLostHits = cms.int32( 1 ),
    maxConsecLostHits = cms.int32( 1 ),
    minimumNumberOfHits = cms.int32( 5 ),
    minPt = cms.double( 0.9 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minHitsMinPt = cms.int32( 3 ),
    maxNumberOfHits = cms.int32( -1 ),
    chargeSignificance = cms.double( -1.0 )
  )
)
process.navigationSchoolESProducer = cms.ESProducer( "NavigationSchoolESProducer",
  ComponentName = cms.string( "SimpleNavigationSchool" )
)
process.pixellayerpairs = cms.ESProducer( "PixelLayerPairsESProducer",
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
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    useErrorsFromParam = cms.untracked.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    useErrorsFromParam = cms.untracked.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    hitErrorRZ = cms.double( 0.0036 )
  )
)
process.TTRHBuilderPixelOnly = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "TTRHBuilderPixelOnly" ),
  StripCPE = cms.string( "Fake" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" )
)
process.WithTrackAngle = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
  ComponentName = cms.string( "WithTrackAngle" ),
  StripCPE = cms.string( "StripCPEfromTrackAngle" ),
  PixelCPE = cms.string( "PixelCPEGeneric" ),
  Matcher = cms.string( "StandardMatcher" )
)
process.pixellayertriplets = cms.ESProducer( "PixelLayerTripletsESProducer",
  ComponentName = cms.string( "PixelLayerTriplets" ),
  layerList = cms.vstring( 'BPix1+BPix2+BPix3',
    'BPix1+BPix2+FPix1_pos',
    'BPix1+BPix2+FPix1_neg',
    'BPix1+FPix1_pos+FPix2_pos',
    'BPix1+FPix1_neg+FPix2_neg' ),
  BPix = cms.PSet( 
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    useErrorsFromParam = cms.untracked.bool( True ),
    hitErrorRPhi = cms.double( 0.0027 ),
    hitErrorRZ = cms.double( 0.0060 )
  ),
  FPix = cms.PSet( 
    HitProducer = cms.string( "hltSiPixelRecHits" ),
    TTRHBuilder = cms.string( "TTRHBuilderPixelOnly" ),
    useErrorsFromParam = cms.untracked.bool( True ),
    hitErrorRPhi = cms.double( 0.0051 ),
    hitErrorRZ = cms.double( 0.0036 )
  )
)
process.sistripconn = cms.ESProducer( "SiStripConnectivity" )
process.softLeptonByDistance = cms.ESProducer( "LeptonTaggerByDistanceESProducer",
  distance = cms.double( 0.5 )
)
process.softLeptonByPt = cms.ESProducer( "LeptonTaggerByPtESProducer",
  ipSign = cms.string( "any" )
)
process.trackCounting3D2nd = cms.ESProducer( "TrackCountingESProducer",
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
  alwaysUseInvalidHits = cms.bool( False )
)
process.trajFilterL3 = cms.ESProducer( "TrajectoryFilterESProducer",
  ComponentName = cms.string( "trajFilterL3" ),
  filterPset = cms.PSet( 
    minHitsMinPt = cms.int32( 3 ),
    nSigmaMinPt = cms.double( 5.0 ),
    minPt = cms.double( 0.9 ),
    minimumNumberOfHits = cms.int32( 5 ),
    maxNumberOfHits = cms.int32( 7 ),
    maxConsecLostHits = cms.int32( 1 ),
    maxLostHits = cms.int32( 1 ),
    chargeSignificance = cms.double( -1.0 ),
    ComponentType = cms.string( "CkfBaseTrajectoryFilter" )
  )
)
process.trajectoryCleanerBySharedHits = cms.ESProducer( "TrajectoryCleanerESProducer",
  ComponentName = cms.string( "TrajectoryCleanerBySharedHits" ),
  fractionShared = cms.double( 0.5 )
)

process.UpdaterService = cms.Service( "UpdaterService",
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
      noTimeStamps = cms.untracked.bool( False ),
      threshold = cms.untracked.string( "INFO" ),
      INFO = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) ),
      FwkReport = cms.untracked.PSet( 
        limit = cms.untracked.int32( 10000000 ),
        reportEvery = cms.untracked.int32( 1 )
      ),
      FwkSummary = cms.untracked.PSet( 
        limit = cms.untracked.int32( 10000000 ),
        reportEvery = cms.untracked.int32( 1 )
      ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) )
    ),
    cout = cms.untracked.PSet(  placeholder = cms.untracked.bool( True ) ),
    errors = cms.untracked.PSet(  placeholder = cms.untracked.bool( True ) ),
    warnings = cms.untracked.PSet(  placeholder = cms.untracked.bool( True ) ),
    infos = cms.untracked.PSet( 
      placeholder = cms.untracked.bool( True ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) )
    ),
    debugs = cms.untracked.PSet(  placeholder = cms.untracked.bool( True ) ),
    fwkJobReports = cms.untracked.vstring( 'FrameworkJobReport' ),
    FrameworkJobReport = cms.untracked.PSet( 
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) )
    ),
)
process.PrescaleService = cms.Service( "PrescaleService",
    lvl1Labels = cms.vstring(  ),
    prescaleTable = cms.VPSet( 
    )
)

process.hltTriggerType = cms.EDFilter( "TriggerTypeFilter",
    InputLabel = cms.string( "rawDataCollector" ),
    TriggerFedId = cms.int32( 812 ),
    SelectedTriggerType = cms.int32( 1 )
)
process.hltGtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
    DaqGtInputTag = cms.InputTag( "rawDataCollector" ),
    DaqGtFedId = cms.untracked.int32( 813 ),
    ActiveBoardsMask = cms.uint32( 0x101 ),
    UnpackBxInEvent = cms.int32( 1 )
)
process.hltGctDigis = cms.EDProducer( "GctRawToDigi",
    inputLabel = cms.InputTag( "rawDataCollector" ),
    gctFedId = cms.int32( 745 ),
    hltMode = cms.bool( False ),
    grenCompatibilityMode = cms.bool( False )
)
process.hltL1GtObjectMap = cms.EDProducer( "L1GlobalTrigger",
    GmtInputTag = cms.InputTag( "hltGtDigis" ),
    GctInputTag = cms.InputTag( "hltGctDigis" ),
    CastorInputTag = cms.InputTag( "castorL1Digis" ),
    TechnicalTriggersInputTag = cms.InputTag( "techTrigDigis" ),
    ProduceL1GtDaqRecord = cms.bool( False ),
    ProduceL1GtEvmRecord = cms.bool( False ),
    ProduceL1GtObjectMapRecord = cms.bool( True ),
    WritePsbL1GtDaqRecord = cms.bool( False ),
    ReadTechnicalTriggerRecords = cms.bool( True ),
    EmulateBxInEvent = cms.int32( 1 ),
    BstLengthBytes = cms.int32( -1 )
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
    centralBxOnly = cms.bool( True )
)
process.hltOfflineBeamSpot = cms.EDProducer( "BeamSpotProducer" )
process.hltL1sDiMuonNoIso = cms.EDFilter( "HLTLevel1GTSeed",
    L1TechTriggerSeeding = cms.bool( False ),
    #L1SeedsLogicalExpression = cms.string( "L1_DoubleMu3" ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleMuOpen" ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" )
)
process.hltPreDoubleMu3 = cms.EDFilter( "HLTPrescaler" )
process.hltDiMuonNoIsoL1Filtered = cms.EDFilter( "HLTMuonL1Filter",
    CandTag = cms.InputTag( "hltL1extraParticles" ),
    PreviousCandTag = cms.InputTag( "hltL1sDiMuonNoIso" ),
    MaxEta = cms.double( 2.5 ),
    MinPt = cms.double( 0.0 ),
    MinQuality = cms.int32( -1 ),
    MinN = cms.int32( 2 )
)
process.hltMuonDTDigis = cms.EDProducer( "DTUnpackingModule",
    dataType = cms.string( "DDU" ),
    fedbyType = cms.untracked.bool( False ),
    inputLabel = cms.untracked.InputTag( "rawDataCollector" ),
    readOutParameters = cms.PSet( 
      localDAQ = cms.untracked.bool( False ),
      performDataIntegrityMonitor = cms.untracked.bool( False ),
      debug = cms.untracked.bool( False ),
      rosParameters = cms.PSet( 
        localDAQ = cms.untracked.bool( False ),
        readingDDU = cms.untracked.bool( True ),
        writeSC = cms.untracked.bool( True ),
        readDDUIDfromDDU = cms.untracked.bool( True ),
        performDataIntegrityMonitor = cms.untracked.bool( False ),
        debug = cms.untracked.bool( False )
      )
    ),
    rosParameters = cms.PSet( 
      localDAQ = cms.untracked.bool( False ),
      readingDDU = cms.untracked.bool( True ),
    )
)
process.hltDt1DRecHits = cms.EDProducer( "DTRecHitProducer",
    debug = cms.untracked.bool( False ),
    dtDigiLabel = cms.InputTag( "hltMuonDTDigis" ),
    recAlgo = cms.string( "DTParametrizedDriftAlgo" ),
    recAlgoConfig = cms.PSet( 
      tTrigMode = cms.string( "DTTTrigSyncFromDB" ),
      tTrigModeConfig = cms.PSet( 
        debug = cms.untracked.bool( False ),
        kFactor = cms.double( -2.0 ),
        vPropWire = cms.double( 24.4 ),
        doT0Correction = cms.bool( True ),
        doTOFCorrection = cms.bool( True ),
        tofCorrType = cms.int32( 1 ),
        doWirePropCorrection = cms.bool( True ),
        wirePropCorrType = cms.int32( 1 )
      ),
      minTime = cms.double( -3.0 ),
      maxTime = cms.double( 415.0 ),
      interpolate = cms.bool( True ),
      debug = cms.untracked.bool( False )
    )
)
process.hltDt4DSegments = cms.EDProducer( "DTRecSegment4DProducer",
    debug = cms.untracked.bool( False ),
    recHits1DLabel = cms.InputTag( "hltDt1DRecHits" ),
    recHits2DLabel = cms.InputTag( "dt2DSegments" ),
    Reco4DAlgoName = cms.string( "DTCombinatorialPatternReco4D" ),
    Reco4DAlgoConfig = cms.PSet( 
      Reco2DAlgoConfig = cms.PSet( 
        recAlgoConfig = cms.PSet( 
          tTrigMode = cms.string( "DTTTrigSyncFromDB" ),
          tTrigModeConfig = cms.PSet( 
            debug = cms.untracked.bool( False ),
            kFactor = cms.double( -2.0 ),
            vPropWire = cms.double( 24.4 ),
            doT0Correction = cms.bool( True ),
            doTOFCorrection = cms.bool( True ),
            tofCorrType = cms.int32( 1 ),
            doWirePropCorrection = cms.bool( True ),
            wirePropCorrType = cms.int32( 1 )
          ),
          minTime = cms.double( -3.0 ),
          maxTime = cms.double( 415.0 ),
          interpolate = cms.bool( True ),
          debug = cms.untracked.bool( False )
        ),
        recAlgo = cms.string( "DTParametrizedDriftAlgo" ),
        MaxAllowedHits = cms.uint32( 50 ),
        AlphaMaxTheta = cms.double( 0.1 ),
        AlphaMaxPhi = cms.double( 1.0 ),
        debug = cms.untracked.bool( False ),
        nSharedHitsMax = cms.int32( 2 ),
        nUnSharedHitsMin = cms.int32( 2 ),
        segmCleanerMode = cms.int32( 1 )
      ),
      Reco2DAlgoName = cms.string( "DTCombinatorialPatternReco" ),
      recAlgoConfig = cms.PSet( 
        tTrigMode = cms.string( "DTTTrigSyncFromDB" ),
        tTrigModeConfig = cms.PSet( 
          debug = cms.untracked.bool( False ),
          kFactor = cms.double( -2.0 ),
          vPropWire = cms.double( 24.4 ),
          doT0Correction = cms.bool( True ),
          doTOFCorrection = cms.bool( True ),
          tofCorrType = cms.int32( 1 ),
          doWirePropCorrection = cms.bool( True ),
          wirePropCorrType = cms.int32( 1 )
        ),
        minTime = cms.double( -3.0 ),
        maxTime = cms.double( 415.0 ),
        interpolate = cms.bool( True ),
        debug = cms.untracked.bool( False )
      ),
      recAlgo = cms.string( "DTParametrizedDriftAlgo" ),
      AllDTRecHits = cms.bool( True ),
      debug = cms.untracked.bool( False ),
      nSharedHitsMax = cms.int32( 2 ),
      nUnSharedHitsMin = cms.int32( 2 ),
      segmCleanerMode = cms.int32( 1 )
    )
)
process.hltMuonCSCDigis = cms.EDProducer( "CSCDCCUnpacker",
    PrintEventNumber = cms.untracked.bool( False ),
    UseExaminer = cms.untracked.bool( False ),
    ExaminerMask = cms.untracked.uint32( 0x1febf3f6 ),
    ErrorMask = cms.untracked.uint32( 0x0 ),
    InputObjects = cms.InputTag( "rawDataCollector" ),
    UseSelectiveUnpacking = cms.untracked.bool( True )
)
process.hltCsc2DRecHits = cms.EDProducer( "CSCRecHitDProducer",
    CSCUseCalibrations = cms.untracked.bool( True ),
    stripDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCStripDigi' ),
    wireDigiTag = cms.InputTag( 'hltMuonCSCDigis','MuonCSCWireDigi' ),
    CSCstripWireDeltaTime = cms.untracked.int32( 8 ),
    CSCStripPeakThreshold = cms.untracked.double( 10.0 ),
    CSCStripClusterChargeCut = cms.untracked.double( 25.0 ),
    CSCWireClusterDeltaT = cms.untracked.int32( 1 ),
    CSCStripxtalksOffset = cms.untracked.double( 0.03 ),
    NoiseLevel_ME1a = cms.untracked.double( 7.0 ),
    XTasymmetry_ME1a = cms.untracked.double( 0.0 ),
    ConstSyst_ME1a = cms.untracked.double( 0.022 ),
    NoiseLevel_ME1b = cms.untracked.double( 7.0 ),
    XTasymmetry_ME1b = cms.untracked.double( 0.0 ),
    ConstSyst_ME1b = cms.untracked.double( 0.02 ),
    NoiseLevel_ME12 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME12 = cms.untracked.double( 0.025 ),
    ConstSyst_ME12 = cms.untracked.double( 0.045 ),
    NoiseLevel_ME13 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME13 = cms.untracked.double( 0.025 ),
    ConstSyst_ME13 = cms.untracked.double( 0.065 ),
    NoiseLevel_ME21 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME21 = cms.untracked.double( 0.025 ),
    ConstSyst_ME21 = cms.untracked.double( 0.06 ),
    NoiseLevel_ME22 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME22 = cms.untracked.double( 0.025 ),
    ConstSyst_ME22 = cms.untracked.double( 0.06 ),
    NoiseLevel_ME31 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME31 = cms.untracked.double( 0.025 ),
    ConstSyst_ME31 = cms.untracked.double( 0.06 ),
    NoiseLevel_ME32 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME32 = cms.untracked.double( 0.025 ),
    ConstSyst_ME32 = cms.untracked.double( 0.06 ),
    NoiseLevel_ME41 = cms.untracked.double( 7.0 ),
    XTasymmetry_ME41 = cms.untracked.double( 0.025 ),
    ConstSyst_ME41 = cms.untracked.double( 0.06 ),
    readBadChannels = cms.bool( False ),
    readBadChambers = cms.bool( False )
)
process.hltCscSegments = cms.EDProducer( "CSCSegmentProducer",
    inputObjects = cms.InputTag( "hltCsc2DRecHits" ),
    algo_type = cms.int32( 4 ),
    algo_psets = cms.VPSet( 
      cms.PSet(  algo_name = cms.string( "CSCSegAlgoSK" ),
        parameters_per_chamber_type = cms.vint32( 2, 1, 1, 1, 1, 1, 1, 1, 1 ),
        chamber_types = cms.vstring( 'ME1/a',
          'ME1/b',
          'ME1/2',
          'ME1/3',
          'ME2/1',
          'ME2/2',
          'ME3/1',
          'ME3/2',
          'ME4/1' ),
        algo_psets = cms.VPSet( 
          cms.PSet(  minLayersApart = cms.int32( 2 ),
            wideSeg = cms.double( 3.0 ),
            chi2Max = cms.double( 99999.0 ),
            dPhiFineMax = cms.double( 0.025 ),
            dRPhiFineMax = cms.double( 8.0 ),
            dPhiMax = cms.double( 0.0030 ),
            dRPhiMax = cms.double( 8.0 ),
            verboseInfo = cms.untracked.bool( True )
          ),
          cms.PSet(  minLayersApart = cms.int32( 2 ),
            wideSeg = cms.double( 3.0 ),
            chi2Max = cms.double( 99999.0 ),
            dPhiFineMax = cms.double( 0.025 ),
            dRPhiFineMax = cms.double( 3.0 ),
            dPhiMax = cms.double( 0.025 ),
            dRPhiMax = cms.double( 8.0 ),
            verboseInfo = cms.untracked.bool( True )
          )
        )
      ),
      cms.PSet(  algo_name = cms.string( "CSCSegAlgoTC" ),
        parameters_per_chamber_type = cms.vint32( 2, 1, 1, 1, 1, 1, 1, 1, 1 ),
        chamber_types = cms.vstring( 'ME1/a',
          'ME1/b',
          'ME1/2',
          'ME1/3',
          'ME2/1',
          'ME2/2',
          'ME3/1',
          'ME3/2',
          'ME4/1' ),
        algo_psets = cms.VPSet( 
          cms.PSet(  SegmentSorting = cms.int32( 1 ),
            minLayersApart = cms.int32( 2 ),
            chi2ndfProbMin = cms.double( 1.0E-4 ),
            chi2Max = cms.double( 6000.0 ),
            dPhiFineMax = cms.double( 0.02 ),
            dRPhiFineMax = cms.double( 6.0 ),
            dPhiMax = cms.double( 0.0030 ),
            dRPhiMax = cms.double( 1.2 ),
            verboseInfo = cms.untracked.bool( True )
          ),
          cms.PSet(  SegmentSorting = cms.int32( 1 ),
            minLayersApart = cms.int32( 2 ),
            chi2ndfProbMin = cms.double( 1.0E-4 ),
            chi2Max = cms.double( 6000.0 ),
            dPhiFineMax = cms.double( 0.013 ),
            dRPhiFineMax = cms.double( 3.0 ),
            dPhiMax = cms.double( 0.00198 ),
            dRPhiMax = cms.double( 0.6 ),
            verboseInfo = cms.untracked.bool( True )
          )
        )
      ),
      cms.PSet(  algo_name = cms.string( "CSCSegAlgoDF" ),
        parameters_per_chamber_type = cms.vint32( 3, 1, 2, 2, 1, 2, 1, 2, 1 ),
        chamber_types = cms.vstring( 'ME1/a',
          'ME1/b',
          'ME1/2',
          'ME1/3',
          'ME2/1',
          'ME2/2',
          'ME3/1',
          'ME3/2',
          'ME4/1' ),
        algo_psets = cms.VPSet( 
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            Pruning = cms.untracked.bool( False ),
            nHitsPerClusterIsShower = cms.int32( 20 ),
            minHitsForPreClustering = cms.int32( 10 ),
            dYclusBoxMax = cms.double( 8.0 ),
            dXclusBoxMax = cms.double( 8.0 ),
            preClustering = cms.untracked.bool( False ),
            chi2Max = cms.double( 5000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            tanThetaMax = cms.double( 1.2 ),
            minLayersApart = cms.int32( 2 ),
            dPhiFineMax = cms.double( 0.025 ),
            dRPhiFineMax = cms.double( 8.0 ),
            minHitsPerSegment = cms.int32( 3 ),
            CSCSegmentDebug = cms.untracked.bool( False )
          ),
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            Pruning = cms.untracked.bool( False ),
            nHitsPerClusterIsShower = cms.int32( 20 ),
            minHitsForPreClustering = cms.int32( 10 ),
            dYclusBoxMax = cms.double( 12.0 ),
            dXclusBoxMax = cms.double( 8.0 ),
            preClustering = cms.untracked.bool( False ),
            chi2Max = cms.double( 5000.0 ),
            tanPhiMax = cms.double( 0.8 ),
            tanThetaMax = cms.double( 2.0 ),
            minLayersApart = cms.int32( 2 ),
            dPhiFineMax = cms.double( 0.025 ),
            dRPhiFineMax = cms.double( 12.0 ),
            minHitsPerSegment = cms.int32( 3 ),
            CSCSegmentDebug = cms.untracked.bool( False )
          ),
          cms.PSet(  maxRatioResidualPrune = cms.double( 3.0 ),
            Pruning = cms.untracked.bool( False ),
            nHitsPerClusterIsShower = cms.int32( 20 ),
            minHitsForPreClustering = cms.int32( 30 ),
            dYclusBoxMax = cms.double( 8.0 ),
            dXclusBoxMax = cms.double( 8.0 ),
            preClustering = cms.untracked.bool( False ),
            chi2Max = cms.double( 5000.0 ),
            tanPhiMax = cms.double( 0.5 ),
            tanThetaMax = cms.double( 1.2 ),
            minLayersApart = cms.int32( 2 ),
            dPhiFineMax = cms.double( 0.025 ),
            dRPhiFineMax = cms.double( 8.0 ),
            minHitsPerSegment = cms.int32( 3 ),
            CSCSegmentDebug = cms.untracked.bool( False )
          )
        )
      ),
      cms.PSet(  algo_name = cms.string( "CSCSegAlgoST" ),
        parameters_per_chamber_type = cms.vint32( 2, 1, 1, 1, 1, 1, 1, 1, 1 ),
        chamber_types = cms.vstring( 'ME1/a',
          'ME1/b',
          'ME1/2',
          'ME1/3',
          'ME2/1',
          'ME2/2',
          'ME3/1',
          'ME3/2',
          'ME4/1' ),
        algo_psets = cms.VPSet( 
          cms.PSet(  curvePenalty = cms.untracked.double( 2.0 ),
            curvePenaltyThreshold = cms.untracked.double( 0.85 ),
            yweightPenalty = cms.untracked.double( 1.5 ),
            yweightPenaltyThreshold = cms.untracked.double( 1.0 ),
            hitDropLimit6Hits = cms.untracked.double( 0.3333 ),
            hitDropLimit5Hits = cms.untracked.double( 0.8 ),
            hitDropLimit4Hits = cms.untracked.double( 0.6 ),
            onlyBestSegment = cms.untracked.bool( False ),
            BrutePruning = cms.untracked.bool( False ),
            Pruning = cms.untracked.bool( False ),
            preClustering = cms.untracked.bool( True ),
            maxRecHitsInCluster = cms.untracked.int32( 20 ),
            dYclusBoxMax = cms.untracked.double( 8.0 ),
            dXclusBoxMax = cms.untracked.double( 4.0 ),
            minHitsPerSegment = cms.untracked.int32( 3 ),
            CSCDebug = cms.untracked.bool( False )
          ),
          cms.PSet(  curvePenalty = cms.untracked.double( 2.0 ),
            curvePenaltyThreshold = cms.untracked.double( 0.85 ),
            yweightPenalty = cms.untracked.double( 1.5 ),
            yweightPenaltyThreshold = cms.untracked.double( 1.0 ),
            hitDropLimit6Hits = cms.untracked.double( 0.3333 ),
            hitDropLimit5Hits = cms.untracked.double( 0.8 ),
            hitDropLimit4Hits = cms.untracked.double( 0.6 ),
            onlyBestSegment = cms.untracked.bool( False ),
            BrutePruning = cms.untracked.bool( False ),
            Pruning = cms.untracked.bool( False ),
            preClustering = cms.untracked.bool( True ),
            maxRecHitsInCluster = cms.untracked.int32( 24 ),
            dYclusBoxMax = cms.untracked.double( 8.0 ),
            dXclusBoxMax = cms.untracked.double( 4.0 ),
            minHitsPerSegment = cms.untracked.int32( 3 ),
            CSCDebug = cms.untracked.bool( False )
          )
        )
      )
    )
)
process.hltMuonRPCDigis = cms.EDProducer( "RPCUnpackingModule",
    InputLabel = cms.untracked.InputTag( "rawDataCollector" )
)
process.hltRpcRecHits = cms.EDProducer( "RPCRecHitProducer",
    rpcDigiLabel = cms.InputTag( "hltMuonRPCDigis" ),
    recAlgo = cms.string( "RPCRecHitStandardAlgo" ),
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
      UseMuonNavigation = cms.untracked.bool( True ),
      RPCLayers = cms.bool( True ),
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny',
        'SteppingHelixPropagatorAlong',
        'SteppingHelixPropagatorOpposite',
        'PropagatorWithMaterial',
        'PropagatorWithMaterialOpposite',
        'SmartPropagator',
        'SmartPropagatorOpposite',
        'SmartPropagatorAnyOpposite',
        'SmartPropagatorAny',
        'SmartPropagatorRK',
        'SmartPropagatorAnyRK' )
    )
)
process.hltL2Muons = cms.EDProducer( "L2MuonProducer",
    InputObjects = cms.InputTag( "hltL2MuonSeeds" ),
    L2TrajBuilderParameters = cms.PSet( 
      SeedPosition = cms.string( "in" ),
      SeedPropagator = cms.string( "SteppingHelixPropagatorAny" ),
      NavigationType = cms.string( "Standard" ),
      RefitterParameters = cms.PSet( 
        FitterName = cms.string( "KFFitterSmootherForL2Muon" ),
        Option = cms.int32( 1 )
      ),
      BWFilterParameters = cms.PSet( 
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        EnableDTMeasurement = cms.bool( True ),
        EnableCSCMeasurement = cms.bool( True ),
        EnableRPCMeasurement = cms.bool( True ),
        BWSeedType = cms.string( "fromGenerator" ),
        FitDirection = cms.string( "outsideIn" ),
        Propagator = cms.string( "SteppingHelixPropagatorAny" ),
        MaxChi2 = cms.double( 25.0 ),
        NumberOfSigma = cms.double( 3.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 25.0 ),
          Granularity = cms.int32( 2 ),
          RescaleError = cms.bool( False ),
          RescaleErrorFactor = cms.double( 100.0 )
        )
      ),
      DoRefit = cms.bool( False ),
      FilterParameters = cms.PSet( 
        DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
        CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
        RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
        EnableDTMeasurement = cms.bool( True ),
        EnableCSCMeasurement = cms.bool( True ),
        EnableRPCMeasurement = cms.bool( True ),
        FitDirection = cms.string( "insideOut" ),
        Propagator = cms.string( "SteppingHelixPropagatorAny" ),
        MaxChi2 = cms.double( 1000.0 ),
        NumberOfSigma = cms.double( 3.0 ),
        MuonTrajectoryUpdatorParameters = cms.PSet( 
          MaxChi2 = cms.double( 1000.0 ),
          Granularity = cms.int32( 0 ),
          RescaleError = cms.bool( False ),
          RescaleErrorFactor = cms.double( 100.0 )
        )
      ),
      DoBackwardFilter = cms.bool( True )
    ),
    ServiceParameters = cms.PSet( 
      UseMuonNavigation = cms.untracked.bool( True ),
      RPCLayers = cms.bool( True ),
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny',
        'SteppingHelixPropagatorAlong',
        'SteppingHelixPropagatorOpposite',
        'PropagatorWithMaterial',
        'PropagatorWithMaterialOpposite',
        'SmartPropagator',
        'SmartPropagatorOpposite',
        'SmartPropagatorAnyOpposite',
        'SmartPropagatorAny',
        'SmartPropagatorRK',
        'SmartPropagatorAnyRK' )
    ),
    TrackLoaderParameters = cms.PSet( 
      MuonUpdatorAtVertexParameters = cms.PSet( 
        Propagator = cms.string( "SteppingHelixPropagatorOpposite" ),
        BeamSpotPosition = cms.vdouble( 0.0, 0.0, 0.0 ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 ),
        MaxChi2 = cms.double( 1000000.0 )
      ),
      VertexConstraint = cms.bool( True ),
      DoSmoothing = cms.bool( False ),
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" )
    )
)
process.hltL2MuonCandidates = cms.EDProducer( "L2MuonCandidateProducer",
    InputObjects = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' )
)
process.hltDiMuonNoIsoL2PreFiltered = cms.EDFilter( "HLTMuonL2PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    SeedTag = cms.InputTag( "hltL2MuonSeeds" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonNoIsoL1Filtered" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 9999.0 ),
    MaxDz = cms.double( 9999.0 ),
    #MinPt = cms.double( 3.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 )
)
process.hltSiPixelDigis = cms.EDProducer( "SiPixelRawToDigi",
    InputLabel = cms.untracked.string( "rawDataCollector" )
)
process.hltSiPixelClusters = cms.EDProducer( "SiPixelClusterProducer",
    src = cms.InputTag( "hltSiPixelDigis" ),
    payloadType = cms.string( "HLT" ),
    ChannelThreshold = cms.int32( 2500 ),
    SeedThreshold = cms.int32( 3000 ),
    ClusterThreshold = cms.double( 5050.0 ),
    VCaltoElectronGain = cms.int32( 65 ),
    VCaltoElectronOffset = cms.int32( 0 ),
    MissCalibrate = cms.untracked.bool( True )
)
process.hltSiPixelRecHits = cms.EDProducer( "SiPixelRecHitConverter",
    src = cms.InputTag( "hltSiPixelClusters" ),
    CPE = cms.string( "PixelCPEGeneric" )
)
process.hltSiStripRawToClustersFacility = cms.EDProducer( "SiStripRawToClusters",
    ProductLabel = cms.untracked.string( "rawDataCollector" ),
    MaxHolesInCluster = cms.untracked.uint32( 0 ),
    ClusterThreshold = cms.untracked.double( 5.0 ),
    SeedThreshold = cms.untracked.double( 3.0 ),
    ChannelThreshold = cms.untracked.double( 2.0 ),
    ClusterizerAlgorithm = cms.untracked.string( "ThreeThreshold" )
)
process.hltSiStripClusters = cms.EDProducer( "MeasurementTrackerSiStripRefGetterProducer",
    InputModuleLabel = cms.InputTag( "hltSiStripRawToClustersFacility" ),
    measurementTrackerName = cms.string( "" )
)
process.hltL3TrajectorySeed = cms.EDProducer( "TSGFromL2Muon",
    PtCut = cms.double( 1.0 ),
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    tkSeedGenerator = cms.string( "TSGFromCombinedHits" ),
    ServiceParameters = cms.PSet( 
      UseMuonNavigation = cms.untracked.bool( True ),
      RPCLayers = cms.bool( True ),
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny',
        'SteppingHelixPropagatorAlong',
        'SteppingHelixPropagatorOpposite',
        'PropagatorWithMaterial',
        'PropagatorWithMaterialOpposite',
        'SmartPropagator',
        'SmartPropagatorOpposite',
        'SmartPropagatorAnyOpposite',
        'SmartPropagatorAny',
        'SmartPropagatorRK',
        'SmartPropagatorAnyRK' )
    ),
    MuonTrackingRegionBuilder = cms.PSet( 
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
      UseVertex = cms.bool( False ),
      Rescale_eta = cms.double( 3.0 ),
      Rescale_phi = cms.double( 3.0 ),
      Rescale_Dz = cms.double( 3.0 ),
      DeltaZ_Region = cms.double( 15.9 ),
      DeltaR = cms.double( 0.2 ),
      EscapePt = cms.double( 1.5 ),
      Phi_min = cms.double( 0.1 ),
      Eta_min = cms.double( 0.1 ),
      UseFixedRegion = cms.bool( False ),
      EtaR_UpperLimit_Par1 = cms.double( 0.25 ),
      EtaR_UpperLimit_Par2 = cms.double( 0.15 ),
      PhiR_UpperLimit_Par1 = cms.double( 0.6 ),
      PhiR_UpperLimit_Par2 = cms.double( 0.2 ),
      vertexCollection = cms.InputTag( "pixelVertices" ),
      Eta_fixed = cms.double( 0.2 ),
      Phi_fixed = cms.double( 0.2 ),
      OnDemand = cms.double( -1.0 )
    ),
    TrackerSeedCleaner = cms.PSet( 
      TTRHBuilder = cms.string( "WithTrackAngle" ),
      beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
      directionCleaner = cms.bool( True ),
      cleanerFromSharedHits = cms.bool( True ),
      ptCleaner = cms.bool( True )
    ),
    TSGFromMixedPairs = cms.PSet( 
      ComponentName = cms.string( "TSGFromOrderedHits" ),
      TTRHBuilder = cms.string( "WithTrackAngle" ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "MixedLayerPairs" )
      )
    ),
    TSGFromPixelTriplets = cms.PSet( 
      ComponentName = cms.string( "TSGFromOrderedHits" ),
      TTRHBuilder = cms.string( "WithTrackAngle" ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitTripletGenerator" ),
        SeedingLayers = cms.string( "PixelLayerTriplets" ),
        GeneratorPSet = cms.PSet( 
          ComponentName = cms.string( "PixelTripletHLTGenerator" ),
          useFixedPreFiltering = cms.bool( False ),
          phiPreFiltering = cms.double( 0.3 ),
          extraHitRZtolerance = cms.double( 0.06 ),
          extraHitRPhitolerance = cms.double( 0.06 ),
          useBending = cms.bool( True ),
          useMultScattering = cms.bool( True )
        )
      )
    ),
    TSGFromPixelPairs = cms.PSet( 
      ComponentName = cms.string( "TSGFromOrderedHits" ),
      TTRHBuilder = cms.string( "WithTrackAngle" ),
      OrderedHitsFactoryPSet = cms.PSet( 
        ComponentName = cms.string( "StandardHitPairGenerator" ),
        SeedingLayers = cms.string( "PixelLayerPairs" )
      )
    ),
    TSGForRoadSearchOI = cms.PSet( 
      ComponentName = cms.string( "TSGForRoadSearch" ),
      option = cms.uint32( 3 ),
      copyMuonRecHit = cms.bool( False ),
      manySeeds = cms.bool( False ),
      maxChi2 = cms.double( 40.0 ),
      propagatorName = cms.string( "SteppingHelixPropagatorAlong" ),
      propagatorCompatibleName = cms.string( "SteppingHelixPropagatorAny" ),
      errorMatrixPset = cms.PSet( 
        errorMatrixValuesPSet = cms.PSet( 
          xAxis = cms.vdouble( 0.0, 3.16, 6.69, 10.695, 15.319, 20.787, 27.479, 36.106, 48.26, 69.03, 200.0 ),
          yAxis = cms.vdouble( 0.0, 0.2, 0.3, 0.7, 0.9, 1.15, 1.35, 1.55, 1.75, 2.2, 2.5 ),
          zAxis = cms.vdouble( -3.14159, 3.14159 ),
          pf3_V11 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 4.593, 5.253, 1.895, 1.985, 2.344, 5.37, 2.059, 2.423, 1.985, 2.054, 2.071, 2.232, 2.159, 2.1, 2.355, 3.862, 1.855, 2.311, 1.784, 1.766, 1.999, 2.18, 2.071, 2.03, 2.212, 2.266, 1.693, 1.984, 1.664, 1.602, 1.761, 2.007, 1.985, 1.982, 2.118, 1.734, 1.647, 1.705, 1.56, 1.542, 1.699, 2.058, 2.037, 1.934, 2.067, 1.555, 1.566, 1.638, 1.51, 1.486, 1.635, 1.977, 1.944, 1.865, 1.925, 1.415, 1.542, 1.571, 1.499, 1.468, 1.608, 1.899, 1.893, 1.788, 1.851, 1.22, 1.49, 1.54, 1.493, 1.457, 1.572, 1.876, 1.848, 1.751, 1.827, 1.223, 1.51, 1.583, 1.486, 1.431, 1.534, 1.79, 1.802, 1.65, 1.755, 1.256, 1.489, 1.641, 1.464, 1.438, 1.48, 1.888, 1.839, 1.657, 1.903, 1.899 ) ),
          pf3_V12 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V13 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V14 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V15 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V22 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 66.152, 3.219, 66.051, 1.298, 1.186, 1.197, 1.529, 2.807, 1.056, 1.092, 1.15, 1.158, 1.163, 1.05, 1.191, 1.287, 1.371, 2.039, 1.02, 1.059, 1.048, 1.087, 1.087, 1.041, 1.072, 1.118, 1.097, 1.229, 1.042, 1.07, 1.071, 1.063, 1.039, 1.038, 1.061, 1.052, 1.058, 1.188, 1.099, 1.075, 1.082, 1.055, 1.084, 1.024, 1.058, 1.069, 1.022, 1.184, 1.117, 1.105, 1.093, 1.082, 1.086, 1.053, 1.097, 1.07, 1.044, 1.125, 1.141, 1.167, 1.136, 1.133, 1.146, 1.089, 1.081, 1.117, 1.085, 1.075, 1.212, 1.199, 1.186, 1.212, 1.168, 1.125, 1.127, 1.119, 1.114, 1.062, 1.273, 1.229, 1.272, 1.293, 1.172, 1.124, 1.141, 1.123, 1.158, 1.115, 1.419, 1.398, 1.425, 1.394, 1.278, 1.132, 1.132, 1.115, 1.26, 1.096 ) ),
          pf3_V23 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V24 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V25 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V33 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 6.174, 56.89, 1.019, 2.206, 1.694, 1.698, 1.776, 3.563, 2.141, 2.432, 1.898, 1.834, 1.763, 1.797, 1.944, 1.857, 2.068, 2.894, 1.76, 2.185, 1.664, 1.656, 1.761, 1.964, 1.925, 1.89, 2.012, 2.014, 1.651, 1.825, 1.573, 1.534, 1.634, 1.856, 1.962, 1.879, 1.95, 1.657, 1.556, 1.639, 1.481, 1.433, 1.605, 1.943, 1.99, 1.885, 1.916, 1.511, 1.493, 1.556, 1.445, 1.457, 1.543, 1.897, 1.919, 1.884, 1.797, 1.394, 1.489, 1.571, 1.436, 1.425, 1.534, 1.796, 1.845, 1.795, 1.763, 1.272, 1.472, 1.484, 1.452, 1.412, 1.508, 1.795, 1.795, 1.773, 1.741, 1.207, 1.458, 1.522, 1.437, 1.399, 1.485, 1.747, 1.739, 1.741, 1.716, 1.187, 1.463, 1.589, 1.411, 1.404, 1.471, 1.92, 1.86, 1.798, 1.867, 1.436 ) ),
          pf3_V34 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V35 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V44 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.622, 2.139, 2.08, 1.178, 1.044, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.01, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.002, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.011, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.002, 1.0, 1.002, 1.013, 1.0, 1.0, 1.0, 1.0, 1.0, 1.005, 1.0, 1.004, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.009, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V45 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V55 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 27.275, 15.167, 13.818, 1.0, 1.0, 1.0, 1.037, 1.129, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.023, 1.028, 1.063, 1.08, 1.077, 1.054, 1.068, 1.065, 1.047, 1.025, 1.046, 1.064, 1.082, 1.078, 1.137, 1.12, 1.163, 1.158, 1.112, 1.072, 1.054, 1.095, 1.101, 1.092, 1.219, 1.167, 1.186, 1.203, 1.144, 1.096, 1.095, 1.109, 1.111, 1.105, 1.236, 1.187, 1.203, 1.262, 1.2, 1.086, 1.106, 1.112, 1.138, 1.076, 1.287, 1.255, 1.241, 1.334, 1.244, 1.112, 1.083, 1.111, 1.127, 1.025, 1.309, 1.257, 1.263, 1.393, 1.23, 1.091, 1.075, 1.078, 1.135, 1.042, 1.313, 1.303, 1.295, 1.436, 1.237, 1.064, 1.078, 1.075, 1.149, 1.037, 1.329, 1.509, 1.369, 1.546, 1.269, 1.079, 1.084, 1.047, 1.183, 1.008 ) )
        ),
        action = cms.string( "use" ),
        atIP = cms.bool( True ),
        assignError = cms.bool( False )
      )
    ),
    TSGForRoadSearchIOpxl = cms.PSet( 
      ComponentName = cms.string( "TSGForRoadSearch" ),
      option = cms.uint32( 4 ),
      copyMuonRecHit = cms.bool( False ),
      manySeeds = cms.bool( False ),
      maxChi2 = cms.double( 40.0 ),
      propagatorName = cms.string( "SteppingHelixPropagatorAlong" ),
      propagatorCompatibleName = cms.string( "SteppingHelixPropagatorAny" ),
      errorMatrixPset = cms.PSet( 
        errorMatrixValuesPSet = cms.PSet( 
          xAxis = cms.vdouble( 0.0, 3.16, 6.69, 10.695, 15.319, 20.787, 27.479, 36.106, 48.26, 69.03, 200.0 ),
          yAxis = cms.vdouble( 0.0, 0.2, 0.3, 0.7, 0.9, 1.15, 1.35, 1.55, 1.75, 2.2, 2.5 ),
          zAxis = cms.vdouble( -3.14159, 3.14159 ),
          pf3_V11 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 4.593, 5.253, 1.895, 1.985, 2.344, 5.37, 2.059, 2.423, 1.985, 2.054, 2.071, 2.232, 2.159, 2.1, 2.355, 3.862, 1.855, 2.311, 1.784, 1.766, 1.999, 2.18, 2.071, 2.03, 2.212, 2.266, 1.693, 1.984, 1.664, 1.602, 1.761, 2.007, 1.985, 1.982, 2.118, 1.734, 1.647, 1.705, 1.56, 1.542, 1.699, 2.058, 2.037, 1.934, 2.067, 1.555, 1.566, 1.638, 1.51, 1.486, 1.635, 1.977, 1.944, 1.865, 1.925, 1.415, 1.542, 1.571, 1.499, 1.468, 1.608, 1.899, 1.893, 1.788, 1.851, 1.22, 1.49, 1.54, 1.493, 1.457, 1.572, 1.876, 1.848, 1.751, 1.827, 1.223, 1.51, 1.583, 1.486, 1.431, 1.534, 1.79, 1.802, 1.65, 1.755, 1.256, 1.489, 1.641, 1.464, 1.438, 1.48, 1.888, 1.839, 1.657, 1.903, 1.899 ) ),
          pf3_V12 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V13 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V14 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V15 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V22 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 66.152, 3.219, 66.051, 1.298, 1.186, 1.197, 1.529, 2.807, 1.056, 1.092, 1.15, 1.158, 1.163, 1.05, 1.191, 1.287, 1.371, 2.039, 1.02, 1.059, 1.048, 1.087, 1.087, 1.041, 1.072, 1.118, 1.097, 1.229, 1.042, 1.07, 1.071, 1.063, 1.039, 1.038, 1.061, 1.052, 1.058, 1.188, 1.099, 1.075, 1.082, 1.055, 1.084, 1.024, 1.058, 1.069, 1.022, 1.184, 1.117, 1.105, 1.093, 1.082, 1.086, 1.053, 1.097, 1.07, 1.044, 1.125, 1.141, 1.167, 1.136, 1.133, 1.146, 1.089, 1.081, 1.117, 1.085, 1.075, 1.212, 1.199, 1.186, 1.212, 1.168, 1.125, 1.127, 1.119, 1.114, 1.062, 1.273, 1.229, 1.272, 1.293, 1.172, 1.124, 1.141, 1.123, 1.158, 1.115, 1.419, 1.398, 1.425, 1.394, 1.278, 1.132, 1.132, 1.115, 1.26, 1.096 ) ),
          pf3_V23 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V24 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V25 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V33 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 6.174, 56.89, 1.019, 2.206, 1.694, 1.698, 1.776, 3.563, 2.141, 2.432, 1.898, 1.834, 1.763, 1.797, 1.944, 1.857, 2.068, 2.894, 1.76, 2.185, 1.664, 1.656, 1.761, 1.964, 1.925, 1.89, 2.012, 2.014, 1.651, 1.825, 1.573, 1.534, 1.634, 1.856, 1.962, 1.879, 1.95, 1.657, 1.556, 1.639, 1.481, 1.433, 1.605, 1.943, 1.99, 1.885, 1.916, 1.511, 1.493, 1.556, 1.445, 1.457, 1.543, 1.897, 1.919, 1.884, 1.797, 1.394, 1.489, 1.571, 1.436, 1.425, 1.534, 1.796, 1.845, 1.795, 1.763, 1.272, 1.472, 1.484, 1.452, 1.412, 1.508, 1.795, 1.795, 1.773, 1.741, 1.207, 1.458, 1.522, 1.437, 1.399, 1.485, 1.747, 1.739, 1.741, 1.716, 1.187, 1.463, 1.589, 1.411, 1.404, 1.471, 1.92, 1.86, 1.798, 1.867, 1.436 ) ),
          pf3_V34 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V35 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V44 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.622, 2.139, 2.08, 1.178, 1.044, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.01, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.002, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.001, 1.0, 1.0, 1.011, 1.001, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.002, 1.0, 1.002, 1.013, 1.0, 1.0, 1.0, 1.0, 1.0, 1.005, 1.0, 1.004, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.009, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V45 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ) ),
          pf3_V55 = cms.PSet(  values = cms.vdouble( 1.0, 1.0, 27.275, 15.167, 13.818, 1.0, 1.0, 1.0, 1.037, 1.129, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.023, 1.028, 1.063, 1.08, 1.077, 1.054, 1.068, 1.065, 1.047, 1.025, 1.046, 1.064, 1.082, 1.078, 1.137, 1.12, 1.163, 1.158, 1.112, 1.072, 1.054, 1.095, 1.101, 1.092, 1.219, 1.167, 1.186, 1.203, 1.144, 1.096, 1.095, 1.109, 1.111, 1.105, 1.236, 1.187, 1.203, 1.262, 1.2, 1.086, 1.106, 1.112, 1.138, 1.076, 1.287, 1.255, 1.241, 1.334, 1.244, 1.112, 1.083, 1.111, 1.127, 1.025, 1.309, 1.257, 1.263, 1.393, 1.23, 1.091, 1.075, 1.078, 1.135, 1.042, 1.313, 1.303, 1.295, 1.436, 1.237, 1.064, 1.078, 1.075, 1.149, 1.037, 1.329, 1.509, 1.369, 1.546, 1.269, 1.079, 1.084, 1.047, 1.183, 1.008 ) )
        ),
        action = cms.string( "use" ),
        atIP = cms.bool( True ),
        assignError = cms.bool( False )
      )
    ),
    TSGFromPropagation = cms.PSet( 
      ComponentName = cms.string( "TSGFromPropagation" ),
      Propagator = cms.string( "SmartPropagatorAnyOpposite" ),
      MaxChi2 = cms.double( 15.0 ),
      ErrorRescaling = cms.double( 3.0 ),
      UseVertexState = cms.bool( True ),
      UpdateState = cms.bool( False ),
      UseSecondMeasurements = cms.bool( False )
    ),
    TSGFromCombinedHits = cms.PSet( 
      ComponentName = cms.string( "CombinedTSG" ),
      PSetNames = cms.vstring( 'firstTSG',
        'secondTSG' ),
      firstTSG = cms.PSet( 
        ComponentName = cms.string( "TSGFromOrderedHits" ),
        TTRHBuilder = cms.string( "WithTrackAngle" ),
        OrderedHitsFactoryPSet = cms.PSet( 
          ComponentName = cms.string( "StandardHitTripletGenerator" ),
          SeedingLayers = cms.string( "PixelLayerTriplets" ),
          GeneratorPSet = cms.PSet( 
            ComponentName = cms.string( "PixelTripletHLTGenerator" ),
            useFixedPreFiltering = cms.bool( False ),
            phiPreFiltering = cms.double( 0.3 ),
            extraHitRZtolerance = cms.double( 0.06 ),
            extraHitRPhitolerance = cms.double( 0.06 ),
            useBending = cms.bool( True ),
            useMultScattering = cms.bool( True )
          )
        )
      ),
      secondTSG = cms.PSet( 
        ComponentName = cms.string( "TSGFromOrderedHits" ),
        TTRHBuilder = cms.string( "WithTrackAngle" ),
        OrderedHitsFactoryPSet = cms.PSet( 
          ComponentName = cms.string( "StandardHitPairGenerator" ),
          SeedingLayers = cms.string( "PixelLayerPairs" )
        )
      ),
      thirdTSG = cms.PSet( 
        ComponentName = cms.string( "DualByEtaTSG" ),
        etaSeparation = cms.double( 2.0 ),
        PSetNames = cms.vstring( 'endcapTSG',
          'barrelTSG' ),
        barrelTSG = cms.PSet(  ),
        endcapTSG = cms.PSet( 
          ComponentName = cms.string( "TSGFromOrderedHits" ),
          TTRHBuilder = cms.string( "WithTrackAngle" ),
          OrderedHitsFactoryPSet = cms.PSet( 
            ComponentName = cms.string( "StandardHitPairGenerator" ),
            SeedingLayers = cms.string( "MixedLayerPairs" )
          )
        )
      )
    )
)
process.hltL3TrackCandidateFromL2 = cms.EDProducer( "CkfTrajectoryMaker",
    trackCandidateAlso = cms.bool( True ),
    SeedProducer = cms.string( "hltL3TrajectorySeed" ),
    SeedLabel = cms.string( "" ),
    TrajectoryBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
    TrajectoryCleaner = cms.string( "TrajectoryCleanerBySharedHits" ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    useHitsSplitting = cms.bool( False ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    doSeedingRegionRebuilding = cms.bool( False ),
    cleanTrajectoryAfterInOut = cms.bool( False )
)
process.hltL3Muons = cms.EDProducer( "L3MuonProducer",
    MuonCollectionLabel = cms.InputTag( 'hltL2Muons','UpdatedAtVtx' ),
    L3TrajBuilderParameters = cms.PSet( 
      TrackTransformer = cms.PSet( 
        Fitter = cms.string( "KFFitterForRefitInsideOut" ),
        Smoother = cms.string( "KFSmootherForRefitInsideOut" ),
        TrackerRecHitBuilder = cms.string( "WithTrackAngle" ),
        MuonRecHitBuilder = cms.string( "MuonRecHitBuilder" ),
        RefitDirection = cms.string( "insideOut" ),
        RefitRPCHits = cms.bool( True )
      ),
      GlobalMuonTrackMatcher = cms.PSet( 
        Chi2Cut = cms.double( 50.0 ),
        MinP = cms.double( 2.5 ),
        MinPt = cms.double( 1.0 ),
        DeltaRCut = cms.double( 0.2 ),
        DeltaDCut = cms.double( 10.0 )
      ),
      TrackerPropagator = cms.string( "SteppingHelixPropagatorAny" ),
      Chi2CutRPC = cms.double( 1.0 ),
      Chi2CutCSC = cms.double( 150.0 ),
      Chi2CutDT = cms.double( 10.0 ),
      HitThreshold = cms.int32( 1 ),
      Chi2ProbabilityCut = cms.double( 30.0 ),
      PtCut = cms.double( 1.0 ),
      Direction = cms.int32( 0 ),
      MuonHitsOption = cms.int32( 1 ),
      TrackRecHitBuilder = cms.string( "WithTrackAngle" ),
      RPCRecSegmentLabel = cms.InputTag( "hltRpcRecHits" ),
      CSCRecSegmentLabel = cms.InputTag( "hltCscSegments" ),
      DTRecSegmentLabel = cms.InputTag( "hltDt4DSegments" ),
      MuonTrackingRegionBuilder = cms.PSet( 
        beamSpot = cms.InputTag( "hltOfflineBeamSpot" ),
        UseVertex = cms.bool( False ),
        Rescale_eta = cms.double( 3.0 ),
        Rescale_phi = cms.double( 3.0 ),
        Rescale_Dz = cms.double( 3.0 ),
        DeltaZ_Region = cms.double( 15.9 ),
        DeltaR = cms.double( 0.2 ),
        EscapePt = cms.double( 1.5 ),
        Phi_min = cms.double( 0.05 ),
        Eta_min = cms.double( 0.05 ),
        UseFixedRegion = cms.bool( False ),
        EtaR_UpperLimit_Par1 = cms.double( 0.25 ),
        EtaR_UpperLimit_Par2 = cms.double( 0.15 ),
        PhiR_UpperLimit_Par1 = cms.double( 0.6 ),
        PhiR_UpperLimit_Par2 = cms.double( 0.2 ),
        vertexCollection = cms.InputTag( "pixelVertices" ),
        Eta_fixed = cms.double( 0.2 ),
        Phi_fixed = cms.double( 0.2 ),
        OnDemand = cms.double( -1.0 )
      ),
      StateOnTrackerBoundOutPropagator = cms.string( "SmartPropagatorAny" ),
      l3SeedLabel = cms.InputTag( "" ),
      tkTrajLabel = cms.InputTag( "hltL3TrackCandidateFromL2" ),
      TkTrackBuilder = cms.string( "muonCkfTrajectoryBuilder" ),
      SeedGeneratorParameters = cms.PSet( 
        ComponentName = cms.string( "TSGFromOrderedHits" ),
        TTRHBuilder = cms.string( "WithTrackAngle" ),
        OrderedHitsFactoryPSet = cms.PSet( 
          ComponentName = cms.string( "StandardHitPairGenerator" ),
          SeedingLayers = cms.string( "PixelLayerPairs" )
        )
      ),
      KFFitter = cms.string( "L3MuKFFitter" ),
      TransformerOutPropagator = cms.string( "SmartPropagatorAny" ),
      MatcherOutPropagator = cms.string( "SmartPropagator" )
    ),
    ServiceParameters = cms.PSet( 
      UseMuonNavigation = cms.untracked.bool( True ),
      RPCLayers = cms.bool( True ),
      Propagators = cms.untracked.vstring( 'SteppingHelixPropagatorAny',
        'SteppingHelixPropagatorAlong',
        'SteppingHelixPropagatorOpposite',
        'PropagatorWithMaterial',
        'PropagatorWithMaterialOpposite',
        'SmartPropagator',
        'SmartPropagatorOpposite',
        'SmartPropagatorAnyOpposite',
        'SmartPropagatorAny',
        'SmartPropagatorRK',
        'SmartPropagatorAnyRK' )
    ),
    TrackLoaderParameters = cms.PSet( 
      MuonUpdatorAtVertexParameters = cms.PSet( 
        Propagator = cms.string( "SteppingHelixPropagatorOpposite" ),
        BeamSpotPosition = cms.vdouble( 0.0, 0.0, 0.0 ),
        BeamSpotPositionErrors = cms.vdouble( 0.1, 0.1, 5.3 ),
        MaxChi2 = cms.double( 1000000.0 )
      ),
      VertexConstraint = cms.bool( False ),
      PutTkTrackIntoEvent = cms.untracked.bool( True ),
      MuonSeededTracksInstance = cms.untracked.string( "L2Seeded" ),
      DoSmoothing = cms.bool( True ),
      SmoothTkTrack = cms.untracked.bool( False ),
      Smoother = cms.string( "KFSmootherForMuonTrackLoader" )
    )
)
process.hltL3MuonCandidates = cms.EDProducer( "L3MuonCandidateProducer",
    InputObjects = cms.InputTag( "hltL3Muons" )
)
process.hltDiMuonNoIsoL3PreFiltered = cms.EDFilter( "HLTMuonL3PreFilter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL3MuonCandidates" ),
    LinksTag = cms.InputTag( "hltL3Muons" ),
    PreviousCandTag = cms.InputTag( "hltDiMuonNoIsoL2PreFiltered" ),
    MinN = cms.int32( 2 ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MaxDr = cms.double( 2.0 ),
    MaxDz = cms.double( 9999.0 ),
    #MinPt = cms.double( 3.0 ),
    MinPt = cms.double( 0.0 ),
    NSigmaPt = cms.double( 0.0 ),
    SaveTag = cms.untracked.bool( True )
)
process.hltBoolEnd = cms.EDFilter( "HLTBool",
    result = cms.bool( True )
)

#process.HLTBeginSequence = cms.Sequence( process.hltTriggerType + process.hltGtDigis + process.hltGctDigis + process.hltL1GtObjectMap + process.hltL1extraParticles + process.hltOfflineBeamSpot )

process.HLTBeginSequence = cms.Sequence(process.hltGtDigis + process.hltGctDigis + process.hltL1GtObjectMap + process.hltL1extraParticles + process.hltOfflineBeamSpot )

process.HLTL2muonrecoNocandSequence = cms.Sequence( process.hltMuonDTDigis + process.hltDt1DRecHits + process.hltDt4DSegments + process.hltMuonCSCDigis + process.hltCsc2DRecHits + process.hltCscSegments + process.hltMuonRPCDigis + process.hltRpcRecHits + process.hltL2MuonSeeds + process.hltL2Muons )
process.HLTL2muonrecoSequence = cms.Sequence( process.HLTL2muonrecoNocandSequence + process.hltL2MuonCandidates )
process.HLTDoLocalPixelSequence = cms.Sequence( process.hltSiPixelDigis + process.hltSiPixelClusters + process.hltSiPixelRecHits )
process.HLTDoLocalStripSequence = cms.Sequence( process.hltSiStripRawToClustersFacility + process.hltSiStripClusters )
process.HLTL3muonTkCandidateSequence = cms.Sequence( process.HLTDoLocalPixelSequence + process.HLTDoLocalStripSequence + process.hltL3TrajectorySeed + process.hltL3TrackCandidateFromL2 )
process.HLTL3muonrecoNocandSequence = cms.Sequence( process.HLTL3muonTkCandidateSequence + process.hltL3Muons )
process.HLTL3muonrecoSequence = cms.Sequence( process.HLTL3muonrecoNocandSequence + process.hltL3MuonCandidates )
process.HLTEndSequence = cms.Sequence( process.hltBoolEnd )

#process.HLT_DoubleMu3 = cms.Path( process.HLTBeginSequence + process.hltL1sDiMuonNoIso + process.hltPreDoubleMu3 + process.hltDiMuonNoIsoL1Filtered + process.HLTL2muonrecoSequence + process.hltDiMuonNoIsoL2PreFiltered + process.HLTL3muonrecoSequence + process.hltDiMuonNoIsoL3PreFiltered + process.HLTEndSequence )
process.HLT_DoubleMu3 = cms.Path( process.HLTBeginSequence + process.hltL1sDiMuonNoIso + process.hltDiMuonNoIsoL1Filtered + process.HLTL2muonrecoSequence + process.hltDiMuonNoIsoL2PreFiltered + process.HLTL3muonrecoSequence + process.hltDiMuonNoIsoL3PreFiltered + process.HLTEndSequence )


### JKim added.

process.hltL1gtTrigReport = cms.EDAnalyzer( "L1GtTrigReport",
    UseL1GlobalTriggerRecord = cms.bool( False ),
    L1GtRecordInputTag = cms.InputTag( "hltGtDigis" )
)
process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)
process.HLTAnalyzerEndpath = cms.EndPath( process.hltL1gtTrigReport + process.hltTrigReport )
#process.schedule.append(process.HLTAnalyzerEndpath)

process.pts = cms.EDFilter("PathTimerInserter")
process.PathTimerService = cms.Service("PathTimerService")
process.MyTimer = cms.Path ( process.pts )

process.load("Configuration.EventContent.EventContent_cff")
process.hltPoolOutput = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'drop *',
        #'keep *'),
        #'keep HLTPerformanceInfo_*_*_*'),
        'keep *_*_*_HLT',
        'keep *_genParticles__*'
        #'keep edmHepMCProduct_source__*',
        #'keep recoCandidatesOwned_genParticleCandidates__*'),

    #process.FEVTDEBUGHLTEventContent,
    #dataset = cms.untracked.PSet(
        #dataTier = cms.untracked.string('RECO')
    ),
    basketSize = cms.untracked.int32(4096),
    #fileName = cms.untracked.string('rfio:/castor/cern.ch/user/k/kumarv/Trigger/HLTFromPureRaw_DoubleMuOpen.root')
    fileName = cms.untracked.string('file:/tmp/kumarv/HLT1000_JPsiMM218_Jpsi_DoubleMuOpen.root')
    #fileName = cms.untracked.string('file:HLT1000k_RVMinbias218_Jpsi218_DoubleMuOpen_v3.root')
    #fileName = cms.untracked.string('file:HLT_L1mureco_changeall_dmoonJpsi100_Jpsi218_DoubleMuOpen_v3.root')
    #fileName = cms.untracked.string('file:/tmp/jkim/HLTDoubleMu3_ppJpsi10k_DoubleMuOpen_nocut_2muon_startup2v2.root')
)
process.HLTOutput = cms.EndPath(process.hltPoolOutput)
process.schedule = cms.Schedule( process.HLT_DoubleMu3, process.HLTAnalyzerEndpath, process.MyTimer, process.HLTOutput)
