
//-------------------------------------------------------------------------------
//
// NOTE: to be run via
// 
//         root rootlogon.C
//         .x fitSmoothingFunctionZtoMuTau_ZmumuJetMisId.C++
//
//-------------------------------------------------------------------------------

#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooPlot.h>
#include <RooFit.h>

#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TMath.h>
#include <TArrayF.h>
#include <TCanvas.h>

Double_t langaufun(Double_t* x, Double_t* par)
{
  //-----------------------------------------------------------------------------
  // Convoluted Landau and Gaussian Fitting Function
  //         (using ROOT's Landau and Gauss functions)
  //
  //  Based on a Fortran code by R.Fruehwirth (fruhwirth@hephy.oeaw.ac.at)
  //  Adapted for C++/ROOT by H.Pernegger (Heinz.Pernegger@cern.ch) and
  //   Markus Friedl (Markus.Friedl@cern.ch)
  //
  // Downloaded from http://root.cern.ch/root/roottalk/roottalk02/att-3361/01-langaus.C
  //
  //-----------------------------------------------------------------------------

  // function parameter
  // ------------------
  //  o par[0]: Width (scale) parameter of Landau density
  //  o par[1]: Most Probable (MP, location) parameter of Landau density
  //  o par[2]: Total area (integral -inf to inf, normalization constant)
  //  o par[3]: Width (sigma) of convoluted Gaussian function
  //
  // In the Landau distribution (represented by the CERNLIB approximation), 
  // the maximum is located at x = -0.22278298 with the location parameter = 0.
  // This shift is corrected within this function, so that the actual
  // maximum is identical to the MP parameter.

  // Numeric constants
  const Double_t invsq2pi = 0.3989422804014; // (2 pi)^(-1/2)
  const Double_t mpshift  = -0.22278298;     // Landau maximum location

  // Control constants
  const Double_t np = 100.0; // number of convolution steps
  const Double_t sc =   5.0; // convolution extends to +-sc Gaussian sigmas

  // Variables
  Double_t xx;
  Double_t mpc;
  Double_t fland;
  Double_t sum = 0.0;
  Double_t xlow, xupp;
  Double_t step;
  Double_t i;

  // MP shift correction
  mpc = par[1] - mpshift * par[0]; 

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp - xlow) / np;

  // Approximate convolution integral of Landau and Gaussian by sum
  for( i = 1.0; i <= np/2; i++ ) {
    xx = xlow + (i - 0.5) * step;
    fland = TMath::Landau(xx, mpc, par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0], xx, par[3]);
    
    xx = xupp - (i - 0.5) * step;
    fland = TMath::Landau(xx, mpc, par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0], xx, par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);
}

class SmoothLandau_x_GaussPdf : public RooAbsPdf 
{
 public:
  // constructor
  SmoothLandau_x_GaussPdf(const char* name, const char* title, 
			  RooAbsReal& x, 
			  RooAbsReal& width, RooAbsReal& mp, RooAbsReal& area, RooAbsReal& gsigma)
    : RooAbsPdf(name, title), 
      x_("x", "Dependent", this, x),
      width_("width", "Width", this, width),
      mp_("mp", "MP", this, mp),
      area_("area", "Area", this, area),
      gsigma_("gsigma", "gSigma", this, gsigma)
  {}
  SmoothLandau_x_GaussPdf(const SmoothLandau_x_GaussPdf& bluePrint, const char* newName)
    : RooAbsPdf(bluePrint, newName), 
      x_("x", this, bluePrint.x_),
      width_("width", this, bluePrint.width_),
      mp_("mp", this, bluePrint.mp_),
      area_("area", this, bluePrint.area_),
      gsigma_("gsigma", this, bluePrint.gsigma_)
  {}
  
  // destructor
  virtual ~SmoothLandau_x_GaussPdf()
  {}

  virtual TObject* clone(const char* newName) const { return new SmoothLandau_x_GaussPdf(*this, newName); }

 protected:
  Double_t evaluate() const
  {
    Double_t x[1];
    x[0] = x_;
    
    Double_t par[4];
    par[0] = width_;
    par[1] = mp_;
    par[2] = area_;
    par[3] = gsigma_;
    
    return langaufun(x, par);
  }

  RooRealProxy x_;

  RooRealProxy width_;
  RooRealProxy mp_;
  RooRealProxy area_;
  RooRealProxy gsigma_;
};

void fitSmoothingFunctionZtoMuTau_ZmumuJetMisId()
{
  TString inputFileName = "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/bgEstTemplateHistZtoMuTau_skimmed.root";
  TFile* inputFile = new TFile(inputFileName.Data());

  TString dqmDirectory = "DQMData/template/harvested/data/";
  dqmDirectory.Append("BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched/afterDiMuonPairInvMassBgEstZmumuJetMisIdEnriched");
  TString meName = "DiTauCandidateQuantities/VisMass";

  TString meName_full = TString(dqmDirectory).Append("/").Append(meName);
  
  TH1* me = (TH1*)inputFile->Get(meName_full.Data());
  if ( !me->GetSumw2N() ) me->Sumw2();
  me->Rebin(2);
  me->Scale(1./me->Integral());

  TArrayF excludedBinsX(2);
  excludedBinsX[0] = 85.;
  excludedBinsX[1] = 95.;
  for ( int iBin = 0; iBin < excludedBinsX.GetSize(); ++iBin ) {
    int binIndex = me->FindBin(excludedBinsX[iBin]);
    if ( binIndex > 0 && binIndex <= me->GetNbinsX() ) {
      std::cout << "--> excluding bin " << binIndex << ":" 
		<< " xCenter = " << me->GetBinCenter(binIndex) << " from fit." << std::endl;
      me->SetBinContent(binIndex, 0.);
      me->SetBinError(binIndex, 1.e+6);
    }
  }

  RooRealVar* fitVar = new RooRealVar("fitVar", "fitVar", 20., 200.);

  RooDataHist* dataHist = new RooDataHist("dataHist", "dataHist", *fitVar, me);

  RooRealVar* width = new RooRealVar("width", "width", 20., 0.01, 50.);
  RooRealVar* mp = new RooRealVar("mp", "mp", 65., 30., 150.);
  RooRealVar* area = new RooRealVar("area", "area", 1., 0.1, 10.);
  RooRealVar* gsigma = new RooRealVar("gsigma", "gsigma", 5., 1., 50.);

  RooAbsPdf* pdf = new SmoothLandau_x_GaussPdf("pdf", "pdf", *fitVar, *width, *mp, *area, *gsigma);

  pdf->fitTo(*dataHist);

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  RooPlot* frame = fitVar->frame(RooFit::Title("ZmumuJetMisId smoothing"));
  dataHist->plotOn(frame);
  pdf->plotOn(frame);
  frame->SetMaximum(1.);
  frame->SetMinimum(0.);
  frame->Draw();

  canvas->Update();
  canvas->Print("fitSmoothingFunctionZtoMuTau_ZmumuJetMisId.png");
 
  delete inputFile;
}
