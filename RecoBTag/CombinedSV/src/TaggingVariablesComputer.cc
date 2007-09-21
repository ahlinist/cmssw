#include "RecoBTag/CombinedSV/interface/TaggingVariablesComputer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

namespace {
  void addIPs ( reco::TaggingVariableList & coll, const vector < combsv::CombinedTrack > & tracks )
  {
    for ( vector< combsv::CombinedTrack >::const_iterator t=tracks.begin(); 
          t!=tracks.end() ; ++t )
    {
      float sip2d=t->ip2D().significance();
      // sip2d=copysign ( sip2d, t->ip3D().significance() );
      coll.push_back(reco::TaggingVariable( reco::btau::trackSip2dSig, sip2d) );
    }
  }

  void addRapidities ( reco::TaggingVariableList & coll, const vector < combsv::CombinedTrack > & tracks )
  {
    for ( vector< combsv::CombinedTrack >::const_iterator t=tracks.begin(); 
          t!=tracks.end() ; ++t )
    {
      coll.push_back(reco::TaggingVariable(reco::btau::trackEtaRel, t->rapidity() ) );
    }
  }
}

reco::TaggingVariableList reco::TaggingVariablesComputer::compute (
    const CombinedSVTaggingVariables & allvars,
    const combsv::CombinedData & data )
{
  reco::TaggingVariableList coll;
  coll.push_back ( reco::TaggingVariable ( reco::btau::jetPt, data.pt() ) );
  coll.push_back ( reco::TaggingVariable ( reco::btau::jetEta, data.eta() ) );
  vector < reco::TaggingVariableName > vars = allvars.variables ( data.vertexType() );
  using namespace btag::Vertices;
  using namespace reco::btau;
  for ( vector< reco::TaggingVariableName >::const_iterator v=vars.begin(); v!=vars.end() ; ++v )
  {
    // compute value, add to coll
    switch (*v)
    {
      /*
      case jetNVertices: 
        coll.push_back ( reco::TaggingVariable ( *v, (double) data.jet().nVertices() ) );
        break;
      case secondaryVtxWeightedEnergyRatio:
        coll.push_back ( reco::TaggingVariable ( *v, (double) data.jet().weightedESVXOverE() ) );
        break;
        */
      case vertexCategory:
        coll.push_back ( reco::TaggingVariable ( *v, (double) data.vertexType() ) );
        break;
      case vertexMass:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().vertexMass() ) );
        break;
      case vertexNTracks:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().vertexMultiplicity() ) );
        break;
      case vertexEnergyRatio:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().eSVXOverE() ) );
        break;
      case flightDistance2dSig:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().minFlightDistanceSignificance2D() ) ); 
        break;
      case trackSip2dSigAboveCharm:
        coll.push_back ( reco::TaggingVariable ( *v, data.jet().first2DSignedIPSigniAboveCut() ) ); 
        break;
      case trackEtaRel:
        addRapidities( coll, data.ctracks() );
        break;
      case trackSip2dSig:
        addIPs ( coll, data.ctracks() );
        break;

      default:
        string name="???";
        if ( *v < reco::btau::lastTaggingVariable )
        {
          name=reco::TaggingVariableTokens[*v];
        }
        edm::LogError("TaggingVariablesComputer") << "Trying to compute unknown variable "
                                                  << name << ".";
    }
  }
  return coll;
}
