#include "RecoBTag/CombinedSV/interface/TrackInfoBuilder.h"
#include "RecoBTag/BTagTools/interface/SignedTransverseImpactParameter.h"
#include "RecoBTag/BTagTools/interface/SignedImpactParameter3D.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "RecoVertex/VertexPrimitives/interface/BeamSpot.h"
#include "RecoBTag/CombinedSV/interface/ParticleMasses.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
namespace {
   typedef numeric_limits<double> num;
}

double TrackInfoBuilder::computeTrackRapidity( const reco::TransientTrack & track,
                                               const GlobalVector & pAll ) const
{
  double y = 5.0; // default value
  if (pAll.mag() > 0)
  {
    GlobalVector track3Vector(track.impactPointState().globalMomentum().x(),
        track.impactPointState().globalMomentum().y(),
        track.impactPointState().globalMomentum().z());

    double pLong   = (pAll * track3Vector) / pAll.mag() ;  // long. comp. of track along jet
    double energy  = sqrt ( track3Vector.mag2() +
                            combsv::ParticleMasses::piPlus() * combsv::ParticleMasses::piPlus() );

    if ( (energy+pLong) > num::min() && (energy-pLong) > num::min() )
      y = 0.5 * log ( (energy+pLong) / (energy-pLong) );
  }
  return y;
}

TrackInfoBuilder::TrackInfoBuilder () : pv_(reco::Vertex() )
{}

void TrackInfoBuilder::setJet ( const reco::Vertex & pv, const GlobalVector & jetdir )
{
  pv_=pv;
  jetdir_=jetdir;
}

combsv::CombinedTrack TrackInfoBuilder::build( const reco::TransientTrack & tt ) const
{
  SignedTransverseImpactParameter ip2DCalculator; // instance of the class computing it
  SignedImpactParameter3D         ip3DCalculator;

  // compute lifetime-signed transverse impact parameter w.r.t. primary vertex
  // CHECK: error seems to be always zero

  pair<bool,Measurement1D> ipReturnValue  =
    ip2DCalculator.apply(tt,jetdir_, pv_ );

  // float sgn = float ( d0Sign ( tt ) );
  Measurement1D ip2d;
  if (ipReturnValue.first)
  {
    // float value= copysign ( ipReturnValue.second.value(), sgn );
    /*
    float value=ipReturnValue.second.value();
    float error=ipReturnValue.second.error();
    ip2d=Measurement1D ( value, error );
    */
    ip2d=ipReturnValue.second;
  }

  // compute lifetime-signed 3D impact parameter w.r.t primary Vertex
  // -> needed for jetDistance cut
  double jetDistance = fabs(ip3DCalculator.distanceWithJetAxis
      (tt, jetdir_, pv_).second.value());
  ipReturnValue = ip3DCalculator.apply(tt, jetdir_, pv_ );

  Measurement1D ip3d;
  if (ipReturnValue.first) { ip3d=ipReturnValue.second; }
  /* LogDebug("") << "Computing signed impact point in 2d: jetdir="
               << jetdir_ << ", pv=" << pv_.position()
               << " significance=" << ip2d.significance()
               << " sgn=" << sgn 
               << " 3ds=" << ip3d.significance();*/

  double rapidity = computeTrackRapidity  ( tt, jetdir_ );

  combsv::CombinedTrack trackData ( tt, jetDistance, ip2d, ip3d, rapidity );
  return trackData;
}

vector < combsv::CombinedTrack > TrackInfoBuilder::build (
    const vector < reco::TransientTrack > & trks ) const
{
  vector < combsv::CombinedTrack > ret;
  for ( vector< reco::TransientTrack >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
  {
    ret.push_back ( build ( *i ) );
  }
  return ret;
}


signed TrackInfoBuilder::d0Sign( const reco::TransientTrack & track ) const
{
  // EXPERIMENTAL
  int sign = +1;
  GlobalPoint trackPCA = track.impactPointState().freeState()->position();
  CLHEP::Hep3Vector dir ( jetdir_.x(), jetdir_.y(), jetdir_.z() );
  BeamSpot spot;
  CLHEP::Hep3Vector vPCA ( trackPCA.x() - spot.position().x(),
                           trackPCA.y() - spot.position().y(),
                           trackPCA.z() - spot.position().z() );
  double angle = dir.angle(vPCA);
  if (angle > M_PI / 2. ) sign = -1;
  return sign;
}

