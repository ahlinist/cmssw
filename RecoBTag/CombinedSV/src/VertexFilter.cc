#include "RecoBTag/CombinedSV/interface/VertexFilter.h"
// #include "RecoVertex/VertexTools/interface/NbSharedTracks.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoBTag/CombinedSV/interface/BKinematics.h"
#include "Geometry/CommonDetAlgo/interface/AlgebraicObjects.h"
#include "RecoBTag/CombinedSV/interface/V0Checker.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

using namespace std;
using namespace reco;
using namespace reco::btag::Vertices;

namespace {
  string toString ( bool b )
  {
    if ( b ) { return "[accepted]"; };
    return "[declined]";
  }
}

VertexFilter::VertexFilter( const MagneticField * f, float vertexV0MassWindow,
         float vertexMassMax, int vertexMultiplicityMin, float vertexFracPV ) :
  field_ ( f ), v0Checker_ ( vertexV0MassWindow), vertexMassMax_(vertexMassMax),
  vertexMultiplicityMin_(vertexMultiplicityMin), vertexFracPV_(vertexFracPV)
{}

void VertexFilter::setPrimary ( const reco::Vertex & primary )
{
  primaryVertex_=primary;
}

const reco::Vertex & VertexFilter::primaryVertex() const
{
  return primaryVertex_;
}

bool VertexFilter::checkDistance ( const reco::Vertex & vertex ) const
{
  double dt = distance ( vertex );
  bool ret = ( dt > 0.01 && dt < 2.5 );
  if (!ret) LogDebug ("") << "distance from primary: 0.01 < " << dt << " < 2.5 " << toString(ret);
  return ret;
}

double VertexFilter::distance ( const reco::Vertex & vertex ) const
{
  double dx = vertex.position().x() - primaryVertex_.position().x();
  double dy = vertex.position().y() - primaryVertex_.position().y();
  return sqrt ( dx * dx + dy * dy );
}

bool VertexFilter::checkReducedDistance ( const reco::Vertex & vertex ) const
{
  double rd=reducedDistance ( vertex );
  bool ret=(rd>3.0);
  if ( !ret )
  {
    LogDebug ("") << "reduced distance: " << rd << " > 3.0 " << toString(ret);
  }
  return ret;
}

double VertexFilter::reducedDistance ( const reco::Vertex & vertex ) const
{
  /*
  double dx = ( vertex.x() - primaryVertex_.x() );
  double dy = ( vertex.y() - primaryVertex_.y() );
  */
  AlgebraicVector d ( 2 );
  d[0]=( vertex.position().x() - primaryVertex_.position().x() );
  d[1]=( vertex.position().y() - primaryVertex_.position().y() );
  AlgebraicSymMatrix C ( 2, 1 );
  C(1,1)=vertex.covariance()(0,0);
  C(2,1)=vertex.covariance()(1,0);
  C(2,2)=vertex.covariance()(1,1);
  /*
  double cxx = vertex.covariance (0,0);
  double cxy = vertex.covariance (1,0);
  double cyy = vertex.covariance (1,1);
  return sqrt ( dx * dx / cxx + 2 * dx * dy / cxy + dy * dy / cyy );
  */
  int fail;
  C.invert(fail);
  if ( fail )
  {
    cout << "[VertexFilter] matrix inversion failed. returning -1."
         << endl;
    return -1.0;
  };
  return C.similarity ( d );
}

bool VertexFilter::checkPseudoVertex ( const reco::Vertex & vertex ) const
{
  if ( vertex.tracksSize() < vertexMultiplicityMin_ )
  {
    LogDebug("") << "is PseudoVertex. " << vertex.tracksSize()
         << " is too few tracks: -> [false]" << endl;
    return false; // too few trks?
  }

  // FIXME must also check impact parameter significance > 2.0!!
  return true;
}

bool VertexFilter::operator() ( const reco::Vertex & vertex, VertexType t ) const
{
  if ( t==NoVertex )
  {
    LogDebug ("") << "is NoVertex " << toString(true);
    return true;
  }

  if ( t==PseudoVertex ) return checkPseudoVertex ( vertex );

  // RecoVertices
  if (!(checkDistance(vertex ))) return false; // must be within limits
  if (!(checkReducedDistance(vertex ))) return false; // must be within limits
  if ( checkV0(vertex) ) return false; // must not be a V0
  if ( vertex.tracksSize() < vertexMultiplicityMin_ )
  {
    LogDebug("") << "has too few tracks. " << vertex.tracksSize() << " < "
                 << vertexMultiplicityMin_ << toString(false);
    return false; // too few trks?
  }

  reco::BKinematics vertexKinematics( field_, vertex );
  double mass = vertexKinematics.getMass();
  if ( mass > vertexMassMax_ )
  {
    LogDebug("") << "is too heavy. " << mass << " > " 
                 << vertexMassMax_ << " " << toString(false);
    return false; // too heavy?
  }

  // determine fraction of tracks also used // to build primary vertex
  int nTracksSV = vertex.tracksSize();
  int nShared=0;

  /*
   * FIXME removed the NbSharedTracks test, because 
   * of the adaptive fitters ....
  double fracPV = 0.;
  if (nTracksSV >0)
  {
    reco::NbSharedTracks numSharedTracksCalc;
    nShared = numSharedTracksCalc.sharedTracks(primaryVertex_, vertex);
    fracPV = (double) nShared/(double)nTracksSV;
  }

  if ( fracPV < 0. || fracPV >= vertexFracPV_)
  {
    LogDebug("") << "too many tracks (" << nShared << " / " << nTracksSV
                 << ") shared with primary vertex " 
                 << fracPV << " >= " << vertexFracPV_ << " " << toString(false);
    return false; // primary vertex fails
  }*/

  return true;
}

const MagneticField & VertexFilter::field() const
{
  return (*field_);
}

void VertexFilter::setMagneticField ( const MagneticField *  m )
{
  field_=m;
}

bool VertexFilter::checkV0 ( const reco::Vertex & v ) const
{
  bool ret= v0Checker_.check ( v );
  if (ret)
  {
    LogDebug("") << "is a V0" << toString(false);
  }
  return ret;
}
