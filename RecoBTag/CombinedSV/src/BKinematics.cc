#include "RecoBTag/CombinedSV/interface/BKinematics.h"
#include "RecoBTag/CombinedSV/interface/TTracksFromRecoVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoBTag/CombinedSV/interface/ParticleMasses.h"

using namespace std;

namespace {
  inline double square ( double t )
  {
    return t*t;
  }
}

reco::BKinematics::BKinematics( const MagneticField * f ) :
      field_(f), mass_(0.), energy_(0.), vec3_(GlobalVector())
{}

reco::BKinematics::BKinematics( const MagneticField * f,
           const reco::Vertex & vertex ) :
      field_(f), mass_(0.), energy_(0.), vec3_(GlobalVector())
{
  TTracksFromRecoVertex converter ( field_ );
  computeKinematics ( converter.create ( vertex ) );
}

reco::BKinematics::BKinematics(
           const MagneticField * f,
           const TransientVertex & vertex ) : field_ ( f ),
      mass_(0.), energy_(0.), vec3_(GlobalVector())
{
  computeKinematics(vertex.originalTracks() );
}


reco::BKinematics::BKinematics( 
    const MagneticField * f, const vector<combsv::CombinedTrack> & trks ) : 
  field_ ( f ), mass_(0.), energy_(0.), vec3_ ( GlobalVector() )
{
  vector < reco::TransientTrack > ntrks;
  for ( vector< combsv::CombinedTrack >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
  {
    ntrks.push_back ( reco::TransientTrack ( *i ) );
  }
  computeKinematics(ntrks);
} 

reco::BKinematics::BKinematics( 
    const MagneticField * f, const vector<reco::TransientTrack> & trks ) : 
  field_ ( f ), mass_(0.), energy_(0.), vec3_ ( GlobalVector() )
{
  computeKinematics(trks);
} 

void reco::BKinematics::computeKinematics( const vector<reco::TransientTrack> & trks )
{
  energy_=0.;
  vec3_ = GlobalVector ( 0., 0., 0. );

  for ( vector<reco::TransientTrack>::const_iterator t = trks.begin(); 
        t != trks.end(); t++ )
  {
    add ( *t, false );
  }

  updateMass();
}

void reco::BKinematics::updateMass()
{
  double p2 = vec3_.mag2();
  if ( p2 == 0. && energy_ == 0. )
  {
    edm::LogError ("BKinematics" ) << "FIXME zero mass vertex. Check this.";
    mass_=0.;
    return;
  }
  if (p2 < energy_* energy_ )
  {
    mass_ = sqrt( energy_ * energy_ - p2);
  } else {
    cout << "[BKinematics] p^2 < E^2:  p^2=" << p2 << ", E^2=" << energy_ * energy_
         << endl;
    mass_ = 0.;
  }
}

void reco::BKinematics::add ( const reco::TransientTrack & t, bool update )
{
  double p2 = t.impactPointState().globalMomentum().mag2();
  double energy = sqrt( p2 + square ( combsv::ParticleMasses::piPlus() ) );
  energy_+=energy;
  vec3_ += t.impactPointState().globalMomentum();
  if (update) updateMass();
}

void reco::BKinematics::add ( const combsv::CombinedTrack & t )
{
  reco::TransientTrack tt ( t );
  add ( tt );
}

double reco::BKinematics::getMass() const
{
  return mass_;
}

double reco::BKinematics::getEnergy() const
{
  return energy_;
}

GlobalVector reco::BKinematics::get3Vector() const
{
  return vec3_;
}
