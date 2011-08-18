
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
#include <TCanvas.h>
#include <TLegend.h>
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

  for ( std::vector<variableEntryType>::const_iterator variable = variables.begin();
	variable != variables.end(); ++variable ) {
    TH1* meData = loadHistogram(inputFile, directoryData, variable->meName_);
    meData->SetLineColor(1);
    meData->SetMarkerColor(1);
    meData->SetMarkerStyle(20);
    
    TH1* meMC = loadHistogram(inputFile, directoryMC, variable->meName_);
    meMC->SetLineColor(2);
    meMC->SetLineWidth(2);

    canvas->Clear();

    meMC->SetStats(false);
    meMC->SetMaximum(1.2*TMath::Max(meData->GetMaximum(), meMC->GetMaximum()));
    meMC->SetMinimum(1.e-1);

    meMC->GetXaxis()->SetTitle(variable->xAxisTitle_.data());
    meMC->GetXaxis()->SetTitleOffset(1.2);
    meMC->GetYaxis()->SetTitle("Events");
    meMC->GetYaxis()->SetTitleOffset(1.4);
    
    meMC->Draw("hist");
    meData->Draw("e1psame");

    TLegend legend(0.60, 0.71, 0.89, 0.89, "", "brNDC"); 
    legend.SetBorderSize(0);
    legend.SetFillColor(0);
    legend.AddEntry(meData, "Data",       "p");
    legend.AddEntry(meMC,   "Simulation", "l");
    legend.Draw();

    canvas->Update();

    size_t idx = outputFileName.find_last_of('.');
    std::string outputFileName_plot = std::string(outputFileName, 0, idx);
    outputFileName_plot.append("_").append(variable->meName_);
    if ( idx != std::string::npos ) outputFileName_plot.append(std::string(outputFileName, idx));
    else                            outputFileName_plot.append(".png");
    canvas->Print(outputFileName_plot.data());

    canvas->Clear();

    if ( !meData->GetSumw2N() ) meData->Sumw2();
    meData->Scale(1./meData->Integral());
    if ( !meMC->GetSumw2N()   ) meMC->Sumw2();
    meMC->Scale(1./meMC->Integral());
    
    meMC->SetMaximum(1.2*TMath::Max(meData->GetMaximum(), meMC->GetMaximum()));
    meMC->SetMinimum(1.e-4);

    meMC->GetYaxis()->SetTitle("a.u.");

    meMC->Draw("hist");
    meData->Draw("e1psame");

    legend.Draw();

    canvas->Update();

    std::string outputFileName_plot_scaled = std::string(outputFileName, 0, idx);
    outputFileName_plot_scaled.append("_").append(variable->meName_).append("_scaled");
    if ( idx != std::string::npos ) outputFileName_plot_scaled.append(std::string(outputFileName, idx));
    else                            outputFileName_plot_scaled.append(".png");
    canvas->Print(outputFileName_plot_scaled.data());
  }

  delete canvas;

  delete inputFile;

//--print time that it took macro to run
  std::cout << "finished executing makeZllRecoilCorrectionFinalPlots macro:" << std::endl;
  clock.Show("makeZllRecoilCorrectionFinalPlots");

  return 0;
}
