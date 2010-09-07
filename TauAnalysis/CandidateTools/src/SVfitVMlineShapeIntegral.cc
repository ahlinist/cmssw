#include "TauAnalysis/CandidateTools/interface/SVfitVMlineShapeIntegral.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

using namespace SVfit_namespace;

SVfitVMlineShapeIntegral::SVfitVMlineShapeIntegral(SVfitVMlineShapeIntegrand::VMtype vmType, 
						   SVfitVMlineShapeIntegrand::VMpol vmPol, bool useCollApproxFormulas)
  : integrand_(useCollApproxFormulas)
{
  integrator_ = new ROOT::Math::Integrator(integrand_);

  integrand_.SetVMtype(vmType);
  integrand_.SetVMpol(vmPol);
  integrand_.SetMode(SVfitVMlineShapeIntegrand::kVMnorm);
//--- CV: need to trigger update of ROOT::Math::Integrator by calling integrator->SetFunction
//        after calling any non-const function of SVfitVMlineShapeIntegrand 
  integrator_->SetFunction(integrand_);

//--- compute lower limit for normalization integral
//   = invariant mass of n-pion system
//   
//    CV: difference in mass between charged and neutral pions is ignored
//
  double numPions = 0.;
  if      ( vmType == SVfitVMlineShapeIntegrand::kVMrho ) numPions = 2;
  else if ( vmType == SVfitVMlineShapeIntegrand::kVMa1  ) numPions = 3;
  else {
    edm::LogError ("SVfitVMlineShapeIntegrand::update")
      << " Invalid vecor meson type = " << vmType << " !!";
  }
  minMass2_ = square(numPions*chargedPionMass);
  std::cout << " minMass2 = " << minMass2_ << std::endl;
  norm_ = integrator_->Integral(minMass2_, tauLeptonMass2); 
  std::cout << " norm = " << norm_ << std::endl;

//--- set integrand to compute vector meson line-shape integrals in the following...  
  integrand_.SetMode(SVfitVMlineShapeIntegrand::kVMlineShape);
  integrator_->SetFunction(integrand_);
}

SVfitVMlineShapeIntegral::~SVfitVMlineShapeIntegral()
{
  delete integrator_;
}

double SVfitVMlineShapeIntegral::operator()(double theta, double tauLeptonPol, double z) const
{
  integrand_.SetParameterTheta(theta);
  integrand_.SetParameterTauLeptonPol(tauLeptonPol);

  integrator_->SetFunction(integrand_);

  double integral = integrator_->Integral(minMass2_, z*tauLeptonMass2)/norm_;
  if ( debugLevel_ > 0 ) {
    edm::LogVerbatim ("SVfitVMlineShapeIntegral::operator()")
      << " theta = " << theta << std::endl
      << " tauLeptonPol = " << tauLeptonPol << std::endl
      << " z = " << z << std::endl
      << "--> integral = " << integral << std::endl;
  }

  return integral;
}  
