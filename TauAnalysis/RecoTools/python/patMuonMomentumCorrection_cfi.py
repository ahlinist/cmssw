import FWCore.ParameterSet.Config as cms

poolDBESSourceMuScleFitCentralValue = cms.ESSource("PoolDBESSource",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string('oracle://cms_orcoff_prod/CMS_COND_31X_PHYSICSTOOLS'),
    appendToDataLabel = cms.string("centralValue"),                                               
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('MuScleFitDBobjectRcd'),
            tag = cms.string('MuScleFit_Scale_Z_20_invNb_innerTrack')
        )
    )
)

patMuonsMuScleFitCorrectedMomentum = cms.EDProducer('MuScleFitMuonProducer',
    MuonLabel = cms.InputTag("cleanPatMuons"),
    PatMuons = cms.bool(True),
    dbObjectLabel = cms.string("centralValue")                                             
)

