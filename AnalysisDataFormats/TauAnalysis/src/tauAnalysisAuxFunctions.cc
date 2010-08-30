#include "AnalysisDataFormats/TauAnalysis/interface/tauAnalysisAuxFunctions.h"

namespace TauAnalysis_namespace 
{
  reco::Candidate::Point operator+(const reco::Candidate::Point& p1, const reco::Candidate::Vector& p2)
  {
    return reco::Candidate::Point(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
  }
}


