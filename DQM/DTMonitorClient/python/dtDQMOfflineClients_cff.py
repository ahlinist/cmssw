import FWCore.ParameterSet.Config as cms

from DQM.DTMonitorClient.dtChamberEfficiencyClient_cfi import *
from DQM.DTMonitorClient.dtChamberEfficiencyTest_cfi import *
from DQM.DTMonitorClient.dtSegmentAnalysisTest_cfi import *
segmentTest.normalizeHistoPlots = True
#segmentTest.detailedAnalysis = True
from DQM.DTMonitorClient.dtOfflineSummaryClients_cfi import *
from DQM.DTMonitorClient.dtResolutionAnalysisTest_cfi import *

dtQualityTests = cms.EDFilter("QualityTester",
                         #reportThreshold = cms.untracked.string('red'),
                         prescaleFactor = cms.untracked.int32(1),
                         qtList = cms.untracked.FileInPath('DQM/DTMonitorClient/test/QualityTests.xml'),
                         getQualityTestsFromFile = cms.untracked.bool(True)
                         )

<<<<<<< dtDQMOfflineClients_cff.py
dtClients = cms.Sequence(segmentTest*chamberEfficiencyClient*dtOfflineSummaryClients)
=======
dtClients = cms.Sequence(segmentTest*dtResolutionAnalysisTest*dtOfflineSummaryClients)
>>>>>>> 1.8

