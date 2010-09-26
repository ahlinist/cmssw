#include "TauAnalysis/CandidateTools/interface/SVfitVMlineShapeIntegrand.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

#include <iostream>

using namespace SVfit_namespace;

SVfitVMlineShapeIntegrand::SVfitVMlineShapeIntegrand(bool useCollApproxFormulas, double minMass2)
  : useCollApproxFormulas_(useCollApproxFormulas),
    minMass2_(minMass2),
    vmType_(kVMtypeUndefined),
    vmPol_(kVMpolUndefined),
    mode_(kVMmodeUndefined)
{
  theta_ = 0.; 
  tauLeptonPol_ = 0.; 
}

SVfitVMlineShapeIntegrand::SVfitVMlineShapeIntegrand(const SVfitVMlineShapeIntegrand& bluePrint)
{
  useCollApproxFormulas_ = bluePrint.useCollApproxFormulas_;
  minMass2_ = bluePrint.minMass2_;
  this->SetVMtype(bluePrint.vmType_);
  this->SetVMpol(bluePrint.vmPol_);
  this->SetMode(bluePrint.mode_);
  this->SetParameterTheta(bluePrint.theta_);
  this->SetParameterTauLeptonPol(bluePrint.tauLeptonPol_);
}

SVfitVMlineShapeIntegrand::~SVfitVMlineShapeIntegrand()
{}

SVfitVMlineShapeIntegrand& SVfitVMlineShapeIntegrand::operator=(const SVfitVMlineShapeIntegrand& bluePrint)
{
  useCollApproxFormulas_ = bluePrint.useCollApproxFormulas_;
  minMass2_ = bluePrint.minMass2_;
  this->SetVMtype(bluePrint.vmType_);
  this->SetVMpol(bluePrint.vmPol_);
  this->SetMode(bluePrint.mode_);
  this->SetParameterTheta(bluePrint.theta_);
  this->SetParameterTauLeptonPol(bluePrint.tauLeptonPol_);
  return (*this);
}

void SVfitVMlineShapeIntegrand::SetParameterTheta(double theta) 
{ 
  theta_ = theta; 

  cosTheta_ = TMath::Cos(theta_);
  sinTheta_ = TMath::Sin(theta_);
  tanTheta_ = TMath::Tan(theta_);
}

void SVfitVMlineShapeIntegrand::SetParameterTauLeptonPol(double tauLeptonPol)
{ 
  tauLeptonPol_ = tauLeptonPol; 
}

void SVfitVMlineShapeIntegrand::SetVMtype(VMtype type) 
{ 
  vmType_ = type; 

  if ( vmType_ == kVMrho ) {
    m0_ = rhoMesonMass;  
    Gamma0_ = rhoMesonWidth;
  } else if ( vmType_ == kVMa1 ) {
    m0_ = a1MesonMass;  
    Gamma0_ = a1MesonWidth;
  } else {
    edm::LogError ("SVfitVMlineShapeIntegrand::SetVMtype")
      << " Invalid vecor meson type = " << vmType_ << " !!";
    return;
  }

  m0square_ = m0_*m0_;
  fv0_ = fv(m0square_);
}

void SVfitVMlineShapeIntegrand::SetVMpol(VMpol pol) 
{ 
  vmPol_ = pol; 
}

void SVfitVMlineShapeIntegrand::SetMode(VMmode mode) 
{ 
  mode_ = mode; 
}

double SVfitVMlineShapeIntegrand::DvNorm2(double mSquare) const
{
//--- compute vector meson propagator for invariant mass^2 = mSquare
//
//    NOTE: norm of complex number 
//         |1./(a + ib)|^2 = |(a - ib)/((a + ib)*(a - ib))|^2 = |(a - ib)/(a^2 + b^2)|^2 = (a^2 + b^2)/(a^2 + b^2)^2
//        = 1./(a^2 + b^2)
//
  return 1./(square(mSquare - m0square_) + square(m0_*Gammav(mSquare))); // [1], formula (2.20)
}

double SVfitVMlineShapeIntegrand::Gammav(double mSquare) const
{
//--- compute vector meson running width

  return Gamma0_*(m0_/TMath::Sqrt(mSquare))*(fv(mSquare)/fv0_); // [1], formula (2.21)
}

double SVfitVMlineShapeIntegrand::fv(double mSquare) const
{
  if ( vmType_ == kVMrho ) {
    if ( (1. - 4.*chargedPionMass2/mSquare) > 0. )
      return TMath::Power(1. - 4.*chargedPionMass2/mSquare, 1.5); // [1], formula (2.24)
    else
      return 0.;
  } else if ( vmType_ == kVMa1 ) {
    if ( mSquare < (rhoMesonMass2 + chargedPionMass2) ) {
      double tmp = mSquare - 9*chargedPionMass2;
      return (4.1/mSquare)*cube(tmp)*(1. - 3.3*(tmp) + 5.8*square(tmp));
    } else {
      return 1.623 + 10.38/mSquare + 9.32/square(mSquare) + 0.65/cube(mSquare); // [1], formula (2.25)
    }
  } else {
    edm::LogError ("SVfitVMlineShapeIntegrand::fv")
      << " Invalid vector meson type = " << vmType_ << " !!";
    return 0.;
  }
}

double SVfitVMlineShapeIntegrand::Fv(double mSquare) const
{
  return square(1. - mSquare/tauLeptonMass2)*square(1. + 2*mSquare/tauLeptonMass2)
        *DvNorm2(mSquare)*fv(mSquare); // [1], formula (2.19)
}

double SVfitVMlineShapeIntegrand::HL(double a, double a2, double cosOmega2, double sinOmega2, double sin2Omega) const
{
  return (a2/((1. - a2)*(1. + 2*a2)))*(cosOmega2/a2 + sinOmega2 
	+ tauLeptonPol_*cosTheta_*(cosOmega2/a2 + (sin2Omega/a)*tanTheta_ - sinOmega2));                // [1], formula (2.16)
}

double SVfitVMlineShapeIntegrand::HT(double a, double a2, double cosOmega2, double sinOmega2, double sin2Omega) const
{
  return (a2/((1. - a2)*(1. + 2*a2)))*(sinOmega2/a2 + 1. + cosOmega2 
	+ tauLeptonPol_*cosTheta_*(sinOmega2/a2 - (sin2Omega/a)*tanTheta_ - 1. - cosOmega2));           // [1], formula (2.17)
}

double SVfitVMlineShapeIntegrand::decayL(double a, double a2, double cosOmega2, double sinOmega2, double sin2Omega) const
{ 
  return (0.5*a2/(1. + 2*a2))*(sinOmega2 + cosOmega2/a2 
	+ tauLeptonPol_*cosTheta_*((sin2Omega/a)*tanTheta_ + cosOmega2/a2 - sinOmega2))*sinTheta_;      // [2], formula (32)
}

double SVfitVMlineShapeIntegrand::decayT(double a, double a2, double cosOmega2, double sinOmega2, double sin2Omega) const
{
  return (0.5*a2/(1. + 2*a2))*(1. + cosOmega2 + sinOmega2/a2
	+ tauLeptonPol_*cosTheta_*(sinOmega2/a2 - (sin2Omega/a)*tanTheta_ - cosOmega2 - 1.))*sinTheta_; // [2], formula (33)
}

double SVfitVMlineShapeIntegrand::DoEval(double mSquare) const
{
  //std::cout << "<SVfitVMlineShapeIntegrand::DoEval>:" << std::endl;  
  //std::cout << " mSquare = " << mSquare << std::endl;
  //std::cout << " vmType = " << vmType_ << std::endl;

//--- check that mass^2 passed as function argument is valid
  const double epsilon = 1.e-4;
  if ( !(mSquare > (minMass2_ + epsilon)) ) return 0.;

  double integrand = 0.;
  if ( mode_ == kVMlineShape ) {

    double a = TMath::Sqrt(mSquare)/tauLeptonMass;
    double a2 = a*a;
    double cosOmega = (1. - a2 + (1. + a2)*cosTheta_)/(1. + a2 + (1. - a2)*cosTheta_); // [1], formula (2.15)
    double cosOmega2 = cosOmega*cosOmega;
    double omega = TMath::ACos(cosOmega);
    double sinOmega = TMath::Sin(omega);
    double sinOmega2 = sinOmega*sinOmega;
    double sin2Omega = TMath::Sin(2*omega);

    if ( vmPol_ == kVMlongitudinalPol ) {
      if ( useCollApproxFormulas_ ) {
	//std::cout << " HL = " << HL(a, a2, cosOmega2, sinOmega2, sin2Omega) << std::endl;
	integrand = Fv(mSquare)*HL(a, a2, cosOmega2, sinOmega2, sin2Omega);
      } else {
	//std::cout << " decayL = " << decayL(a, a2, cosOmega2, sinOmega2, sin2Omega) << std::endl;
	integrand = Fv(mSquare)*decayL(a, a2, cosOmega2, sinOmega2, sin2Omega);
      }
    } else if ( vmPol_ == kVMtransversePol ) {
      if ( useCollApproxFormulas_ ) {
	//std::cout << " HT = " << HT(a, a2, cosOmega2, sinOmega2, sin2Omega) << std::endl;
	integrand = Fv(mSquare)*HT(a, a2, cosOmega2, sinOmega2, sin2Omega);
      } else {
	//std::cout << " decayT = " << decayT(a, a2, cosOmega2, sinOmega2, sin2Omega) << std::endl;
	integrand = Fv(mSquare)*decayT(a, a2, cosOmega2, sinOmega2, sin2Omega);
      }
    } else {
      edm::LogError ("SVfitVMlineShapeIntegrand::DoEval")
	<< " Invalid vector meson polarization = " << vmPol_ << " !!";
    }
  } else if ( mode_ == kVMnorm ) {
    integrand = Fv(mSquare);
  } else {
    edm::LogError ("SVfitVMlineShapeIntegrand::DoEval")
      << " Invalid mode = " << mode_ << " !!";
  }

  //std::cout << "--> integrand = " << integrand << std::endl;

  return integrand;
}  
