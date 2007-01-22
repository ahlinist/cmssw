#include "RecoBTag/CombinedSV/interface/TrackFilter.h"

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
  cout << " - pt          _" <<data.pt()           << "_ > " << pTMin_        << endl;
//  cout << " - #hits       _" <<data.nHitsTotal()   << "_ > " << nHitAllMin_   << endl;
//cout << " - #hits pixel " <<data.nHitsPixel()   << " > " << nHitPixelMin_ << endl;
//cout << " - chi2        " <<data.chi2()         << " < " << normedChi2Max_      << endl;
//  cout << " - eta         _" <<data.eta()          << "_ > " << etaMin_       << endl;
//  cout << " - eta         _" <<data.eta()          << "_ < " << etaMax_       << endl;
  cout << " - ip2D        _" <<data.ip2D().value() << "_ > " << ip2DMin_      << endl;
  cout << " - ip2D        _" <<data.ip2D().value() << "_ < " << ip2DMax_      << endl;
  cout << " - ip2D signi  _" <<data.ip2D().significance() << "_ > " << ipSigni2DCut     << endl;
  cout << " - jetDistance _" << data.jetDistance() << "_ < " << jetDistanceMax_ << endl;
  cout << " - accept: " << this->operator() ( data, ipSigni2DCut, false ) << endl;
}

bool TrackFilter::operator()( const combsv::CombinedTrack & data, 
                              double ipSigni2DCut, bool prt ) const
{
  if ( prt ) print ( data, ipSigni2DCut );
  return( data.pt() > pTMin_ && 
          // data.nHitsTotal() >= nHitAllMin_ &&
          // data.nHitsPixel() >= nHitPixelMin_ &&
          // data.chi2() / data.ndf() < normedChi2Max_ &&
          // data.eta() > etaMin_ && data.eta() < etaMax_ &&
          data.ip2D().value() > ip2DMin_ && data.ip2D().value() < ip2DMax_ &&
          data.ip2D().significance() > ipSigni2DCut &&
          data.jetDistance() < jetDistanceMax_ );
  // return true;
}
