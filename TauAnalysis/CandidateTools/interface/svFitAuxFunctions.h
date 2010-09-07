#ifndef TauAnalysis_CandidateTools_svFitAuxFunctions_h
#define TauAnalysis_CandidateTools_svFitAuxFunctions_h

#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "AnalysisDataFormats/TauAnalysis/interface/SVfitDiTauSolution.h"

namespace SVfit_namespace 
{
  const double tauLeptonMass = 1.77685; // GeV
  const double tauLeptonMass2 = tauLeptonMass*tauLeptonMass;

  const double cTauLifetime = 8.711e-3; // centimeters

  inline double square(double x)
  {
    return x*x;	
  }

  inline double cube(double x)
  {
    return x*x*x;	
  }

  inline double fourth(double x)
  {
    return x*x*x*x;	
  }

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

  /// Compute logarithm of Gaussion probability density function
  double logGaussian(double residual, double sigma);

  /// Auxiliary functions to determine if one or two neutrinos are produced in tau lepton decay;
  /// in case only one neutrino is produced, the resulting neutrino system is massless,
  /// while a neutrino system of in general non-zero invariant mass is produced in the two neutrino case
  template <typename T>
  inline bool isMasslessNuSystem() 
  {
    // massless neutrino system produced in hadronic tau --> X + nu decays;
    // use this implementation for the generic particle Candidate case 
    return true; 
  }

  template <>
  inline bool isMasslessNuSystem<pat::Electron>()
  {
    // neutrino system of in general non-zero mass produced in tau --> electron nu nu decays
    return false; 
  }

  template <>
  inline bool isMasslessNuSystem<pat::Muon>()
  {
    // neutrino system of in general non-zero mass produced in tau --> muon nu nu decays
    return false; 
  }
}

#endif
