#include "RecoBTag/CombinedSV/interface/PseudoVertexBuilder.h"
#include "RecoVertex/VertexTools/interface/BeamSpot.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace reco::btag::Vertices;

PseudoVertexBuilder::PseudoVertexBuilder ( double tmin,
    const TrackFilter & filter, TrackInfoBuilder * b, const MagneticField * f )
  : trackIpSignificanceMin2DMin_ ( tmin ), filter_ ( filter ),
  trackInfoBuilder_ ( b ), field_(f)
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
  // cout << "[PseudoVertexBuilder] try PseudoVertex" << endl;
  reco::Vertex vtx = build ( t, trackColl);
  if ( !(filter ( vtx, PseudoVertex )) )
  {
    t=NoVertex;
    vtx= build ( t, trackColl );
  }
  LogDebug ("") << "building " << name ( t );
  /*
  edm::LogError("PseudoVertexBuilder")
    << "there is sth wrong with the builder."
    << " Crashes on amilo. Zero mass on centurion";
    */
  return vtx;
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

  vector<combsv::CombinedTrack> filteredTrackColl = filterTracks ( trackColl, ipSigni2DCut );
  int nTracks = filteredTrackColl.size();
  edm::LogInfo ( "PseudoVertexBuilder" ) << nTracks << " accepted tracks." << endl;
  /*
  if (nTracks < 2) {
    throw cms::Exception("NoTracksForVertex")
      << "while building vertex in PseudoVertexBuilder::buildVertex"
      << " no track survived cuts, cannot build vertex" << endl;
  }*/

  double chi2 = -1.;
  double ndof = 2*nTracks-3;

  // now setup vertex
  BeamSpot s;
  vector< reco::TransientTrack > trks;
  for ( vector< combsv::CombinedTrack >::const_iterator i=trackColl.begin(); 
        i!=trackColl.end() ; ++i )
  {
    trks.push_back ( *i );
  }
  TransientVertex vtx ( s.position(), s.error(), trks, chi2, ndof );
  return vtx;
  // return reco::Vertex ( s.position(), s.error(), chi2, ndof, trackColl.size() );
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
