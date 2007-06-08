#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

void combsv::CombinedVertex::print() const
{
  LogDebug("") 
    << "****** print CombinedVertex from extended bTag information (combined bTag) " << endl
    << "chi2                         " << chi2()       << endl
    << "ndof                         " << ndof()       << endl
    << "nTracks                      " << nTracks()    << endl 
    << "mass                         " << mass()       << endl   
    << "isV0                         " << isV0()       << endl     
  //   << "fracPV                       " << fracPV()     << endl    
    << "flightDistanced2             " << flightDistance2D().value() << endl
    << "flightDistanced3             " << flightDistance3D().value() << endl;
}

combsv::CombinedVertex::CombinedVertex( const reco::Vertex & vertex,
        const vector < combsv::CombinedTrack > & trks, const GlobalVector & trackVector, double mass,
        bool isV0, const Measurement1D & d2, const Measurement1D & d3 ) : 
  reco::Vertex(vertex), trackVector_ ( trackVector ), mass_ ( mass ), isV0_ ( isV0 ), 
  d2_(d2), d3_(d3), isValid_(true), btagTracks_ ( trks )
{
  for ( vector< combsv::CombinedTrack >::const_iterator i=trks.begin(); 
        i!=trks.end() ; ++i )
  {
    weightedTracks_[*i]=1.0;
  }
}

combsv::CombinedVertex::CombinedVertex( const reco::Vertex & vertex,
        const map < combsv::CombinedTrack, float > & trks, const GlobalVector & trackVector, double mass,
        bool isV0, const Measurement1D & d2, const Measurement1D & d3 ) : 
  reco::Vertex(vertex), trackVector_ ( trackVector ), mass_ ( mass ), isV0_ ( isV0 ), 
  d2_(d2), d3_(d3), isValid_(true), weightedTracks_ ( trks )
{
  for ( map < combsv::CombinedTrack, float >::const_iterator i=weightedTracks_.begin(); 
        i!=weightedTracks_.end() ; ++i )
  {
    btagTracks_.push_back ( i->first );
  }
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

map < combsv::CombinedTrack, float > combsv::CombinedVertex::weightedTracks() const
{
  return weightedTracks_;
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
