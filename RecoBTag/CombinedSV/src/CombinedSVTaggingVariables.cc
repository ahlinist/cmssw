#include "RecoBTag/CombinedSV/interface/CombinedSVTaggingVariables.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
using namespace reco::btag;

CombinedSVTaggingVariables::CombinedSVTaggingVariables (
    const vector < TaggingVariableName > & recovertex,
    const vector < TaggingVariableName > & pseudovertex,
    const vector < TaggingVariableName > & novertex ) :
  theRecoVars ( recovertex ), thePseudoVars ( pseudovertex ), theNoVars ( novertex )
{}

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
