
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections u1, u2 
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.1 2011/08/13 12:50:28 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionParameterSet.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooLinkedList.h"
#include "RooCmdArg.h"

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>

#include <vector>
#include <string>

typedef std::vector<std::string> vstring;

void showControlPlot(TCanvas* canvas, 
		     TH1* histogram, const std::string& histogramLegendEntry, TGraph* graph_fit, TGraphAsymmErrors* graph_fitErr,    
		     const std::string& yAxisLabel,
		     const std::string& outputFileName, const std::string& outputFileLabel)
{
  canvas->Clear();

  TAxis* xAxis = graph_fitErr->GetXaxis();
  xAxis->SetTitle("q_{T} / GeV");
  xAxis->SetTitleOffset(1.20);

  TAxis* yAxis = graph_fitErr->GetYaxis();
  yAxis->SetTitle(yAxisLabel.data());
  yAxis->SetTitleOffset(1.20);

  graph_fitErr->SetFillColor(46);
  graph_fitErr->SetFillStyle(3002);
  graph_fitErr->Draw("a3");

  graph_fit->SetLineColor(2);
  graph_fit->SetLineWidth(2);
  graph_fit->Draw("L");

  histogram->SetStats(false);
  histogram->SetMarkerColor(1);
  histogram->SetMarkerStyle(20);
  histogram->Draw("e1psame");

  TLegend legend(0.64, 0.64, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(histogram,    histogramLegendEntry.data(), "p");
  legend.AddEntry(graph_fit,    "Fit",                       "l");
  legend.AddEntry(graph_fitErr, "Fit Uncertainty",           "a");
  legend.Draw();

  canvas->Update();

  std::string outputFileName_plot = std::string(outputFileName, 0, outputFileName.find_last_of('.'));
  outputFileName_plot.append("_").append(outputFileLabel);
  outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

double square(double x)
{
  return x*x;
}

void makeControlPlots(TTree* tree, bool isData, 
		      const ZllRecoilCorrectionParameterSet& fitResultsfitResults, const std::string& outputFileName)
{
  Float_t qT, u1, u2, evtWeight;
  tree->SetBranchAddress("qT", &qT);
  tree->SetBranchAddress("u1", &u1);
  tree->SetBranchAddress("u2", &u2);
  tree->SetBranchAddress("evtWeight", &evtWeight);

  const int numBins = 10;
  double binning[numBins + 1] = { 0., 5., 10., 15., 20., 30., 40., 60., 80., 100., 150. };
 
  TH2* histogram_u1_diff = new TH2D("histogram_u1_diff", "histogram_u1_diff", numBins, binning, 200, -50., +50.);
  TH2* histogram_u2_diff = new TH2D("histogram_u2_diff", "histogram_u2_diff", numBins, binning, 200, -50., +50.);

  int numEntries = tree->GetEntries();
  for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
    tree->GetEntry(iEntry);

    double u1_fit = fitResultsfitResults.d() + fitResultsfitResults.k()*qT;
    histogram_u1_diff->Fill(qT, u1 - u1_fit, evtWeight);

    double u2_fit = 0.;
    histogram_u2_diff->Fill(qT, u2 - u2_fit, evtWeight);
  }

  TH1* histogram_u1     = new TH1D("histogram_u1",     "<u_{1}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_u1_rms = new TH1D("histogram_u1_rms", "rms(u_{1} - d + k*q_{T}) as function of q_{T}", numBins, binning);
  TH1* histogram_u2     = new TH1D("histogram_u2",     "<u_{2}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_u2_rms = new TH1D("histogram_u2_rms", "rms(u_{2}) as function of q_{T}",               numBins, binning);

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    TString histogramName_u1_proj = Form("%s_py_%i", histogram_u1_diff->GetName(), iBin);
    TH1D* histogram_u1_proj = histogram_u1_diff->ProjectionY(histogramName_u1_proj.Data(), iBin, iBin, "e");
    histogram_u1->SetBinContent(iBin, histogram_u1_proj->GetMean());
    histogram_u1->SetBinError(iBin, histogram_u1_proj->GetMeanError());
    histogram_u1_rms->SetBinContent(iBin, histogram_u1_proj->GetRMS());
    histogram_u1_rms->SetBinError(iBin, histogram_u1_proj->GetRMSError());

    TString histogramName_u2_proj = Form("%s_py_%i", histogram_u2_diff->GetName(), iBin);
    TH1D* histogram_u2_proj = histogram_u2_diff->ProjectionY(histogramName_u2_proj.Data(), iBin, iBin, "e");
    histogram_u2->SetBinContent(iBin, histogram_u2_proj->GetMean());
    histogram_u2->SetBinError(iBin, histogram_u2_proj->GetMeanError());
    histogram_u2_rms->SetBinContent(iBin, histogram_u2_proj->GetRMS());
    histogram_u2_rms->SetBinError(iBin, histogram_u2_proj->GetRMSError());
  }

  double xStepSize = 1.;
  double xMin = binning[0] + 0.5*xStepSize;
  double xMax = binning[numBins];

  int numPoints = TMath::FloorNint((xMax - xMin)/xStepSize);

  TGraph*            graph_u1Fit        = new TGraph(numPoints);
  TGraphAsymmErrors* graph_u1FitErr     = new TGraphAsymmErrors(numPoints);
  TGraph*            graph_u1_rmsFit    = new TGraph(numPoints);
  TGraphAsymmErrors* graph_u1_rmsFitErr = new TGraphAsymmErrors(numPoints);

  TGraph*            graph_u2Fit        = new TGraph(numPoints);
  TGraphAsymmErrors* graph_u2FitErr     = new TGraphAsymmErrors(numPoints);
  TGraph*            graph_u2_rmsFit    = new TGraph(numPoints);
  TGraphAsymmErrors* graph_u2_rmsFitErr = new TGraphAsymmErrors(numPoints);

  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*xStepSize;

    double u1_av             = fitResultsfitResults.d() + fitResultsfitResults.k()*x;
    double u1_dUp            = (fitResultsfitResults.d() + fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;    
    double u1_dDown          = (fitResultsfitResults.d() - fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;
    double u1_kUp            = fitResultsfitResults.d() + (fitResultsfitResults.k() + fitResultsfitResults.kErr())*x;
    double u1_kDown          = fitResultsfitResults.d() + (fitResultsfitResults.k() - fitResultsfitResults.kErr())*x;
    
    double u1ErrUp           = u1_av + TMath::Sqrt(square(u1_dUp   - u1_av) + square(u1_kUp   - u1_av));
    double u1ErrDown         = u1_av + TMath::Sqrt(square(u1_dDown - u1_av) + square(u1_kDown - u1_av));

    graph_u1Fit->SetPoint(iPoint, x, u1_av);
    graph_u1FitErr->SetPoint(iPoint, x, u1_av);
    graph_u1FitErr->SetPointError(iPoint, x, x, u1ErrUp, u1ErrDown);

    double u1_rms_av         = fitResultsfitResults.sigma1()*(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double u1_rms_sigma1Up   = (fitResultsfitResults.sigma1() + fitResultsfitResults.sigma1Err())
                              *(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double u1_rms_sigma1Down = (fitResultsfitResults.sigma1() - fitResultsfitResults.sigma1Err())
                              *(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double u1_rms_b1Up       = fitResultsfitResults.sigma1()
                              *(1. + (fitResultsfitResults.b1() + fitResultsfitResults.b1Err())*x + fitResultsfitResults.c1()*x*x);
    double u1_rms_b1Down     = fitResultsfitResults.sigma1()
                              *(1. + (fitResultsfitResults.b1() - fitResultsfitResults.b1Err())*x + fitResultsfitResults.c1()*x*x);
    double u1_rms_c1Up       = fitResultsfitResults.sigma1()
                              *(1. + fitResultsfitResults.b1()*x + (fitResultsfitResults.c1() + fitResultsfitResults.c1Err())*x*x);
    double u1_rms_c1Down     = fitResultsfitResults.sigma1()
                              *(1. + fitResultsfitResults.b1()*x + (fitResultsfitResults.c1() - fitResultsfitResults.c1Err())*x*x);
    
    double u1_rmsErrUp       = u1_rms_av + TMath::Sqrt(square(u1_rms_sigma1Up   - u1_rms_av) 
                                                     + square(TMath::Max(u1_rms_b1Up, u1_rms_b1Down) - u1_rms_av) 
                                                     + square(TMath::Max(u1_rms_c1Up, u1_rms_c1Down) - u1_rms_av));
    double u1_rmsErrDown     = u1_rms_av + TMath::Sqrt(square(u1_rms_sigma1Down - u1_rms_av) 
                                                     + square(TMath::Min(u1_rms_b1Up, u1_rms_b1Down) - u1_rms_av) 
                                                     + square(TMath::Min(u1_rms_c1Up, u1_rms_c1Down) - u1_rms_av));
    
    graph_u1_rmsFit->SetPoint(iPoint, x, u1_rms_av);
    graph_u1_rmsFitErr->SetPoint(iPoint, x, u1_rms_av);
    graph_u1_rmsFitErr->SetPointError(iPoint, x, x, u1_rmsErrUp, u1_rmsErrDown);
    
    graph_u2Fit->SetPoint(iPoint, x, 0.);
    graph_u2FitErr->SetPoint(iPoint, x, 0.);
    graph_u2FitErr->SetPointError(iPoint, x, x, +1.e+1, -1.e+1);

    double u2_rms_av         = fitResultsfitResults.sigma2()*(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double u2_rms_sigma2Up   = (fitResultsfitResults.sigma2() + fitResultsfitResults.sigma2Err())
                              *(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double u2_rms_sigma2Down = (fitResultsfitResults.sigma2() - fitResultsfitResults.sigma2Err())
                              *(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double u2_rms_b2Up       = fitResultsfitResults.sigma2()
                              *(1. + (fitResultsfitResults.b2() + fitResultsfitResults.b2Err())*x + fitResultsfitResults.c2()*x*x);
    double u2_rms_b2Down     = fitResultsfitResults.sigma2()
                              *(1. + (fitResultsfitResults.b2() - fitResultsfitResults.b2Err())*x + fitResultsfitResults.c2()*x*x);
    double u2_rms_c2Up       = fitResultsfitResults.sigma2()
                              *(1. + fitResultsfitResults.b2()*x + (fitResultsfitResults.c2() + fitResultsfitResults.c2Err())*x*x);
    double u2_rms_c2Down     = fitResultsfitResults.sigma2()
                              *(1. + fitResultsfitResults.b2()*x + (fitResultsfitResults.c2() - fitResultsfitResults.c2Err())*x*x);
    
    double u2_rmsErrUp       = u2_rms_av + TMath::Sqrt(square(u2_rms_sigma2Up   - u2_rms_av) 
                                                     + square(TMath::Max(u2_rms_b2Up, u2_rms_b2Down) - u2_rms_av) 
                                                     + square(TMath::Max(u2_rms_c2Up, u2_rms_c2Down) - u2_rms_av));
    double u2_rmsErrDown     = u2_rms_av + TMath::Sqrt(square(u2_rms_sigma2Down - u2_rms_av) 
                                                     + square(TMath::Min(u2_rms_b2Up, u2_rms_b2Down) - u2_rms_av) 
                                                     + square(TMath::Min(u2_rms_c2Up, u2_rms_c2Down) - u2_rms_av));
    
    graph_u2_rmsFit->SetPoint(iPoint, x, u2_rms_av);
    graph_u2_rmsFitErr->SetPoint(iPoint, x, u2_rms_av);
    graph_u2_rmsFitErr->SetPointError(iPoint, x, x, u2_rmsErrUp, u2_rmsErrDown);
  }

  TCanvas* canvas = new TCanvas();
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string histogramLegendEntry = ( isData ) ? "Data" : "Simulation";

  showControlPlot(canvas, 
		  histogram_u1, histogramLegendEntry, graph_u1Fit, graph_u1FitErr,     
		  "u_{1} / GeV", 
		  outputFileName, "u1");
  showControlPlot(canvas, 
		  histogram_u1_rms, histogramLegendEntry, graph_u1_rmsFit, graph_u1_rmsFitErr, 
		  "rms(u_{1}) / GeV", 
		  outputFileName, "u1_rms");
  showControlPlot(canvas, 
		  histogram_u2, histogramLegendEntry, graph_u2Fit, graph_u2FitErr,     
		  "u_{2} / GeV", 
		  outputFileName, "u2");
  showControlPlot(canvas, 
		  histogram_u2_rms, histogramLegendEntry, graph_u2_rmsFit, graph_u2_rmsFitErr, 
		  "rms(u_{2}) / GeV",
		  outputFileName, "u2_rms");
  
  delete canvas;
}

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("fitZllRecoilCorrection");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("fitZllRecoilCorrection") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgFitZllRecoilCorrection = cfg.getParameter<edm::ParameterSet>("fitZllRecoilCorrection");

  std::string directory = cfgFitZllRecoilCorrection.getParameter<std::string>("directory");
  std::string treeName = "ZllRecoilCorrectionNtuple";

  std::string outputFileName = cfgFitZllRecoilCorrection.getParameter<std::string>("outputFileName");

  std::string processType = cfgFitZllRecoilCorrection.getParameter<std::string>("type");
  std::cout << " type = " << processType << std::endl;
  bool isData = (processType == "Data");

  fwlite::InputSource inputFiles(cfg); 

  TChain* tree = new TChain(std::string(directory).append("/").append(treeName).data());
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    tree->AddFile(inputFileName->data());
  }

  //tree->Scan("qT:u1:u2:evtWeight");

  RooRealVar qT("qT", "qT", 0.,    0.,  150.);
  RooRealVar u1("u1", "u1", 0., -100., +100.);
  RooRealVar u2("u2", "u2", 0.,  -50.,  +50.);
  RooRealVar evtWeight("evtWeight", "evtWeight", 1.);

  //-----------------------------------------------------------------------------
  // define variables and pdfs for u1 fit
  RooRealVar d("d", "d",  0., -5., +5.);
  d.setConstant(true);
  RooRealVar k("k", "k", -1., -2.,  0.);

  RooRealVar sigma1("sigma1", "sigma1", 5., 0., 25.);
  RooRealVar b1("b1", "b1", +1.e-2, -1.e-1, +1.e-1);
  RooRealVar c1("c1", "c1", -1.e-4, -1.e-2, +1.e-2);

  RooFormulaVar u1_slope("u1_slope", "d + k*qT", RooArgList(d, k, qT));
  RooFormulaVar u1_rms("u1_rms", "sigma1*(1 + b1*qT + c1*qT*qT)", RooArgList(sigma1, b1, c1, qT));

  RooGaussian u1_pdf("u1_pdf", "u1_pdf", u1, u1_slope, u1_rms);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // define variables and pdfs for u2 fit
  RooRealVar sigma2("sigma2", "sigma2", 5., 0., 25.);
  RooRealVar b2("b2", "b2", +1.e-2, -1.e-1, +1.e-1);
  RooRealVar c2("c2", "c2", -1.e-4, -1.e-2, +1.e-2);

  RooRealVar u2_mean("u2_mean", "u2_mean", 0.,  -10., +10.);
  u2_mean.setConstant(true);
  RooFormulaVar u2_rms("u2_rms", "sigma2*(1 + b2*qT + c2*qT*qT)", RooArgList(sigma2, b2, c2, qT));

  RooGaussian u2_pdf("u2_pdf", "u2_pdf", u2, u2_mean, u2_rms);
  //-----------------------------------------------------------------------------
  
  RooDataSet dataset("dataset", "dataset", RooArgSet(qT, u1, u2, evtWeight), RooFit::Import(*tree), RooFit::WeightVar(evtWeight));

  RooLinkedList options;
  options.Add(new RooCmdArg(RooFit::PrintLevel(-1)));
  options.Add(new RooCmdArg(RooFit::PrintEvalErrors(-1)));
  options.Add(new RooCmdArg(RooFit::Warnings(-1)));

  std::cout << "starting u1 fit..." << std::endl;
  u1_pdf.fitTo(dataset, options);

  std::cout << "starting u2 fit..." << std::endl;
  u2_pdf.fitTo(dataset, options);

  std::cout << "done." << std::endl;

  ZllRecoilCorrectionParameterSet fitResults(
    d.getVal(), d.getError(), k.getVal(), k.getError(), 
    sigma1.getVal(), sigma1.getError(), b1.getVal(), b1.getError(), c1.getVal(), c1.getError(), 
    sigma2.getVal(), sigma2.getError(), b2.getVal(), b2.getError(), c2.getVal(), c2.getError());

  fitResults.print(std::cout);

  fitResults.writePythonConfig(outputFileName);

  makeControlPlots(tree, isData, fitResults, outputFileName);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
