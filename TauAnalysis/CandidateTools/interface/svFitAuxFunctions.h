#ifndef TauAnalysis_CandidateTools_svFitAuxFunctions_h
#define TauAnalysis_CandidateTools_svFitAuxFunctions_h

namespace SVfit_namespace 
{
  double pVisRestFrame(double, double);

  reco::Candidate::Point operator+(const reco::Candidate::Point&, const reco::Candidate::Point&);
}

#endif
