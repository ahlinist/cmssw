import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.hadSUSYHLTPaths_cfi import *
hadSUSYdiElecFilter = cms.EDFilter("HadSUSYdiElecSkim",
    ElecPtmin = cms.double(30.0),
    PtmindiElec = cms.double(150.0),
    Elecsrc = cms.InputTag("pixelMatchGsfElectrons"),
    NminElec = cms.int32(2)
)

hadSUSYdiElecSeq = cms.Sequence(hadSUSYHLTdiElecPath+hadSUSYdiElecFilter)

