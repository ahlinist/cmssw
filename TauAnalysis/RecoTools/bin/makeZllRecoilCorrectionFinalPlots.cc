
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/FWLite/interface/InputSource.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TFile.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TBenchmark.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TString.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

typedef std::vector<std::string> vstring;
typedef std::vector<edm::ParameterSet> vParameterSet;

struct variableEntryType
{
  variableEntryType(const edm::ParameterSet& cfg)
    : meName_(cfg.getParameter<std::string>("meName")),
      xAxisTitle_(cfg.getParameter<std::string>("xAxisTitle"))
  {}
  ~variableEntryType() {}
  std::string meName_;
  std::string xAxisTitle_;
};

TH1* loadHistogram(TFile* inputFile, const std::string& directory, const std::string& meName)
{
  std::string meName_full = directory;

//--- add trailing '/'
  if ( meName_full != "" && meName_full.find_last_of("/") != (meName_full.length() - 1) ) meName_full.append("/");

  meName_full.append(meName);

  TH1* me = dynamic_cast<TH1*>(inputFile->Get(meName_full.data()));
  if ( !me ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Failed to load histogram = " << meName_full << " from file = " << inputFile->GetName() << " !!\n";

  return me;
}

void drawHistogram1d(TFile* inputFile, TCanvas* canvas, const variableEntryType& variable, 
		     const std::string& directoryData, const std::string& directoryMC, const std::string& outputFileName)
{
  TH1* meData = loadHistogram(inputFile, directoryData, variable.meName_);
  meData->SetLineColor(1);
  meData->SetMarkerColor(1);
  meData->SetMarkerStyle(20);
  
  TH1* meMC = loadHistogram(inputFile, directoryMC, variable.meName_);
  meMC->SetLineColor(2);
  meMC->SetLineWidth(2);

  canvas->Clear();

  meMC->SetStats(false);
  meMC->SetMaximum(3.e1*TMath::Max(meData->GetMaximum(), meMC->GetMaximum()));
  meMC->SetMinimum(5.e-1);

  meMC->GetXaxis()->SetTitle(variable.xAxisTitle_.data());
  meMC->GetXaxis()->SetTitleOffset(1.2);
  meMC->GetYaxis()->SetTitle("Events");
  meMC->GetYaxis()->SetTitleOffset(1.4);
    
  meMC->Draw("hist");
  meData->Draw("e1psame");
  
  TLegend legend(0.14, 0.74, 0.49, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(meData, "Data",       "p");
  legend.AddEntry(meMC,   "Simulation", "l");
  legend.Draw();

  TPaveText statsData(0.64, 0.79, 0.89, 0.89, "brNDC"); 
  statsData.SetBorderSize(0);
  statsData.SetFillColor(0);
  statsData.AddText(Form("Mean = %2.2f", meData->GetMean()));
  statsData.AddText(Form("RMS  = %2.2f", meData->GetRMS()));
  statsData.SetTextColor(1);
  statsData.SetTextSize(0.045);
  statsData.Draw();
  
  TPaveText statsMC(0.64, 0.69, 0.89, 0.79, "brNDC"); 
  statsMC.SetBorderSize(0);
  statsMC.SetFillColor(0);
  statsMC.AddText(Form("Mean = %2.2f", meMC->GetMean()));
  statsMC.AddText(Form("RMS  = %2.2f", meMC->GetRMS()));
  statsMC.SetTextColor(2);
  statsMC.SetTextSize(0.045);
  statsMC.Draw();

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(variable.meName_);
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  else                            outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());

  canvas->Clear();
  
  if ( !meData->GetSumw2N() ) meData->Sumw2();
  meData->Scale(1./meData->Integral());
  if ( !meMC->GetSumw2N()   ) meMC->Sumw2();
  meMC->Scale(1./meMC->Integral());
  
  meMC->SetMaximum(3.e1*TMath::Max(meData->GetMaximum(), meMC->GetMaximum()));
  meMC->SetMinimum(1.e-5);
  
  meMC->GetYaxis()->SetTitle("a.u.");
  
  meMC->Draw("hist");
  meData->Draw("e1psame");
  
  legend.Draw();
    
  statsData.Draw();
  statsMC.Draw();
  
  canvas->Update();
  
  std::string outputFileName_plot_scaled = std::string(outputFileName, 0, idx);
  outputFileName_plot_scaled.append("_").append(variable.meName_).append("_scaled");
  if ( idx != std::string::npos ) outputFileName_plot_scaled.append(std::string(outputFileName, idx));
  else                            outputFileName_plot_scaled.append(".png");
  canvas->Print(outputFileName_plot_scaled.data());
}	     

double getMaximum(TGraphErrors* graph)
{
  double retVal = 0.;

  int numPoints = graph->GetN();
  for ( int iPoint = 0; iPoint < numPoints; ++iPoint ) {
    Double_t x, y;
    graph->GetPoint(iPoint, x, y);

    Double_t yErr = graph->GetErrorY(iPoint);

    if ( (y + yErr) > retVal ) retVal = y + yErr;
  }

  return retVal;
}

void drawGraphs(TCanvas* canvas, const std::string& yAxisTitle, const std::string& outputFileName, const std::string& outputFileLabel,
		TGraphErrors* graphData1, const std::string& legendEntryData1, 
		TGraphErrors* graphMC1, const std::string& legendEntryMC1, 
		TGraphErrors* graphData2 = 0, const std::string& legendEntryData2 = "", 
		TGraphErrors* graphMC2 = 0, const std::string& legendEntryMC2 = "", 
		TGraphErrors* graphData3 = 0, const std::string& legendEntryData3 = "", 
		TGraphErrors* graphMC3 = 0, const std::string& legendEntryMC3 = "", 
		TGraphErrors* graphData4 = 0, const std::string& legendEntryData4 = "", 
		TGraphErrors* graphMC4 = 0, const std::string& legendEntryMC4 = "", 
		TGraphErrors* graphData5 = 0, const std::string& legendEntryData5 = "", 
		TGraphErrors* graphMC5 = 0, const std::string& legendEntryMC5 = "")
{
  canvas->Clear();

  canvas->SetLogy(false);

  double yMax = getMaximum(graphData1);
  yMax = TMath::Max(yMax, getMaximum(graphMC1));
  unsigned numGraphs = 2;
  if ( graphData2 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData2));
    ++numGraphs;
  }
  if ( graphMC2   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC2));
    ++numGraphs;
  }
  if ( graphData3 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData3));
    ++numGraphs;
  }
  if ( graphMC3   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC3));
    ++numGraphs;
  }
  if ( graphData4 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData4));
    ++numGraphs;
  }
  if ( graphMC4   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC4));
    ++numGraphs;
  }
  if ( graphData5 ) {
    yMax = TMath::Max(yMax, getMaximum(graphData5));
    ++numGraphs;
  }
  if ( graphMC5   ) {
    yMax = TMath::Max(yMax, getMaximum(graphMC5));
    ++numGraphs;
  }
  
  graphData1->SetMaximum(1.2*yMax);
  graphData1->SetMinimum(0.);

  graphData1->GetXaxis()->SetTitle("q_{T} / GeV");
  graphData1->GetXaxis()->SetTitleOffset(1.2);
  graphData1->GetYaxis()->SetTitle(yAxisTitle.data());
  graphData1->GetYaxis()->SetTitleOffset(1.4);

  graphData1->SetLineColor(1);
  graphData1->SetMarkerColor(1);
  graphData1->SetMarkerStyle(24);
  graphData1->Draw("ap");

  graphMC1->SetLineColor(1);
  graphMC1->SetMarkerColor(1);
  graphMC1->SetMarkerStyle(24);
  graphMC1->Draw("ap");

  double textSize = ( numGraphs <= 8 ) ? 0.045 : 0.035;
  TLegend legend(0.64, 0.89 - textSize*numGraphs, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(graphData1, legendEntryData1.data(), "p");
  legend.AddEntry(graphMC1,   legendEntryMC1.data(),   "p");

  if ( graphData2 ) {
    graphData2->SetLineColor(2);
    graphData2->SetMarkerColor(2);
    graphData2->SetMarkerStyle(21);
    graphData2->Draw("p");
    legend.AddEntry(graphData2, legendEntryData2.data(), "p");
  }

  if ( graphMC2 ) {
    graphMC2->SetLineColor(2);
    graphMC2->SetMarkerColor(2);
    graphMC2->SetMarkerStyle(25);
    graphMC2->Draw("p");
    legend.AddEntry(graphMC2, legendEntryMC2.data(), "p");
  }

  if ( graphData3 ) {
    graphData3->SetLineColor(3);
    graphData3->SetMarkerColor(3);
    graphData3->SetMarkerStyle(22);
    graphData3->Draw("p");
    legend.AddEntry(graphData3, legendEntryData3.data(), "p");
  }

  if ( graphMC3 ) {
    graphMC3->SetLineColor(3);
    graphMC3->SetMarkerColor(3);
    graphMC3->SetMarkerStyle(26);
    graphMC3->Draw("p");
    legend.AddEntry(graphMC3, legendEntryMC3.data(), "p");
  }

  if ( graphData4 ) {
    graphData4->SetLineColor(4);
    graphData4->SetMarkerColor(4);
    graphData4->SetMarkerStyle(23);
    graphData4->Draw("p");
    legend.AddEntry(graphData4, legendEntryData4.data(), "p");
  }

  if ( graphMC4 ) {
    graphMC4->SetLineColor(4);
    graphMC4->SetMarkerColor(4);
    graphMC4->SetMarkerStyle(32);
    graphMC4->Draw("p");
    legend.AddEntry(graphMC4, legendEntryMC4.data(), "p");
  }

  if ( graphData5 ) {
    graphData5->SetLineColor(6);
    graphData5->SetMarkerColor(6);
    graphData5->SetMarkerStyle(34);
    graphData5->Draw("p");
    legend.AddEntry(graphData5, legendEntryData5.data(), "p");
  }

  if ( graphMC5 ) {
    graphMC5->SetLineColor(6);
    graphMC5->SetMarkerColor(6);
    graphMC5->SetMarkerStyle(28);
    graphMC5->Draw("p");
    legend.AddEntry(graphMC5, legendEntryMC5.data(), "p");
  }

  legend.Draw();

  canvas->Update();

  size_t idx = outputFileName.find_last_of('.');
  std::string outputFileName_plot = std::string(outputFileName, 0, idx);
  outputFileName_plot.append("_").append(outputFileLabel);
  if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
  else                            outputFileName_plot.append(".png");
  canvas->Print(outputFileName_plot.data());
}

std::pair<TGraphErrors*, TGraphErrors*> makeGraphs_mean_and_rms(TH2* histogram)
{
  TAxis* xAxis = histogram->GetXaxis();

  int numBins = xAxis->GetNbins();

  TGraphErrors* graph_mean = new TGraphErrors(numBins);
  TGraphErrors* graph_rms  = new TGraphErrors(numBins);

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    TString histogramName_proj = Form("%s_py_%i", histogram->GetName(), iBin);
    TH1D* histogram_proj = histogram->ProjectionY(histogramName_proj.Data(), iBin, iBin, "e");

    double x = xAxis->GetBinCenter(iBin);

    double mean = histogram_proj->GetMean();
    double meanErr = histogram_proj->GetMeanError();
    
    double rms = histogram_proj->GetRMS();
    double rmsErr = histogram_proj->GetRMSError();

    int iPoint = iBin - 1;

    graph_mean->SetPoint(iPoint, x, mean);
    graph_mean->SetPointError(iPoint, 0., meanErr);

    graph_rms->SetPoint(iPoint, x, rms);
    graph_rms->SetPointError(iPoint, 0., rmsErr);
  }

  return std::pair<TGraphErrors*, TGraphErrors*>(graph_mean, graph_rms);
}

TGraphErrors* makeGraph_mean(TH2* histogram)
{
  return makeGraphs_mean_and_rms(histogram).first;
}

TGraphErrors* makeGraph_rms(TH2* histogram)
{
  return makeGraphs_mean_and_rms(histogram).second;
}

struct plotUvsQtNumVtxType
{
  plotUvsQtNumVtxType(TFile* inputFile, int numVtxMin, int numVtxMax, 
		      const std::string& directoryData, const std::string& directoryMC)
    : numVtxMin_(numVtxMin),
      numVtxMax_(numVtxMax)
  {
    TString label;
    if      ( numVtxMin_ == -1 ) label = Form("Le%i",   numVtxMax_);
    else if ( numVtxMax_ == -1 ) label = Form("Ge%i",   numVtxMin_);
    else                         label = Form("%ito%i", numVtxMin_, numVtxMax_);

    meUparlDivQtVsQtData_ = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uParlDivQtVsQt").Append(label).Data()));
    meUparlVsQtData_      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uParlVsQt").Append(label).Data()));
    meUperpVsQtData_      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, TString("uPerpVsQt").Append(label).Data()));

    graphUparlResponseData_   = makeGraph_mean(meUparlDivQtVsQtData_);
    graphUparlResolutionData_ = makeGraph_rms(meUparlVsQtData_);
    graphUperpResolutionData_ = makeGraph_rms(meUperpVsQtData_);

    meUparlDivQtVsQtMC_   = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uParlDivQtVsQt").Append(label).Data()));
    meUparlVsQtMC_        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uParlVsQt").Append(label).Data()));
    meUperpVsQtMC_        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   TString("uPerpVsQt").Append(label).Data()));

    graphUparlResponseMC_     = makeGraph_mean(meUparlDivQtVsQtMC_);
    graphUparlResolutionMC_   = makeGraph_rms(meUparlVsQtMC_);
    graphUperpResolutionMC_   = makeGraph_rms(meUperpVsQtMC_);

    if      ( numVtxMin_ == -1 ) label = Form("N_{vtx} < %i",      numVtxMax_);
    else if ( numVtxMax_ == -1 ) label = Form("N_{vtx} > %i",      numVtxMin_);
    else                         label = Form("%i < N_{vtx} < %i", numVtxMin_, numVtxMax_);

    legendEntryData_ = std::string("Data").append(": ").append(label);
    legendEntryMC_   = std::string("Sim.").append(": ").append(label);
  }
  ~plotUvsQtNumVtxType() {}

  int numVtxMin_;
  int numVtxMax_;

  TH2* meUparlDivQtVsQtData_;
  TH2* meUparlVsQtData_;
  TH2* meUperpVsQtData_;

  TGraphErrors* graphUparlResponseData_;
  TGraphErrors* graphUparlResolutionData_;
  TGraphErrors* graphUperpResolutionData_;

  TH2* meUparlDivQtVsQtMC_;
  TH2* meUparlVsQtMC_;
  TH2* meUperpVsQtMC_;

  TGraphErrors* graphUparlResponseMC_;
  TGraphErrors* graphUparlResolutionMC_;
  TGraphErrors* graphUperpResolutionMC_;

  std::string legendEntryData_;
  std::string legendEntryMC_;
};

int main(int argc, const char* argv[])
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<makeZllRecoilCorrectionFinalPlots>:" << std::endl;  

//--- disable pop-up windows showing graphics output
  gROOT->SetBatch(true);

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("makeZllRecoilCorrectionFinalPlots");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgMakeZllRecoilCorrectionPlots = cfg.getParameter<edm::ParameterSet>("makeZllRecoilCorrectionFinalPlots");

  std::string directoryData = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryData");
  std::string directoryMC   = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("directoryMC");

  vParameterSet cfgVariables = cfgMakeZllRecoilCorrectionPlots.getParameter<vParameterSet>("variables");
  std::vector<variableEntryType> variables;
  for ( vParameterSet::const_iterator cfgVariable = cfgVariables.begin();
	cfgVariable != cfgVariables.end(); ++cfgVariable ) {
    variables.push_back(variableEntryType(*cfgVariable));
  }

  std::string outputFileName = cfgMakeZllRecoilCorrectionPlots.getParameter<std::string>("outputFileName");

  fwlite::InputSource inputFiles(cfg); 
  if ( inputFiles.files().size() != 1 ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Input file must be unique, got = " << format_vstring(inputFiles.files()) << " !!\n";
  std::string inputFileName = (*inputFiles.files().begin());
  
//--- open input file
  TFile* inputFile = TFile::Open(inputFileName.data());
  if ( !inputFile ) 
    throw cms::Exception("makeZllRecoilCorrectionFinalPlots") 
      << "Failed to open inputFile = " << inputFileName << " !!\n";

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  canvas->SetLogy();

//--- make control plots of different variable distributions
  for ( std::vector<variableEntryType>::const_iterator variable = variables.begin();
	variable != variables.end(); ++variable ) {
    drawHistogram1d(inputFile, canvas, *variable, directoryData, directoryMC, outputFileName);
  }

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
  TH2* meUparlDivQtVsQt_data = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, "uParlDivQtVsQt"));
  TH2* meUparlVsQt_data      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, "uParlVsQt"));
  TH2* meUperpVsQt_data      = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryData, "uPerpVsQt"));

  TGraphErrors* graphUparlResponse_data   = makeGraph_mean(meUparlDivQtVsQt_data);
  TGraphErrors* graphUparlResolution_data = makeGraph_rms(meUparlVsQt_data);
  TGraphErrors* graphUperpResolution_data = makeGraph_rms(meUperpVsQt_data);

  TH2* meUparlDivQtVsQt_mc   = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   "uParlDivQtVsQt"));
  TH2* meUparlVsQt_mc        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   "uParlVsQt"));
  TH2* meUperpVsQt_mc        = dynamic_cast<TH2*>(loadHistogram(inputFile, directoryMC,   "uPerpVsQt"));

  TGraphErrors* graphUparlResponse_mc     = makeGraph_mean(meUparlDivQtVsQt_mc);
  TGraphErrors* graphUparlResolution_mc   = makeGraph_rms(meUparlVsQt_mc);
  TGraphErrors* graphUperpResolution_mc   = makeGraph_rms(meUperpVsQt_mc);

  drawGraphs(canvas, "<u_{parl}>/q_{T}", outputFileName, "uParlResponse",
	     graphUparlResponse_data, "Data", graphUparlResponse_mc, "Simulation");
  drawGraphs(canvas, "RMS(u_{parl})", outputFileName, "uParlResolution",
	     graphUparlResolution_data, "Data", graphUparlResolution_mc, "Simulation");
  drawGraphs(canvas, "RMS(u_{perp})", outputFileName, "uPerpResolution",
	     graphUperpResolution_data, "Data", graphUperpResolution_mc, "Simulation");

//--- make plots of mean(uParl)/qT, rms(uParl)/qT, rms(uPerp)/qT
//    in different bins of reconstructed vertex multiplicity
  plotUvsQtNumVtxType* plotUvsQtNumVtxLe2   = new plotUvsQtNumVtxType(inputFile, -1,  2, directoryData, directoryMC);
  plotUvsQtNumVtxType* plotUvsQtNumVtx3to5  = new plotUvsQtNumVtxType(inputFile,  3,  5, directoryData, directoryMC);
  plotUvsQtNumVtxType* plotUvsQtNumVtx6to8  = new plotUvsQtNumVtxType(inputFile,  6,  8, directoryData, directoryMC);
  plotUvsQtNumVtxType* plotUvsQtNumVtx9to11 = new plotUvsQtNumVtxType(inputFile,  9, 11, directoryData, directoryMC);
  plotUvsQtNumVtxType* plotUvsQtNumVtxGe12  = new plotUvsQtNumVtxType(inputFile, 12, -1, directoryData, directoryMC);
  
  drawGraphs(canvas, "<u_{parl}>/q_{T}", outputFileName, "uParlResponse_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUparlResponseData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUparlResponseMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUparlResponseData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUparlResponseMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUparlResponseData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUparlResponseMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUparlResponseData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUparlResponseMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUparlResponseData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUparlResponseMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  drawGraphs(canvas, "RMS(u_{parl})", outputFileName, "uParlResolution_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUparlResolutionData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUparlResolutionMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUparlResolutionData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUparlResolutionMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUparlResolutionData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUparlResolutionMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUparlResolutionData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUparlResolutionMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUparlResolutionData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUparlResolutionMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  drawGraphs(canvas, "RMS(u_{perp})", outputFileName, "uPerpResolution_binnedVtxMultiplicity",
	     plotUvsQtNumVtxLe2->graphUperpResolutionData_, plotUvsQtNumVtxLe2->legendEntryData_,
	     plotUvsQtNumVtxLe2->graphUperpResolutionMC_, plotUvsQtNumVtxLe2->legendEntryMC_,
	     plotUvsQtNumVtx3to5->graphUperpResolutionData_, plotUvsQtNumVtx3to5->legendEntryData_,
	     plotUvsQtNumVtx3to5->graphUperpResolutionMC_, plotUvsQtNumVtx3to5->legendEntryMC_,
	     plotUvsQtNumVtx6to8->graphUperpResolutionData_, plotUvsQtNumVtx6to8->legendEntryData_,
	     plotUvsQtNumVtx6to8->graphUperpResolutionMC_, plotUvsQtNumVtx6to8->legendEntryMC_,
	     plotUvsQtNumVtx9to11->graphUperpResolutionData_, plotUvsQtNumVtx9to11->legendEntryData_,
	     plotUvsQtNumVtx9to11->graphUperpResolutionMC_, plotUvsQtNumVtx9to11->legendEntryMC_,
	     plotUvsQtNumVtxGe12->graphUperpResolutionData_, plotUvsQtNumVtxGe12->legendEntryData_,
	     plotUvsQtNumVtxGe12->graphUperpResolutionMC_, plotUvsQtNumVtxGe12->legendEntryMC_);
  
  delete canvas;

  delete inputFile;

//--print time that it took macro to run
  std::cout << "finished executing makeZllRecoilCorrectionFinalPlots macro:" << std::endl;
  clock.Show("makeZllRecoilCorrectionFinalPlots");

  return 0;
}
