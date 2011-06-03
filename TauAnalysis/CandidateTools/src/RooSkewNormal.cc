/*
RooFit --- Copyright (c) 2000-2005, Regents of the University of California and Stanford University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "TauAnalysis/CandidateTools/interface/RooSkewNormal.h"
#include <memory>
#include "RooMath.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "TMath.h"
#include "TauAnalysis/CandidateTools/interface/owens.h"
#include "TH1F.h"

RooSkewNormal::RooSkewNormal(const char *name, const char *title,
			 RooAbsReal& _x, RooAbsReal& _location,
			 RooAbsReal& _scale, RooAbsReal& _skew) :
  RooAbsPdf(name,title),
  x("x","Observable",this,_x),
  location("location","Location",this,_location),
  scale("scale","Scale",this,_scale),
  skew("skew","Skew",this,_skew)
{
}

//_____________________________________________________________________________
RooSkewNormal::RooSkewNormal(const RooSkewNormal& other, const char* name) :
  RooAbsPdf(other,name), x("x",this,other.x),
  location("location",this,other.location), scale("scale",this,other.scale),
  skew("skew", this, other.skew)
{
}

//_____________________________________________________________________________
Double_t RooSkewNormal::evaluate() const
{
  Double_t arg = (x-location)/scale;

  Double_t gaussianPart = TMath::Gaus(arg, 0.0, 1.0, true);

  Double_t skewPart = 0.5*(1.0 + TMath::Erf(skew*arg/TMath::Sqrt2()));

  //cout << "gauss x = " << x << " location = " << location << " scale = " << scale << endl ;
  return (2.0/scale)*gaussianPart*skewPart;
}


//_____________________________________________________________________________
Int_t RooSkewNormal::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const
{
  if (matchArgs(allVars,analVars,x)) return 1 ;
  //if (matchArgs(allVars,analVars,location)) return 2 ;
  return 0 ;
}

namespace {
  inline Double_t cdfWrtX(Double_t x, Double_t location, Double_t scale,
      Double_t skew) {
    Double_t arg = (x-location)/scale;
    double gaussianCDF = 0.5*(1.0 + TMath::Erf(arg/TMath::Sqrt2()));
    return gaussianCDF - 2*t(arg, skew);
  }
}

//_____________________________________________________________________________
Double_t RooSkewNormal::analyticalIntegral(Int_t code, const char* rangeName) const
{
  //assert(code==1 || code==2) ;
  assert(code==1) ;

  Double_t maxArg = (x.max(rangeName) - location)/scale;
  Double_t minArg = (x.min(rangeName) - location)/scale;

  Double_t ret = 0;
  if(code==1){
    ret = cdfWrtX(maxArg, location, scale, skew) - cdfWrtX(minArg, location, scale, skew);
    //cout << "Int_gauss_dx(location=" << location << ",scale=" << scale << ", xmin=" << x.min(rangeName) << ", xmax=" << x.max(rangeName) << ")=" << ret << endl ;
  } else if(code==2) {
    // To be implemented when calculus doesn't fail me.
    ret = 0.0;
  } else{
    cout << "Error in RooSkewNormal::analyticalIntegral" << endl;
  }
  return ret ;

}

RooArgSet RooSkewNormal::estimateParameters(RooAbsData& data, double errorFactor) {

  std::auto_ptr<TH1> histo(data.createHistogram(x.arg().GetName()));
  TH1* histogram = histo.get();
  assert(histogram);

  std::cout << "<" << this->GetName() << "::estimateParameters("
    << data.GetName() << "[" << x.arg().GetName() << "])>" << std::endl;

  double sampleSkew = histogram->GetSkewness();
  std::cout << " - initial sample skew is " << sampleSkew << std::endl;

  if (TMath::Abs(sampleSkew) > 0.80) {
    sampleSkew = 0.80*sampleSkew/TMath::Abs(sampleSkew);
  }
  double sampleSigma = histogram->GetRMS();
  double sampleMean = histogram->GetMean();

  std::cout << " - corrected sample skew is " << sampleSkew << std::endl;
  std::cout << " - sample mean is " << sampleMean << std::endl;
  std::cout << " - sample sigam is " << sampleSigma << std::endl;

  // c.f. http://en.wikipedia.org/wiki/Skew_normal_distribution#Estimation
  double thirdMomentToTwoThirds = TMath::Power(TMath::Abs(sampleSkew), 2.0/3.0);

  double absDelta = TMath::Sqrt(
      TMath::Pi()*0.5*(thirdMomentToTwoThirds)/(
        thirdMomentToTwoThirds + TMath::Power((4 - TMath::Pi())/2.0, 2.0/3.0)));

  double delta = absDelta*sampleSkew/TMath::Abs(sampleSkew);

  double estSkew = delta/TMath::Sqrt(1- delta*delta);

  double estScale = sampleSigma/TMath::Sqrt(1 - 2*delta*delta/TMath::Pi());

  double estLocation = sampleMean - estScale*delta*TMath::Sqrt(2/TMath::Pi());

  location = estLocation;
  scale = estScale;
  skew = estSkew;

  std::cout << "location: " << location << std::endl;
  std::cout << "scale: " << scale << std::endl;
  std::cout << "skew: " << skew << std::endl;

  RooArgSet output;
  output.add(location.arg());
  output.add(scale.arg());
  output.add(skew.arg());

  return output;
}

ClassImp(RooSkewNormal)
