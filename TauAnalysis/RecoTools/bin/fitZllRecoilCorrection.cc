
/** \executable fitZllRecoilCorrection
 *
 * Determine Z-recoil correction parameters by fitting MEt projections uParl, uPerp
 * as function of Z transverse momentum qT in Z --> mu+ mu- events
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.11 $
 *
 * $Id: fitZllRecoilCorrection.cc,v 1.11 2011/12/01 15:42:00 veelken Exp $
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

void makeControlPlots(TH1* dummyHistogram,
		      TGraphAsymmErrors* graph_uParl_mean, TF1* f_uParl_mean,
		      TGraphAsymmErrors* graph_uParl_div_qT_mean, TF1* f_uParl_div_qT_mean, 
		      TGraphAsymmErrors* graph_uParl_rms, TF1* f_uParl_rms,
		      TGraphAsymmErrors* graph_uPerp_mean, TF1* f_uPerp_mean,
		      TGraphAsymmErrors* graph_uPerp_rms, TF1* f_uPerp_rms,
		      bool isData, const std::string& outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 1000);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLeftMargin(0.12);
  canvas->SetBottomMargin(0.12);

  std::string legendEntry = ( isData ) ? "Data" : "Simulation";

  drawZllRecoilFitResult(canvas, dummyHistogram, "", graph_uParl_mean, f_uParl_mean,
			 legendEntry, 0.64, 0.665, true, false, "u_{#parallel} / GeV", -600., +100., false, 10.,
			 outputFileName, "uParl_mean");
  drawZllRecoilFitResult(canvas, dummyHistogram, "", graph_uParl_div_qT_mean, f_uParl_div_qT_mean, 
			 legendEntry, 0.64, 0.165, false, true, "u_{#parallel} /q_{T}", 0.4, 1.2, true, 0.10,
			 outputFileName, "uParl_div_qT_mean");
  drawZllRecoilFitResult(canvas, dummyHistogram, "", graph_uParl_rms, f_uParl_rms,
			 legendEntry,  0.19, 0.64, false, false, "RMS(u_{#parallel} ) / GeV", 0., 50., true, 0.50, 
			 outputFileName, "uParl_rms");
  drawZllRecoilFitResult(canvas, dummyHistogram, "", graph_uPerp_mean, f_uPerp_mean,
			 legendEntry, 0.64, 0.665, false, false, "u_{#perp}  / GeV", -25., +25., false, 2.5,
			 outputFileName, "uPerp_mean");
  drawZllRecoilFitResult(canvas, dummyHistogram, "", graph_uPerp_rms, f_uPerp_rms,
			 legendEntry, 0.19, 0.64, false, false, "RMS(u_{#perp}  ) / GeV", 0., 50., true, 0.50,
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
    makeGraph_mean("graph_uParl_mean", "<u_{#parallel} > as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uParl_div_qT_mean = 
    makeGraph_uParl_div_qT("graph_uParl_div_qT_mean", "<u_{#parallel} >/q_{T} as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uParl_rms = 
    makeGraph_rms("graph_uParl_rms", "RMS(u_{#perp}  ) as function of q_{T}", histogram_uParl, histogram_qT);
  TGraphAsymmErrors* graph_uPerp_mean = 
    makeGraph_mean("graph_uPerp_mean", "<u_{#parallel} > as function of q_{T}", histogram_uPerp, histogram_qT);
  TGraphAsymmErrors* graph_uPerp_rms =  
    makeGraph_rms("graph_uPerp_rms", "RMS(u_{#perp}  ) as function of q_{T}", histogram_uPerp, histogram_qT);

  std::cout << "starting uParl fit..." << std::endl;
  TF1* f_uParl_mean = fitGraph_uParl_mean("f_uParl_mean", graph_uParl_mean, 0., 500.);
  TF1* f_uParl_div_qT_mean = fitGraph_uParl_div_qT("f_uParl_div_qT_mean", graph_uParl_div_qT_mean, 0., 500.);
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
  size_t idx = outputFileName.find_last_of("_cfi.py");
  std::string outputFileName_plots = std::string(outputFileName, 0, idx - (strlen("_cfi.py") - 1));
  outputFileName_plots.append(".pdf");
  makeControlPlots(dummyHistogram,
		   graph_uParl_mean, f_uParl_mean,
		   graph_uParl_div_qT_mean, f_uParl_div_qT_mean, 
		   graph_uParl_rms, f_uParl_rms,
		   graph_uPerp_mean, f_uPerp_mean,
		   graph_uPerp_rms, f_uPerp_rms,
		   isData, outputFileName_plots);

  int numEvents_processed = tree->GetEntries();

  delete tree;

  std::cout << "<fitZllRecoilCorrection>:" << std::endl;
  std::cout << " numEvents_processed: " << numEvents_processed << std::endl;

  clock.Show("fitZllRecoilCorrection");

  return 0;
}
