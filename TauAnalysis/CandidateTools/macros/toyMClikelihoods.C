
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TArrayD.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TRandom3.h>
#include <TFitterMinuit.h>
#include <Minuit2/FCNBase.h>
#include <TFile.h>

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

void showFunctions1d(std::vector<TF1*>& functions, const std::vector<std::string>& legendEntries,
		     const TString& xAxisTitle, double yMin, double yMax, const TString& yAxisTitle, 
		     const std::string& outputFileName)
{
  assert(functions.size() == legendEntries.size());

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  assert(functions.size() > 0);
  TF1* refFunction = functions[0];

  TH1* dummyHistogram = new TH1F("dummyHistogram", "dummyHistogram", 10, refFunction->GetXmin(), refFunction->GetXmax());
  dummyHistogram->SetStats(false);
  dummyHistogram->SetTitle("");
  dummyHistogram->SetMinimum(-TMath::Log(yMax));
  dummyHistogram->SetMaximum(-TMath::Log(yMax) + 5.);
  
  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.Data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(Form("-log(%s)", yAxisTitle.Data()));
  yAxis->SetTitleOffset(1.30);

  dummyHistogram->Draw("axis");

  int colors[] = { 1, 2, 3, 4, 5, 6, 7, 15 };
  int numFunctions = functions.size();
  if ( numFunctions > 8 ) {
    std::cerr << "<showFunctions1d>:" << std::endl;
    std::cerr << "Number of functions must not exceed 8 !!" << std::endl;
    assert(0);
  }
  
  for ( int iFunction = 0; iFunction < numFunctions; ++iFunction ) {
    TF1* function = functions[iFunction];
    function->SetLineColor(colors[iFunction]);
    function->SetLineWidth(2);
    function->Draw("same");
  }

  TLegend* legend = new TLegend(0.68, 0.89 - (0.03 + 0.040*numFunctions), 0.89, 0.89, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  for ( int iFunction = 0; iFunction < numFunctions; ++iFunction ) {
    TF1* function = functions[iFunction];
    const std::string& legendEntry = legendEntries[iFunction];
    legend->AddEntry(function, legendEntry.data(), "l");
  }
  legend->Draw();

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());

  delete dummyHistogram;
  delete legend;
  delete canvas;
}

void showFunction1d(TF1* function, 
		    const TString& xAxisTitle, double yMax, const TString& yAxisTitle,
		    const std::string& outputFileName)
{
  std::vector<TF1*> functions;
  std::string functionName_nll = Form("%s_nll", function->GetName());
  TF1* function_nll = 
    new TF1(functionName_nll.data(), 
	    Form("-TMath::Log(%s)", 
		 function->GetTitle()),
	    function->GetXmin(), function->GetXmax());
  functions.push_back(function_nll);
  std::vector<std::string> legendEntries;
  legendEntries.push_back(std::string(function->GetTitle()));
  showFunctions1d(functions, legendEntries, xAxisTitle, 0., yMax, yAxisTitle, outputFileName);
}

void showFunction2d(TF1* function, 
		    const TArrayD& testValuesX, const TArrayD& testValuesY, double metResolution, double metErr,
		    const TString& xAxisTitle, const TString& yAxisTitle,
		    const std::string& outputFileName)
{
  int numTestValuesX = testValuesX.GetSize();
  for ( int iTestValueX = 0; iTestValueX < numTestValuesX; ++iTestValueX ) {
    double testValueX = testValuesX[iTestValueX];

    std::vector<TF1*> functions;
    std::vector<std::string> legendEntries;

    int numTestValuesY = testValuesY.GetSize();
    for ( int iTestValueY = 0; iTestValueY < numTestValuesY; ++iTestValueY ) {
      double testValueY = testValuesY[iTestValueY];

      std::string functionName_nll = Form("%s_nll_%i", function->GetName(), iTestValueY);
      TF1* function_nll = 0;
      if ( metResolution > 0. ) {
	function_nll = 
	  new TF1(functionName_nll.data(), 
		  Form("-TMath::Log((%s)*TMath::Gaus(([1] - [0])*x, ([1] - [0]) + [2], [3]))", 
		       function->GetTitle()),
		  function->GetXmin(), function->GetXmax());
      } else {
	function_nll = new TF1(functionName_nll.data(), function->GetTitle(), function->GetXmin(), function->GetXmax());
      }
      function_nll->SetParameter(0, testValueX);
      function_nll->SetParameter(1, testValueY);
      function_nll->SetParameter(2, metErr);
      function_nll->SetParameter(3, metResolution);
      functions.push_back(function_nll);

      legendEntries.push_back(std::string(Form("m_{2} = %1.1f", testValueY)));
    }
 
    double yMax = 0.5*(0.1 + testValueX);

    size_t idx = outputFileName.find_last_of('.');
    TString outputFileName_plot = std::string(outputFileName, 0, idx).data();
    outputFileName_plot.Append(Form("_m1Eq%1.1f", testValueX));
    outputFileName_plot.ReplaceAll(".", "_");
    if ( idx != std::string::npos ) outputFileName_plot.Append(std::string(outputFileName, idx).data());
    showFunctions1d(functions, legendEntries, xAxisTitle, 0., yMax, yAxisTitle, outputFileName_plot.Data());

    for ( std::vector<TF1*>::iterator it = functions.begin();
	  it != functions.end(); ++it ) {
      delete (*it);
    }
  }
}

double sampleXgenFromFunction1d(TRandom& rnd, TF1* function, double xMin, double xMax, double pMax)
{
  //std::cout << "<sampleXgenFromFunction1d>:" << std::endl;
  //std::cout << " function = " << function->GetTitle() << std::endl;
  //int numParameter = function->GetNpar();
  //for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
  //  std::cout << " parameter #" << iParameter << " = " << function->GetParameter(iParameter) << std::endl;
  //}
  //assert(0);
  double x;
  bool isDone = false;
  while ( !isDone ) {
    x = rnd.Uniform(xMin, xMax);
    double u = rnd.Rndm();
    double f_x = function->Eval(x);
    //std::cout << "x = " << x << ": f(x) = " << f_x << " (pMax = " << pMax << ")" << std::endl;
    isDone = ((u*pMax) < f_x);
  }
  return x;
}

struct MinuitFCNadapter : public ROOT::Minuit2::FCNBase
{
  MinuitFCNadapter(TF1* function)
    : function_(function)
  {}
  ~MinuitFCNadapter() {}

  /// define "objective" function called by Minuit
  double operator()(const std::vector<double>& x) const 
  {
    assert(x.size() == 1);
    double retVal = function_->Eval(x[0]);
    //std::cout << "<MinuitFCNadapter::operator()>: retVal = " << retVal << std::endl;
    return retVal;
  }

  /// define increase in "objective" function used by Minuit to determine one-sigma error contours;
  /// in case of (negative) log-likelihood functions, the value needs to be 0.5
  double Up() const { return 0.5; }

  TF1* function_;
};

void fitM(TF1* function, double& M, double& sigmaM, int& fitStatus)
{
//--- initialize Minuit
  MinuitFCNadapter minuitFCNadapter(function);

  TFitterMinuit* minuit = new TFitterMinuit();
  minuit->SetMinuitFCN(&minuitFCNadapter);
//--- set Minuit strategy = 2,
//    in order to enable reliable error estimates
//    ( cf. http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html )
  minuit->SetStrategy(2);
  minuit->SetMaxIterations(1000);
  minuit->SetPrintLevel(-1);
  minuit->SetErrorDef(0.5);

  minuit->CreateMinimizer();

  minuit->SetParameter(0, "M", 0., 2., 1.e-3, 1.e+1);

  fitStatus = minuit->Minimize();

  M = minuit->GetParameter(0);
  sigmaM = minuit->GetParError(0);
}

void runToyMC(TF1* likelihoodFunctionX1, TF1* likelihoodFunctionX2, TF1* likelihoodFunctionX1and2, 
	      double metResolution, 
	      const TString& xAxisTitle, const TString& yAxisTitle,
	      const std::string& outputFileName)
{
  TH2* histogramM_vs_X1gen = 
    new TH2D("histogramM_vs_X1gen", 
	     "M vs. X_{1}^{gen}", 200, 0., 10., 100, 0., 1.);
  TH2* histogramM_vs_X2gen = 
    new TH2D("histogramM_vs_X2gen", 
	     "M vs. X_{2}^{gen}", 200, 0., 10., 100, 0., 1.);
  TH2* histogramSigmaM_vs_X1gen = 
    new TH2D("histogramSigmaM_vs_X1gen", 
	     "#sigmaM vs. X_{1}^{gen}", 200, 0., 5., 100, 0., 1.);
  TH2* histogramSigmaM_vs_X2gen = 
    new TH2D("histogramSigmaM_vs_X2gen", 
	     "#sigmaM vs. X_{2}^{gen}", 200, 0., 5., 100, 0., 1.);
  TH1* histogramM = 
    new TH1D("histogramM", 
	     "M", 200, 0., 5.);
  TH1* histogramSigmaM = 
    new TH1D("histogramSigmaM", 
	     "#sigmaM", 100, 0., 2.5);
  TH2* histogramM_vs_sigmaM = 
    new TH2D("histogramM_vs_sigmaM", 
	     "M vs. #sigmaM", 50, 0., 5., 25, 0., 2.5);

  const double xMin = 0.;
  const double xMax = 1.;
  
  std::string functionX1gen_name = "functionX1";
  TString functionX1gen_formula = likelihoodFunctionX1->GetTitle();
  functionX1gen_formula.ReplaceAll("TMath::Max", "TMATH::MAX");
  functionX1gen_formula.ReplaceAll("x", "X");
  functionX1gen_formula.ReplaceAll("[0]", "x");
  functionX1gen_formula.ReplaceAll("TMATH::MAX", "TMath::Max");
  functionX1gen_formula.ReplaceAll("X", "[0]");
  TF1* functionX1gen =
    new TF1(functionX1gen_name.data(),
	    functionX1gen_formula.Data(),
	    xMin, xMax);
  functionX1gen->SetParameter(0, 1.);
  double pMax_X1 = functionX1gen->GetMaximum(xMin, xMax);

  std::string functionX2gen_name = "functionX2";
  TString functionX2gen_formula = likelihoodFunctionX2->GetTitle();
  functionX2gen_formula.ReplaceAll("TMath::Max", "TMATH::MAX");
  functionX2gen_formula.ReplaceAll("x", "X");
  functionX2gen_formula.ReplaceAll("[0]", "x");
  functionX2gen_formula.ReplaceAll("TMATH::MAX", "TMath::Max");
  functionX2gen_formula.ReplaceAll("X", "[0]");
  TF1* functionX2gen =
    new TF1(functionX2gen_name.data(),
	    functionX2gen_formula.Data(),
	    xMin, xMax);
  functionX2gen->SetParameter(0, 0.5);
  double pMax_X2 = functionX2gen->GetMaximum(xMin, xMax);

  std::string functionName_nll = Form("%s_nll", likelihoodFunctionX1and2->GetName());
  TF1* function_nll = 
    new TF1(functionName_nll.data(), 
	    Form("-TMath::Log((%s)*TMath::Gaus(([1] - [0])*x, ([1] - [0]) + [2], [3]))", 
		 likelihoodFunctionX1and2->GetTitle()),
	    1.e-3, 1.e+1);
  
  TRandom3 rnd;

  const int numToys = 100000;
  for ( int iToy = 0; iToy < numToys; ++iToy ) {
    if ( (iToy % 1000) == 0 ) std::cout << "running Toy " << iToy << std::endl;

    double X1gen  = sampleXgenFromFunction1d(rnd, functionX1gen, 0., 1., pMax_X1);
    double X2gen  = sampleXgenFromFunction1d(rnd, functionX2gen, 0., 1., pMax_X2);
    double metErr = rnd.Gaus(0., metResolution);

    function_nll->SetParameter(0, X1gen);
    function_nll->SetParameter(1, X2gen);
    function_nll->SetParameter(2, metErr);
    function_nll->SetParameter(3, metResolution);

    double M, sigmaM;
    int fitStatus;
    fitM(function_nll, M, sigmaM, fitStatus);
    //std::cout << "fitStatus = " << fitStatus << ": M = " << M << ", sigmaM = " << sigmaM << std::endl;

    if ( fitStatus == 0 ) {
      histogramM_vs_X1gen->Fill(X1gen, M);
      histogramM_vs_X2gen->Fill(X2gen, M);
      histogramSigmaM_vs_X1gen->Fill(X1gen, sigmaM);
      histogramSigmaM_vs_X2gen->Fill(X2gen, sigmaM);
      histogramM->Fill(M);
      histogramSigmaM->Fill(sigmaM);
      histogramM_vs_sigmaM->Fill(sigmaM, M);
    }
  }

  std::cout << "creating outputFile = " << outputFileName << std::endl;
  TFile* outputFile = new TFile(outputFileName.data(), "RECREATE");
  histogramM_vs_X1gen->Write();
  histogramM_vs_X2gen->Write();
  histogramSigmaM_vs_X1gen->Write();
  histogramSigmaM_vs_X2gen->Write();
  histogramM->Write();
  histogramSigmaM->Write();
  histogramM_vs_sigmaM->Write();
  delete outputFile;

  delete histogramM_vs_X1gen;
  delete histogramM_vs_X2gen;
  delete histogramSigmaM_vs_X1gen;
  delete histogramSigmaM_vs_X2gen;
  delete histogramM;
  delete histogramSigmaM;
  delete histogramM_vs_sigmaM;

  delete functionX1gen;
  delete functionX2gen;
  delete function_nll;
}

void toyMClikelihoods()
{
  gROOT->SetBatch(true);

  std::string likelihoodFunctionX1_formula = "-8.*[0]/(x*x) + 4./x";
  TF1* likelihoodFunctionX1 = 
    new TF1("likelihoodFunctionX1", 
	    Form("TMath::Max(1.e-9, %s)", 
		 likelihoodFunctionX1_formula.data()), 
	    1.e-3, 1.e+1);
  likelihoodFunctionX1->SetParameter(0, 1.);
  std::cout << "likelihoodFunctionX1(1) = " << likelihoodFunctionX1->Eval(1) << std::endl;  
  //showFunction1d(likelihoodFunctionX1, "M", 1.e0, "L", "plots/toyMClikelihoods_X1.eps");

  std::string likelihoodFunctionX2_formula = "0.5*(TMath::Sign(+1,x - [0]) + 1.)*(8.*[0]/(x*x))";
  TF1* likelihoodFunctionX2 = 
    new TF1("likelihoodFunctionX2", 
	    Form("TMath::Max(1.e-9, %s)", 
		 likelihoodFunctionX2_formula.data()), 
	    1.e-3, 1.e+1);
  likelihoodFunctionX2->SetParameter(0, 1.);
  std::cout << "likelihoodFunctionX2(1) = " << likelihoodFunctionX2->Eval(1) << std::endl;  
  //  showFunction1d(likelihoodFunctionX2, "M", 1.e+1, "L", "plots/toyMClikelihoods_X2.eps");
  
  TF1* likelihoodFunctionX1and2 = 
    new TF1("likelihoodFunctionX1and2", 
	    Form("TMath::Max(1.e-9, (%s)*(%s))", 
		 likelihoodFunctionX1_formula.data(),
		 TString(likelihoodFunctionX2_formula.data()).ReplaceAll("[0]", "[1]").Data()),
	    1.e-3, 1.e+1);
  TArrayD testValuesX(5);
  testValuesX[0] = 0.1;
  testValuesX[1] = 0.3;
  testValuesX[2] = 0.5;
  testValuesX[3] = 0.7;
  testValuesX[4] = 0.9;
  TArrayD testValuesY = testValuesX;
  //showFunction2d(likelihoodFunctionX1and2, testValuesX, testValuesY, -1., 0., "M", "log(L)", "plots/toyMClikelihoods_X1and2.eps");
  TArrayD metResolutions(4);
  metResolutions[0] = 0.05;
  metResolutions[1] = 0.10;
  metResolutions[2] = 0.15;
  metResolutions[3] = 0.20;
  TArrayD metErrs(5);
  metErrs[0] = 0.00;
  metErrs[1] = 0.05;
  metErrs[2] = 0.10;
  metErrs[3] = 0.15;
  metErrs[4] = 0.20;
  int numMEtResolutions = metResolutions.GetSize();
  for ( int iMEtResolution = 0; iMEtResolution < numMEtResolutions; ++iMEtResolution ) {
    double metResolution = metResolutions[iMEtResolution];
    int numMEtErrs = metErrs.GetSize();
    for ( int iMEtErr = 0; iMEtErr < numMEtErrs; ++iMEtErr ) {
      double metErr = metErrs[iMEtErr];
      TString outputFileName = Form("plots/toyMClikelihoods_X1and2_metRes%1.2f_metErr%1.2f", metResolution, metErr);
      outputFileName.ReplaceAll(".", "_");
      outputFileName.Append(".eps");
      //showFunction2d(likelihoodFunctionX1and2, testValuesX, testValuesY, metResolution, metErr, "M", "log(L)", outputFileName.Data());
    }
    TString outputFileName_toyMC = Form("plots/toyMC_metRes%1.2f.root", metResolution);
    outputFileName_toyMC.ReplaceAll(".", "_");
    runToyMC(likelihoodFunctionX1, likelihoodFunctionX2, likelihoodFunctionX1and2, 
	     metResolution, "M", "#sigma_{M}", outputFileName_toyMC.Data());
  }
}


