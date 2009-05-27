#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGlobalFunc.h" 
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TROOT.h"

int main() {
  gROOT->SetBatch(kTRUE);
  gROOT->SetStyle("Plain");

  RooRealVar x("x", "x", -10, 10);

  RooRealVar mu("mu", "average", 0, -1, 1);
  RooRealVar sigma("sigma", "sigma", 1, 0, 5);
  RooGaussian gauss("gauss","gaussian PDF", x, mu, sigma);

  RooRealVar lambda("lambda", "slope", -0.1, -5., 0.);
  RooExponential expo("expo", "exponential PDF", x, lambda);

  RooRealVar s("s", "signal yield", 100, 0, 10000);
  RooRealVar b("b", "background yield", 100, 0, 10000);

  RooAddPdf sum("sum", "gaussian plus exponential PDF", 
		RooArgList(gauss, expo), RooArgList(s, b));

  RooDataSet * data = sum.generate(x, 2000);
  x.setBins(50);
  RooDataHist hist("hist", "hist", RooArgSet(x), *data);
  RooChi2Var chi2("chi2","chi2", sum, hist, true);
  RooMinuit minuit(chi2);
  minuit.migrad();
  minuit.hesse();

  RooPlot * xFrame = x.frame() ;
  hist.plotOn(xFrame) ;
  sum.plotOn(xFrame) ;
  sum.plotOn(xFrame, RooFit::Components(expo), RooFit::LineStyle(kDashed)) ;
  TCanvas c;
  xFrame->Draw();
  c.SaveAs("binnedChi2Fit.eps");
  
  sum.getVariables()->Print();
  mu.Print();
  sigma.Print();
  lambda.Print();
  s.Print();
  b.Print();

  return 0;
}
 
