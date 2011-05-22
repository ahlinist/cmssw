import FWCore.ParameterSet.Config as cms

## various track sources
## stuff for the z->mmg pixel match veto efficiency measurement
trackMultiplicities = cms.EDProducer('TrackCounter',
    sources = cms.VPSet(
        cms.PSet( src = cms.InputTag('ckfInOutTracksFromConversions')        ),
        cms.PSet( src = cms.InputTag('ckfOutInTracksFromConversions')        ),
        cms.PSet( src = cms.InputTag('cosmicsVetoTracks')                    ),
        cms.PSet( src = cms.InputTag('generalTracks')                        ),
        cms.PSet( src = cms.InputTag('pixelTracks')                          ),
        cms.PSet( src = cms.InputTag('regionalCosmicTracks')                 ),
        cms.PSet( src = cms.InputTag('impactParameterTagInfos:ghostTracks'),
                  label = cms.untracked.string('ghostTracks')                ),
        cms.PSet( src = cms.InputTag( 'generalTracks' ),
                  cut = cms.untracked.string( 'quality("highPurity")' ),
                  label = cms.untracked.string( 'highPurityTracks' )         )
    ) # end of sources
)

gsfTrackMultiplicities = cms.EDProducer('GsfTrackCounter',
    sources = cms.VPSet(
        cms.PSet( src = cms.InputTag('electronGsfTracks') ),
    ) # end of sources
)
