#include "TauAnalysis/FittingTools/interface/PartonLuminosityIntegrand.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "LHAPDF/LHAPDF.h"

namespace LHAPDF {
  double xfx(int nset, double x, double Q, int fl);
}

#include <iostream>

PartonLuminosityIntegrand::PartonLuminosityIntegrand()
  : sqrtS_(0.),
    Q_(0.),
    tau_(0.),
    flavor1_(LHAPDF::GLUON),
    flavor2_(LHAPDF::GLUON)
{}

PartonLuminosityIntegrand::PartonLuminosityIntegrand(const PartonLuminosityIntegrand& bluePrint)
{
  sqrtS_ = bluePrint.sqrtS_;
  Q_ = bluePrint.Q_;
  tau_ = bluePrint.tau_;
  flavor1_ = bluePrint.flavor1_;
  flavor2_ = bluePrint.flavor2_;
}

PartonLuminosityIntegrand::~PartonLuminosityIntegrand()
{}

PartonLuminosityIntegrand& PartonLuminosityIntegrand::operator=(const PartonLuminosityIntegrand& bluePrint)
{
  sqrtS_ = bluePrint.sqrtS_;
  Q_ = bluePrint.Q_;
  tau_ = bluePrint.tau_;
  flavor1_ = bluePrint.flavor1_;
  flavor2_ = bluePrint.flavor2_;
  return (*this);
}

void PartonLuminosityIntegrand::SetParameterSqrtS(double sqrtS) 
{ 
  sqrtS_ = sqrtS;
}

void PartonLuminosityIntegrand::SetParameterQ(double Q) 
{ 
  Q_ = Q;
}

void PartonLuminosityIntegrand::SetParameterTau(double tau) 
{ 
  if ( !(tau > 0. && tau < 1.) ) {
    edm::LogError ("SetParameterTau") 
      << " Parameter tau = " << tau << " passed as function argument not within physical range 0..1 !!";
    return;
  }

  tau_ = tau;
}

void PartonLuminosityIntegrand::SetParameterFlavor1(int flavor1) 
{ 
  flavor1_ = flavor1;
}

void PartonLuminosityIntegrand::SetParameterFlavor2(int flavor2) 
{ 
  flavor2_ = flavor2;
}

double PartonLuminosityIntegrand::DoEval(double x) const
{
  if ( !(x > 0. && x < 1.) ) return 0.;

  double integrand = LHAPDF::xfx(0, x, Q_, flavor1_)*LHAPDF::xfx(0, tau_/x, Q_, flavor2_)/x;
  return integrand;
}  
