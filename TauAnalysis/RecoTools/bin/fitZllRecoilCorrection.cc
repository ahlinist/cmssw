
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections uParl, uPerp
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.8 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.8 2011/10/19 14:41:09 veelken Exp $
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
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>

#include <vector>
#include <string>

typedef std::vector<std::string> vstring;

double square(double x)
{
  return x*x;
}

void showControlPlot(TCanvas* canvas, 
		     TH1* dummyHistogram, 
		     TGraphAsymmErrors* graph, const std::string& legendEntry, double legendX0, double legendY0,
		     TGraph* graph_fit, TGraphErrors* graph_fitErr, 
		     bool showIdLine, bool showConstLine, const std::string& yAxisLabel, double yMin, double yMax, double yDiffMax, 
		     const std::string& outputFileName, const std::string& outputFileLabel)
{
  canvas->SetLogy(false);
  canvas->Clear();
  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  TPad* topPad = new TPad("topPad", "topPad", 0.00, 0.35, 1.00, 1.00);
  topPad->SetFillColor(10);
  topPad->SetTopMargin(0.04);
  topPad->SetLeftMargin(0.15);
  topPad->SetBottomMargin(0.03);
  topPad->SetRightMargin(0.05);

  TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0.00, 0.00, 1.00, 0.35);
  bottomPad->SetFillColor(10);
  bottomPad->SetTopMargin(0.02);
  bottomPad->SetLeftMargin(0.15);
  bottomPad->SetBottomMargin(0.24);
  bottomPad->SetRightMargin(0.05);

  canvas->cd();
  topPad->Draw();
  topPad->cd();

  TAxis* xAxis = dummyHistogram->GetXaxis();
  xAxis->SetLabelColor(10);
  xAxis->SetTitleColor(10);
    
  TAxis* yAxis = dummyHistogram->GetYaxis();
  yAxis->SetTitle(yAxisLabel.data());
  yAxis->SetTitleOffset(1.20);
  yAxis->SetTitleSize(0.06);

  dummyHistogram->SetTitle("");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMaximum(yMax);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMarkerColor(1);
  dummyHistogram->SetMarkerStyle(20);
  dummyHistogram->Draw("axis");

  graph_fitErr->SetLineColor(2);
  graph_fitErr->SetLineWidth(0);
  graph_fitErr->SetFillColor(46);
  graph_fitErr->SetFillStyle(3002);
  graph_fitErr->Draw("3");

  graph_fit->SetLineColor(2);
  graph_fit->SetLineWidth(2);
  graph_fit->Draw("L");

  if ( showIdLine || showConstLine ) {
    int numPoints = graph_fit->GetN();
    TGraph* graph_line = new TGraph(numPoints);
    for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
      Double_t x, y;
      graph_fit->GetPoint(iPoint, x, y);
      if      ( showIdLine    ) graph_line->SetPoint(iPoint, x,  -x);
      else if ( showConstLine ) graph_line->SetPoint(iPoint, x, 1.0);
      else assert(0);
    }
    graph_line->SetLineColor(8);
    graph_line->SetLineWidth(1);
    graph_line->Draw("L");
  }

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1);
  graph->SetMarkerColor(1);
  graph->SetLineColor(1);
  graph->SetLineWidth(1);
  graph->Draw("P");

  TLegend legend(legendX0, legendY0, legendX0 + 0.25, legendY0 + 0.25, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(graph,        legendEntry.data(), "p");
  legend.AddEntry(graph_fit,    "Fit",              "l");
  legend.AddEntry(graph_fitErr, "Fit Uncertainty",  "f");
  legend.Draw();

  canvas->cd();
  bottomPad->Draw();
  bottomPad->cd();

  int numPoints = graph->GetN();
  TGraphAsymmErrors* graphDiff = new TGraphAsymmErrors(numPoints);
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    Double_t x, xErrUp, xErrDown, y, yErr;
    graph->GetPoint(iPoint, x, y);
    xErrUp = graph->GetErrorXhigh(iPoint);
    xErrDown = graph->GetErrorXlow(iPoint);
    yErr = graph->GetErrorY(iPoint);

    int numPoints_fit = graph_fit->GetN();
    Double_t y_fit_matched = 0.;
    Double_t yErr_fit_matched = 1.e+6;
    Double_t dxMin = 1.e+6;
    for ( int iPoint_fit = 0; iPoint_fit < numPoints_fit; ++iPoint_fit ) {
      Double_t x_fit, y_fit;
      graph_fit->GetPoint(iPoint_fit, x_fit, y_fit);
      Double_t dx = x_fit - x;
      if ( TMath::Abs(dx) < dxMin ) {
	y_fit_matched = y_fit;
	yErr_fit_matched = graph_fitErr->GetErrorY(iPoint_fit);
	dxMin = TMath::Abs(dx);
      }
    }

    //std::cout << "x = " << x << " (dxMin = " << dxMin << "): y = " << y << ", y_fit_matched = " << y_fit_matched << std::endl;

    Double_t yErr_diff = TMath::Sqrt(square(yErr) + square(yErr_fit_matched));
    
    graphDiff->SetPoint(iPoint, x, y - y_fit_matched);
    graphDiff->SetPointError(iPoint, xErrDown, xErrUp, yErr_diff, yErr_diff);
  }

  TAxis* xAxisDiff = graphDiff->GetXaxis();
  xAxisDiff->SetTitle("q_{T} / GeV");
  xAxisDiff->SetTitleOffset(1.20);
  xAxisDiff->SetTitleSize(0.08);
  xAxisDiff->SetLabelOffset(0.02);
  xAxisDiff->SetLabelSize(0.08);
  xAxisDiff->SetTickLength(0.055);

  TAxis* yAxisDiff = graphDiff->GetYaxis();
  yAxisDiff->SetTitle("Measured - Fit");
  yAxisDiff->SetTitleOffset(0.85);
  yAxisDiff->SetNdivisions(505);
  yAxisDiff->CenterTitle();
  yAxisDiff->SetTitleSize(0.08);
  yAxisDiff->SetLabelSize(0.08);
  yAxisDiff->SetTickLength(0.04);

  graphDiff->SetTitle("");
  graphDiff->SetMaximum(+yDiffMax);
  graphDiff->SetMinimum(-yDiffMax);
  graphDiff->SetMarkerStyle(20);
  graphDiff->SetMarkerSize(1);
  graphDiff->SetMarkerColor(1);
  graphDiff->SetLineColor(1);
  graphDiff->SetLineWidth(1);
  graphDiff->Draw("AP");

  canvas->Update();

  size_t idx = outputFileName.find_last_of("_cfi.py");
  std::string outputFileName_plot = std::string(outputFileName, 0, idx - (strlen("_cfi.py") - 1));
  outputFileName_plot.append("_").append(outputFileLabel);
  outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

void makeControlPlots(TH1* dummyHistogram,
		      TGraphAsymmErrors* graph_uParl_mean, TGraphAsymmErrors* graph_uParl_div_qT_mean, 
		      TGraphAsymmErrors* graph_uParl_rms, 
		      TGraphAsymmErrors* graph_uPerp_mean, 
		      TGraphAsymmErrors* graph_uPerp_rms, 
		      const ZllRecoilCorrectionParameterSet& fitResultsfitResults, bool isData, const std::string& outputFileName)
{
  TAxis* xAxis = dummyHistogram->GetXaxis();

  double xStepSize = 1.;
  double xMin = xAxis->GetXmin() + 0.5*xStepSize;
  double xMax = xAxis->GetXmax();

  int numPoints = TMath::FloorNint((xMax - xMin)/xStepSize);

  TGraph*       graph_uParlFit           = new TGraph(numPoints);
  TGraphErrors* graph_uParlFitErr        = new TGraphErrors(numPoints);
  TGraph*       graph_uParl_div_qTfit    = new TGraph(numPoints);
  TGraphErrors* graph_uParl_div_qTfitErr = new TGraphErrors(numPoints);
  TGraph*       graph_uParl_rmsFit       = new TGraph(numPoints);
  TGraphErrors* graph_uParl_rmsFitErr    = new TGraphErrors(numPoints);

  TGraph*       graph_uPerpFit           = new TGraph(numPoints);
  TGraphErrors* graph_uPerpFitErr        = new TGraphErrors(numPoints);
  TGraph*       graph_uPerp_rmsFit       = new TGraph(numPoints);
  TGraphErrors* graph_uPerp_rmsFitErr    = new TGraphErrors(numPoints);

  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*xStepSize;

    double uParl_av             = fitResultsfitResults.d() 
                                 + fitResultsfitResults.k1()*x
                                  *0.5*(1.0 - TMath::Erf(-fitResultsfitResults.k2()*TMath::Power(x, fitResultsfitResults.k3())));
    double uParl_dUp            = uParl_av + fitResultsfitResults.dErr();
    double uParl_dDown          = uParl_av - fitResultsfitResults.dErr();
    double d_uParl_by_k1        = 0.5*(1.0 - TMath::Erf(-fitResultsfitResults.k2()*TMath::Power(x, fitResultsfitResults.k3())))*x;
    double uParl_k1Up           = uParl_av + TMath::Abs(d_uParl_by_k1)*fitResultsfitResults.k1Err();
    double uParl_k1Down         = uParl_av - TMath::Abs(d_uParl_by_k1)*fitResultsfitResults.k1Err();
    double d_uParl_by_k2        = (1./TMath::Sqrt(TMath::Pi()))
                                 *TMath::Exp(-square(fitResultsfitResults.k2()*TMath::Power(x, fitResultsfitResults.k3())))
                                 *fitResultsfitResults.k1()*TMath::Power(x, fitResultsfitResults.k3() + 1.);
    double uParl_k2Up           = uParl_av + TMath::Abs(d_uParl_by_k2)*fitResultsfitResults.k2Err();
    double uParl_k2Down         = uParl_av - TMath::Abs(d_uParl_by_k2)*fitResultsfitResults.k2Err();
    double d_uParl_by_k3        = (1./TMath::Sqrt(TMath::Pi()))
                                 *TMath::Exp(-square(fitResultsfitResults.k2()*TMath::Power(x, fitResultsfitResults.k3())))
                                 *fitResultsfitResults.k1()*fitResultsfitResults.k2()*TMath::Power(x, fitResultsfitResults.k3() + 1.)
                                 *TMath::Log(x);
    double uParl_k3Up           = uParl_av + TMath::Abs(d_uParl_by_k3)*fitResultsfitResults.k3Err();
    double uParl_k3Down         = uParl_av - TMath::Abs(d_uParl_by_k3)*fitResultsfitResults.k3Err();
    
    double uParlErrUp           = uParl_av 
                                 + TMath::Sqrt(square(uParl_dUp - uParl_av) 
                                              + square(uParl_k1Up - uParl_av)
                                              + square(uParl_k2Up - uParl_av)
                                              + square(uParl_k3Up - uParl_av));
    double uParlErrDown         = uParl_av 
                                 - TMath::Sqrt(square(uParl_dDown - uParl_av) 
                                              + square(uParl_k1Down - uParl_av)
                                              + square(uParl_k2Down - uParl_av)
                                              + square(uParl_k3Down - uParl_av));

    graph_uParlFit->SetPoint(iPoint, x, uParl_av);
    graph_uParlFitErr->SetPoint(iPoint, x, 0.5*(uParlErrUp + uParlErrDown));
    graph_uParlFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(uParlErrUp - uParlErrDown));
    
    graph_uParl_div_qTfit->SetPoint(iPoint, x, -uParl_av/x);
    graph_uParl_div_qTfitErr->SetPoint(iPoint, x, -0.5*(uParlErrUp + uParlErrDown)/x);
    graph_uParl_div_qTfitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(uParlErrUp - uParlErrDown)/x);

    double uParl_rms_av         = fitResultsfitResults.sigma1()*(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double uParl_rms_sigma1Up   = (fitResultsfitResults.sigma1() + fitResultsfitResults.sigma1Err())
                              *(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double uParl_rms_sigma1Down = (fitResultsfitResults.sigma1() - fitResultsfitResults.sigma1Err())
                              *(1. + fitResultsfitResults.b1()*x + fitResultsfitResults.c1()*x*x);
    double uParl_rms_b1Up       = fitResultsfitResults.sigma1()
                              *(1. + (fitResultsfitResults.b1() + fitResultsfitResults.b1Err())*x + fitResultsfitResults.c1()*x*x);
    double uParl_rms_b1Down     = fitResultsfitResults.sigma1()
                              *(1. + (fitResultsfitResults.b1() - fitResultsfitResults.b1Err())*x + fitResultsfitResults.c1()*x*x);
    double uParl_rms_c1Up       = fitResultsfitResults.sigma1()
                              *(1. + fitResultsfitResults.b1()*x + (fitResultsfitResults.c1() + fitResultsfitResults.c1Err())*x*x);
    double uParl_rms_c1Down     = fitResultsfitResults.sigma1()
                              *(1. + fitResultsfitResults.b1()*x + (fitResultsfitResults.c1() - fitResultsfitResults.c1Err())*x*x);
    
    double uParl_rmsErrUp       = uParl_rms_av + TMath::Sqrt(square(uParl_rms_sigma1Up   - uParl_rms_av) 
                                                           + square(TMath::Max(uParl_rms_b1Up, uParl_rms_b1Down) - uParl_rms_av) 
                                                           + square(TMath::Max(uParl_rms_c1Up, uParl_rms_c1Down) - uParl_rms_av));
    double uParl_rmsErrDown     = uParl_rms_av - TMath::Sqrt(square(uParl_rms_sigma1Down - uParl_rms_av) 
                                                           + square(TMath::Min(uParl_rms_b1Up, uParl_rms_b1Down) - uParl_rms_av) 
                                                           + square(TMath::Min(uParl_rms_c1Up, uParl_rms_c1Down) - uParl_rms_av));
    
    graph_uParl_rmsFit->SetPoint(iPoint, x, uParl_rms_av);
    graph_uParl_rmsFitErr->SetPoint(iPoint, x, 0.5*(uParl_rmsErrUp + uParl_rmsErrDown));
    graph_uParl_rmsFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(uParl_rmsErrUp - uParl_rmsErrDown));
    
    graph_uPerpFit->SetPoint(iPoint, x, 0.);
    graph_uPerpFitErr->SetPoint(iPoint, x, 0.);
    graph_uPerpFitErr->SetPointError(iPoint, 0., 1.e0);

    double uPerp_rms_av         = fitResultsfitResults.sigma2()*(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double uPerp_rms_sigma2Up   = (fitResultsfitResults.sigma2() + fitResultsfitResults.sigma2Err())
                              *(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double uPerp_rms_sigma2Down = (fitResultsfitResults.sigma2() - fitResultsfitResults.sigma2Err())
                              *(1. + fitResultsfitResults.b2()*x + fitResultsfitResults.c2()*x*x);
    double uPerp_rms_b2Up       = fitResultsfitResults.sigma2()
                              *(1. + (fitResultsfitResults.b2() + fitResultsfitResults.b2Err())*x + fitResultsfitResults.c2()*x*x);
    double uPerp_rms_b2Down     = fitResultsfitResults.sigma2()
                              *(1. + (fitResultsfitResults.b2() - fitResultsfitResults.b2Err())*x + fitResultsfitResults.c2()*x*x);
    double uPerp_rms_c2Up       = fitResultsfitResults.sigma2()
                              *(1. + fitResultsfitResults.b2()*x + (fitResultsfitResults.c2() + fitResultsfitResults.c2Err())*x*x);
    double uPerp_rms_c2Down     = fitResultsfitResults.sigma2()
                              *(1. + fitResultsfitResults.b2()*x + (fitResultsfitResults.c2() - fitResultsfitResults.c2Err())*x*x);
    
    double uPerp_rmsErrUp       = uPerp_rms_av + TMath::Sqrt(square(uPerp_rms_sigma2Up   - uPerp_rms_av) 
                                                           + square(TMath::Max(uPerp_rms_b2Up, uPerp_rms_b2Down) - uPerp_rms_av) 
                                                           + square(TMath::Max(uPerp_rms_c2Up, uPerp_rms_c2Down) - uPerp_rms_av));
    double uPerp_rmsErrDown     = uPerp_rms_av - TMath::Sqrt(square(uPerp_rms_sigma2Down - uPerp_rms_av) 
                                                           + square(TMath::Min(uPerp_rms_b2Up, uPerp_rms_b2Down) - uPerp_rms_av) 
                                                           + square(TMath::Min(uPerp_rms_c2Up, uPerp_rms_c2Down) - uPerp_rms_av));
    
    graph_uPerp_rmsFit->SetPoint(iPoint, x, uPerp_rms_av);
    graph_uPerp_rmsFitErr->SetPoint(iPoint, x, 0.5*(uPerp_rmsErrUp + uPerp_rmsErrDown));
    graph_uPerp_rmsFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(uPerp_rmsErrUp - uPerp_rmsErrDown));
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 1000);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string legendEntry = ( isData ) ? "Data" : "Simulation";

  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_mean, legendEntry, 0.64, 0.68, graph_uParlFit, graph_uParlFitErr,     
		  true, false, "u_{parl} / GeV", -600., +100., 10.,
		  outputFileName, "uParl_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_div_qT_mean, legendEntry, 0.64, 0.68, graph_uParl_div_qTfit, graph_uParl_div_qTfitErr,     
		  false, true, "u_{parl}/q_{T}", 0., 1.5, 0.25,
		  outputFileName, "uParl_div_qT_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_rms, legendEntry,  0.21, 0.64, graph_uParl_rmsFit, graph_uParl_rmsFitErr, 
		  false, false, "rms(u_{parl}) / GeV", 0., 50., 10., 
		  outputFileName, "uParl_rms");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uPerp_mean, legendEntry, 0.64, 0.68, graph_uPerpFit, graph_uPerpFitErr,     
		  false, false, "u_{perp} / GeV", -25., +25., 2.5,
		  outputFileName, "uPerp_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uPerp_rms, legendEntry, 0.21, 0.64, graph_uPerp_rmsFit, graph_uPerp_rmsFitErr, 
		  false, false, "rms(u_{perp}) / GeV", 0., 50., 10.,
		  outputFileName, "uPerp_rms");
  
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

  std::string refBranchName = cfgFitZllRecoilCorrection.getParameter<std::string>("refBranchName");
  std::string projParlBranchName = cfgFitZllRecoilCorrection.getParameter<std::string>("projParlBranchName");
  std::string projPerpBranchName = cfgFitZllRecoilCorrection.getParameter<std::string>("projPerpBranchName");

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

  //tree->Scan("qT:uParl:uPerp:rT_ii:vParl_ii:vPerp_ii:evtWeight");

  Double_t refT_value, projParl_value, projPerp_value, evtWeight_value;
  tree->SetBranchAddress(refBranchName.data(), &refT_value);
  tree->SetBranchAddress(projParlBranchName.data(), &projParl_value);
  tree->SetBranchAddress(projPerpBranchName.data(), &projPerp_value);
  tree->SetBranchAddress("evtWeight", &evtWeight_value);

  const int numBins = 37;
  double binning[numBins + 1] = { 
    0., 2.5, 5., 7.5, 10., 12.5, 15., 17.5, 20., 22.5, 25., 27.5, 30., 35., 40., 45., 50., 
    60., 70., 80., 90., 100., 110., 120., 130., 140., 150., 160., 170., 180., 200., 220., 240., 260., 300., 350., 400., 500.
  };
 
  TH2* histogram_uParl = new TH2D("histogram_uParl", "histogram_uParl", numBins, binning, 2000, -500., +500.);
  TH2* histogram_uPerp = new TH2D("histogram_uPerp", "histogram_uPerp", numBins, binning, 2000, -500., +500.);

  TH1* histogram_qT    = new TH1D("histogram_qT",    "histogram_qT",    1000, 0., 500.);

  int numEntries = tree->GetEntries();
  for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
    tree->GetEntry(iEntry);

    histogram_uParl->Fill(refT_value, projParl_value, evtWeight_value);
    histogram_uPerp->Fill(refT_value, projPerp_value, evtWeight_value);

    histogram_qT->Fill(refT_value, evtWeight_value);
  }

  TGraphAsymmErrors* graph_uParl_mean = new TGraphAsymmErrors(numBins);
  graph_uParl_mean->SetName("graph_uParl_mean");
  graph_uParl_mean->SetTitle("<u_{parl}> as function of q_{T}");  
  TGraphAsymmErrors* graph_uParl_div_qT_mean = new TGraphAsymmErrors(numBins);
  graph_uParl_div_qT_mean->SetName("graph_uParl_div_qT_mean");
  graph_uParl_div_qT_mean->SetTitle("<u_{parl}>/q_{T} as function of q_{T}");
  TGraphAsymmErrors* graph_uParl_rms  = new TGraphAsymmErrors(numBins);
  graph_uParl_rms->SetName("graph_uParl_rms");
  graph_uParl_rms->SetTitle("rms(u_{perp} - d + k*q_{T}) as function of q_{T}");
  TGraphAsymmErrors* graph_uPerp_mean = new TGraphAsymmErrors(numBins); 
  graph_uPerp_mean->SetName("graph_uPerp_mean");
  graph_uPerp_mean->SetTitle("<u_{parl}> as function of q_{T}");
  TGraphAsymmErrors* graph_uPerp_rms  =  new TGraphAsymmErrors(numBins);
  graph_uPerp_rms->SetName("graph_uPerp_rms");
  graph_uPerp_rms->SetTitle("rms(u_{perp}) as function of q_{T}");

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double qTmin = histogram_uParl->GetXaxis()->GetBinLowEdge(iBin);
    double qTmax = histogram_uParl->GetXaxis()->GetBinUpEdge(iBin);

    int binLowIndex = histogram_qT->FindBin(qTmin);
    int binUpIndex  = histogram_qT->FindBin(qTmax);
    histogram_qT->GetXaxis()->SetRange(binLowIndex, binUpIndex);

    double x        = histogram_qT->GetMean();
    double xErrUp   = qTmax - x;
    double xErrDown = x - qTmin;

    TString histogramName_uParl_proj = Form("%s_py_%i", histogram_uParl->GetName(), iBin);
    TH1D* histogram_uParl_proj = histogram_uParl->ProjectionY(histogramName_uParl_proj.Data(), iBin, iBin, "e");
    double y_uParl_mean = histogram_uParl_proj->GetMean();
    double yErr_uParl_mean = histogram_uParl_proj->GetMeanError();
    graph_uParl_mean->SetPoint(iBin - 1, x, y_uParl_mean);
    graph_uParl_mean->SetPointError(iBin - 1, xErrDown, xErrUp, yErr_uParl_mean, yErr_uParl_mean);
    if ( x > 0. ) {
      graph_uParl_div_qT_mean->SetPoint(iBin - 1, x, -y_uParl_mean/x);
      graph_uParl_div_qT_mean->SetPointError(iBin - 1, xErrDown, xErrUp, yErr_uParl_mean/x, yErr_uParl_mean/x);
    }
    double y_uParl_rms = histogram_uParl_proj->GetRMS();
    double yErr_uParl_rms = histogram_uParl_proj->GetRMSError();
    graph_uParl_rms->SetPoint(iBin - 1, x, y_uParl_rms);
    graph_uParl_rms->SetPointError(iBin - 1, xErrDown, xErrUp, yErr_uParl_rms, yErr_uParl_rms);

    TString histogramName_uPerp_proj = Form("%s_py_%i", histogram_uPerp->GetName(), iBin);
    TH1D* histogram_uPerp_proj = histogram_uPerp->ProjectionY(histogramName_uPerp_proj.Data(), iBin, iBin, "e");
    double y_uPerp_mean = histogram_uPerp_proj->GetMean();
    double yErr_uPerp_mean = histogram_uPerp_proj->GetMeanError();
    graph_uPerp_mean->SetPoint(iBin - 1, x, y_uPerp_mean);
    graph_uPerp_mean->SetPointError(iBin - 1, xErrDown, xErrUp, yErr_uPerp_mean, yErr_uPerp_mean);
    double y_uPerp_rms = histogram_uPerp_proj->GetRMS();
    double yErr_uPerp_rms = histogram_uPerp_proj->GetRMSError();
    graph_uPerp_rms->SetPoint(iBin - 1, x, y_uPerp_rms);
    graph_uPerp_rms->SetPointError(iBin - 1, xErrDown, xErrUp, yErr_uPerp_rms, yErr_uPerp_rms);
  }

  // reset x-axis range selection 
  histogram_qT->GetXaxis()->SetRange(1, 0);

  std::cout << "starting uParl fit..." << std::endl;
  TF1* f_uParl_mean = new TF1("f_uParl_mean", "[0]*x*0.5*(1.0 - TMath::Erf(-[1]*TMath::Power(x, [2])))", 0., 500.);
  f_uParl_mean->SetLineWidth(0);
  f_uParl_mean->SetParameter(0, -1.0);
  f_uParl_mean->SetParameter(1,  5.e-2);
  f_uParl_mean->SetParameter(2,  1.0);
  graph_uParl_mean->Fit(f_uParl_mean, "E");
  //graph_uParl_mean->Fit(f_uParl_mean, "W");

  TF1* f_uParl_rms = new TF1("f_uParl_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 500.);
  f_uParl_rms->SetLineWidth(0);
  f_uParl_rms->SetParameter(0, 10.);
  f_uParl_rms->SetParameter(0,  5.e-2);
  f_uParl_rms->SetParameter(0,  0.);
  graph_uParl_rms->Fit(f_uParl_rms, "E");

  std::cout << "starting uPerp fit..." << std::endl;
  TF1* f_uPerp_rms = new TF1("f_uPerp_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 500.);
  f_uPerp_rms->SetLineWidth(0);
  f_uPerp_rms->SetParameter(0, 10.);
  f_uPerp_rms->SetParameter(0,  5.e-2);
  f_uPerp_rms->SetParameter(0,  0.);
  graph_uPerp_rms->Fit(f_uPerp_rms, "E");

  std::cout << "done." << std::endl;

  ZllRecoilCorrectionParameterSet fitResults(
    0., 0., 
    f_uParl_mean->GetParameter(0), f_uParl_mean->GetParError(0), 
    f_uParl_mean->GetParameter(1), f_uParl_mean->GetParError(1), 
    f_uParl_mean->GetParameter(2), f_uParl_mean->GetParError(2), 
    f_uParl_rms->GetParameter(0), f_uParl_rms->GetParError(0), 
    f_uParl_rms->GetParameter(1), f_uParl_rms->GetParError(1), 
    f_uParl_rms->GetParameter(2), f_uParl_rms->GetParError(2),
    f_uPerp_rms->GetParameter(0), f_uPerp_rms->GetParError(0), 
    f_uPerp_rms->GetParameter(1), f_uPerp_rms->GetParError(1), 
    f_uPerp_rms->GetParameter(2), f_uPerp_rms->GetParError(2));

  fitResults.print(std::cout);

  fitResults.writePythonConfig(outputFileName);

  TH1* dummyHistogram = new TH1D("dummyHistogram", "dummyHistogram", 50, 0., 500.);
  makeControlPlots(dummyHistogram,
		   graph_uParl_mean, graph_uParl_div_qT_mean, graph_uParl_rms, graph_uPerp_mean, graph_uPerp_rms, 
		   fitResults, isData, outputFileName);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
