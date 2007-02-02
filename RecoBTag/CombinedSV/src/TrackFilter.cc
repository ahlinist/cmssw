#include "RecoBTag/CombinedSV/interface/TrackFilter.h"
#include <limits>

using namespace std;

TrackFilter::TrackFilter ( float pTMin, int nHitAllMin, int nHitPixelMin,
  float n_chi2Max, float etaMin, float etaMax, float ip2DMin,
  float ip2DMax, float jetDistanceMax ) :
  pTMin_(pTMin), nHitAllMin_(nHitAllMin), nHitPixelMin_(nHitPixelMin),
  normedChi2Max_(n_chi2Max), etaMin_(etaMin), etaMax_(etaMax), ip2DMin_(ip2DMin),
  ip2DMax_(ip2DMax), jetDistanceMax_(jetDistanceMax)
{}

void TrackFilter::print ( const combsv::CombinedTrack & data, double ipSigni2DCut ) const
{
  cout << endl;
  cout << "[TrackFilter]" << endl;
  cout << " - pt          '" <<data.track().pt()           << "' > " << pTMin_        << endl;
  #ifdef RAVE
  cout << " - #hits       '" <<data.raveTrack().tag() << "' == 1 " << endl;
  #else
  cout << " - #hits       '" <<data.nHitsTotal()   << "' > " << nHitAllMin_   << endl;
  cout << " - #hits pixel " <<data.nHitsPixel()   << " > " << nHitPixelMin_ << endl;
  #endif
  cout << " - chi2 / ndof '" <<data.chi2() / data.ndof() << "' < " << normedChi2Max_      << endl;
//  cout << " - eta         '" <<data.eta()          << "' > " << etaMin_       << endl;
//  cout << " - eta         '" <<data.eta()          << "' < " << etaMax_       << endl;
  cout << " - ip2D        '" <<data.ip2D().value() << "' > " << ip2DMin_      << endl;
  cout << " - ip2D        '" <<data.ip2D().value() << "' < " << ip2DMax_      << endl;
  cout << " - ip2D signi  '" <<data.ip2D().significance() << "' > " << ipSigni2DCut     << endl;
  cout << " - jetDistance '" << data.jetDistance() << "' < " << jetDistanceMax_ << endl;
  // cout << " - accept: " << this->operator() ( data, ipSigni2DCut, false ) << endl;
}

bool TrackFilter::operator()( const combsv::CombinedTrack & data,
                              double ipSigni2DCut, bool prt ) const
{
  if ( prt ) print ( data, ipSigni2DCut );
  bool ret = operator() ( data, false );
  if (!ret) return false;
  return ( data.ip2D().significance() > ipSigni2DCut );
}

bool TrackFilter::operator()( const combsv::CombinedTrack & data, bool prt ) const
{
  if ( prt ) print ( data, -numeric_limits<double>::infinity() );
  return( data.track().pt() > pTMin_ &&
  #ifdef RAVE
          data.raveTrack().tag() == "1" &&
  #else
          data.nHitsTotal() >= nHitAllMin_ &&
          data.nHitsPixel() >= nHitPixelMin_ &&
  #endif
          data.chi2() / data.ndof() < normedChi2Max_ &&
          // data.eta() > etaMin_ && data.eta() < etaMax_ &&
          data.ip2D().value() > ip2DMin_ && data.ip2D().value() < ip2DMax_ &&
          data.jetDistance() < jetDistanceMax_ );
}
