#ifndef RecoBTag_CombinedSV_CombinedJet_h
#define RecoBTag_CombinedSV_CombinedJet_h

#include <vector>
#include <map>

#include "Geometry/Vector/interface/GlobalVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "RecoBTag/CombinedSV/interface/MinMeanMax.h"

#include <map>

namespace combsv {
  class CombinedJet {
  public:
    CombinedJet();
    CombinedJet( double VertexMass, double VertexMultiplicity, double ESVXOverE,
        double MinFlightDistanceSignificance2D, double First2DSignedIPSigniAboveCut );

    double vertexMass() const;
    double vertexMultiplicity() const;
    double eSVXOverE() const;
    double minFlightDistanceSignificance2D() const;
    double first2DSignedIPSigniAboveCut() const;
    bool isValid() const;

  private:
    double vertexMass_;
    double vertexMultiplicity_;
    double eSVXOverE_;
    double minFlightDistanceSignificance2D_;
    double first2DSignedIPSigniAboveCut_;
    bool isValid_;
  };
}

#endif
