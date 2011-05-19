from JetMETAnalysis.ecalDeadCellTools.EcalDeadCellEventFilter_cfi import *

ecalDeadCellTPfilter = EcalDeadCellEventFilter.clone()
ecalDeadCellTPfilter.tpDigiCollection = cms.InputTag("ecalTPSkim")
ecalDeadCellTPfilter.etValToBeFlagged = cms.double(63.75)
ecalDeadCellTPfilter.ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB")
ecalDeadCellTPfilter.eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE")
