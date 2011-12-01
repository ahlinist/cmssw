
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections uParl, uPerp
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.10 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.10 2011/11/22 10:04:51 veelken Exp $
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
#include "TauAnalysis/RecoTools/bin/fitZllRecoilCorrectionAuxFunctions.h"

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

void showControlPlot(TCanvas* canvas, 
		     TH1* dummyHistogram, 
		     TGraphAsymmErrors* graph, const std::string& legendEntry, double legendX0, double legendY0,
		     TGraph* graph_fit, TGraphAsymmErrors* graph_fitErr, 
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

  TH1* dummyHistogram_top = (TH1*)dummyHistogram->Clone("dummyHistogram_top");

  TAxis* xAxis_top = dummyHistogram_top->GetXaxis();
  xAxis_top->SetLabelColor(10);
  xAxis_top->SetTitleColor(10);
    
  TAxis* yAxis_top = dummyHistogram_top->GetYaxis();
  yAxis_top->SetTitle(yAxisLabel.data());
  yAxis_top->SetTitleOffset(1.20);
  yAxis_top->SetTitleSize(0.06);

  dummyHistogram_top->SetTitle("");
  dummyHistogram_top->SetStats(false);
  dummyHistogram_top->SetMaximum(yMax);
  dummyHistogram_top->SetMinimum(yMin);
  dummyHistogram_top->SetMarkerColor(1);
  dummyHistogram_top->SetMarkerStyle(20);
  dummyHistogram_top->Draw("axis");

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

  TLegend legend(legendX0, legendY0, legendX0 + 0.30, legendY0 + 0.285, "", "brNDC"); 
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

  TH1* dummyHistogram_bottom = (TH1*)dummyHistogram->Clone("dummyHistogram_bottom");

  TAxis* xAxis_bottom = dummyHistogram_bottom->GetXaxis();
  xAxis_bottom->SetTitle("q_{T} / GeV");
  xAxis_bottom->SetTitleOffset(1.20);
  xAxis_bottom->SetTitleSize(0.08);
  xAxis_bottom->SetLabelOffset(0.02);
  xAxis_bottom->SetLabelSize(0.08);
  xAxis_bottom->SetTickLength(0.055);

  TAxis* yAxis_bottom = dummyHistogram_bottom->GetYaxis();
  yAxis_bottom->SetTitle("Measured - Fit");
  yAxis_bottom->SetTitleOffset(0.85);
  yAxis_bottom->SetNdivisions(505);
  yAxis_bottom->CenterTitle();
  yAxis_bottom->SetTitleSize(0.08);
  yAxis_bottom->SetLabelSize(0.08);
  yAxis_bottom->SetTickLength(0.04);

  dummyHistogram_bottom->SetTitle("");
  dummyHistogram_bottom->SetStats(false);
  dummyHistogram_bottom->SetMaximum(+yDiffMax);
  dummyHistogram_bottom->SetMinimum(-yDiffMax);
  dummyHistogram_bottom->SetMarkerColor(1);
  dummyHistogram_bottom->SetMarkerStyle(20);
  dummyHistogram_bottom->Draw("axis");

  graphDiff->SetMarkerStyle(20);
  graphDiff->SetMarkerSize(1);
  graphDiff->SetMarkerColor(1);
  graphDiff->SetLineColor(1);
  graphDiff->SetLineWidth(1);
  graphDiff->Draw("P");

  canvas->Update();

  size_t idx = outputFileName.find_last_of("_cfi.py");
  std::string outputFileName_plot = std::string(outputFileName, 0, idx - (strlen("_cfi.py") - 1));
  outputFileName_plot.append("_").append(outputFileLabel);
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());

  delete dummyHistogram_top;
  delete topPad;
  delete dummyHistogram_bottom;
  delete bottomPad;
}

void makeControlPlots(TH1* dummyHistogram,
		      TGraphAsymmErrors* graph_uParl_mean, TF1* f_uParl_mean,
		      TGraphAsymmErrors* graph_uParl_div_qT_mean, TF1* f_uParl_div_qT_mean, 
		      TGraphAsymmErrors* graph_uParl_rms, TF1* f_uParl_rms,
		      TGraphAsymmErrors* graph_uPerp_mean, TF1* f_uPerp_mean,
		      TGraphAsymmErrors* graph_uPerp_rms, TF1* f_uPerp_rms,
		      bool isData, const std::string& outputFileName)
{
  TAxis* xAxis = dummyHistogram->GetXaxis();
  double xMin = xAxis->GetXmin();
  double xMax = xAxis->GetXmax();
  double dx = 1.;

  TGraphAsymmErrors* graph_uParlFit = 
    makeGraph_fitValue("graph_uParlFit", "", 
		       f_uParl_mean, xMin, xMax, dx);
  TGraphAsymmErrors* graph_uParlFitErr = 
    makeGraph_fitUncertainty("graph_uParlFitErr", "", 
			     f_uParl_mean, makeFitUncertaintyFunctions(f_uParl_mean), xMin, xMax, dx);
  TGraphAsymmErrors* graph_uParl_div_qTfit =
    makeGraph_fitValue("graph_uParl_div_qTfit", "",
		       f_uParl_div_qT_mean, xMin, xMax, dx);
  TGraphAsymmErrors* graph_uParl_div_qTfitErr = 
    makeGraph_fitUncertainty("graph_uParl_div_qTfitErr", "", 
			     f_uParl_div_qT_mean, makeFitUncertaintyFunctions(f_uParl_div_qT_mean), xMin, xMax, dx);
  TGraphAsymmErrors* graph_uParl_rmsFit =
    makeGraph_fitValue("graph_uParl_rmsFit", "", 
		       f_uParl_rms, xMin, xMax, dx);
  TGraphAsymmErrors* graph_uParl_rmsFitErr = 
    makeGraph_fitUncertainty("graph_uParl_rmsFitErr", "", 
			     f_uParl_rms, makeFitUncertaintyFunctions(f_uParl_rms), xMin, xMax, dx);
  
  TGraphAsymmErrors* graph_uPerpFit = 
    makeGraph_fitValue("graph_uPerpFit", "", 
		       f_uPerp_mean, xMin, xMax, dx);
  TGraphAsymmErrors* graph_uPerpFitErr = 
    makeGraph_fitUncertainty("graph_uPerpFitErr", "", 
			     f_uPerp_mean, makeFitUncertaintyFunctions(f_uPerp_mean), xMin, xMax, dx);
  TGraphAsymmErrors* graph_uPerp_rmsFit =
    makeGraph_fitValue("graph_uPerp_rmsFit", "", 
		       f_uPerp_rms, xMin, xMax, dx);
  TGraphAsymmErrors* graph_uPerp_rmsFitErr = 
    makeGraph_fitUncertainty("graph_uPerp_rmsFitErr", "", 
			     f_uPerp_rms, makeFitUncertaintyFunctions(f_uPerp_rms), xMin, xMax, dx);

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 1000);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string legendEntry = ( isData ) ? "Data" : "Simulation";

  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_mean, legendEntry, 0.64, 0.665, graph_uParlFit, graph_uParlFitErr,     
		  true, false, "u_{#parallel} / GeV", -600., +100., 10.,
		  outputFileName, "uParl_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_div_qT_mean, legendEntry, 0.64, 0.165, graph_uParl_div_qTfit, graph_uParl_div_qTfitErr,     
		  false, true, "u_{#parallel} /q_{T}", 0.4, 1.2, 0.10,
		  outputFileName, "uParl_div_qT_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uParl_rms, legendEntry,  0.19, 0.64, graph_uParl_rmsFit, graph_uParl_rmsFitErr, 
		  false, false, "rms(u_{#parallel} ) / GeV", 0., 50., 10., 
		  outputFileName, "uParl_rms");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uPerp_mean, legendEntry, 0.64, 0.665, graph_uPerpFit, graph_uPerpFitErr,     
		  false, false, "u_{#perp}  / GeV", -25., +25., 2.5,
		  outputFileName, "uPerp_mean");
  showControlPlot(canvas, 
		  dummyHistogram, graph_uPerp_rms, legendEntry, 0.19, 0.64, graph_uPerp_rmsFit, graph_uPerp_rmsFitErr, 
		  false, false, "rms(u_{#perp}  ) / GeV", 0., 50., 10.,
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

  TGraphAsymmErrors* graph_uParl_mean = 
    makeGraph_mean("graph_uParl_mean", "<u_{parl}> as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uParl_div_qT_mean = 
    makeGraph_uParl_div_qT("graph_uParl_div_qT_mean", "<u_{parl}>/q_{T} as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uParl_rms = 
    makeGraph_rms("graph_uParl_rms", "rms(u_{perp} - d + k*q_{T}) as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uPerp_mean = 
    makeGraph_mean("graph_uPerp_mean", "<u_{parl}> as function of q_{T}", histogram_uPerp, histogram_qT);
  TGraphAsymmErrors* graph_uPerp_rms =  
    makeGraph_rms("graph_uPerp_rms", "rms(u_{perp}) as function of q_{T}", histogram_uPerp, histogram_qT);

  std::cout << "starting uParl fit..." << std::endl;
  TF1* f_uParl_mean = fitGraph_uParl_mean("f_uParl_mean", graph_uParl_mean, 0., 500.);
  TF1* f_uParl_div_qT_mean = fitGraph_uParl_dix_qT("f_uParl_div_qT_mean", graph_uParl_div_qT_mean, 0., 500.);
  TF1* f_uParl_rms = fitGraph_uParl_rms("f_uParl_rms", graph_uParl_rms, 0., 500.);

  std::cout << "starting uPerp fit..." << std::endl;
  TF1* f_uPerp_mean = new TF1("f_uPerp_mean", "[0]", 0., 500.);
  f_uPerp_mean->SetParameter(0, 0.);
  f_uPerp_mean->SetParError(0, 1.);
  TF1* f_uPerp_rms = fitGraph_uPerp_rms("f_uPerp_rms", graph_uPerp_rms, 0., 500.);

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
		   graph_uParl_mean, f_uParl_mean,
		   graph_uParl_div_qT_mean, f_uParl_div_qT_mean, 
		   graph_uParl_rms, f_uParl_rms,
		   graph_uPerp_mean, f_uPerp_mean,
		   graph_uPerp_rms, f_uPerp_rms,
		   isData, outputFileName);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
