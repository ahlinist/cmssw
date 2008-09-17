import FWCore.ParameterSet.Config as cms

from DQMOffline.Muon.muonQualityTests_cff import *
from DQMOffline.Ecal.ecal_dqm_client_offline_cosmic_cff import *
from DQM.HcalMonitorModule.hcal_dqm_client_fileT0_cff import *
from DQM.SiPixelCommon.SiPixelOfflineDQM_client_cff import *
from DQMOffline.Trigger.DQMOffline_Trigger_Client_cff import *


DQMOfflineCosmics_SecondStep = cms.Sequence(ecal_dqm_client_offline*muonQualityTests*hcalOfflineDQMClient*sipixelEDAClient*triggerOfflineDQMClient)
DQMOfflineCosmics_SecondStep_woHcal = cms.Sequence(ecal_dqm_client_offline*muonQualityTests*sipixelEDAClient*triggerOfflineDQMClient)
