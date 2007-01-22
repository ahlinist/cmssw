#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"

using namespace std;

void combsv::CombinedVertex::print() const
{
  cout << "****** print CombinedVertex from extended bTag information (combined bTag) " << endl;
  cout << "chi2                         " << chi2()       << endl;
  cout << "ndof                         " << ndof()       << endl;
  cout << "nTracks                      " << nTracks()    << endl; 
  cout << "mass                         " << mass()       << endl;   
  cout << "isV0                         " << isV0()       << endl;     
  // cout << "fracPV                       " << fracPV()     << endl;    
  cout << "flightDistanced2             " << flightDistance2D().value() << endl;
  cout << "flightDistanced3             " << flightDistance3D().value() << endl;
}

combsv::CombinedVertex::CombinedVertex( const reco::Vertex & vertex,
        const vector < combsv::CombinedTrack > & trks, const GlobalVector & trackVector, double mass,
        bool isV0, const Measurement1D & d2, const Measurement1D & d3 ) : 
  reco::Vertex(vertex), trackVector_ ( trackVector ), mass_ ( mass ), isV0_ ( isV0 ), 
  d2_(d2), d3_(d3), isValid_(true), btagTracks_ ( trks )
{
}

combsv::CombinedVertex::CombinedVertex() : reco::Vertex(), mass_(0.), isV0_(false),
  isValid_(false)
{
}

void combsv::CombinedVertex::setFlightDistance2D ( const Measurement1D & d )
{
  d2_=d;
}

void combsv::CombinedVertex::setFlightDistance3D ( const Measurement1D & d )
{
  d3_=d;
}

int combsv::CombinedVertex::nTracks() const
{
  return tracksSize();
}

double combsv::CombinedVertex::mass() const
{
  return mass_;
}

const GlobalVector & combsv::CombinedVertex::trackVector() const
{
  return trackVector_;
}

vector < combsv::CombinedTrack > combsv::CombinedVertex::bTagTracks() const
{
  return btagTracks_;
}

bool combsv::CombinedVertex::isV0() const
{
  return isV0_;
}

Measurement1D combsv::CombinedVertex::flightDistance2D() const
{
  return d2_;
}

Measurement1D combsv::CombinedVertex::flightDistance3D() const
{
  return d3_;
}

bool combsv::CombinedVertex::isValid() const
{
  return isValid_;
}
