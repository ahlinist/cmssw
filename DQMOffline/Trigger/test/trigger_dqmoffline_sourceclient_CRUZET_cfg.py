import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")
#
#  DQM SERVICES
#
process.load("DQMServices.Core.DQM_cfg")

#
#  DQM SOURCES
#
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")

#process.load("Configuration.GlobalRuns.ReconstructionGR_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("L1Trigger.Configuration.L1Config_cff")

#process.load("L1Trigger.HardwareValidation.L1HardwareValidation_cff")

process.load("DQMServices.Components.DQMEnvironment_cfi")

#process.load("DQMOffline.Trigger.FourVectorHLTOffline_cfi")
#process.load("DQMOffline.Trigger.FourVectorHLTOfflineClient_cfi")
#process.load("DQMOffline.Trigger.HLTEventInfoClient_cfi")
process.load("DQMOffline.Trigger.HLT_DQM_Offline_cff")
#process.load("DQMOffline.Trigger.L1TMonitor_dqmoffline_cff")
#process.load("DQMOffline.Trigger.Tau.HLTTauDQMOffline_cff")
#process.load("DQMOffline.Trigger.EgammaHLTOffline_cfi")
#process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

#from DQMOffline.Trigger.FourVectorHLTOffline_cfi import *
#process.hltmonitor = cms.Sequence(hltResults)
#hltResults.triggerSummaryLabel = cms.InputTag("hltTriggerSummaryAOD","","HLT")
#hltResults.triggerResultsLabel = cms.InputTag("TriggerResults","","HLT")
#
#from DQMOffline.Trigger.HLTEventInfoClient_cfi import *
#process.hltEInfoclient = cms.Sequence(hltEventInfoClient)
#
#from DQMOffline.Trigger.FourVectorHLTOfflineClient_cfi import *
#process.hltclient = cms.Sequence(hltFourVectorClient)
##hltFourVectorClient.prescaleLS = cms.untracked.int32(-1)
##hltFourVectorClient.monitorDir = cms.untracked.string('')
##hltFourVectorClient.prescaleEvt = cms.untracked.int32(1)



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = 
	#cms.untracked.vstring('file:test.root')
	cms.untracked.vstring(
	#Zee
	#'file:0EF324BD-9160-DD11-B591-000423D944F8.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/0EF324BD-9160-DD11-B591-000423D944F8.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/08D532C8-9C60-DD11-AB1C-000423D99996.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/0EF324BD-9160-DD11-B591-000423D944F8.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/2E43BB99-9A60-DD11-849B-000423D99660.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/58C37B65-9B60-DD11-8EE6-001617E30D40.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/6424C71D-9E60-DD11-9766-001617DBD316.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/B002E723-9E60-DD11-A91B-000423D6CAF2.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/B025647D-9060-DD11-AC12-001617C3B70E.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/C83605E7-9860-DD11-A255-000423D951D4.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/D2935E80-9460-DD11-B076-000423D9880C.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/EE1FC1CC-9C60-DD11-86CC-001617DBD230.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/F032F11B-9F60-DD11-B9E6-001617DBD556.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/FAC5DD31-9B60-DD11-B837-000423D944DC.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/FC17A765-9A60-DD11-B9ED-000423D98834.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0EC3CE7D-9D60-DD11-89F6-000423D98950.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1458A41C-A760-DD11-9ADE-000423D99996.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/2ACDA607-A260-DD11-BD95-001617C3B5E4.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/5EAD9022-A260-DD11-AE05-000423D6C8E6.root',
#	'/store/relval/CMSSW_2_1_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/E0928507-A260-DD11-9220-000423D95030.root',
#	#Zmumu
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/08D532C8-9C60-DD11-AB1C-000423D99996.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/022E6660-6E61-DD11-8800-001A92971BDC.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/04679983-6E61-DD11-B078-001BFCDBD19E.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/066748A9-6F61-DD11-948A-001A92971B04.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/0839883D-6D61-DD11-8823-001A928116BE.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/0CF07984-6F61-DD11-B3ED-001A9281170A.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/0E4B00F8-6D61-DD11-B943-001BFCDBD19E.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/103766B6-6F61-DD11-BCDD-001731AF6A8D.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/12491AEA-6E61-DD11-8E01-001A92971BCA.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/12E738D9-6D61-DD11-A3BB-0018F3D096B6.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/14340899-6C61-DD11-AF46-001731230FC9.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/14C24D98-6C61-DD11-8885-001BFCDBD154.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/182D1B7D-6E61-DD11-AFA8-001A92971BD6.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/186F0867-6E61-DD11-86A9-00304876A139.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/18E12799-6E61-DD11-B9DC-001731AF671B.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/1A0228AD-6E61-DD11-A5C3-001731AF6847.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/1C434AE8-6E61-DD11-A952-001A92810AA2.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/1E312D49-6D61-DD11-8FB2-0018F3D0968A.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/209CA2A5-6F61-DD11-9C1F-001A92810AB8.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/22BA1249-6E61-DD11-8342-00304875AAED.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/2A3234E2-6F61-DD11-B395-0018F3D09706.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/2A435233-6D61-DD11-919E-001A92810AA2.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/2A546865-6E61-DD11-A95C-001A92971B84.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/2A694EC4-6E61-DD11-88D7-001A92811736.root',
#	'/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0002/2A95DF9C-6F61-DD11-B747-001A92811720.root',
#	#MinBias
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0024DAA6-2461-DD11-8E6D-001731AF68B3.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/007C02BF-2761-DD11-9E31-0018F3D09616.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/00C7F564-2561-DD11-8FAE-001731AF698D.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/025870A4-2861-DD11-A5B5-001A92971B54.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/04613070-1E61-DD11-8D48-001A92971B9A.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/062E8751-2161-DD11-AEB6-0018F3D09706.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/08518DBE-1A61-DD11-80C0-001A92971B06.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0867EF71-2661-DD11-8C93-001A928116E6.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0A80B430-1E61-DD11-9BCD-001A92810AC8.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0CAED94C-2061-DD11-8B3A-001731AF6B77.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/0CB188B2-2661-DD11-AFD3-001731A28EDB.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/122D9E7B-2561-DD11-9231-001A928116C6.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1292E604-2161-DD11-94F9-001A92811726.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/12E66BAD-2661-DD11-B3FC-001A92971ACC.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/149D7566-1E61-DD11-BB2B-0017312B56A7.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/14ECDD6B-2061-DD11-BF7B-001731AF68CF.root',
#	#'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/164F06AD-1A61-DD11-A0BE-001A92810A9A.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1819AFC7-1C61-DD11-A9BF-001A92810AE2.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/187DF635-2161-DD11-90B0-001A928116EA.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1A7FD3BB-2861-DD11-ADA5-001A92971BC8.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1ACDBEB1-2661-DD11-9B3A-003048767F27.root',
#	'/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0001/1C709C66-2661-DD11-8C4C-001A928116D8.root'
#'file:/data/rekovic/RelVal_219/inputRelValMinBias/0C5562F6-B885-DD11-A464-000423D98A44.root',
#'file:/data/rekovic/RelVal_219/inputRelValMinBias/44F393BC-B885-DD11-B720-000423D6A6F4.root',
#'file:/data/rekovic/RelVal_219/inputRelValMinBias/4850ED6B-B885-DD11-A152-000423D98F98.root',
  #'file:/data/rekovic/RelVal_219/inputRelValMinBias/6C955288-B985-DD11-A64A-000423D98F98.root',
  #'file:/data/rekovic/RelVal_219/inputRelValMinBias/90A7E2B9-B785-DD11-A459-0016177CA778.root',
  #'file:/data/rekovic/RelVal_219/inputRelValMinBias/C29532CA-B985-DD11-8C5C-000423D99264.root'
  #'file:/data/rekovic/RelVal_219/inputRelValMinBias/C29532CA-B985-DD11-8C5C-000423D99264.root'
	'/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/555/02E4041E-1571-DD11-98CE-001D09F241B9.root'
#	'rfio:///castor/cern.ch/cms/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/468FB321-2060-DD11-8359-000423D6CAF2.root'
	)
	##cms.untracked.vstring('/store/relval/CMSSW_2_1_0/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/08D532C8-9C60-DD11-AB1C-000423D99996.root')
	#cms.untracked.vstring('rfio:///castor/cern.ch/cms/store/relval/CMSSW_2_1_0/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V4_v1/0000/468FB321-2060-DD11-8359-000423D6CAF2.root')
)

process.MessageLogger = cms.Service("MessageLogger",
    detailedInfo = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    critical = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noLineBreaks = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('detailedInfo', 
        'critical', 
        'cout')
)

#process.psource = cms.Path(process.hltmonitor*process.hltclient*process.hltEInfoclient)
process.psource = cms.Path(process.hltDqmOffline)
process.p = cms.EndPath(process.dqmSaver)
process.DQMStore.verbose = 0
process.DQM.collectorHost = ''
process.dqmSaver.convention = 'Online'
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True


process.DQMStore.referenceFileName = 'DQMOffline/Trigger/test/DQM_V0001_HLTOffline_R000000001_RelValZeeZmmMinBias.root'
