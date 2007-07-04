#include "RecoBTag/CombinedSV/interface/TTracksFromRecoVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

using namespace std;

vector < reco::TransientTrack > TTracksFromRecoVertex::create ( const reco::Vertex & v,
    const TransientTrackBuilder & builder )
{
  /*
  #ifdef RAVE
  return static_cast < const TransientVertex &> (v).originalTracks();
  #else
  */
  vector < reco::TransientTrack > ret;
  for ( reco::track_iterator i=v.tracks_begin(); i!=v.tracks_end() ; ++i )
  {
    reco::TransientTrack tmp = builder.build ( *i );
    ret.push_back ( tmp );
  }
  return ret;
  // #endif
}
