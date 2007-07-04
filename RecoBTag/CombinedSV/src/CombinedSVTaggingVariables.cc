#include <algorithm>
#include "RecoBTag/CombinedSV/interface/CombinedSVTaggingVariables.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace reco::btau;

void CombinedSVTaggingVariables::pseudoFromNo()
{
  for ( vector< reco::TaggingVariableName >::const_iterator i=theNoVars.begin(); 
        i!=theNoVars.end() ; ++i )
  {
    thePseudoVars.push_back ( *i );
  }
}

void CombinedSVTaggingVariables::recoFromPseudo()
{
  for ( vector< reco::TaggingVariableName >::const_iterator i=thePseudoVars.begin(); 
        i!=thePseudoVars.end() ; ++i )
  {
    theRecoVars.push_back ( *i );
  }
}

void CombinedSVTaggingVariables::fullSet()
{
  theNoVars.push_back ( reco::btau::vertexCategory );
  theNoVars.push_back ( reco::btau::trackSip2d );
  pseudoFromNo();
  thePseudoVars.push_back ( reco::btau::vertexMass );
  thePseudoVars.push_back ( reco::btau::vertexMultiplicity );
  thePseudoVars.push_back ( reco::btau::secondaryVtxEnergyRatio );
  thePseudoVars.push_back ( reco::btau::trackSip2dAbCharm );
  thePseudoVars.push_back ( reco::btau::trackEtaRel );
  // thePseudoVars.push_back ( reco::btau::secondaryVtxWeightedEnergyRatio );
  recoFromPseudo();
  theRecoVars.push_back ( reco::btau::flightDistance2DSignificance );
  // theRecoVars.push_back ( reco::btau::jetNVertices );
}

void CombinedSVTaggingVariables::vertexSet()
{
  theNoVars.push_back ( reco::btau::vertexCategory );
  pseudoFromNo();
  /*
  thePseudoVars.push_back ( reco::btau::vertexMass );
  thePseudoVars.push_back ( reco::btau::vertexMultiplicity );
  thePseudoVars.push_back ( reco::btau::secondaryVtxEnergyRatio );
   */
  recoFromPseudo();
  theRecoVars.push_back ( reco::btau::flightDistance2DSignificance );
}

CombinedSVTaggingVariables::CombinedSVTaggingVariables ( 
    CombinedSVTaggingVariables::TaggingVariableSet s )
{
  switch ( s )
  {
    case FullSet:
      { fullSet(); break; }
    case VertexSet:
      { vertexSet(); break; }
    default:
      edm::LogError("") << "TaggingVariableSet \"" << s << "\" unknown. Use \"full\" or \"vertex\"";
      exit(0);
  }
}

CombinedSVTaggingVariables::CombinedSVTaggingVariables (
    const vector < TaggingVariableName > & recovertex,
    const vector < TaggingVariableName > & pseudovertex,
    const vector < TaggingVariableName > & novertex ) :
  theRecoVars ( recovertex ), thePseudoVars ( pseudovertex ), theNoVars ( novertex )
{
  // the variables in TaggingVariableList have to be sorted
  // by the TaggingVariableName
  std::sort(theRecoVars.begin(),   theRecoVars.end());
  std::sort(thePseudoVars.begin(), thePseudoVars.end());
  std::sort(theNoVars.begin(),     theNoVars.end());
}

const vector < TaggingVariableName > & CombinedSVTaggingVariables::variables ( 
    reco::btag::Vertices::VertexType t ) const
{
  using namespace reco::btag::Vertices;
  switch ( t )
  {
    case RecoVertex:
      return recoVertex();
    case PseudoVertex:
      return pseudoVertex();
    case NoVertex:
      return noVertex();
    default:
      edm::LogError("CombinedSVTaggingVariables")
        << "trying to fetch variables for unknown category";
      exit (-1);
      // return vector < TaggingVariableName > ();
  }
}

const vector < TaggingVariableName > & CombinedSVTaggingVariables::recoVertex() const
{
  return theRecoVars;
}

const vector < TaggingVariableName > & CombinedSVTaggingVariables::pseudoVertex() const
{
  return thePseudoVars;
}

const vector < TaggingVariableName > & CombinedSVTaggingVariables::noVertex() const
{
  return theNoVars;
}
