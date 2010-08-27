#ifndef TauAnalysis_CandidateTools_svFitAuxFunctions_h
#define TauAnalysis_CandidateTools_svFitAuxFunctions_h

#include "DataFormats/Candidate/interface/Candidate.h"

namespace SVfit_namespace 
{
   /// Determine visible tau rest frame energy given visible mass and neutrino mass
   double pVisRestFrame(double tauVisMass, double tauNuNuMass);

   /// Convert the Gottfried-Jackson angle into a LAB frame opening angle
   double gjAngleToLabFrame(double pVisRestFrame, double gjAngle, double pVisLabFrame);

   /// Determine the tau momentum in the lab frame given the rest frame assumptions
   double tauMomentumLabFrame(double tauVisMass, double pVisRestFrame, double gjAngle, double pVisLabFrame);

   /// Determine the tau direction given our parameterization 
   reco::Candidate::Vector tauDirection(const reco::Candidate::Vector& pVisLabFrame, double angleVisLabFrame, double phiLab);
   
   /// Compute the tau four vector given the tau direction and momentum
   reco::Candidate::LorentzVector tauP4(const reco::Candidate::Vector& tauDirection, double tauMomentumLabFrame);

   /// Vector sum of two points
   reco::Candidate::Point operator+(const reco::Candidate::Point&, const reco::Candidate::Point&);
}

#endif
