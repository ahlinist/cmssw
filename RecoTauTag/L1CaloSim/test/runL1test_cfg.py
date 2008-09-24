process = cms.Process("test")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(250)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/06C16DFA-9182-DD11-A4CC-000423D6CA6E.root',
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/0A0241FB-9182-DD11-98E1-001617E30D40.root',
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/12B41BA7-9282-DD11-9E7F-000423D94E70.root',
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/14AD9BA0-9182-DD11-82D3-000423D987E0.root',
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/1A5FD19D-9282-DD11-B0BB-000423D9970C.root',
       '/store/relval/CMSSW_2_1_8/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0003/1CA9C760-9282-DD11-99C2-001617DC1F70.root'
    )
)

#process.load("Geometry.CMSCommonData.cmsAllGeometryXML_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryDB_cff")
#process.load("FastSimulation.Configuration.CommonInputs_cff")
#process.load("FastSimulation.Configuration.FamosSequences_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("Configuration.StandardSequences.GeometryECALHCAL_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cff")
#process.load("Geometry.CMSCommonData.cmsPilot2IdealGeometryXML_cff")
#process.load("Geometry.CMSCommonData.cmsRecoIdealGeometryXML_cff")
#process.load("Geometry.CMSCommonData.ecalhcalGeometryXML_cfi")
process.load("Geometry.CaloEventSetup.CaloGeometry_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.CaloTowerConstituents_cfi")
#process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

#process.CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
#     MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz')
#)

process.load("RecoTauTag.L1CaloSim.l1calosim_cfi")
process.l1CaloSim.AlgorithmSource = "RecHits" 
process.l1CaloSim.EmInputs = cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"), cms.InputTag("ecalRecHit","EcalRecHitsEE"))

process.p  = cms.Path(process.l1CaloSim)
