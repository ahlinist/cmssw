#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h" 
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooFormulaVar.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH2.h"

int main() {
  gROOT->SetBatch(kTRUE);
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

  RooRealVar x("x", "x", -10, 10);
  RooRealVar y("y", "y", -10, 10);
  RooRealVar mux("mux", "average-x'", 0, -1, 1);
  RooRealVar sigmax("sigmax", "sigma-x'", 0.5, 0, 5);
  RooGaussian gaussx("gaussx","gaussian PDF x'", x, mux, sigmax);
  RooRealVar muy("muy", "average-y'", 0, -1, 1);
  RooRealVar sigmay("sigmay", "sigma-y'", 1.5, 0, 5);
  RooGaussian gaussy("gaussy","gaussian PDF y'", y, muy, sigmay);
  RooProdPdf gaussxy("gaussxy", "gaussxy", RooArgSet(gaussx, gaussy));
 
  RooRealVar lambdax("lambdax", "log-slope x", -0.1, -5., 0.);
  RooExponential expox("expox", "exponential PDF x", x, lambdax);
  RooRealVar lambday("lambday", "log-slope y", -0.1, -5., 0.);
  RooExponential expoy("expoy", "exponential PDF y", y, lambday);
  RooProdPdf expoxy("expoxy", "expoxy", RooArgSet(expox, expoy));

  RooRealVar s("s", "signal yield", 50, 0, 10000);
  RooRealVar b("b", "background yield", 200, 0, 10000);

  RooAddPdf sum("sum", "gaussian plus exponential PDF", 
		RooArgList(gaussxy, expoxy), RooArgList(s, b));
  RooDataSet * data = sum.generate(RooArgSet(x,y), 250);
  RooFitResult* result = sum.fitTo(*data, RooFit::Extended(), RooFit::Minos(), RooFit::Save());
  assert(result != 0);
  result->Print();
  
  RooPlot * xFrame = x.frame() ;
  data->plotOn(xFrame) ;
  sum.plotOn(xFrame) ;
  sum.plotOn(xFrame, RooFit::Components(expoxy), RooFit::LineStyle(kDashed)) ;

  RooPlot * yFrame = y.frame() ;
  data->plotOn(yFrame) ;
  sum.plotOn(yFrame) ;
  sum.plotOn(yFrame, RooFit::Components(expoxy), RooFit::LineStyle(kDashed)) ;

  TCanvas c;
  xFrame->Draw();
  c.SaveAs("twoDimensionalFit-yProjection.eps");  
  yFrame->Draw();
  c.SaveAs("twoDimensionalFit-xProjection.eps");
  TH2 * surf = dynamic_cast<TH2*>(sum.createHistogram("surf", x, 
						      RooFit::Binning(100),
						      RooFit::YVar(y, RooFit::Binning(100))));
  surf->Draw("contz");
  c.SaveAs("twoDimensionalFit-xyContour.eps");
  TH2 * surfData = data->createHistogram(x, y, 100, 100, "", "surfData");
  surfData->Draw("contz");
  c.SaveAs("twoDimensionalFit-xyDataContour.eps");

  sum.getVariables()->Print();
  mux.Print();
  muy.Print();
  sigmax.Print();
  sigmay.Print();
  lambdax.Print();
  lambday.Print();
  s.Print();
  b.Print();
  return 0;
}
