#include "AnalysisDataFormats/TauAnalysis/interface/tauAnalysisAuxFunctions.h"

#include <TMath.h>

namespace TauAnalysis_namespace 
{
  double compDecayDistance(const AlgebraicVector3& eventVertexPos, const AlgebraicVector3& decayVertexPos)
  {
    double mag2 = 0.;
    for ( unsigned iDimension = 0; iDimension < 3; ++iDimension ) {
      double d = eventVertexPos(iDimension) - decayVertexPos(iDimension);
      mag2 += d*d;
    }
    return TMath::Sqrt(mag2);
  }
}


