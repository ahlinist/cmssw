#include "RecoBTag/CombinedSV/interface/BTagVertexReconstructor.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoBTag/CombinedSV/interface/BKinematics.h"

using namespace std;

BTagVertexReconstructor::BTagVertexReconstructor ( 
    const VertexReconstructor & v, const TrackFilter & f ) :
  vtxrector_ ( v.clone() ), trkfilter_(f)
{}

BTagVertexReconstructor::~BTagVertexReconstructor()
{
  delete vtxrector_;
}

combsv::CombinedVertex BTagVertexReconstructor::buildVertexInfo (
    const TransientVertex & vtx, reco::btag::Vertices::VertexType vtxType,
    const VertexFilter & vtxfilter, const TrackInfoBuilder & trackinfobuilder ) const
{
  vector < reco::TransientTrack > ttrks = vtx.originalTracks();
  vector < combsv::CombinedTrack > trks = trackinfobuilder.build ( ttrks );
  reco::BKinematics kin( trks );
  combsv::CombinedVertex vertexData ( vtx, trks, kin.get3Vector(),
                                    kin.getMass(), vtxfilter.checkV0(vtx) );
  return vertexData;
}

combsv::CombinedVertex BTagVertexReconstructor::buildVertexInfo (
    const reco::Vertex & vtx,
    const vector < combsv::CombinedTrack > & trks,
    reco::btag::Vertices::VertexType vtxType,
    const VertexFilter & vtxfilter, const TrackInfoBuilder & trackinfobuilder ) const
{
  reco::BKinematics kin( trks );
  combsv::CombinedVertex vertexData ( vtx, trks, kin.get3Vector(),
                                    kin.getMass(), vtxfilter.checkV0(vtx) );
  return vertexData;
}


vector< combsv::CombinedVertex > BTagVertexReconstructor::fit (
    const vector < reco::TransientTrack > & tracks,
    const VertexFilter & filter, const TrackInfoBuilder & trackinfobuilder ) const
{
  vector< combsv::CombinedVertex > ret;
  vector < TransientVertex > fittedSVColl = vtxrector_->vertices( tracks );
  int nFittedVertices = fittedSVColl.size();

  edm::LogInfo ( "BTagVertexReconstructor" ) << "found " << nFittedVertices << " vertices at:";
  for ( vector< TransientVertex >::const_iterator i=fittedSVColl.begin();
        i!=fittedSVColl.end() ; ++i )
  {
    edm::LogInfo ( "BTagVertexReconstructor" ) << "      " <<  i->position();
  }

  // apply vertex cuts, filter out primary vertex if found again
  for ( vector<TransientVertex>::const_iterator vtx = fittedSVColl.begin();
        vtx != fittedSVColl.end(); vtx++ )
  {
    if ( filter ( *vtx, reco::btag::Vertices::RecoVertex ))
    {
      ret.push_back( buildVertexInfo ( *vtx, reco::btag::Vertices::RecoVertex,
            filter, trackinfobuilder ) );
    }
  }
  return ret;
}

pair < reco::btag::Vertices::VertexType, vector< combsv::CombinedVertex > > 
    BTagVertexReconstructor::vertices (
        const vector < reco::TransientTrack > & tracks,
        const vector < combsv::CombinedTrack > & etracks,
        const VertexFilter & vfilter,
        const PseudoVertexBuilder & pvtxbuilder ) const
{
  vector< combsv::CombinedVertex > ret = 
    fit ( tracks, vfilter, pvtxbuilder.trackInfoBuilder() );
  reco::btag::Vertices::VertexType type=reco::btag::Vertices::UndefVertex;

  if (ret.size() > 0)
  {
    type=reco::btag::Vertices::RecoVertex;
  } else {
    reco::Vertex vtx = pvtxbuilder.build ( etracks, vfilter, type ); 
    ret.push_back( buildVertexInfo ( vtx, etracks,
          reco::btag::Vertices::PseudoVertex, vfilter, pvtxbuilder.trackInfoBuilder() ) );

    // combsv::CombinedVertex cvtx ( vtx );
    // ret.push_back ( cvtx, etracks,  );
  }

  return pair < reco::btag::Vertices::VertexType, vector< combsv::CombinedVertex > >
            ( type, ret );
}
