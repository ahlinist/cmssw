#include "RecoBTag/CombinedSV/interface/PseudoVertexBuilder.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace reco::btag::Vertices;

PseudoVertexBuilder::PseudoVertexBuilder ( double tmin,
    const TrackFilter & filter, TrackInfoBuilder * b )
  : trackIpSignificanceMin2DMin_ ( tmin ), filter_ ( filter ),
  trackInfoBuilder_ ( b )
{}

PseudoVertexBuilder::PseudoVertexBuilder () : 
    trackIpSignificanceMin2DMin_ ( 0. ), filter_ ( TrackFilter() ),
    trackInfoBuilder_ ( 0 )
{}

vector < reco::Vertex > PseudoVertexBuilder::buildVector 
    ( reco::btag::Vertices::VertexType t,
    const std::vector<combsv::CombinedTrack> & trackColl ) const
{
  vector < reco::Vertex > ret;
  try {
    ret.push_back ( build ( t, trackColl ) );
  } catch ( ... ) {
    if (t==PseudoVertex && ret.size()==0 )
    {
      // try NoVertex now
      ret.push_back ( build ( NoVertex, trackColl ) );
    }
  }
  if ( ret.size() !=  1)
  {
    // this should not have happened
    throw cms::Exception("InvalidNumberVertices")
      << "while building NoVertex: found " << ret.size()
      << " vertices in PseudoVertexBuilder, expect exactly 1"
      << "cannot tag this jet." << endl;
  }
  return ret;
}

reco::Vertex PseudoVertexBuilder::build(
    const std::vector<combsv::CombinedTrack> & trackColl, const VertexFilter & filter,
    VertexType & t ) const
{
  t=PseudoVertex;
  reco::Vertex vtx;
  try {
    vtx = build ( t, trackColl);
    if ( !(filter ( vtx, PseudoVertex )) )
    {
      t=NoVertex;
      vtx= build ( t, trackColl );
    }
  } catch ( ... ) {
    t=NoVertex;
    vtx= build ( t, trackColl );
  }
  LogDebug ("") << "building " << name ( t ) << " with " << vtx.tracksSize() << " tracks.";
  /*
  edm::LogError("PseudoVertexBuilder")
    << "there is sth wrong with the builder."
    << " Crashes on amilo. Zero mass on centurion";
    */
  return vtx;
}

const std::vector < combsv::CombinedTrack > &  PseudoVertexBuilder::lastTracks() const
{
  return tracks_;
}

reco::Vertex PseudoVertexBuilder::build( VertexType t,
    const std::vector<combsv::CombinedTrack> & trackColl ) const
{
  // build an "artificial" vertex now 
  // Note that cut on IP 2D significance different for case
  //      "PseudoVertex" and "NoVertex"
  // PseudoVertex: ipSigni2DCut=2.0
  // NoVertex ipSigni2DCut=0.0
  double ipSigni2DCut = 0.0;
  if (t == reco::btag::Vertices::PseudoVertex) 
    ipSigni2DCut = trackIpSignificanceMin2DMin_;

  tracks_ = filterTracks ( trackColl, ipSigni2DCut );
  int nTracks = tracks_.size();
  edm::LogInfo ( "PseudoVertexBuilder" ) << nTracks << " tracks accepted for " 
    << reco::btag::Vertices::name( t ) << endl;
  if ( t == reco::btag::Vertices::PseudoVertex && nTracks < 2) {
    throw cms::Exception ("cannot build pseudo vertex" );
  }

  double chi2 = -1.;
  double ndof = 2*nTracks-3;

  // now setup vertex
  reco::BeamSpot s;
  vector< reco::TransientTrack > trks;
  for ( vector< combsv::CombinedTrack >::const_iterator i=tracks_.begin(); 
        i!=tracks_.end() ; ++i )
  {
    trks.push_back ( *i );
  }
  VertexState vs ( s );

  TransientVertex vtx ( vs, trks, chi2, ndof );
  return vtx;
}

bool PseudoVertexBuilder::acceptTrack(
    const combsv::CombinedTrack & t, double ipSigni2DCut ) const
{
  // CAVE: IP significance just for pseudo/no-vertex ?
  //       jet distance just for reco-vertex?
  return filter_.operator() ( t, ipSigni2DCut );
}

std::vector < combsv::CombinedTrack > PseudoVertexBuilder::filterTracks (
    const std::vector< combsv::CombinedTrack > & source, double ipSigni2DCut ) const
{
  vector<combsv::CombinedTrack> ret;
  std::vector<combsv::CombinedTrack>::const_iterator t = source.begin();
  for ( ; t != source.end(); t++ )
  {
    if ( acceptTrack(*t, ipSigni2DCut) ) ret.push_back( *t );
  }
  return ret;
}

const TrackInfoBuilder & PseudoVertexBuilder::trackInfoBuilder() const
{
  return (*trackInfoBuilder_ );
}

const TrackFilter & PseudoVertexBuilder::trackFilter() const
{
  return filter_;
}
