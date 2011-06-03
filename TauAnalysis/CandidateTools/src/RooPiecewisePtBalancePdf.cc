#include "TauAnalysis/CandidateTools/interface/RooPiecewisePtBalancePdf.h"
#include "TauAnalysis/CandidateTools/interface/owens.h"

#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFuncMathCore.h>

#include "TMath.h"
#include "TH1.h"
#include "TF1.h"

namespace {

double compEdgePos(bool falling, TH1* histogram,
    double& errEdgePosRight, double& errEdgePosLeft)
{
  std::cout << "<compFallingEdgePos>:" << std::endl;

  int numBins = histogram->GetNbinsX();

  std::vector<double> window5derrivatives(numBins);

  for ( int iBin = 3; iBin <= (numBins - 2); ++iBin ) {
    double diffBinContent = histogram->GetBinContent(iBin + 2) - histogram->GetBinContent(iBin - 2);
    double diffBinCenter  = histogram->GetBinCenter(iBin + 2) - histogram->GetBinCenter(iBin - 2);

    window5derrivatives[iBin] = diffBinContent/diffBinCenter;
  }

  double minWindow5derrivative = 0;
  int binFallingEdge = -1.;
  double fallindEdgePos = -1.;
  for ( int iBin = 3; iBin <= (numBins - 2); ++iBin ) {
    if ( (falling && window5derrivatives[iBin] < minWindow5derrivative) ||
         (!falling && window5derrivatives[iBin] > minWindow5derrivative) ) {
      binFallingEdge = iBin;
      fallindEdgePos = histogram->GetBinCenter(iBin);
      minWindow5derrivative = window5derrivatives[iBin];
    }
  }

  //binFallingEdge -= 2; // CV: "phenomenological" correction...

  if ( binFallingEdge > 0 ) {
    errEdgePosRight = TMath::Abs(histogram->GetBinContent(binFallingEdge)/minWindow5derrivative);
    std::cout << " errEdgePosRight = " << errEdgePosRight << std::endl;

    double histogramMax_y = histogram->GetBinContent(histogram->GetMaximumBin());
    errEdgePosLeft  = TMath::Abs((histogramMax_y - histogram->GetBinContent(binFallingEdge))/minWindow5derrivative);
    std::cout << " errEdgePosLeft = " << errEdgePosLeft << std::endl;
  }

  return fallindEdgePos;
}

double findHalfMaxPointRight(TH1* histogram) {
  double histogramMax_bin = histogram->GetMaximumBin();
  double histogramMax_y = histogram->GetBinContent(histogram->GetMaximumBin());
  for (int iBin = histogramMax_bin+1; iBin < histogram->GetNbinsX(); ++iBin) {
    if (histogram->GetBinContent(iBin) <= 0.5*histogramMax_y) {
      return histogram->GetBinCenter(iBin);
    }
  }
  return -1;
}

double findHalfMaxPointLeft(TH1* histogram) {
  double histogramMax_bin = histogram->GetMaximumBin();
  double histogramMax_y = histogram->GetBinContent(histogram->GetMaximumBin());
  for (int iBin = histogramMax_bin-1; iBin > 0; --iBin) {
    if (histogram->GetBinContent(iBin) <= 0.5*histogramMax_y) {
      return histogram->GetBinCenter(iBin);
    }
  }
  return -1;
}

}

RooPiecewisePtBalancePdf::RooPiecewisePtBalancePdf(
    const char *name, const char *title,
    RooAbsReal& _x,
    // Skew normal turn on
    RooAbsReal& _sn_location,
    RooAbsReal& _sn_scale,
    RooAbsReal& _sn_skew,
    // End of skew normal
    RooAbsReal& _x0,
    // Gaussian cap
    RooAbsReal& _cap_location,
    RooAbsReal& _cap_sigmaL,
    RooAbsReal& _cap_sigmaR,
    // Total width of Gaussian cap piece
    RooAbsReal& _dx1,
    // Landau tail
    RooAbsReal& _tail_mp,
    RooAbsReal& _tail_width) :
  RooAbsPdf(name,title),
  x("x","Observable",this,_x),
  sn_location("sn_location","Turnon PDF Location",this,_sn_location),
  sn_scale("sn_scale","Turnon PDF Scale",this,_sn_scale),
  sn_skew("skew","Turnon PDF Skew",this,_sn_skew),
  x0("x0", "Transition point",this,_x0),
  cap_location("cap_location", "Cap PDF mean", this, _cap_location),
  cap_sigmaL("cap_sigmaL", "Cap PDF left sigma", this, _cap_sigmaL),
  cap_sigmaR("cap_sigmaR", "Cap PDF left sigma", this, _cap_sigmaR),
  dx1("dx1", "Cap width", this,_dx1),
  tail_mp("tail_mp", "Tail PDF MP", this, _tail_mp),
  tail_width("tail_width", "Tail PDF width", this, _tail_width)
  //turnonPDF("turnonPDF", "turnonPDF", _x, _sn_location, _sn_scale, _sn_skew),
  //capPDF("capPDF", "capPDF", _x, _cap_location, _cap_sigmaL, _cap_sigmaR),
  //tailPDF("tailPDF", "tailPDF", _x, _tail_mp, _tail_width),
  //nset(_x, "toIntegrate"),
  //turnonPDF_func(NULL), // TF1s are built lazily
  //capPDF_func(NULL),
  //tailPDF_func(NULL)
{

}

//_____________________________________________________________________________
RooPiecewisePtBalancePdf::RooPiecewisePtBalancePdf(const RooPiecewisePtBalancePdf& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  sn_location("sn_location",this,other.sn_location),
  sn_scale("sn_scale",this,other.sn_scale),
  sn_skew("skew",this,other.sn_skew),
  x0("x0",this,other.x0),
  cap_location("cap_location", this,other.cap_location),
  cap_sigmaL("cap_sigmaL", this,other.cap_sigmaL),
  cap_sigmaR("cap_sigmaR", this,other.cap_sigmaR),
  dx1("dx1", this,other.dx1),
  tail_mp("tail_mp", this,other.tail_mp),
  tail_width("tail_width", this,other.tail_width)
  //turnonPDF(other.turnonPDF, "turnonPDF"),
  //capPDF(other.capPDF, "capPDF"),
  //tailPDF(other.tailPDF, "tailPDF"),
  //nset(other.nset, "toIntegrate"),
  //turnonPDF_func(NULL), // TF1s are built lazily
  //capPDF_func(NULL),
  //tailPDF_func(NULL)
{
}

Double_t RooPiecewisePtBalancePdf::getTurnonScaleFactor() const {
  // Build the functions if we don't already have them
  // Figure out what the value is at the transition point
  Double_t turnonValue = evaluateTurnon(x0);
  Double_t capValue = evaluateCap(x0);
  Double_t scaleFactor = capValue/turnonValue;
  return scaleFactor;
}

Double_t RooPiecewisePtBalancePdf::getTailScaleFactor() const {
  Double_t capValue = evaluateCap(capToTailTransitionPoint());
  Double_t tailValue = evaluateTail(capToTailTransitionPoint());
  Double_t scaleFactor = capValue/tailValue;
  return scaleFactor;
}

Double_t RooPiecewisePtBalancePdf::capToTailTransitionPoint() const {
  return (Double_t)x0 + (Double_t)dx1;
}

Double_t RooPiecewisePtBalancePdf::evaluate() const
{
  if ((Double_t)x < (Double_t)x0) {
    // in turn on region
    return getTurnonScaleFactor()*evaluateTurnon(x);
  } else if ((Double_t)x > capToTailTransitionPoint()) {
    return getTailScaleFactor()*evaluateTail(x);
  }  //otherwise just return the cap value
  return evaluateCap(x);
}

Double_t RooPiecewisePtBalancePdf::evaluateTurnon(double xx) const {
  Double_t arg = (xx-sn_location)/sn_scale;

  Double_t gaussianPart = TMath::Gaus(arg, 0.0, 1.0, true);

  Double_t skewPart = 0.5*(1.0 + TMath::Erf(sn_skew*arg/TMath::Sqrt2()));

  //cout << "gauss x = " << x << " location = " << location << " scale = " << scale << endl ;
  return (2.0/sn_scale)*gaussianPart*skewPart;
}

Double_t RooPiecewisePtBalancePdf::evaluateCap(double xx) const {
  if (xx > cap_location) {
    return TMath::Gaus(xx, cap_location, cap_sigmaR, false);
  } else {
    return TMath::Gaus(xx, cap_location, cap_sigmaL, false);
  }
}

Double_t RooPiecewisePtBalancePdf::evaluateTail(double xx) const {
  return ::ROOT::Math::landau_pdf((xx - tail_mp)/tail_width);
}

/*
//_____________________________________________________________________________
Int_t RooPiecewisePtBalancePdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char*) const
{
  return 0;
  if (matchArgs(allVars,analVars,x)) return 1 ;
  //if (matchArgs(allVars,analVars,location)) return 2 ;
  return 0 ;
}

//_____________________________________________________________________________
Double_t RooPiecewisePtBalancePdf::analyticalIntegral(Int_t code, const char* rangeName) const
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
    cout << "Error in RooPiecewisePtBalancePdf::analyticalIntegral" << endl;
  }
  return ret ;

}
*/
RooPiecewisePtBalancePdf::~RooPiecewisePtBalancePdf() { }

Int_t RooPiecewisePtBalancePdf::getAnalyticalIntegral(
    RooArgSet& allVars, RooArgSet& analVars, const char*) const
{
  if (matchArgs(allVars, analVars, x) ) return 1;
  return 0 ;
}

Double_t RooPiecewisePtBalancePdf::turnonCDF(double xx) const {
  Double_t arg = (xx-sn_location)/sn_scale;
  double gaussianCDF = 0.5*(1.0 + TMath::Erf(arg/TMath::Sqrt2()));
  return (gaussianCDF - 2*t(arg, sn_skew));
}

Double_t RooPiecewisePtBalancePdf::capCDF(double xx) const {
  // Do left part first
  Double_t arg = (TMath::Min(xx, cap_location) -cap_location)/cap_sigmaL;
  double gaussianCDF = TMath::Sqrt(2.0*TMath::Pi())*cap_sigmaL*0.5*
    (1.0 + TMath::Erf(arg/TMath::Sqrt2()));

  // Add the stuff on the right as well
  if (xx > cap_location) {
    Double_t rightArg = (xx-cap_location)/cap_sigmaR;
    double gaussianRightCDF = TMath::Sqrt(2.0*TMath::Pi())*cap_sigmaR*0.5*
      (1.0 + TMath::Erf(rightArg/TMath::Sqrt2()));
    // The integral of a "sigmaR" type gaussian for arg going from -Inf to 0.0
    double offset = TMath::Sqrt(2.0*TMath::Pi())*cap_sigmaR*0.5;
    gaussianCDF += gaussianRightCDF - offset;
  }
  return gaussianCDF;
}

Double_t RooPiecewisePtBalancePdf::tailCDF(double xx) const {
  return tail_width*(::ROOT::Math::landau_cdf( (xx - tail_mp)/tail_width ));
}


Double_t RooPiecewisePtBalancePdf::analyticalIntegral(
    Int_t code, const char* rangeName) const {
  assert(code == 1);
  Double_t output = 0;

  double xMin = x.min(rangeName);
  double xMax = x.max(rangeName);

  double turnonMin = xMin;
  double turnonMax = TMath::Min(x0, xMax);

  if (turnonMax > turnonMin) {
    output += getTurnonScaleFactor()*(turnonCDF(turnonMax) - turnonCDF(turnonMin));
  }

  double capMin = TMath::Max(xMin, x0);
  double capMax = TMath::Min(xMax, capToTailTransitionPoint());

  if (capMax > capMin) {
    output += (capCDF(capMax) - capCDF(capMin));
  }

  double tailMin = TMath::Max(xMin, capToTailTransitionPoint());
  double tailMax = xMax;

  if (tailMax > tailMin) {
    output += getTailScaleFactor()*(tailCDF(tailMax) - tailCDF(tailMin));
  }

  return output;
}

RooArgSet RooPiecewisePtBalancePdf::estimateParameters(RooAbsData& data, double errorFactor) {
  // First estimate RooSkewNormal
  std::auto_ptr<TH1> histogram(data.createHistogram(x.arg().GetName()));

  double histogramMax_x = histogram->GetBinCenter(histogram->GetMaximumBin());

  //double edgeErrorRight=-1;
  //double edgeErrorLeft=-1;
  //double fallingEdge = compEdgePos(true, histogram.get(), edgeErrorRight, edgeErrorLeft);

  //double risingEdge = compEdgePos(false, histogram.get(), edgeErrorRight, edgeErrorLeft);
  double fallingEdge = findHalfMaxPointRight(histogram.get());
  double risingEdge = findHalfMaxPointLeft(histogram.get());

  RooRealVar* _x = convertProxy(x);

  // Convert all the proxies
  RooRealVar* _sn_location = convertProxy(sn_location);
  RooRealVar* _sn_scale = convertProxy(sn_scale);
  RooRealVar* _sn_skew = convertProxy(sn_skew);
  // End of skew normal
  RooRealVar* _x0 = convertProxy(x0);
  // Gaussian cap
  RooRealVar* _cap_location = convertProxy(cap_location);
  RooRealVar* _cap_sigmaL = convertProxy(cap_sigmaL);
  RooRealVar* _cap_sigmaR = convertProxy(cap_sigmaR);
  // Total width of Gaussian cap piece
  RooRealVar* _dx1 = convertProxy(dx1);
  // Landau tail
  RooRealVar* _tail_mp = convertProxy(tail_mp);
  RooRealVar* _tail_width = convertProxy(tail_width);

  std::cout << "Making copy" << std::endl;
  RooPiecewisePtBalancePdf copy(*this, "copy");

  bool fitAlpha = false;
  bool fitTailPos = !_tail_mp->isConstant();

  if (!_sn_skew->isConstant()) {
    fitAlpha = true;
    std::cout << "Estimating alpha" << std::endl;
    double sampleSkew = histogram->GetSkewness();
    std::cout << " - initial sample skew is " << sampleSkew << std::endl;

    if (TMath::Abs(sampleSkew) > 0.95) {
      sampleSkew = 0.95*sampleSkew/TMath::Abs(sampleSkew);
    }
    double sampleSigma = histogram->GetRMS();
    double sampleMean = histogram->GetMean();

    std::cout << " - corrected sample skew is " << sampleSkew << std::endl;
    std::cout << " - sample mean is " << sampleMean << std::endl;
    std::cout << " - sample sigma is " << sampleSigma << std::endl;

    double thirdMomentToTwoThirds = TMath::Power(TMath::Abs(sampleSkew), 2.0/3.0);

    double absDelta = TMath::Sqrt(
        TMath::Pi()*0.5*(thirdMomentToTwoThirds)/(
          thirdMomentToTwoThirds + TMath::Power((4 - TMath::Pi())/2.0, 2.0/3.0)));

    double delta = absDelta*sampleSkew/TMath::Abs(sampleSkew);

    double estSkew = delta/TMath::Sqrt(1- delta*delta);
    double estScale = sampleSigma/TMath::Sqrt(1 - 2*delta*delta/TMath::Pi());
    double estLocation = sampleMean - estScale*delta*TMath::Sqrt(2/TMath::Pi());

    std::cout << "Est Location: " << estLocation << std::endl;
    std::cout << "Est Scale: " << estScale << std::endl;
    std::cout << "Est Skew: " << estSkew << std::endl;

    *_sn_location = estLocation;
    *_sn_scale = estScale;
    *_sn_skew = estSkew;
  } else {
    *_sn_location = histogram->GetMean();
    *_sn_scale = histogram->GetRMS();
  }

  _x0->setVal(risingEdge);
  _dx1->setVal(fallingEdge - risingEdge);

  *_tail_mp = histogramMax_x;
  *_tail_width = 0.01*histogram->GetRMS();

  std::cout << "Tail width is: " << _tail_width->getVal() << std::endl;

  std::cout << "Fitting cap between " << x0 << " and "
    << (Double_t)x0 + (Double_t)dx1 << std::endl;

  std::cout << "Cap location is: " << histogramMax_x << std::endl;

  _cap_location->setVal(histogramMax_x);
  _cap_sigmaR->setVal(histogram->GetRMS());
  _cap_sigmaL->setVal(histogram->GetRMS());

  _x->setRange("capfit", x0, capToTailTransitionPoint());

  _sn_location->setConstant(true);
  _sn_scale->setConstant(true);
  _sn_skew->setConstant(true);
  _x0->setConstant(true);
  _cap_location->setConstant(true);
  _cap_sigmaL->setConstant(true);
  _cap_sigmaR->setConstant(true);
  _dx1->setConstant(true);
  _tail_mp->setConstant(true);
  _tail_width->setConstant(true);

  _cap_location->setConstant(false);
  _cap_sigmaL->setConstant(false);
  _cap_sigmaR->setConstant(false);

  copy.fitTo(data, RooFit::Range("capfit"));

  std::cout << "Fitting turnon region" << std::endl;

  _sn_location->setConstant(true);
  _sn_scale->setConstant(true);
  _sn_skew->setConstant(true);
  _x0->setConstant(true);
  _cap_location->setConstant(true);
  _cap_sigmaL->setConstant(true);
  _cap_sigmaR->setConstant(true);
  _dx1->setConstant(true);
  _tail_mp->setConstant(true);
  _tail_width->setConstant(true);

  _sn_location->setConstant(false);
  _sn_scale->setConstant(false);

  _sn_skew->setConstant(!fitAlpha);

  _x->setRange("turnonfit", 0.0, x0);

  copy.fitTo(data, RooFit::Range("turnonfit"));

  std::cout << "Fitting tail region" << std::endl;

  _sn_location->setConstant(true);
  _sn_scale->setConstant(true);
  _sn_skew->setConstant(true);
  _x0->setConstant(true);
  _cap_location->setConstant(true);
  _cap_sigmaL->setConstant(true);
  _cap_sigmaR->setConstant(true);
  _dx1->setConstant(true);
  _tail_mp->setConstant(true);
  _tail_width->setConstant(true);

  _tail_mp->setConstant(false);
  _tail_width->setConstant(false);

  *_tail_mp = histogramMax_x;
  *_tail_width = 0.04*histogram->GetRMS();

  double rightmostPoint = histogram->GetXaxis()->GetXmax();
  _x->setRange("tailfit", capToTailTransitionPoint(), rightmostPoint);

  copy.fitTo(data, RooFit::Range("tailfit"));

  _sn_location->setConstant(false);
  _sn_scale->setConstant(false);
  _sn_skew->setConstant(!fitAlpha);
  _x0->setConstant(false);
  _cap_location->setConstant(false);
  _cap_sigmaL->setConstant(false);
  _cap_sigmaR->setConstant(false);
  _dx1->setConstant(false);
  _tail_mp->setConstant(false);
  _tail_width->setConstant(false);

  /*
  data.Print("v");
  std::auto_ptr<RooArgSet> capObservables(capPDF.getObservables(data));
  std::cout << "observables: " << *capObservables << std::endl;
  capPDF.fitTo(data, RooFit::Range("capfit"));
  */

  /*
  std::cout << "Fitting turn on" << std::endl;
  _x->setRange("turnonfit", 0.0, x0);
  turnonPDF.estimateParameters(data, 1.0);
  turnonPDF.fitTo(data, RooFit::Range("turnonfit"));

  std::cout << "Fitting tail" << std::endl;
  _x->setRange("tailfit", capToTailTransitionPoint(),
      histo->GetXaxis()->GetXmax());

  //tailPDF.fitTo(data, RooFit::Range("tailfit"));
  */

  RooArgSet output;
  return output;
}

// generate CInt dictionaries
ClassImp(RooPiecewisePtBalancePdf)
