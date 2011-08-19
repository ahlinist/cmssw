
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections u1, u2 
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.3 2011/08/18 17:51:30 veelken Exp $
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
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>

#include <vector>
#include <string>

typedef std::vector<std::string> vstring;

void showControlPlot(TCanvas* canvas, 
		     TH1* histogram, const std::string& histogramLegendEntry, TGraph* graph_fit, TGraphErrors* graph_fitErr, 
		     bool showIdGraph, const std::string& yAxisLabel, double yMin, double yMax, 
		     const std::string& outputFileName, const std::string& outputFileLabel)
{
  canvas->Clear();

  TAxis* xAxis = histogram->GetXaxis();
  xAxis->SetTitle("q_{T} / GeV");
  xAxis->SetTitleOffset(1.20);

  TAxis* yAxis = histogram->GetYaxis();
  yAxis->SetTitle(yAxisLabel.data());
  yAxis->SetTitleOffset(1.20);

  histogram->SetTitle("");
  histogram->SetStats(false);
  histogram->SetMaximum(yMax);
  histogram->SetMinimum(yMin);
  histogram->SetMarkerColor(1);
  histogram->SetMarkerStyle(20);
  histogram->Draw("e1p");

  graph_fitErr->SetLineColor(46);
  graph_fitErr->SetLineWidth(1);
  graph_fitErr->SetFillColor(46);
  graph_fitErr->SetFillStyle(3002);
  graph_fitErr->Draw("3");

  graph_fit->SetLineColor(2);
  graph_fit->SetLineWidth(2);
  graph_fit->Draw("L");

  if ( showIdGraph ) {
    int numPoints = graph_fit->GetN();
    TGraph* graph_id = new TGraph(numPoints);
    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      Double_t x, y;
      graph_fit->GetPoint(iPoint, x, y);
      graph_id->SetPoint(iPoint, x, -x);
    }
    graph_id->SetLineColor(8);
    graph_id->SetLineWidth(1);
    graph_id->Draw("L");
  }

  histogram->Draw("e1psame");

  TLegend legend(0.64, 0.64, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(histogram,    histogramLegendEntry.data(), "p");
  legend.AddEntry(graph_fit,    "Fit",                       "l");
  legend.AddEntry(graph_fitErr, "Fit Uncertainty",           "f");
  legend.Draw();

  canvas->Update();

  size_t idx = outputFileName.find_last_of("_cfi.py");
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

double square(double x)
{
  return x*x;
}

void makeControlPlots(TH1* histogram_u1_mean, TH1* histogram_u1_rms, TH1* histogram_u2_mean, TH1* histogram_u2_rms, 
		      const ZllRecoilCorrectionParameterSet& fitResultsfitResults, bool isData, const std::string& outputFileName)
{
  TAxis* xAxis = histogram_u1_mean->GetXaxis();

  int numBins = xAxis->GetNbins();
  const Double_t* binning = xAxis->GetXbins()->GetArray();

  double xStepSize = 1.;
  double xMin = binning[0] + 0.5*xStepSize;
  double xMax = binning[numBins];

  int numPoints = TMath::FloorNint((xMax - xMin)/xStepSize);

  TGraph*       graph_u1Fit        = new TGraph(numPoints);
  TGraphErrors* graph_u1FitErr     = new TGraphErrors(numPoints);
  TGraph*       graph_u1_rmsFit    = new TGraph(numPoints);
  TGraphErrors* graph_u1_rmsFitErr = new TGraphErrors(numPoints);

  TGraph*       graph_u2Fit        = new TGraph(numPoints);
  TGraphErrors* graph_u2FitErr     = new TGraphErrors(numPoints);
  TGraph*       graph_u2_rmsFit    = new TGraph(numPoints);
  TGraphErrors* graph_u2_rmsFitErr = new TGraphErrors(numPoints);

  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*xStepSize;

    double u1_av             = fitResultsfitResults.d() + fitResultsfitResults.k()*x;
    double u1_dUp            = (fitResultsfitResults.d() + fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;    
    double u1_dDown          = (fitResultsfitResults.d() - fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;
    double u1_kUp            = fitResultsfitResults.d() + (fitResultsfitResults.k() + fitResultsfitResults.kErr())*x;
    double u1_kDown          = fitResultsfitResults.d() + (fitResultsfitResults.k() - fitResultsfitResults.kErr())*x;
    
    double u1ErrUp           = u1_av + TMath::Sqrt(square(u1_dUp   - u1_av) + square(u1_kUp   - u1_av));
    double u1ErrDown         = u1_av - TMath::Sqrt(square(u1_dDown - u1_av) + square(u1_kDown - u1_av));

    graph_u1Fit->SetPoint(iPoint, x, u1_av);
    graph_u1FitErr->SetPoint(iPoint, x, 0.5*(u1ErrUp + u1ErrDown));
    graph_u1FitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(u1ErrUp - u1ErrDown));

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
    double u1_rmsErrDown     = u1_rms_av - TMath::Sqrt(square(u1_rms_sigma1Down - u1_rms_av) 
                                                     + square(TMath::Min(u1_rms_b1Up, u1_rms_b1Down) - u1_rms_av) 
                                                     + square(TMath::Min(u1_rms_c1Up, u1_rms_c1Down) - u1_rms_av));
    
    graph_u1_rmsFit->SetPoint(iPoint, x, u1_rms_av);
    graph_u1_rmsFitErr->SetPoint(iPoint, x, 0.5*(u1_rmsErrUp + u1_rmsErrDown));
    graph_u1_rmsFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(u1_rmsErrUp - u1_rmsErrDown));
    
    graph_u2Fit->SetPoint(iPoint, x, 0.);
    graph_u2FitErr->SetPoint(iPoint, x, 0.);
    graph_u2FitErr->SetPointError(iPoint, 0., 1.e0);

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
    double u2_rmsErrDown     = u2_rms_av - TMath::Sqrt(square(u2_rms_sigma2Down - u2_rms_av) 
                                                     + square(TMath::Min(u2_rms_b2Up, u2_rms_b2Down) - u2_rms_av) 
                                                     + square(TMath::Min(u2_rms_c2Up, u2_rms_c2Down) - u2_rms_av));
    
    graph_u2_rmsFit->SetPoint(iPoint, x, u2_rms_av);
    graph_u2_rmsFitErr->SetPoint(iPoint, x, 0.5*(u2_rmsErrUp + u2_rmsErrDown));
    graph_u2_rmsFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(u2_rmsErrUp - u2_rmsErrDown));
  }

  TCanvas* canvas = new TCanvas();
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string histogramLegendEntry = ( isData ) ? "Data" : "Simulation";

  showControlPlot(canvas, 
		  histogram_u1_mean, histogramLegendEntry, graph_u1Fit, graph_u1FitErr,     
		  true, "u_{1} / GeV", -200., +50.,
		  outputFileName, "u1_mean");
  showControlPlot(canvas, 
		  histogram_u1_rms, histogramLegendEntry, graph_u1_rmsFit, graph_u1_rmsFitErr, 
		  false, "rms(u_{1}) / GeV", 0., 25.,
		  outputFileName, "u1_rms");
  showControlPlot(canvas, 
		  histogram_u2_mean, histogramLegendEntry, graph_u2Fit, graph_u2FitErr,     
		  true, "u_{2} / GeV", -25., +25.,
		  outputFileName, "u2_mean");
  showControlPlot(canvas, 
		  histogram_u2_rms, histogramLegendEntry, graph_u2_rmsFit, graph_u2_rmsFitErr, 
		  false, "rms(u_{2}) / GeV", 0., 25.,
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

  Double_t qT_value, u1_value, u2_value, evtWeight_value;
  tree->SetBranchAddress("qT", &qT_value);
  tree->SetBranchAddress("u1", &u1_value);
  tree->SetBranchAddress("u2", &u2_value);
  tree->SetBranchAddress("evtWeight", &evtWeight_value);

  const int numBins = 22;
  double binning[numBins + 1] = { 
    0., 2.5, 5., 7.5, 10., 12.5, 15., 17.5, 20., 22.5, 25., 27.5, 30., 35., 40., 45., 50., 60., 70., 80., 100., 120., 150. 
  };
 
  TH2* histogram_u1 = new TH2D("histogram_u1", "histogram_u1", numBins, binning, 2000, -500., +500.);
  TH2* histogram_u2 = new TH2D("histogram_u2", "histogram_u2", numBins, binning, 2000, -500., +500.);

  int numEntries = tree->GetEntries();
  for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
    tree->GetEntry(iEntry);

    histogram_u1->Fill(qT_value, u1_value, evtWeight_value);
    histogram_u2->Fill(qT_value, u2_value, evtWeight_value);
  }

  TH1* histogram_u1_mean = new TH1D("histogram_u1_mean", "<u_{1}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_u1_rms  = new TH1D("histogram_u1_rms",  "rms(u_{1} - d + k*q_{T}) as function of q_{T}", numBins, binning);
  TH1* histogram_u2_mean = new TH1D("histogram_u2_mean", "<u_{2}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_u2_rms  = new TH1D("histogram_u2_rms",  "rms(u_{2}) as function of q_{T}",               numBins, binning);

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    TString histogramName_u1_proj = Form("%s_py_%i", histogram_u1->GetName(), iBin);
    TH1D* histogram_u1_proj = histogram_u1->ProjectionY(histogramName_u1_proj.Data(), iBin, iBin, "e");
    histogram_u1_mean->SetBinContent(iBin, histogram_u1_proj->GetMean());
    histogram_u1_mean->SetBinError(iBin, histogram_u1_proj->GetMeanError());
    histogram_u1_rms->SetBinContent(iBin, histogram_u1_proj->GetRMS());
    histogram_u1_rms->SetBinError(iBin, histogram_u1_proj->GetRMSError());

    TString histogramName_u2_proj = Form("%s_py_%i", histogram_u2->GetName(), iBin);
    TH1D* histogram_u2_proj = histogram_u2->ProjectionY(histogramName_u2_proj.Data(), iBin, iBin, "e");
    histogram_u2_mean->SetBinContent(iBin, histogram_u2_proj->GetMean());
    histogram_u2_mean->SetBinError(iBin, histogram_u2_proj->GetMeanError());
    histogram_u2_rms->SetBinContent(iBin, histogram_u2_proj->GetRMS());
    histogram_u2_rms->SetBinError(iBin, histogram_u2_proj->GetRMSError());
  }

  std::cout << "starting u1 fit..." << std::endl;
  TF1* f_u1_mean = new TF1("f_u1_mean", "[0]*x", 0., 150.);
  f_u1_mean->SetLineWidth(0);
  f_u1_mean->SetParameter(0, -0.9);
  histogram_u1_mean->Fit(f_u1_mean, "E");

  TF1* f_u1_rms = new TF1("f_u1_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 150.);
  f_u1_rms->SetLineWidth(0);
  f_u1_rms->SetParameter(0, 10.);
  f_u1_rms->SetParameter(0,  5.e-2);
  f_u1_rms->SetParameter(0,  0.);
  histogram_u1_rms->Fit(f_u1_rms, "E");

  std::cout << "starting u2 fit..." << std::endl;
  TF1* f_u2_rms = new TF1("f_u2_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 150.);
  f_u2_rms->SetLineWidth(0);
  f_u2_rms->SetParameter(0, 10.);
  f_u2_rms->SetParameter(0,  5.e-2);
  f_u2_rms->SetParameter(0,  0.);
  histogram_u2_rms->Fit(f_u2_rms, "E");

  std::cout << "done." << std::endl;

  ZllRecoilCorrectionParameterSet fitResults(
    0., 0., 
    f_u1_mean->GetParameter(0), f_u1_mean->GetParError(0), 
    f_u1_rms->GetParameter(0), f_u1_rms->GetParError(0), 
    f_u1_rms->GetParameter(1), f_u1_rms->GetParError(1), 
    f_u1_rms->GetParameter(2), f_u1_rms->GetParError(2),
    f_u2_rms->GetParameter(0), f_u2_rms->GetParError(0), 
    f_u2_rms->GetParameter(1), f_u2_rms->GetParError(1), 
    f_u2_rms->GetParameter(2), f_u2_rms->GetParError(2));

  fitResults.print(std::cout);

  fitResults.writePythonConfig(outputFileName);

  makeControlPlots(histogram_u1_mean, histogram_u1_rms, histogram_u2_mean, histogram_u2_rms, fitResults, isData, outputFileName);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
