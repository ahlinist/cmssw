#ifndef RecoBTag_TTracksFromRecoVertex
#define RecoBTag_TTracksFromRecoVertex

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include <vector>

class TTracksFromRecoVertex {
  /**
   *  gives the TransientTracks of a reco::Vertex,
   *  using TransientTrackBuilder
   */
   public:
     TTracksFromRecoVertex ( const MagneticField * );
     std::vector < reco::TransientTrack > create ( const reco::Vertex & ) const;

   private:
     TransientTrackBuilder theBuilder;
};

#endif
