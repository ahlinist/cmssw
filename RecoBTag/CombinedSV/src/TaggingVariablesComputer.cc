#include "RecoBTag/CombinedSV/interface/TaggingVariablesComputer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  void addIPs ( reco::TaggingVariableList & coll, const vector < combsv::CombinedTrack > & tracks )
  {
    for ( vector< combsv::CombinedTrack >::const_iterator t=tracks.begin(); 
          t!=tracks.end() ; ++t )
    {
      coll.push_back(reco::TaggingVariable( reco::btag::trackSip2d, t->ip2D().significance() ) );
    }
  }

  void addRapidities ( reco::TaggingVariableList & coll, const vector < combsv::CombinedTrack > & tracks )
  {
    for ( vector< combsv::CombinedTrack >::const_iterator t=tracks.begin(); 
          t!=tracks.end() ; ++t )
    {
      coll.push_back(reco::TaggingVariable(reco::btag::trackEtaRel, t->rapidity() ) );
    }
  }
}

reco::TaggingVariableList reco::TaggingVariablesComputer::compute (
    const CombinedSVTaggingVariables & allvars,
    const combsv::CombinedData & data )
{
  reco::TaggingVariableList coll;
  coll.push_back ( reco::TaggingVariable ( reco::btag::jetPt, data.pt() ) );
  coll.push_back ( reco::TaggingVariable ( reco::btag::jetEta, data.eta() ) );
  vector < reco::btag::TaggingVariableName > vars = allvars.variables ( data.vertexType() );
  using namespace btag::Vertices;
  using namespace reco::btag;
  for ( vector< reco::btag::TaggingVariableName >::const_iterator v=vars.begin(); v!=vars.end() ; ++v )
  {
    // compute value, add to coll
    switch (*v)
    {
      case vertexCategory:
        coll.push_back ( reco::TaggingVariable ( *v, (double) data.vertexType() ) );
        break;
      case vertexMass:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().vertexMass() ) );
        break;
      case vertexMultiplicity:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().vertexMultiplicity() ) );
        break;
      case eSVXOverE:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().eSVXOverE() ) );
        break;
      case flightDistance2DSignificance:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().minFlightDistanceSignificance2D() ) ); 
        break;
      case trackSip2dAbCharm:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().first2DSignedIPSigniAboveCut() ) ); 
        break;
      case trackEtaRel:
        addRapidities( coll, data.ctracks() );
        break;
      case trackSip2d:
        addIPs ( coll, data.ctracks() );
        break;

      default:
        edm::LogError("TaggingVariablesComputer") << "Trying to compute unknown variable";
    }
  }
  return coll;
}
