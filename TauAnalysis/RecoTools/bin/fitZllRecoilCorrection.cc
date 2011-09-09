
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections uParl, uPerp
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.5 2011/08/31 12:18:02 veelken Exp $
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
  std::string outputFileName_plot = std::string(outputFileName, 0, idx - (strlen("_cfi.py") - 1));
  outputFileName_plot.append("_").append(outputFileLabel);
  outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

double square(double x)
{
  return x*x;
}

void makeControlPlots(TH1* histogram_uParl_mean, TH1* histogram_uParl_rms, TH1* histogram_uPerp_mean, TH1* histogram_uPerp_rms, 
		      const ZllRecoilCorrectionParameterSet& fitResultsfitResults, bool isData, const std::string& outputFileName)
{
  TAxis* xAxis = histogram_uParl_mean->GetXaxis();

  int numBins = xAxis->GetNbins();
  const Double_t* binning = xAxis->GetXbins()->GetArray();

  double xStepSize = 1.;
  double xMin = binning[0] + 0.5*xStepSize;
  double xMax = binning[numBins];

  int numPoints = TMath::FloorNint((xMax - xMin)/xStepSize);

  TGraph*       graph_uParlFit        = new TGraph(numPoints);
  TGraphErrors* graph_uParlFitErr     = new TGraphErrors(numPoints);
  TGraph*       graph_uParl_rmsFit    = new TGraph(numPoints);
  TGraphErrors* graph_uParl_rmsFitErr = new TGraphErrors(numPoints);

  TGraph*       graph_uPerpFit        = new TGraph(numPoints);
  TGraphErrors* graph_uPerpFitErr     = new TGraphErrors(numPoints);
  TGraph*       graph_uPerp_rmsFit    = new TGraph(numPoints);
  TGraphErrors* graph_uPerp_rmsFitErr = new TGraphErrors(numPoints);

  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    double x = xMin + iPoint*xStepSize;

    double uParl_av             = fitResultsfitResults.d() + fitResultsfitResults.k()*x;
    double uParl_dUp            = (fitResultsfitResults.d() + fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;    
    double uParl_dDown          = (fitResultsfitResults.d() - fitResultsfitResults.dErr()) + fitResultsfitResults.k()*x;
    double uParl_kUp            = fitResultsfitResults.d() + (fitResultsfitResults.k() + fitResultsfitResults.kErr())*x;
    double uParl_kDown          = fitResultsfitResults.d() + (fitResultsfitResults.k() - fitResultsfitResults.kErr())*x;
    
    double uParlErrUp           = uParl_av + TMath::Sqrt(square(uParl_dUp   - uParl_av) + square(uParl_kUp   - uParl_av));
    double uParlErrDown         = uParl_av - TMath::Sqrt(square(uParl_dDown - uParl_av) + square(uParl_kDown - uParl_av));

    graph_uParlFit->SetPoint(iPoint, x, uParl_av);
    graph_uParlFitErr->SetPoint(iPoint, x, 0.5*(uParlErrUp + uParlErrDown));
    graph_uParlFitErr->SetPointError(iPoint, 0., 0.5*TMath::Abs(uParlErrUp - uParlErrDown));

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

  TCanvas* canvas = new TCanvas();
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string histogramLegendEntry = ( isData ) ? "Data" : "Simulation";

  showControlPlot(canvas, 
		  histogram_uParl_mean, histogramLegendEntry, graph_uParlFit, graph_uParlFitErr,     
		  true, "u_{parl} / GeV", -200., +50.,
		  outputFileName, "uParl_mean");
  showControlPlot(canvas, 
		  histogram_uParl_rms, histogramLegendEntry, graph_uParl_rmsFit, graph_uParl_rmsFitErr, 
		  false, "rms(u_{parl}) / GeV", 0., 25.,
		  outputFileName, "uParl_rms");
  showControlPlot(canvas, 
		  histogram_uPerp_mean, histogramLegendEntry, graph_uPerpFit, graph_uPerpFitErr,     
		  false, "u_{perp} / GeV", -25., +25.,
		  outputFileName, "uPerp_mean");
  showControlPlot(canvas, 
		  histogram_uPerp_rms, histogramLegendEntry, graph_uPerp_rmsFit, graph_uPerp_rmsFitErr, 
		  false, "rms(u_{perp}) / GeV", 0., 25.,
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

  const int numBins = 22;
  double binning[numBins + 1] = { 
    0., 2.5, 5., 7.5, 10., 12.5, 15., 17.5, 20., 22.5, 25., 27.5, 30., 35., 40., 45., 50., 60., 70., 80., 100., 120., 150. 
  };
 
  TH2* histogram_uParl = new TH2D("histogram_uParl", "histogram_uParl", numBins, binning, 2000, -500., +500.);
  TH2* histogram_uPerp = new TH2D("histogram_uPerp", "histogram_uPerp", numBins, binning, 2000, -500., +500.);

  int numEntries = tree->GetEntries();
  for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
    tree->GetEntry(iEntry);

    histogram_uParl->Fill(refT_value, projParl_value, evtWeight_value);
    histogram_uPerp->Fill(refT_value, projPerp_value, evtWeight_value);
  }

  TH1* histogram_uParl_mean = new TH1D("histogram_uParl_mean", "<u_{parl}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_uParl_rms  = new TH1D("histogram_uParl_rms",  "rms(u_{perp} - d + k*q_{T}) as function of q_{T}", numBins, binning);
  TH1* histogram_uPerp_mean = new TH1D("histogram_uPerp_mean", "<u_{parl}> as function of q_{T}",                  numBins, binning);
  TH1* histogram_uPerp_rms  = new TH1D("histogram_uPerp_rms",  "rms(u_{perp}) as function of q_{T}",               numBins, binning);

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    TString histogramName_uParl_proj = Form("%s_py_%i", histogram_uParl->GetName(), iBin);
    TH1D* histogram_uParl_proj = histogram_uParl->ProjectionY(histogramName_uParl_proj.Data(), iBin, iBin, "e");
    histogram_uParl_mean->SetBinContent(iBin, histogram_uParl_proj->GetMean());
    histogram_uParl_mean->SetBinError(iBin, histogram_uParl_proj->GetMeanError());
    histogram_uParl_rms->SetBinContent(iBin, histogram_uParl_proj->GetRMS());
    histogram_uParl_rms->SetBinError(iBin, histogram_uParl_proj->GetRMSError());

    TString histogramName_uPerp_proj = Form("%s_py_%i", histogram_uPerp->GetName(), iBin);
    TH1D* histogram_uPerp_proj = histogram_uPerp->ProjectionY(histogramName_uPerp_proj.Data(), iBin, iBin, "e");
    histogram_uPerp_mean->SetBinContent(iBin, histogram_uPerp_proj->GetMean());
    histogram_uPerp_mean->SetBinError(iBin, histogram_uPerp_proj->GetMeanError());
    histogram_uPerp_rms->SetBinContent(iBin, histogram_uPerp_proj->GetRMS());
    histogram_uPerp_rms->SetBinError(iBin, histogram_uPerp_proj->GetRMSError());
  }

  std::cout << "starting uParl fit..." << std::endl;
  TF1* f_uParl_mean = new TF1("f_uParl_mean", "[0]*x", 0., 150.);
  f_uParl_mean->SetLineWidth(0);
  f_uParl_mean->SetParameter(0, -0.9);
  histogram_uParl_mean->Fit(f_uParl_mean, "E");

  TF1* f_uParl_rms = new TF1("f_uParl_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 150.);
  f_uParl_rms->SetLineWidth(0);
  f_uParl_rms->SetParameter(0, 10.);
  f_uParl_rms->SetParameter(0,  5.e-2);
  f_uParl_rms->SetParameter(0,  0.);
  histogram_uParl_rms->Fit(f_uParl_rms, "E");

  std::cout << "starting uPerp fit..." << std::endl;
  TF1* f_uPerp_rms = new TF1("f_uPerp_rms", "[0]*(1.0 + [1]*x + [2]*x*x)", 0., 150.);
  f_uPerp_rms->SetLineWidth(0);
  f_uPerp_rms->SetParameter(0, 10.);
  f_uPerp_rms->SetParameter(0,  5.e-2);
  f_uPerp_rms->SetParameter(0,  0.);
  histogram_uPerp_rms->Fit(f_uPerp_rms, "E");

  std::cout << "done." << std::endl;

  ZllRecoilCorrectionParameterSet fitResults(
    0., 0., 
    f_uParl_mean->GetParameter(0), f_uParl_mean->GetParError(0), 
    f_uParl_rms->GetParameter(0), f_uParl_rms->GetParError(0), 
    f_uParl_rms->GetParameter(1), f_uParl_rms->GetParError(1), 
    f_uParl_rms->GetParameter(2), f_uParl_rms->GetParError(2),
    f_uPerp_rms->GetParameter(0), f_uPerp_rms->GetParError(0), 
    f_uPerp_rms->GetParameter(1), f_uPerp_rms->GetParError(1), 
    f_uPerp_rms->GetParameter(2), f_uPerp_rms->GetParError(2));

  fitResults.print(std::cout);

  fitResults.writePythonConfig(outputFileName);

  makeControlPlots(histogram_uParl_mean, histogram_uParl_rms, 
		   histogram_uPerp_mean, histogram_uPerp_rms, 
		   fitResults, isData, outputFileName);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
