#include "TauAnalysis/CandidateTools/interface/RooSmearedIsotropicDecayPdf.h"
#include <cmath>
#include "TMath.h"

namespace {

#define E (TMath::E())
#define Pi (TMath::Pi())
#define Power(x, n) (std::pow((x), (n)))
#define Sqrt(x) (TMath::Sqrt((x)))
#define Erf(x) (TMath::Erf((x)))

// Copied from Mathematic CForm[...] output
inline Double_t smearedIsoDecayImpl(Double_t x, Double_t a, Double_t s) {
  return (-(s*(2*Power(E,(a*x)/Power(s,2))*s*(8*Power(a,2) + 4*a*(Power(a,2) + 2*Power(s,2) + a*x + Power(x,2)) +
            3*(Power(a,4) + 8*Power(s,4) + Power(a,3)*x + Power(a,2)*Power(x,2) + a*Power(x,3) +
              Power(x,4) + Power(s,2)*(4*Power(a,2) + 7*a*x + 9*Power(x,2)))) -
          Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2)))*Sqrt(2*Pi)*x*
          (8*Power(a,2) + 4*a*(3*Power(s,2) + Power(x,2)) +
           3*(15*Power(s,4) + 10*Power(s,2)*Power(x,2) + Power(x,4)))*Erf((a - x)/(Sqrt(2)*s))))/
      (2.*Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2)))) +
      (s*(2*s*(8*Power(a,2) + 4*a*(2*Power(s,2) + Power(x,2)) +
               3*(8*Power(s,4) + 9*Power(s,2)*Power(x,2) + Power(x,4))) +
          Power(E,Power(x,2)/(2.*Power(s,2)))*Sqrt(2*Pi)*x*
          (8*Power(a,2) + 4*a*(3*Power(s,2) + Power(x,2)) +
           3*(15*Power(s,4) + 10*Power(s,2)*Power(x,2) + Power(x,4)))*Erf(x/(Sqrt(2)*s))))/
      (2.*Power(E,Power(x,2)/(2.*Power(s,2)))))/(8.*Power(a,2.5)*Sqrt(2*Pi)*s);
}

// Inline this so the compiler can optimize it.
inline Double_t smearedIsoDecayCDFXImpl(Double_t x, Double_t a, Double_t s) {
  return ((s*((s*x*(8*Power(a,2) + 10*a*Power(s,2) + 33*Power(s,4) + 2*(a + 7*Power(s,2))*Power(x,2) + Power(x,4)))/
       Power(E,Power(x,2)/(2.*Power(s,2))) +
       Sqrt(Pi/2.)*(15*Power(s,6) + 45*Power(s,4)*Power(x,2) + 15*Power(s,2)*Power(x,4) + Power(x,6) +
         8*Power(a,2)*(Power(s,2) + Power(x,2)) +
         2*a*(3*Power(s,4) + 6*Power(s,2)*Power(x,2) + Power(x,4)))*Erf(x/(Sqrt(2)*s))))/2. -
   (s*((-2*Power(s,2)*(-(a*s*(8*a + 12*Power(a,2) + 69*Power(s,2))) -
                       Power(E,Power(a,2)/(2.*Power(s,2)))*Sqrt(2*Pi)*
                       (4*Power(a,3) + 6*Power(a,4) + 2*a*Power(s,2) + 39*Power(a,2)*Power(s,2) + 18*Power(s,4))*
                       Erf(a/(Sqrt(2)*s))))/Power(E,Power(a,2)/(2.*Power(s,2))) -
       (2*a*s*(2*Power(a,3) + Power(a,4) + 22*a*Power(s,2) + 153*Power(s,4) +
               Power(a,2)*(8 + 29*Power(s,2))) +
        Power(E,Power(a,2)/(2.*Power(s,2)))*Sqrt(2*Pi)*
        (2*Power(a,5) + Power(a,6) + 16*Power(a,3)*Power(s,2) + 2*a*Power(s,4) + 57*Power(s,6) +
         8*Power(a,4)*(1 + 3*Power(s,2)) + 4*Power(a,2)*Power(s,2)*(-2 + 39*Power(s,2)))*
        Erf(a/(Sqrt(2)*s)))/(2.*Power(E,Power(a,2)/(2.*Power(s,2)))) +
       (2*Power(s,2)*(-(Power(E,(a*x)/Power(s,2))*s*
                        (12*Power(a,3) + Power(a,2)*(8 + 9*x) + 3*x*(12*Power(s,2) + Power(x,2)) +
                         a*(69*Power(s,2) + 2*x*(2 + 3*x)))) +
                      Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2)))*Sqrt(2*Pi)*
                      (4*Power(a,3) + 6*Power(a,4) + 2*a*Power(s,2) + 39*Power(a,2)*Power(s,2) + 18*Power(s,4))*
                      Erf((-a + x)/(Sqrt(2)*s))))/Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2))) +
       (2*Power(E,(a*x)/Power(s,2))*s*(Power(a,5) + 105*Power(s,4)*x + 20*Power(s,2)*Power(x,3) +
                                       Power(x,5) + Power(a,4)*(2 + x) + Power(a,3)*(8 + 29*Power(s,2) + 2*x + Power(x,2)) +
                                       a*(153*Power(s,4) + Power(x,3)*(2 + x) + 6*Power(s,2)*x*(3 + 4*x)) +
                                       Power(a,2)*(Power(s,2)*(22 + 27*x) + x*(8 + 2*x + Power(x,2)))) -
        Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2)))*Sqrt(2*Pi)*Power(x,2)*
        (8*Power(a,2) + 45*Power(s,4) + 15*Power(s,2)*Power(x,2) + Power(x,4) +
         2*a*(6*Power(s,2) + Power(x,2)))*Erf((a - x)/(Sqrt(2)*s)) -
        Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2)))*Sqrt(2*Pi)*
        (2*Power(a,5) + Power(a,6) + 16*Power(a,3)*Power(s,2) + 2*a*Power(s,4) + 57*Power(s,6) +
         8*Power(a,4)*(1 + 3*Power(s,2)) + 4*Power(a,2)*Power(s,2)*(-2 + 39*Power(s,2)))*
        Erf((-a + x)/(Sqrt(2)*s)))/(2.*Power(E,(Power(a,2) + Power(x,2))/(2.*Power(s,2))))))/2.)/
        (8.*Power(a,2.5)*Sqrt(2*Pi)*s);
}
} // end anonymous namespace

RooSmearedIsotropicDecayPdf::RooSmearedIsotropicDecayPdf(
    const char *name, const char *title,
    RooAbsReal& _x, RooAbsReal& _smear, RooAbsReal& _location):
  RooAbsPdf(name, title),
  x("x", "Observable", this, _x),
  smear("smear", "Smear", this, _smear),
  location("location", "Location", this, _location)
{}

// Copy constructor
RooSmearedIsotropicDecayPdf::RooSmearedIsotropicDecayPdf(
    const RooSmearedIsotropicDecayPdf& other, const char *name):
  RooAbsPdf(other, name),
  x("x", this, other.x),
  smear("smear", this, other.smear),
  location("location", this, other.location) {}

Double_t RooSmearedIsotropicDecayPdf::evaluate() const {
  Double_t s = smear;
  Double_t xVal = x;
  Double_t a = location;
  if (x < 0)
    return 0;
  return smearedIsoDecayImpl(xVal, a, s);
}

Int_t RooSmearedIsotropicDecayPdf::getAnalyticalIntegral(
    RooArgSet& allVars, RooArgSet& analVars, const char *rangeName) const {
  if (matchArgs(allVars, analVars, x)) {
    return 1;
  }
  return 0;
}

Double_t RooSmearedIsotropicDecayPdf::analyticalIntegral(Int_t code,
    const char *rangeName) const {
  if (code == 1) {
    //Integrate w.r.t scaled decay Pt
    Double_t max = x.max(rangeName);
    Double_t min = x.min(rangeName);
    if (min < 0)
      min = 0;
    if (max < 0)
      max = 0;
    return smearedIsoDecayCDFXImpl(max, location, smear) -
      smearedIsoDecayCDFXImpl(min, location, smear);
  }
  assert(false);
  return 0;
}
