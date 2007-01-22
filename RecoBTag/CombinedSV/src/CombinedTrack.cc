#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include <limits>
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  typedef std::numeric_limits<double> num;
}

combsv::CombinedTrack::CombinedTrack::CombinedTrack() :
  reco::TransientTrack(),
  usedInSVX_ ( false ), rapidity_ ( num::quiet_NaN() ), 
  ip2D_ ( Measurement1D() ),
  ip3D_ ( Measurement1D() ),
  aboveCharmMass_ ( false ), isValid_ ( false )
{} 

combsv::CombinedTrack::CombinedTrack::CombinedTrack(
           const reco::TransientTrack & ref, double jetDistance,
           const Measurement1D & ip2d, const Measurement1D & ip3d,
           double rapidity, bool usedInSVX, bool aboveCharmMass ) :
  reco::TransientTrack(ref),usedInSVX_(usedInSVX),rapidity_(rapidity),
  jetDistance_(jetDistance),
  ip2D_ ( ip2d ), ip3D_ ( ip3d ), aboveCharmMass_(aboveCharmMass), isValid_(true)
{}

void combsv::CombinedTrack::CombinedTrack::print() const
{
  cout << "*** printing trackData for combined b-tag info " << endl;
  cout << "    aboveCharmMass   " << aboveCharmMass()   << endl;
  cout << "    pt               " << pt()               << endl;
  cout << "    rapidity         " << rapidity()         << endl;
  cout << "    jetDistance      " << jetDistance()      << endl;
  cout << "    nHitsTotal       " << nHitsTotal()       << endl;
  cout << "    nHitsPixel       " << nHitsPixel()       << endl;
  cout << "    firstHitPixel    " << firstHitPixel()    << endl;
  cout << "    ip2D             " << ip2D().value()     << endl;
  cout << "    ip3D             " << ip3D().value()     << endl;
}

int combsv::CombinedTrack::nHitsTotal() const
{
  #ifdef RAVE
  cout << "[CombinedTrack] nHitsTotal not available" << endl;
  return 0;
  #else
  return recHitsSize();
  #endif
}

int combsv::CombinedTrack::nHitsPixel() const
{
  #ifdef RAVE
  cout << "[CombinedTrack] nHitsPixel not available" << endl;
  return 0;
  #else
  return hitPattern().numberOfValidPixelHits();
  #endif
}

bool combsv::CombinedTrack::firstHitPixel() const
{
  #ifdef RAVE
  cout << "[CombinedTrack] firstHitPixel not available" << endl;
  return false;
  #else
  return hitPattern().hasValidHitInFirstPixelBarrel();
  #endif
}

bool combsv::CombinedTrack::isValid() const
{
  return isValid_;
} 

double combsv::CombinedTrack::rapidity() const
{
  return rapidity_;
}

double combsv::CombinedTrack::jetDistance() const
{
  return jetDistance_;
}

Measurement1D combsv::CombinedTrack::ip2D() const
{
  return ip2D_;
}

Measurement1D combsv::CombinedTrack::ip3D() const
{
  return ip3D_;
}

bool combsv::CombinedTrack::aboveCharmMass() const
{
  return aboveCharmMass_;
}

void combsv::CombinedTrack::setAboveCharmMass( bool s )
{
  aboveCharmMass_=s;
}
