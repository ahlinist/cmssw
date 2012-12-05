
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/RecoTools/bin/fitSysMEtShiftAuxFunctions.h"

#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TBenchmark.h>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <assert.h>

TH1* getHistogram(TFile* inputFile, const TString& dqmDirectory, const TString& meName)
{  
  //std::cout << "inputFile = " << inputFile->GetName() << std::endl;

  TString histogramName = TString(dqmDirectory);
  if ( !histogramName.EndsWith("/") ) histogramName.Append("/");
  histogramName.Append(meName);

  TH1* histogram = (TH1*)inputFile->Get(histogramName.Data());
  //std::cout << "histogramName = " << histogramName.Data() << ": histogram = " << histogram;
  //if ( histogram ) std::cout << ", integral = " << histogram->Integral();
  //std::cout << std::endl; 

  if ( histogram && !histogram->GetSumw2N() ) histogram->Sumw2();
  else if ( !histogram) 
    std::cerr << "Failed to load histogram = " << histogramName << " from file = " << inputFile->GetName() << " !!" << std::endl;

  if ( histogram->Integral() > 0. ) histogram->Scale(histogram->GetEntries()/histogram->Integral());

  return histogram;
}

void runFitVsSumEt(TFile* inputFile, const std::string& sample, const std::string& runPeriod, 
		   const std::string& dqmDirectory, 
		   const std::string& meNameMETxVsSumEt, const std::string& meNameMETyVsSumEt, 
		   const std::string& label)
{
  
  TH2* histogramMETxVsSumEt = dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, meNameMETxVsSumEt.data()));
  TH2* histogramMETyVsSumEt = dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, meNameMETyVsSumEt.data()));
  
  std::cout << "running PFMET sys. Shift fits vs. sumEt... (sample = " << sample << ", all jet multiplicities)" << std::endl;
  std::string outputFileNameMETxVsSumEt = Form("plots/sysMETxShiftVsSumEt_%s_%s_%s.eps", sample.data(), runPeriod.data(), label.data());
  fitMETxyVsSumEt(histogramMETxVsSumEt, 120., 800., 5., 
		  "#Sigma E_{T}", "#Sigma E_{T} / GeV", "<E_{x}^{miss}> / GeV", outputFileNameMETxVsSumEt);
  std::string outputFileNameMETyVsSumEt = Form("plots/sysMETyShiftVsSumEt_%s_%s_%s.eps", sample.data(), runPeriod.data(), label.data());
  fitMETxyVsSumEt(histogramMETyVsSumEt, 120., 800., 5.,
		  "#Sigma E_{T}", "#Sigma E_{T} / GeV", "<E_{y}^{miss}> / GeV", outputFileNameMETyVsSumEt);
}

void runFitVsNumVertices(TFile* inputFile, const std::string& sample, const std::string& runPeriod, 
			 const std::string& dqmDirectory, 
			 const std::string& meNameMETxVsNumVertices, const std::string& meNameMETyVsNumVertices, 
			 const std::string& label)
{
  TH2* histogramMETxVsNumVertices = dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, meNameMETxVsNumVertices.data()));
  TH2* histogramMETyVsNumVertices = dynamic_cast<TH2*>(getHistogram(inputFile, dqmDirectory, meNameMETyVsNumVertices.data()));

  std::cout << "running PFMET sys. Shift fits vs. Nvtx... (sample = " << sample << ", all jet multiplicities)" << std::endl;
  std::string outputFileNameMETxVsNumVertices = Form("plots/sysMETxShiftVsNumVertices_%s_%s_%s.eps", sample.data(), runPeriod.data(), label.data());
  fitMETxyVsSumEt(histogramMETxVsNumVertices, 0., 25., 0.5,
		  "N_{vtx}", "N_{vtx}", "<E_{x}^{miss}> / GeV", outputFileNameMETxVsNumVertices);
  std::string outputFileNameMETyVsNumVertices = Form("plots/sysMETyShiftVsNumVertices_%s_%s_%s.eps", sample.data(), runPeriod.data(), label.data());
  fitMETxyVsSumEt(histogramMETyVsNumVertices, 0., 25., 0.5, 
		  "N_{vtx}", "N_{vtx}", "<E_{y}^{miss}> / GeV", outputFileNameMETyVsNumVertices);
}

int main(int argc, const char* argv[])
{
  std::cout << "<fitSysMEtShift>:" << std::endl;  

//--- stop ROOT from keeping references to all histograms
  TH1::AddDirectory(false);

//--- disable pop-up windows showing graphics output
  gROOT->SetBatch(true);

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("fitSysMEtShift");

  typedef std::vector<std::string> vstring;
  vstring samples;
  samples.push_back("Data");
  samples.push_back("ZplusJets_madgraph");

  std::string runPeriod = "2012RunABC";

  std::string inputFilePath;
  if ( runPeriod == "2012RunABC" ) inputFilePath = "/data1/veelken/tmp/ZllRecoilCorrection/v7_00_woMEtShiftCorr_pfMEtCov/2012RunABC/";
  else assert(0);

  std::map<std::string, std::string> inputFileNames; // key = sample
  inputFileNames["Data"] = "analyzeZllRecoilCorrectionHistograms_Data_%s_central.root";
  inputFileNames["ZplusJets_madgraph"] = "analyzeZllRecoilCorrectionHistograms_ZplusJets_madgraph_%s_central.root";

  std::map<std::string, vstring> metTypes; // key = sample
  //metTypes["Data"].push_back("pfMEtTypeIcorrectedSmeared");
  //metTypes["Data"].push_back("pfMEtNoPileUpSmeared");
  //metTypes["Data"].push_back("pfMEtMVASmeared");
  metTypes["Data"].push_back("caloMEtTypeIcorrected");
  //metTypes["ZplusJets_madgraph"].push_back("pfMEtTypeIcorrectedSmeared");
  //metTypes["ZplusJets_madgraph"].push_back("pfMEtNoPileUpSmeared");
  //metTypes["ZplusJets_madgraph"].push_back("pfMEtMVASmeared");
  metTypes["ZplusJets_madgraph"].push_back("caloMEtTypeIcorrected");
  
  for ( vstring::const_iterator sample = samples.begin();
	sample != samples.end(); ++sample ) {
    for ( vstring::const_iterator metType = metTypes[*sample].begin();
	  metType != metTypes[*sample].end(); ++metType ) {
      std::string inputFileName = Form(inputFileNames[*sample].data(), metType->data());
      std::cout << "processing sample = " << (*sample) << ", inputFileName = " << inputFileName << std::endl;
      
      TString inputFileName_full = inputFilePath.data();
      if ( !inputFileName_full.EndsWith("/") ) inputFileName_full.Append("/");
      inputFileName_full.Append(inputFileName.data());
      
      TFile* inputFile = new TFile(inputFileName_full.Data());

      gROOT->SetBatch(true);

      TString dqmDirectory = Form("%s/beforeAddPUreweight/", sample->data());

      std::string meNameMETxVsSumEt       = "metXvsSumEt";
      std::string meNameMETyVsSumEt       = "metYvsSumEt";
      std::string meNameMETxVsNumVertices = "metXvsNumVertices";
      std::string meNameMETyVsNumVertices = "metYvsNumVertices";

      // run fit for inclusive event sample (= all jet multiplicities)
      //std::cout << "running PFMET sys. Shift fits vs. sumEt... (sample = " << (*sample) << ", all jet multiplicities)" << std::endl;
      //runFitVsSumEt(inputFile, *sample, runPeriod, 
      //	      dqmDirectory.Data(), meNameMETxVsSumEt, meNameMETyVsSumEt,
      //	      Form("%s_all", metType->data()));

      std::cout << "running PFMET sys. Shift fits vs. Nvtx... (sample = " << (*sample) << ", all jet multiplicities)" << std::endl;
      runFitVsNumVertices(inputFile, *sample, runPeriod, 
			  dqmDirectory.Data(), meNameMETxVsNumVertices, meNameMETyVsNumVertices,
			  Form("%s_all", metType->data()));

      // run fits in bits of jet multiplicity
      typedef std::pair<int, int> pint;
      std::vector<pint> jetMultiplicities;
      //jetMultiplicities.push_back(pint( 0, 0));
      //jetMultiplicities.push_back(pint( 1, 1));
      //jetMultiplicities.push_back(pint( 2, 2));
      //jetMultiplicities.push_back(pint( 3,-1));
      std::vector<double> jetPtThresholds;
      //jetPtThresholds.push_back(10.);
      //jetPtThresholds.push_back(20.);
      //jetPtThresholds.push_back(30.);

      for ( std::vector<pint>::const_iterator jetMultiplicity = jetMultiplicities.begin();
	    jetMultiplicity != jetMultiplicities.end(); ++jetMultiplicity ) {
	for ( std::vector<double>::const_iterator jetPtThreshold = jetPtThresholds.begin();
	      jetPtThreshold != jetPtThresholds.end(); ++jetPtThreshold ) {
	  int numJetsMin = jetMultiplicity->first;
	  int numJetsMax = jetMultiplicity->second;
	  TString label = Form("NumJetsPtGt%1.0f", *jetPtThreshold);
	  if      ( numJetsMin == -1         ) label.Append(Form("Le%i",   numJetsMax));
	  else if ( numJetsMax == -1         ) label.Append(Form("Ge%i",   numJetsMin));
	  else if ( numJetsMin == numJetsMax ) label.Append(Form("Eq%i",   numJetsMin));
	  else                                 label.Append(Form("%ito%i", numJetsMin, numJetsMax));
	  
	  std::cout << "running PFMET sys. Shift fits vs. sumEt... (sample = " << (*sample) << "," 
		    << Form(" #jets(Pt > %1.0f GeV) = %i..%i", *jetPtThreshold, numJetsMin, numJetsMax) << std::endl;
	  runFitVsSumEt(inputFile, *sample, runPeriod, 
			dqmDirectory.Data(), 
			Form("%s%s", meNameMETxVsSumEt.data(), label.Data()),
			Form("%s%s", meNameMETyVsSumEt.data(), label.Data()),
			Form("%s_%s", metType->data(), label.Data()));
	  
	  std::cout << "running PFMET sys. Shift fits vs. Nvtx... (sample = " << (*sample) << "," 
		    << Form(" #jets(Pt > %1.0f GeV) = %i..%i", *jetPtThreshold, numJetsMin, numJetsMax) << std::endl;
	  runFitVsNumVertices(inputFile, *sample, runPeriod, 
			      dqmDirectory.Data(), 
			      Form("%s%s", meNameMETxVsNumVertices.data(), label.Data()),
			      Form("%s%s", meNameMETyVsNumVertices.data(), label.Data()),
			      Form("%s_%s", metType->data(), label.Data()));
	}
      }
      delete inputFile;
    }
  }

//--print time that it took macro to run
  std::cout << "finished executing fitSysMEtShift macro:" << std::endl;
  clock.Show("fitSysMEtShift");

  return 0;
}
