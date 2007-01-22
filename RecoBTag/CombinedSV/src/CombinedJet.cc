#include "RecoBTag/CombinedSV/interface/CombinedJet.h"

combsv::CombinedJet::CombinedJet(
    double VertexMass, double VertexMultiplicity, double ESVXOverE,
    double MinFlightDistanceSignificance2D, double First2DSignedIPSigniAboveCut ) :
  vertexMass_ ( VertexMass ), vertexMultiplicity_ ( VertexMultiplicity ),
  eSVXOverE_ ( ESVXOverE ), minFlightDistanceSignificance2D_ ( MinFlightDistanceSignificance2D ),
  first2DSignedIPSigniAboveCut_ ( First2DSignedIPSigniAboveCut ), isValid_ ( true )
{}

combsv::CombinedJet::CombinedJet() : isValid_(false)
{}

bool combsv::CombinedJet::isValid() const
{
  return isValid_;
}

double combsv::CombinedJet::vertexMass() const
{
  return vertexMass_;
}

double combsv::CombinedJet::vertexMultiplicity() const
{
  return vertexMultiplicity_; 
}

double combsv::CombinedJet::eSVXOverE() const
{
  return eSVXOverE_;
}

double combsv::CombinedJet::minFlightDistanceSignificance2D() const
{
  return minFlightDistanceSignificance2D_;
}

double combsv::CombinedJet::first2DSignedIPSigniAboveCut() const
{
  return first2DSignedIPSigniAboveCut_;
}

