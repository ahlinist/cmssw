#include "RecoBTag/CombinedSV/interface/TTracksFromRecoVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

using namespace std;

TTracksFromRecoVertex::TTracksFromRecoVertex ( const MagneticField * f ) :
  theBuilder ( TransientTrackBuilder ( f ) )
{}

vector < reco::TransientTrack > TTracksFromRecoVertex::create ( const reco::Vertex & v ) const
{
  #ifdef RAVE
  return static_cast < const TransientVertex &> (v).originalTracks();
  #else
  vector < reco::TransientTrack > ret;
  for ( reco::track_iterator i=v.tracks_begin(); i!=v.tracks_end() ; ++i )
  {
    const reco::TransientTrack * tmp = theBuilder.build ( &(**i) );
    LogDebug("") << "TransientTrack: " << tmp;
    ret.push_back ( *(tmp) );
  }
  return ret;
  #endif
}
