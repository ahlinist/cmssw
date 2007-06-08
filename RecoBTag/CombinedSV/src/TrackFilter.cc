#include "RecoBTag/CombinedSV/interface/TrackFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
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
  LogDebug("") << endl
    << "[TrackFilter]" << endl
    << " - pt          '" <<data.track().pt()           << "' > " << pTMin_        << endl
  #ifdef RAVE
    << " - #hits       '" <<data.raveTrack().tag() << "' == 1 " << endl
  #else
    << " - #hits       '" <<data.nHitsTotal()   << "' > " << nHitAllMin_   << endl
    << " - #hits pixel " <<data.nHitsPixel()   << " > " << nHitPixelMin_ << endl
  #endif
    << " - chi2 / ndof '" <<data.chi2() / data.ndof() << "' < " << normedChi2Max_      << endl
//    << " - eta         '" <<data.eta()          << "' > " << etaMin_       << endl
//    << " - eta         '" <<data.eta()          << "' < " << etaMax_       << endl
    << " - ip2D        '" <<data.ip2D().value() << "' > " << ip2DMin_      << endl
    << " - ip2D        '" <<data.ip2D().value() << "' < " << ip2DMax_      << endl
    << " - ip2D signi  '" <<data.ip2D().significance() << "' > " << ipSigni2DCut     << endl
    << " - jetDistance '" << data.jetDistance() << "' < " << jetDistanceMax_ << endl;
  //   << " - accept: " << this->operator() ( data, ipSigni2DCut, false ) << endl;
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
