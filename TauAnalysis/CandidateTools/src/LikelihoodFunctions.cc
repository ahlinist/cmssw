#include <iostream>

#include "TMath.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/LikelihoodFunctions.h"


double 
nllMET(double dMETX, double dMETY, double covDet, const TMatrixD& covInv, double power, bool verbose)
{
  double nll = 0.;
  if( covDet != 0. ){
    nll = TMath::Log(2*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(covDet)) + 0.5*(dMETX*(covInv(0,0)*dMETX + covInv(0,1)*dMETY)+dMETY*(covInv(1,0)*dMETX + covInv(1,1)*dMETY));
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  if( verbose ){
    std::cout << " >> [single likelihhod term nllMET     ] nll = " << nll << " (power = " << power << ")" << std::endl;
  }
  return power*nll; 
}

double 
nllTauToLepPhaseSpace(double decayAngle, double nunuMass, double visMass, double x, bool verbose)
{
  double tauLeptonMass2 = SVfit_namespace::tauLeptonMass2;
  double nuMass2 = nunuMass*nunuMass;
  // protect against rounding errors that may lead to negative masses
  if( nunuMass < 0. ){ nunuMass = 0.; }
  double nll = 0.;
  double prob = 0.0;
  if(nunuMass < TMath::Sqrt((1. - x)*SVfit_namespace::tauLeptonMass2))
    {
      prob = (13./TMath::Power(tauLeptonMass2,2))*(tauLeptonMass2 - nuMass2)*(tauLeptonMass2 + 2.*nuMass2)*nunuMass;
    }
  else
    {
      double nuMass_limit  = TMath::Sqrt( TMath::Sqrt((1. - x)*SVfit_namespace::tauLeptonMass2));
      double nuMass2_limit = TMath::Power(nuMass_limit,2);
      prob = (13./TMath::Power(tauLeptonMass2,2))*(tauLeptonMass2 - nuMass2_limit)*(tauLeptonMass2 + 2.*nuMass2_limit)*nuMass_limit;
      prob /= (1. + 1.e+6*TMath::Power(nunuMass - nuMass_limit,2));
    }
  //prob = 0.5*nunuMass*TMath::Sqrt((SVfit_namespace::tauLeptonMass2 - SVfit_namespace::square(nunuMass + visMass))*(SVfit_namespace::tauLeptonMass2 - SVfit_namespace::square(nunuMass - visMass)))/(2*SVfit_namespace::tauLeptonMass)*TMath::Sin(decayAngle);
  prob = prob * TMath::Sin(decayAngle);
  if( prob > 0. ){
    nll = -TMath::Log(prob);
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  //if(nunuMass  > 1.77*TMath::Sqrt(1-x))   nll = std::numeric_limits<float>::max();
  if( verbose ){
    std::cout << " >> [single likelihood term nllTauToLep] nll = " << nll << std::endl;
  }
  return nll;
}

double 
nllTauToHadPhaseSpace(double decayAngle,double nunuMass, double visMass, double x, bool verbose)
{
  double Pvis_rf = SVfit_namespace::pVisRestFrame(visMass, nunuMass);
  double tauLeptonMass = SVfit_namespace::tauLeptonMass;
  double tauLeptonMass2 = SVfit_namespace::tauLeptonMass2;
  double visMass2 = visMass*visMass;
  double prob = tauLeptonMass / (2.*Pvis_rf);
  if(x < (visMass2/tauLeptonMass2))
    {
      double x_limit = visMass2/tauLeptonMass2;
      prob /= (1. + 1.e+6*TMath::Power(x - x_limit,2));
    }
  prob *= TMath::Sin(decayAngle);
  //double prob =0.5*TMath::Abs(SVfit_namespace::tauLeptonMass2 - SVfit_namespace::square(visMass))/(SVfit_namespace::tauLeptonMass*SVfit_namespace::tauLeptonMass)*TMath::Sin(decayAngle);
  double nll = 0.;
  if( prob > 0. ){
    nll = -TMath::Log(prob);
  } 
  else{
    nll = std::numeric_limits<float>::max();
  }
  if( verbose ){
    std::cout << " >> [single likelihood term nllTauToHad] nll = " << nll << std::endl;
  }
  return nll;
}
