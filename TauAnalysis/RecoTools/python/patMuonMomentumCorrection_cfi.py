import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBSetup_cfi import *
poolDBESSourceMuScleFitCentralValue = cms.ESSource("PoolDBESSource",
    CondDBSetup,
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS'),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    timetype = cms.untracked.string('runnumber'),                                                
    appendToDataLabel = cms.string("centralValue"),                                               
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('MuScleFitDBobjectRcd'),
            label = cms.untracked.string(''),                                               
            tag = cms.string('MuScleFit_Scale_Z_36_invPb_innerTrack_Dec22_v1')
        )
    )
)

patMuonsMuScleFitCorrectedMomentum = cms.EDProducer('MuScleFitMuonProducer',
    MuonLabel = cms.InputTag("cleanPatMuons"),
    PatMuons = cms.bool(True),
    DbObjectLabel = cms.untracked.string("centralValue"),
    doApplyCorrection = cms.bool(True)
)
