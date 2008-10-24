import FWCore.ParameterSet.Config as cms

# Pixel RawDataError Monitoring
from DQM.SiPixelMonitorRawData.SiPixelMonitorRawData_cfi import * 
process.SiPixelRawDataErrorSource.saveFile = False
process.SiPixelRawDataErrorSource.isPIB = False
process.SiPixelRawDataErrorSource.slowDown = False
process.SiPixelRawDataErrorSource.reducedSet = False

# Pixel Digi Monitoring
from DQM.SiPixelMonitorDigi.SiPixelMonitorDigi_cfi import *
SiPixelDigiSource.saveFile = False
SiPixelDigiSource.isPIB = False
SiPixelDigiSource.slowDown = False

# Pixel Cluster Monitoring
from DQM.SiPixelMonitorCluster.SiPixelMonitorCluster_cfi import *
SiPixelClusterSource.saveFile = False

# Pixel RecHit Monitoring
from DQM.SiPixelMonitorRecHit.SiPixelMonitorRecHit_cfi import *
SiPixelRecHitSource.saveFile = False

        
##online/offline
#RawDataErrors
SiPixelRawDataErrorSource.modOn = False
SiPixelRawDataErrorSource.ladOn = True
SiPixelRawDataErrorSource.layOn = True
SiPixelRawDataErrorSource.phiOn = True
SiPixelRawDataErrorSource.bladeOn = True
SiPixelRawDataErrorSource.diskOn = True
SiPixelRawDataErrorSource.ringOn = True
#Digi
SiPixelDigiSource.modOn = False
SiPixelDigiSource.twoDimOn = False
SiPixelDigiSource.hiRes = False
SiPixelDigiSource.ladOn = True
SiPixelDigiSource.layOn = True
SiPixelDigiSource.phiOn = True
SiPixelDigiSource.bladeOn = True
SiPixelDigiSource.diskOn = True
SiPixelDigiSource.ringOn = True
#Cluster
SiPixelClusterSource.modOn = False
SiPixelClusterSource.twoDimOn = False
SiPixelClusterSource.reducedSet = False
SiPixelClusterSource.ladOn = True
SiPixelClusterSource.layOn = True
SiPixelClusterSource.phiOn = True
SiPixelClusterSource.bladeOn = True
SiPixelClusterSource.diskOn = True
SiPixelClusterSource.ringOn = True
#RecHit
SiPixelRecHitSource.modOn = False
SiPixelRecHitSource.twoDimOn = False
SiPixelRecHitSource.ladOn = True
SiPixelRecHitSource.layOn = True
SiPixelRecHitSource.phiOn = True	
SiPixelRecHitSource.bladeOn = True
SiPixelRecHitSource.diskOn = True
SiPixelRecHitSource.ringOn = True

#DQM service
dqmInfo = cms.EDFilter("DQMEventInfo",
    subSystemFolder = cms.untracked.string('Pixel')
)

siPixelOfflineDQM_source = cms.Sequence(SiPixelRawDataErrorSource + SiPixelDigiSource + SiPixelRecHitSource + SiPixelClusterSource + dqmInfo)
