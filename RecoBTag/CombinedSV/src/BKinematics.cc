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

reco::BKinematics::BKinematics() :
      mass_(0.), energy_(0.),  fractionalEnergy_(0.),
      vec3_(GlobalVector())
{}

reco::BKinematics::BKinematics(
           const TransientVertex & vertex ) :
      mass_(0.), energy_(0.),
      fractionalEnergy_(0.), vec3_(GlobalVector())
{
  add ( vertex );
}

void reco::BKinematics::add ( const TransientVertex & vertex )
{
  if ( vertex.hasTrackWeight() )
  {
    computeKinematics(vertex.weightMap() );
  }else{
    computeKinematics(vertex.originalTracks() );
  }
}

void reco::BKinematics::add ( const combsv::CombinedVertex & vertex )
{
  map < combsv::CombinedTrack, float > trks = vertex.weightedTracks();
  for ( map < combsv::CombinedTrack, float > ::const_iterator t=trks.begin();
        t!=trks.end() ; ++t )
  {
    add ( t->first, t->second );
  }
}


reco::BKinematics::BKinematics( const vector<combsv::CombinedTrack> & trks ) :
  mass_(0.), energy_(0.), fractionalEnergy_(0.), vec3_ ( GlobalVector() )
{
  vector < reco::TransientTrack > ntrks;
  for ( vector< combsv::CombinedTrack >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
  {
    ntrks.push_back ( reco::TransientTrack ( *i ) );
  }
  computeKinematics(ntrks);
}

reco::BKinematics::BKinematics( const vector<reco::TransientTrack> & trks ) :
  mass_(0.), energy_(0.), fractionalEnergy_(0.), vec3_ ( GlobalVector() )
{
  computeKinematics(trks);
}

void reco::BKinematics::reset()
{
  mass_=0.;
  energy_=0.;
  fractionalEnergy_=0.;
  vec3_ = GlobalVector ( 0., 0., 0. );
}

void reco::BKinematics::computeKinematics( const vector<reco::TransientTrack> & trks )
{
  for ( vector<reco::TransientTrack>::const_iterator t = trks.begin();
        t != trks.end(); t++ )
  {
    add ( *t, 1.0, false );
  }

  updateMass();
}

void reco::BKinematics::computeKinematics( const map<reco::TransientTrack, float> & trks )
{
  for ( map<reco::TransientTrack, float >::const_iterator t = trks.begin();
        t != trks.end(); t++ )
  {
    add ( t->first, t->second, false );
  }

  updateMass();
}


void reco::BKinematics::updateMass()
{
  double p2 = vec3_.mag2();
  if ( p2 == 0. && energy_ == 0. )
  {
    // edm::LogError ("BKinematics" ) << "FIXME zero mass vertex. Check this.";
    mass_=0.;
    return;
  }
  if (p2 < energy_* energy_ )
  {
    mass_ = sqrt( energy_ * energy_ - p2);
  } else {
    LogDebug("") << "p^2 < E^2:  p^2=" << p2 << ", E^2=" << energy_ * energy_;
    mass_ = 0.;
  }
}

void reco::BKinematics::add ( const reco::TransientTrack & t, float weight, bool update )
{
  double p2 = t.impactPointState().globalMomentum().mag2();
  double energy = sqrt( p2 + square ( combsv::ParticleMasses::piPlus() ) );
  energy_+=energy;
  fractionalEnergy_+=weight*energy;
  // LogDebug("") << "e=" << energy << ", w=" << weight;
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

double reco::BKinematics::getWeightedEnergy() const
{
  return fractionalEnergy_;
}


GlobalVector reco::BKinematics::get3Vector() const
{
  return vec3_;
}
