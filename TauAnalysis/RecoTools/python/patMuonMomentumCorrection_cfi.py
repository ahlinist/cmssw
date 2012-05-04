import FWCore.ParameterSet.Config as cms

poolDBESSourceMuScleFitCentralValue = cms.ESSource("PoolDBESSource",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(2),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_PHYSICSTOOLS'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('MuScleFitDBobjectRcd'),
            tag = cms.string('MuScleFit_Scale_Z_36_invPb_innerTrack_Dec22_v1')
        )
    )
)

patMuonsMuScleFitCorrectedMomentum = cms.EDProducer('MuScleFitMuonProducer',
    MuonLabel = cms.InputTag("cleanPatMuons"),
    PatMuons = cms.bool(True),
    DbObjectLabel = cms.untracked.string(""),
    doApplyCorrection = cms.bool(True)
)
