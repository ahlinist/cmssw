#ifndef TauAnalysis_CandidateTools_TauDecayKinePdf2_h
#define TauAnalysis_CandidateTools_TauDecayKinePdf2_h

/** \class TauDecayKinePdf2
 *
 * Class to parametrize probability density for tau leptons of given energy/Pt
 * to decay into systems of visible + invisible decay products (neutrinos produced in tau decay)
 * with a certain angle/eta-phi distance between the visible and invisible momenta.
 *
 * The probability density is modelled by a piece-wise continuous function, consisting of three pieces:
 *
 *  (1) Landau1(x, mp1, width1)                                          if x <= x1      with mp1 real, width1 > 0
 *  (2) C*exp(-0.5*((x-gmean)/gsigma)^2) + (1 - C)*(slope*(x - offset))) if x1 < x <= x2 with gmean > 0, gsigma > 0, C in [0..1]
 *  (3) Landau2(x, mp2, width2)                                          if x2 < x <= x3 with mp2 real, width2 > 0
 *  (4) Landau3(x, mp3, width3)                                          if      x >  x3 with mp3 real, width3 > 0
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: TauDecayKinePdf2.h,v 1.1 2011/05/27 11:56:20 veelken Exp $
 *
 */

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsData.h"
#include "TauAnalysis/CandidateTools/interface/RooAbsEstimatablePdf.h"

#include <ostream>

class TauDecayKinePdf2 : public RooAbsPdf
{
 public:
  // constructor(s)
  TauDecayKinePdf2();
  TauDecayKinePdf2(const char*, const char*,
		   RooAbsReal&,
		   RooAbsReal&, RooAbsReal&, 
		   RooAbsReal&, RooAbsReal&, RooAbsReal&, RooAbsReal&, RooAbsReal&, RooAbsReal&,
		   RooAbsReal&, RooAbsReal&, 
		   RooAbsReal&, RooAbsReal&,
		   RooAbsReal&, RooAbsReal&, RooAbsReal&);
  TauDecayKinePdf2(const TauDecayKinePdf2&, const char* = "");

  // destructor
  virtual ~TauDecayKinePdf2();

  virtual TObject* clone(const char* newName) const { return new TauDecayKinePdf2(*this, newName); }

  Int_t getAnalyticalIntegral(RooArgSet&, RooArgSet&, const char* = 0) const;
  Double_t analyticalIntegral(Int_t, const char* = 0) const;

  void disableAnalyticIntegration() { doAnalyticIntegration_ = false; }
  void enableAnalyticIntegration()  { doAnalyticIntegration_ = true;  }

  void print(std::ostream&) const;

 protected:
  Double_t evaluate() const;

  void updateParameters() const;

  Double_t evaluateLandau1(Double_t,  bool = true, bool = true) const;
  Double_t evaluateGaussian(Double_t, bool = true, bool = true) const;
  Double_t evaluateLandau2(Double_t , bool = true, bool = true) const;
  Double_t evaluateLandau3(Double_t,  bool = true, bool = true) const;

  void updateNormFactorGaussian() const;
  void updateNormFactorLandau2()  const;
  void updateNormFactorLandau3()  const;

  RooRealProxy x_;

  RooRealProxy mp1_;
  RooRealProxy width1_;
  RooRealProxy gmean_;
  RooRealProxy gsigma_;
  RooRealProxy alpha_;
  RooRealProxy slope_;
  RooRealProxy offset_;
  RooRealProxy C_;
  RooRealProxy mp2_;
  RooRealProxy width2_;
  RooRealProxy mp3_;
  RooRealProxy width3_;
  RooRealProxy x0_;
  RooRealProxy dx1_;
  RooRealProxy dx2_;

  mutable Double_t normGaussian_;
  mutable Double_t normLandau2_;
  mutable Double_t normLandau3_;

  mutable Double_t mp3Value_;
  mutable Double_t width3Value_;
  mutable Double_t x0Value_;
  mutable Double_t x1Value_;
  mutable Double_t x2Value_;

  bool doAnalyticIntegration_;

  mutable int verbosity_;

 private:
  // generate CInt dictionaries
  ClassDef(TauDecayKinePdf2, 1);
};

#endif

