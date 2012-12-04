
#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMarker.h>
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
		     Float_t* genX, Float_t* genY, 
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

  TMarker* genMarker = 0;
  if ( genX && genY ) {
    genMarker = new TMarker(*genX, *genY, 34);
    genMarker->SetMarkerColor(1);
    genMarker->SetMarkerSize(2);
    genMarker->Draw();
  }

  canvas->Update();
  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  if ( idx != std::string::npos ) canvas->Print(std::string(outputFileName_plot).append(std::string(outputFileName, idx)).data());
  canvas->Print(std::string(outputFileName_plot).append(".png").data());
  canvas->Print(std::string(outputFileName_plot).append(".pdf").data());
  canvas->Print(std::string(outputFileName_plot).append(".root").data());

  delete genMarker;
  delete canvas;  
}

void showHistogram1d(TH1* histogram, 
		     const std::string& xAxisTitle,
		     Float_t* genX, 
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

  TMarker* genMarker = 0;
  if ( genX ) {
    genMarker = new TMarker(*genX, 0.10, 34);
    genMarker->SetMarkerColor(1);
    genMarker->SetMarkerSize(2);
    genMarker->Draw();
  }

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

void getGen(const std::string& genNtupleFileName, int run, int ls, int event, Float_t& genX1, Float_t& genX2, Float_t& genPt, Float_t& genEta, Float_t& genPhi, Float_t& genMass)
{
  TFile* genNtupleFile = new TFile(genNtupleFileName.data());
  std::string genNtupleName = "neuralMtautauNtupleProducer/neuralMtautauNtuple";
  TTree* genNtuple = dynamic_cast<TTree*>(genNtupleFile->Get(genNtupleName.data()));
  if ( !genNtuple ) {
    std::cerr << "Failed to find tree with name = " << genNtupleName << " in file = " << genNtupleFileName << " --> aborting !!" << std::endl;
    assert(0);
  }

  Float_t genNtuple_X1, genNtuple_X2, genNtuple_pt, genNtuple_eta, genNtuple_phi, genNtuple_mass;
  genNtuple->SetBranchAddress("genX1", &genNtuple_X1);
  genNtuple->SetBranchAddress("genX2", &genNtuple_X2);
  genNtuple->SetBranchAddress("genDiTauPt", &genNtuple_pt);
  genNtuple->SetBranchAddress("genDiTauEta", &genNtuple_eta);
  genNtuple->SetBranchAddress("genDiTauPhi", &genNtuple_phi);
  genNtuple->SetBranchAddress("genMtautau", &genNtuple_mass);
  
  ULong64_t genNtuple_run, genNtuple_ls, genNtuple_event;
  genNtuple->SetBranchAddress("run", &genNtuple_run);
  genNtuple->SetBranchAddress("lumisection", &genNtuple_ls);
  genNtuple->SetBranchAddress("event", &genNtuple_event);
    
  bool isFound = false;

  int numEvents = genNtuple->GetEntries();
  std::cout << "numEvents = " << numEvents << std::endl;
  for ( int iEvent = 0; iEvent < numEvents; ++iEvent ) {
    genNtuple->GetEvent(iEvent);
    std::cout << "run " << genNtuple_run << ", ls = " << genNtuple_ls << ", event " << genNtuple_event << std::endl;
    if ( genNtuple_run == run && genNtuple_ls == ls && genNtuple_event == event ) {
      genX1   = genNtuple_X1;
      genX2   = genNtuple_X2;
      genPt   = genNtuple_pt;
      genEta  = genNtuple_eta;
      genPhi  = genNtuple_phi;
      genMass = genNtuple_mass;
      isFound = true;
    }
  }

  if ( !isFound ) {
    std::cerr << "Failed to find run# " << run << ", #ls " << ls << ", #event " << event << " in file = " << genNtupleFileName << " --> aborting !!" << std::endl;
    assert(0);
  }

  delete genNtupleFile;
}

void plotMarkovChainMonitorTree()
{
  std::string inputFilePath = "/data1/veelken/tmp/svFitStudies/";

  std::vector<std::string> eventsToProcess;
  eventsToProcess.push_back("1:1673:501590");
  eventsToProcess.push_back("1:1673:501684");
  eventsToProcess.push_back("1:1673:501726");
  eventsToProcess.push_back("1:1681:504011");

  std::string genNtupleFileName = "/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods3_ntuple_Higgs_mutau_125_2012Nov30.root";

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

    std::string inputFileName = Form("%s/nSVfitProducerByIntegration2WOtracksMax5000kCalls_run%i_ls%i_ev%i_mc.root", inputFilePath.data(), run, ls, event);
    TFile* inputFile = new TFile(inputFileName.data());
    assert(inputFile);

    std::string treeName = "monitorTree";
    TTree* monitorTree = dynamic_cast<TTree*>(inputFile->Get(treeName.data()));
    assert(monitorTree);

    gROOT->SetBatch(true);

    TH2* histogramDensity_vs_X1_and_X2 = new TH2D("histogramDensity_vs_X1_and_X2", "histogramDensity_vs_X1_and_X2", 100, 0., 1., 100, 0., 1.);
    TH2* histogramMass_vs_X1_and_X2    = new TH2D("histogramMass_vs_X1_and_X2",    "histogramMass_vs_X1_and_X2",    100, 0., 1., 100, 0., 1.);
    TH1* histogramPtDistribution       = new TH1D("histogramPtDistribution",       "histogramPtDistribution",       250, 0., 250.);
    TH1* histogramEtaDistribution      = new TH1D("histogramEtaDistribution",      "histogramEtaDistribution",      198, -9.9, +9.9);
    TH1* histogramPhiDistribution      = new TH1D("histogramPhiDistribution",      "histogramPhiDistribution",      360, -TMath::Pi(), +TMath::Pi());
    TH1* histogramMassDistribution     = new TH1D("histogramMassDistribution",     "histogramMassDistribution",     250, 0., 250.);

    Float_t X1, decayDistance1, X2, decayDistance2, diTauPt, diTauEta, diTauPhi, diTauMass;
    monitorTree->SetBranchAddress("x0", &X1);
    monitorTree->SetBranchAddress("x1", &decayDistance1);
    monitorTree->SetBranchAddress("x2", &X2);
    monitorTree->SetBranchAddress("x3", &decayDistance2);
    monitorTree->SetBranchAddress("diTauPt", &diTauPt);
    monitorTree->SetBranchAddress("diTauEta", &diTauEta);
    monitorTree->SetBranchAddress("diTauPhi", &diTauPhi);
    monitorTree->SetBranchAddress("diTauMass", &diTauMass);
    
    int numEntries = monitorTree->GetEntries();
    for ( int iEntry = 0; iEntry < numEntries; ++iEntry ) {
      monitorTree->GetEntry(iEntry);
      
      if ( (iEntry % 100000) == 0 ) 
	std::cout << "entry #" << iEntry << ":" 
		  << " X1 = " << X1 << ", decayDistance1 = " << decayDistance1 << "," 
		  << " X2 = " << X2 << ", decayDistance2 = " << decayDistance2 << ", Mass = " << diTauMass << std::endl;
      
      histogramDensity_vs_X1_and_X2->Fill(X1, X2);
      histogramMass_vs_X1_and_X2->Fill(X1, X2, diTauMass);
      histogramPtDistribution->Fill(diTauPt);
      histogramEtaDistribution->Fill(diTauEta);
      histogramPhiDistribution->Fill(diTauPhi);
      histogramMassDistribution->Fill(diTauMass);
    }
    
    histogramMass_vs_X1_and_X2->Divide(histogramDensity_vs_X1_and_X2);
    
    Float_t genX1, genX2, genPt, genEta, genPhi, genMass;
    getGen(genNtupleFileName, run, ls, event, genX1, genX2, genPt, genEta, genPhi, genMass);

    showHistogram2d(histogramDensity_vs_X1_and_X2, "X_{1}", "X_{2}", kNormByNegLogMax, 0.,   2., &genX1, &genX2, Form("plots/plotMarkovChainMonitorTree_density_vs_X1andX2_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram2d(histogramMass_vs_X1_and_X2,    "X_{1}", "X_{2}", kNormByValue,     0., 250., 0, 0, Form("plots/plotMarkovChainMonitorTree_mass_vs_X1andX2_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram1d(histogramPtDistribution, "P_{T}^{#tau#tau}", &genPt, Form("plots/plotMarkovChainMonitorTree_ptDistribution_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram1d(histogramEtaDistribution, "#eta_{#tau#tau}", &genEta, Form("plots/plotMarkovChainMonitorTree_etaDistribution_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram1d(histogramPhiDistribution, "#phi_{#tau#tau}", &genPhi, Form("plots/plotMarkovChainMonitorTree_phiDistribution_run%i_ls%i_ev%i.root", run, ls, event));
    showHistogram1d(histogramMassDistribution, "M_{#tau#tau}", &genMass, Form("plots/plotMarkovChainMonitorTree_massDistribution_run%i_ls%i_ev%i.root", run, ls, event));
    
    delete histogramDensity_vs_X1_and_X2;
    delete histogramMass_vs_X1_and_X2;
    delete histogramPtDistribution;
    delete histogramEtaDistribution;
    delete histogramPhiDistribution;
    delete histogramMassDistribution;

    delete inputFile;
  }
}
