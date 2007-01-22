#include "RecoBTag/CombinedSV/interface/TrackInfoBuilder.h"
#include "RecoBTag/BTagTools/interface/SignedTransverseImpactParameter.h"
#include "RecoBTag/BTagTools/interface/SignedImpactParameter3D.h"
#include "MagneticField/Engine/interface/MagneticField.h"                                                       
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

TrackInfoBuilder::TrackInfoBuilder () : m_(0), pv_(reco::Vertex() )
{}

void TrackInfoBuilder::setJet ( const reco::Vertex & pv, const GlobalVector & jetdir )
{
  pv_=pv;
  jetdir_=jetdir;
}

TrackInfoBuilder::TrackInfoBuilder ( const MagneticField * m ) :
  m_ (m)
{}

combsv::CombinedTrack TrackInfoBuilder::build( const reco::TransientTrack & tt ) const
{
  SignedTransverseImpactParameter ip2DCalculator; // instance of the class computing it
  SignedImpactParameter3D         ip3DCalculator;

  // compute lifetime-signed transverse impact parameter w.r.t. primary vertex
  // CHECK: error seems to be always zero

  pair<bool,Measurement1D> ipReturnValue  = 
    ip2DCalculator.apply(tt,jetdir_, pv_ );

  Measurement1D ip2d;
  if (ipReturnValue.first) { ip2d=ipReturnValue.second; }

  // compute lifetime-signed 3D impact parameter w.r.t primary Vertex
  // -> needed for jetDistance cut
  double jetDistance = fabs(ip3DCalculator.distanceWithJetAxis
      (tt, jetdir_, pv_).second.value());
  ipReturnValue = ip3DCalculator.apply(tt, jetdir_, pv_ );

  Measurement1D ip3d;
  if (ipReturnValue.first) { ip3d=ipReturnValue.second; }

  double rapidity = computeTrackRapidity  ( tt, jetdir_ );

  /* d0Sign(tt, jetdir_), */
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


double TrackInfoBuilder::d0Sign( const reco::TransientTrack & track,
    const GlobalVector & jetdirection ) const
{
  edm::LogWarning("TrackInfoBuilder") << "d0Sign not computed";
  return -1.;
  /** compute signed d0 of track w.r.t. jet axis?
   */
  // double d0 = track.d0();  // 2D impact parameter, no life-time sign
                              // computed w.r.t beamline
  /*
  double d0 = 0.;

  // get position of point-of-closest-approach to beamline
  math::XYZPoint trackPCA = track.vertex();

  // get beamline as origin
  math::XYZPoint beamline(0,0,0); // use geometric origin for now->to be changed

  // define vector connecting origin->PCA
  Hep3Vector vPCA (trackPCA.X() - beamline.X(),
                   trackPCA.Y() - beamline.Y(),
                   trackPCA.Z() - beamline.Z());

  Hep3Vector dir ( jetdirection.x(), jetdirection.y(), jetdirection.z() );

  double angle = dir.angle(vPCA);

  // determine sign for impact parameter
  int sign = +1;
  if (angle > M_PI) sign = -1;

  double returnValue = sign*d0;

  return returnValue;
   */
}

