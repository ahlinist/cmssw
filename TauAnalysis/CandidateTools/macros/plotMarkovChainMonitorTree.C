
#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>

enum { kNormByQuantiles, kNormByNegLogMax, kNormByValue };

void showHistogram2d(TH2* histogram, 
		     const std::string& xAxisTitle, const std::string& yAxisTitle,
		     int zAxisNormOption, double zMin, double zMax, 
		     const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 900, 800);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetTopMargin(0.10);
  canvas->SetLeftMargin(0.12);
  canvas->SetRightMargin(0.14);
  canvas->SetBottomMargin(0.12);

  histogram->SetTitle("");
  histogram->SetStats(false);
  int numBinsX = histogram->GetNbinsX();
  int numBinsY = histogram->GetNbinsY();
  if ( zAxisNormOption == kNormByQuantiles ) {
    std::vector<double> binContents;
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	binContents.push_back(histogram->GetBinContent(iBinX, iBinY));
      }
    }
    std::sort(binContents.begin(), binContents.end());
    histogram->SetMinimum(binContents[TMath::Nint(0.05*binContents.size())]);
    histogram->SetMaximum(binContents[TMath::Nint(0.95*binContents.size())]);
  } else if ( zAxisNormOption == kNormByNegLogMax ) {
    double maxBinContent = 0.;
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binContent = histogram->GetBinContent(iBinX, iBinY);
	if ( binContent > maxBinContent ) maxBinContent = binContent;
      }
    }
    double logMaxBinContent = TMath::Log(maxBinContent);
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binContent = histogram->GetBinContent(iBinX, iBinY);
	if ( binContent > 0. ) {
	  histogram->SetBinContent(iBinX, iBinY, -TMath::Log(binContent) + logMaxBinContent);
	} else {
	  histogram->SetBinContent(iBinX, iBinY, -1.);
	}
      }
    }
    histogram->SetMinimum(0.);
    histogram->SetMaximum(zMax);
  } else if ( zAxisNormOption == kNormByValue ) {
    histogram->SetMinimum(zMin);
    histogram->SetMaximum(zMax);
  } else assert(0);

  TAxis* xAxis = histogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = histogram->GetYaxis();
  yAxis->SetTitle(yAxisTitle.data());
  yAxis->SetTitleOffset(1.30);

  gStyle->SetPalette(1,0);
  histogram->Draw("COLZ");

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete canvas;  
}

void showHistogram1d(TH1* histogram, 
		     const std::string& xAxisTitle,
		     const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetTopMargin(0.10);
  canvas->SetLeftMargin(0.16);
  canvas->SetRightMargin(0.14);
  canvas->SetBottomMargin(0.12);

  TAxis* xAxis = histogram->GetXaxis();
  xAxis->SetTitle(xAxisTitle.data());
  xAxis->SetTitleOffset(1.15);

  TAxis* yAxis = histogram->GetYaxis();
  yAxis->SetTitle("Sampling Points");
  yAxis->SetTitleOffset(1.60);

  histogram->SetLineColor(1);
  histogram->SetLineWidth(2);
  histogram->SetMarkerColor(1);
  histogram->SetMarkerStyle(20);
  histogram->Draw("e1p");

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete canvas;  
}

int convertToInt(const TObjArray& run_ls_event, int idx)
{
  TObjString* entry = dynamic_cast<TObjString*>(run_ls_event.At(idx));
  assert(entry);
  return atoi(entry->GetString().Data());
}

void plotMarkovChainMonitorTree()
{
  std::string inputFilePath = "../test";

  std::vector<std::string> eventsToProcess;
  //eventsToProcess.push_back("1:2399:719456");
  //eventsToProcess.push_back("1:2418:725094");
  //eventsToProcess.push_back("1:2418:725139");
  //eventsToProcess.push_back("1:2418:725278");
  eventsToProcess.push_back("1:2367:709922");
  eventsToProcess.push_back("1:2398:719150");
  eventsToProcess.push_back("1:2398:719242");
  eventsToProcess.push_back("1:2449:734448");
  eventsToProcess.push_back("1:2449:734503");
  eventsToProcess.push_back("1:2449:734537");
  eventsToProcess.push_back("1:2450:734750");

  for ( std::vector<std::string>::const_iterator eventToProcess = eventsToProcess.begin();
	eventToProcess != eventsToProcess.end(); ++eventToProcess ) {
    std::cout << "processing " << (*eventToProcess) << std::endl;
    TObjArray* run_ls_event = TString(eventToProcess->data()).Tokenize(":");
    assert(run_ls_event);
    if ( run_ls_event->GetEntries() != 3 ) {
      std::cerr << "Failed to parse " << (*eventToProcess) << " !!" << std::endl;
      assert(0);
    }

    int run = convertToInt(*run_ls_event, 0);
    int ls = convertToInt(*run_ls_event, 1);
    int event = convertToInt(*run_ls_event, 2);

    std::string inputFileName = Form("%s/nSVfitAlgorithmByIntegration2_run%i_ls%i_ev%i_mc.root", inputFilePath.data(), run, ls, event);
    TFile* inputFile = new TFile(inputFileName.data());
    assert(inputFile);

    std::string treeName = "monitorTree";
    TTree* monitorTree = dynamic_cast<TTree*>(inputFile->Get(treeName.data()));
    assert(monitorTree);

    gROOT->SetBatch(true);

    TH2* histogramDensity_vs_X1_and_X2 = new TH2D("histogramDensity_vs_X1_and_X2", "histogramDensity_vs_X1_and_X2", 100, 0., 1., 100, 0., 1.);
    TH2* histogramMass_vs_X1_and_X2    = new TH2D("histogramMass_vs_X1_and_X2",    "histogramMass_vs_X1_and_X2",    100, 0., 1., 100, 0., 1.);
    TH1* histogramMassDistribution     = new TH1D("histogramMassDistribution",     "histogramMassDistribution",     250, 0., 250.);

    Float_t X1, decayDistance1, X2, decayDistance2, Mass;
    monitorTree->SetBranchAddress("x0", &X1);
    monitorTree->SetBranchAddress("x1", &decayDistance1);
    monitorTree->SetBranchAddress("x2", &X2);
    monitorTree->SetBranchAddress("x3", &decayDistance2);
    monitorTree->SetBranchAddress("f", &Mass);
    
    int numEntries = monitorTree->GetEntries();
    for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
      monitorTree->GetEntry(iEntry);
      
      if ( (iEntry % 100000) == 0 ) 
	std::cout << "entry #" << iEntry << ":" 
		  << " X1 = " << X1 << ", decayDistance1 = " << decayDistance1 << "," 
		  << " X2 = " << X2 << ", decayDistance2 = " << decayDistance2 << ", Mass = " << Mass << std::endl;
      
      histogramDensity_vs_X1_and_X2->Fill(X1, X2);
      histogramMass_vs_X1_and_X2->Fill(X1, X2, Mass);
      histogramMassDistribution->Fill(Mass);
    }
    
    histogramMass_vs_X1_and_X2->Divide(histogramDensity_vs_X1_and_X2);
    
    showHistogram2d(histogramDensity_vs_X1_and_X2, "X_{1}", "X_{2}", kNormByNegLogMax, 0.,   2., Form("plotMarkovChainMonitorTree_density_vs_X1andX2_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram2d(histogramMass_vs_X1_and_X2,    "X_{1}", "X_{2}", kNormByValue,     0., 250., Form("plotMarkovChainMonitorTree_mass_vs_X1andX2_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram1d(histogramMassDistribution, "M_{#tau#tau}", Form("plotMarkovChainMonitorTree_massDistribution_run%i_ls%i_ev%i.root", run, ls, event));
    
    delete histogramDensity_vs_X1_and_X2;
    delete histogramMass_vs_X1_and_X2;
    delete histogramMassDistribution;

    delete inputFile;
  }
}
