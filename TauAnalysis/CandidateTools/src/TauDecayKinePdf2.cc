#include "TauAnalysis/CandidateTools/interface/TauDecayKinePdf2.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/owens.h"

#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFuncMathCore.h>

#include <TMath.h>

#include <string>
#include <iomanip>

TauDecayKinePdf2::TauDecayKinePdf2()
  : RooAbsPdf(),
    doAnalyticIntegration_(true),
    verbosity_(0)
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::TauDecayKinePdf2(1)>:" << std::endl;
}

TauDecayKinePdf2::TauDecayKinePdf2(
  const char* name, const char* title,
  RooAbsReal& x,
  RooAbsReal& mp1, RooAbsReal& width1, 
  RooAbsReal& gmean, RooAbsReal& gsigma, RooAbsReal& alpha, RooAbsReal& slope, RooAbsReal& offset,  RooAbsReal& C,
  RooAbsReal& mp2, RooAbsReal& width2, 
  RooAbsReal& mp3, RooAbsReal& width3,
  RooAbsReal& x0, RooAbsReal& dx1, RooAbsReal& dx2)
  : RooAbsPdf(name, title),
    x_("x", "x", this, x),
    mp1_("mp1", "mp1", this, mp1),
    width1_("width1", "width1", this, width1),
    gmean_("gmean", "gmean", this, gmean),
    gsigma_("gsigma", "gsigma", this, gsigma),
    alpha_("alpha", "alpha", this, alpha),
    slope_("slope", "slope", this, slope),
    offset_("offset", "offset", this, offset),
    C_("C", "C", this, C),
    mp2_("mp2", "mp2", this, mp2),
    width2_("width2", "width2", this, width2),
    mp3_("mp3", "mp3", this, mp3),
    width3_("width3", "width3", this, width3),
    x0_("x0", "x0", this, x0),
    dx1_("dx1", "dx1", this, dx1),
    dx2_("dx2", "dx2", this, dx2),
    doAnalyticIntegration_(true),
    verbosity_(0)
{
  if ( this->verbosity_ ) {
    std::cout << "<TauDecayKinePdf2::TauDecayKinePdf2(2)>:" << std::endl;
    this->print(std::cout);
  }
}

TauDecayKinePdf2::TauDecayKinePdf2(const TauDecayKinePdf2& bluePrint, const char* newName)
  : RooAbsPdf(bluePrint, newName),
    x_("x", this, bluePrint.x_),
    mp1_("mp1", this, bluePrint.mp1_),
    width1_("width1", this, bluePrint.width1_),
    gmean_("gmean", this, bluePrint.gmean_),
    gsigma_("gsigma", this, bluePrint.gsigma_),
    alpha_("alpha", this, bluePrint.alpha_),
    slope_("slope", this, bluePrint.slope_),
    offset_("offset", this, bluePrint.offset_),
    C_("C", this, bluePrint.C_),
    mp2_("mp2", this, bluePrint.mp2_),
    width2_("width2", this, bluePrint.width2_),
    mp3_("mp3", this, bluePrint.mp3_),
    width3_("width3", this, bluePrint.width3_),
    x0_("x0", this, bluePrint.x0_),
    dx1_("dx1", this, bluePrint.dx1_),
    dx2_("dx2", this, bluePrint.dx2_),
    doAnalyticIntegration_(bluePrint.doAnalyticIntegration_),
    verbosity_(0)
{
  if ( this->verbosity_ ) {
    std::cout << "<TauDecayKinePdf2::TauDecayKinePdf2(3)>:" << std::endl;
    this->print(std::cout);
  }
}

TauDecayKinePdf2::~TauDecayKinePdf2()
{
//--- nothing to be done yet...
}

Double_t TauDecayKinePdf2::evaluate() const
{
  if ( this->verbosity_ ) {
    std::cout << "<TauDecayKinePdf2::evaluate>:" << std::endl;
    this->print(std::cout);
  }

  updateParameters();
 
  Double_t retVal = 0.;
  if      ( x_ < x0Value_ ) retVal = evaluateLandau1(x_);
  else if ( x_ < x1Value_ ) retVal = evaluateGaussian(x_);
  else if ( x_ < x2Value_ ) retVal = evaluateLandau2(x_);
  else                      retVal = evaluateLandau3(x_);
  
  const double epsilon = 1.e-12;
  if ( retVal < epsilon || TMath::IsNaN(retVal) ) retVal = epsilon;

  return retVal;
}

void TauDecayKinePdf2::updateParameters() const
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::updateParameters>:" << std::endl;

  x0Value_ = TMath::Max(x0_,  0.);
  x1Value_ = x0Value_ + TMath::Max(dx1_, 0.);
  x2Value_ = x1Value_ + TMath::Max(dx2_, 0.);

  //-----------------------------------------------------------------------------
  // CV: "hack" to resolve ambiguity between parameters
  //     of second and third Landau distribution in case x1 + dx2 > 11.5
  //
  //    (third Landau distribution models tails at large x,
  //     where event statistics is low and width not well constrained)
  //
  //    --> take width from fit of second Landau distribution instead
  //
  if ( dx2_ < 1.e-6 ) {
    mp3Value_    = mp2_;
    width3Value_ = width2_;
  } else {
    mp3Value_    = mp3_;
    width3Value_ = width3_;
  }
  //-----------------------------------------------------------------------------

  if ( this->verbosity_  ) {
    std::cout << " x0Value     = " << x0Value_     << std::endl;
    std::cout << " x1Value     = " << x1Value_     << std::endl;
    std::cout << " x2Value     = " << x2Value_     << std::endl;
    std::cout << " mp3Value    = " << mp3Value_    << std::endl;
    std::cout << " width3Value = " << width3Value_ << std::endl;
  }
}

Double_t TauDecayKinePdf2::evaluateLandau1(Double_t x, bool inclNormFactor, bool updateNormFactor) const
{
  double landau1 = ( width1_ > 0. ) ? ::ROOT::Math::landau_pdf((x - mp1_)/width1_) : 0.;

  if ( this->verbosity_ ) std::cout << "--> returning Landau1 = " << landau1 << std::endl;
  return landau1;
}

Double_t TauDecayKinePdf2::evaluateGaussian(Double_t x, bool inclNormFactor, bool updateNormFactor) const
{
  double gaussian = 0.;
  if ( gsigma_ > 0. ) {
    Double_t pull = (x - gmean_)/gsigma_;
    gaussian += C_*(1./(TMath::Sqrt(2.*TMath::Pi())*gsigma_))*TMath::Exp(-0.5*pull*pull)*(1. + TMath::Erf(alpha_*pull));
  }

  gaussian += (1. - C_)*TMath::Max(0., slope_*(x - offset_));

//--- normalize Gaussian such that
//      C*exp(-0.5*((x - gmean)/2sigma)^2) + (1 - C)*(slope*x + offset)) = Landau1(x, mp1, width1)
//    at x = x0
  if ( inclNormFactor ) {
    if ( updateNormFactor ) updateNormFactorGaussian();
    gaussian *= normGaussian_;
  }

  if ( this->verbosity_ ) std::cout << "--> returning Gaussian = " << gaussian << std::endl;
  return gaussian;
}

Double_t TauDecayKinePdf2::evaluateLandau2(Double_t x, bool inclNormFactor, bool updateNormFactor) const
{
  double landau2 = ( width2_ > 0. ) ? ::ROOT::Math::landau_pdf((x - mp2_)/width2_) : 0.;

//--- normalize Landau2 such that
//      Landau2(x, mp2, width2) = C*exp(-0.5*((x - gmean)/2sigma)^2) + (1 - C)*(slope*x + offset)) 
//    at x = x1
  if ( inclNormFactor ) {
    if ( updateNormFactor ) {
      updateNormFactorGaussian();
      updateNormFactorLandau2();
    }
    landau2 *= normLandau2_;
  }

  if ( this->verbosity_ ) std::cout << "--> returning Landau2 = " << landau2 << std::endl;
  return landau2;
}

Double_t TauDecayKinePdf2::evaluateLandau3(Double_t x, bool inclNormFactor, bool updateNormFactor) const
{
  double landau3 = ( width3Value_ > 0. ) ? ::ROOT::Math::landau_pdf((x - mp3Value_)/width3Value_) : 0.;

//--- normalize exponential such that
//       Landau3(x, mp3, width3) = Landau2(x, mp2, width2)
//    at x = x2
  if ( inclNormFactor ) {
    if ( updateNormFactor ) {
      updateNormFactorGaussian();
      updateNormFactorLandau2();
      updateNormFactorLandau3();
    }
    landau3 *= normLandau3_;
  }

  if ( this->verbosity_ ) std::cout << "--> returning Landau3 = " << landau3 << std::endl;
  return landau3;
}

void TauDecayKinePdf2::updateNormFactorGaussian() const
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::updateNormFactorGaussian>:" << std::endl;

  double ref_at_x0 = 0.;
  bool ref_exists = false;
  if ( x0Value_ > 0. ) {
    ref_at_x0 = evaluateLandau1(x0Value_,  true,  false);
    ref_exists = true;
  }

  if ( ref_exists ) {
    double gaussian_at_x0 = evaluateGaussian(x0Value_, false, false);
    normGaussian_ = ( gaussian_at_x0 > 0. ) ? (ref_at_x0/gaussian_at_x0) : 0.;
  } else {
    normGaussian_ = 1.;
  }

  if ( this->verbosity_ ) std::cout << "--> setting normGaussian = " << normGaussian_ << std::endl;
}

void TauDecayKinePdf2::updateNormFactorLandau2() const
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::updateNormFactorLandau2>:" << std::endl;

  double ref_at_x1 = 0.;
  bool ref_exists = false;
  if ( x1Value_ > x0Value_ ) {
    ref_at_x1 = evaluateGaussian(x1Value_, true, false);
    ref_exists = true;
  } else if ( x0Value_ > 0. ) {
    ref_at_x1 = evaluateLandau1(x1Value_, true, false);
    ref_exists = true;
  }

  if ( ref_exists ) {
    double landau2_at_x1  = evaluateLandau2(x1Value_, false, false);
    normLandau2_ = ( landau2_at_x1 > 0. ) ? (ref_at_x1/landau2_at_x1) : 0.;
  } else {
    normLandau2_ = 1.;
  }

  if ( this->verbosity_ ) std::cout << "--> setting normLandau2 = " << normLandau2_ << std::endl;
}

void TauDecayKinePdf2::updateNormFactorLandau3() const
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::updateNormFactorLandau3>:" << std::endl;

  double ref_at_x2 = 0.;
  bool ref_exists = false;
  if ( x2Value_ > x1Value_ ) {
    ref_at_x2 = evaluateLandau2(x2Value_, true, false);
    ref_exists = true;
  } else if ( x1Value_ > x0Value_ ) {
    ref_at_x2 = evaluateGaussian(x2Value_, true, false);
    ref_exists = true;
  } else if ( x0Value_ > 0. ) {
    ref_at_x2 = evaluateLandau1(x2Value_, true, false);
    ref_exists = true;
  }
  
  if ( ref_exists ) {
    double landau3_at_x2 = evaluateLandau3(x2Value_, false, false);  
    normLandau3_ = ( landau3_at_x2 > 0. ) ? (ref_at_x2/landau3_at_x2) : 0.;
  } else {
    normLandau3_ = 1.;
  }

  if ( this->verbosity_ ) std::cout << "--> setting normLandau3 = " << normLandau3_ << std::endl;
}

//
//-------------------------------------------------------------------------------
//

Int_t TauDecayKinePdf2::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char*) const
{
  if ( doAnalyticIntegration_ && matchArgs(allVars, analVars, x_) ) return 1;
  return 0 ;
}

Double_t TauDecayKinePdf2::analyticalIntegral(Int_t code, const char* rangeName) const
{
  if ( this->verbosity_ ) std::cout << "<TauDecayKinePdf2::analyticalIntegral>:" << std::endl;

  assert(code == 1);

  Double_t retVal = 0.;

  if ( code == 1 ) {
    updateParameters();

    updateNormFactorGaussian();
    updateNormFactorLandau2();
    updateNormFactorLandau3();

    double xMin = x_.min(rangeName);
    double xMax = x_.max(rangeName);

    //---------------------------------------------------------------------------
    // compute contribution to integral of Landau1
    double landau1_integral = 0.;

    double xMin_landau1 = xMin;
    double xMax_landau1 = TMath::Min(xMax, x0Value_);

    if ( xMax_landau1 > xMin_landau1 ) {
      if ( width1_ > 0. ) {
	landau1_integral +=
	  width1_
	 *(::ROOT::Math::landau_cdf((xMax_landau1 - mp1_)/width1_) - ::ROOT::Math::landau_cdf((xMin_landau1 - mp1_)/width1_));
      }
    }

    if ( this->verbosity_ ) std::cout << "--> Landau1_integral = " << landau1_integral << std::endl;
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    // compute contribution to integral of Gaussian
    double gaussian_integral = 0.;

    double xMin_gaussian = TMath::Max(xMin, x0Value_);
    double xMax_gaussian = TMath::Min(xMax, x1Value_);

    if ( xMax_gaussian > xMin_gaussian ) {
      if ( gsigma_ > 0. ) {
	Double_t sqrt2_times_sigma = TMath::Sqrt(2.)*gsigma_;
	double cdfHi = 0.5*(1. + TMath::Erf((xMax_gaussian - gmean_)/sqrt2_times_sigma));
	if ( alpha_ != 0. ) cdfHi -= 2.*t((xMax_gaussian - gmean_)/gsigma_, alpha_);
	double cdfLo = 0.5*(1. + TMath::Erf((xMin_gaussian - gmean_)/sqrt2_times_sigma));
	if ( alpha_ != 0. ) cdfLo -= 2.*t((xMin_gaussian - gmean_)/gsigma_, alpha_);
	gaussian_integral += C_*(cdfHi - cdfLo);
      }

      double xMin_linear = xMin_gaussian;
      double xMax_linear = xMax_gaussian;
      if ( slope_ > 0. ) xMin_linear = TMath::Max(xMin_gaussian, offset_);
      if ( slope_ < 0. ) xMax_linear = TMath::Min(xMax_gaussian, offset_);
      if ( xMax_linear > xMin_linear ) {
	gaussian_integral += (1. - C_)
	                    *(0.5*slope_*(xMax_linear*xMax_linear - xMin_linear*xMin_linear) 
                             - slope_*offset_*(xMax_linear - xMin_linear));
      }

      gaussian_integral *= normGaussian_;
    }

    if ( this->verbosity_ ) std::cout << "--> Gaussian_integral = " << gaussian_integral << std::endl;
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    // compute contribution to integral of Landau2
    double landau2_integral = 0.;

    double xMin_landau2 = TMath::Max(xMin, x1Value_);
    double xMax_landau2 = TMath::Min(xMax, x2Value_);

    if ( xMax_landau2 > xMin_landau2 ) {
      if ( width2_ > 0. ) {
	landau2_integral +=
	  normLandau2_*width2_
	 *(::ROOT::Math::landau_cdf((xMax_landau2 - mp2_)/width2_) - ::ROOT::Math::landau_cdf((xMin_landau2 - mp2_)/width2_));
      }
    }

    if ( this->verbosity_ ) std::cout << "--> Landau2_integral = " << landau2_integral << std::endl;
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    // compute contribution to integral of Landau3
    double landau3_integral = 0.;

    double xMin_landau3 = TMath::Max(xMin, x2Value_);
    double xMax_landau3 = xMax;

    if ( xMax_landau3 > xMin_landau3 ) {
      if ( width3Value_ > 0. ) {
        landau3_integral +=
          normLandau3_*width3Value_
         *(::ROOT::Math::landau_cdf((xMax_landau3 - mp3Value_)/width3Value_)
          - ::ROOT::Math::landau_cdf((xMin_landau3 - mp3Value_)/width3Value_));
      }
    }

    if ( this->verbosity_ ) std::cout << "--> Landau3_integral = " << landau3_integral << std::endl;
    //---------------------------------------------------------------------------

    retVal = landau1_integral + gaussian_integral + landau2_integral + landau3_integral;

    if ( retVal < 1.e-6 ) retVal = 1.e-6; 
    //if ( retVal < 1.e-6 ) {
    //  edm::LogWarning ("<TauDecayKinePdf2::analyticalIntegral>:")
    //	  << " Return value = " << retVal << " !!";
    //  print(std::cerr);
    //  bool verbosity_backup = this->verbosity_;
    //  this->verbosity_ = true;
    //  updateParameters();
    //  updateNormFactorGaussian();
    //  updateNormFactorLandau2();
    //  updateNormFactorLandau3();
    //  this->verbosity_ = verbosity_backup;
    //  std::cerr << "xMin = " << xMin << ", xMax = " << xMax << ":" << std::endl;
    //  std::cerr << " Landau1_integral  = " << landau1_integral  << std::endl;
    //  std::cerr << " Gaussian_integral = " << gaussian_integral << std::endl;
    //  std::cerr << " Landau2_integral  = " << landau2_integral  << std::endl;
    //  std::cerr << " Landau3_integral  = " << landau3_integral  << std::endl;
    //}
  }

  return retVal;
}

//
//-------------------------------------------------------------------------------
//

void TauDecayKinePdf2::print(std::ostream& stream) const
{
  stream << "<TauDecayKinePdf2::print>:" << std::endl;
  stream << " x: name = " << x_.absArg()->GetName() << ", value = " << x_ << std::endl;
  stream << " mp1: name = " << mp1_.absArg()->GetName() << ", value = " << mp1_ << std::endl;
  stream << " width1: name = " << width1_.absArg()->GetName() << ", value = " << width1_ << std::endl;
  stream << " gmean: name = " << gmean_.absArg()->GetName() << ", value = " << gmean_ << std::endl;
  stream << " gsigma: name = " << gsigma_.absArg()->GetName() << ", value = " << gsigma_ << std::endl;
  stream << " alpha: name = " << alpha_.absArg()->GetName() << ", value = " << alpha_ << std::endl;
  stream << " slope: name = " << slope_.absArg()->GetName() << ", value = " << slope_ << std::endl;
  stream << " offset: name = " << offset_.absArg()->GetName() << ", value = " << offset_ << std::endl;
  stream << " C: name = " << C_.absArg()->GetName() << ", value = " << C_ << std::endl;
  stream << " mp2: name = " << mp2_.absArg()->GetName() << ", value = " << mp2_ << std::endl;
  stream << " width2: name = " << width2_.absArg()->GetName() << ", value = " << width2_ << std::endl;
  stream << " mp3: name = " << mp3_.absArg()->GetName() << ", value = " << mp3_ << std::endl;
  stream << " width3: name = " << width3_.absArg()->GetName() << ", value = " << width3_ << std::endl;
  stream << " x0: name = " << x0_.absArg()->GetName() << ", value = " << x0_ << std::endl;
  stream << " dx1: name = " << dx1_.absArg()->GetName() << ", value = " << dx1_ << std::endl;
  stream << " dx2: name = " << dx2_.absArg()->GetName() << ", value = " << dx2_ << std::endl;
  stream << "(normGaussian = " << normGaussian_ << ", normLandau2 = " << normLandau2_ << ","
	 << " normLandau3 = " << normLandau3_ << ")" << std::endl;
}

// generate CInt dictionaries
ClassImp(TauDecayKinePdf2)

