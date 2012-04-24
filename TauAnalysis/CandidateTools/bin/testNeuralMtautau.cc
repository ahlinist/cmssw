
/** \executable testNeuralMtautau
 *
 * Test neural network for reconstruction of tau+ tau- invariant mass
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: testNeuralMtautau.cc,v 1.1 2012/03/06 17:34:42 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/NeuralMtautauAlgorithm.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH2.h>
#include <TBenchmark.h>
#include <TMath.h>
#include <TMatrixD.h>

#include <iostream>
#include <string>
#include <vector>

struct inputVarEntryType
{
  TTreeFormula* treeFormula_;
  Float_t value_;
};

namespace
{
  double square(double x)
  { 
    return x*x;
  }
}

using namespace NSVfitStandalone;

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<testNeuralMtautau>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("testNeuralMtautau");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("testNeuralMtautau") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgTestNeuralMtautau = cfg.getParameter<edm::ParameterSet>("testNeuralMtautau");

  std::string treeName = cfgTestNeuralMtautau.getParameter<std::string>("treeName");

  typedef std::vector<std::string> vstring;
  vstring inputBranchNames = cfgTestNeuralMtautau.getParameter<vstring>("inputBranchNames");
  std::string targetBranchName = cfgTestNeuralMtautau.getParameter<std::string>("targetBranchName");
  std::string NNconfigFileName = cfgTestNeuralMtautau.getParameter<std::string>("NNconfigFileName");

  NeuralMtautauAlgorithm* neuralMtautauAlgorithm = new NeuralMtautauAlgorithm(NNconfigFileName.data());

  bool runSVfit = cfgTestNeuralMtautau.getParameter<bool>("runSVfit");
  std::cout << " runSVfit = " << runSVfit << std::endl;

  fwlite::InputSource inputFiles(cfg); 
  int maxEvents = inputFiles.maxEvents();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());
  TH1* histogramRecVsGenMass1 = 
    fs.make<TH2D>("histogramRecVsGenMass1", 
  		  "rec. NeuralM_{#tau#tau} vs. M_{#tau#tau}^{gen}", 400, 0., 2000., 400, 0., 2000.);
  TH1* histogramRecVsGenMass2 = 
    fs.make<TH2D>("histogramRecVsGenMass2", 
  		  "rec. NeuralM_{#tau#tau} vs. M_{#tau#tau}^{gen}", 400, 0., 2000., 400, 0., 2000.);

  TMVA::Tools::Instance();

  TMVA::Reader* NN = new TMVA::Reader("!Color:!Silent");   

  std::map<std::string, inputVarEntryType*> inputBranchMap; // key = branch name/TTreeFormula
  for ( vstring::const_iterator inputBranchName = inputBranchNames.begin();
	inputBranchName != inputBranchNames.end(); ++inputBranchName ) {
    inputVarEntryType* inputVarEntry = new inputVarEntryType();
    inputBranchMap[*inputBranchName] = inputVarEntry;
    NN->AddVariable(inputBranchName->data(), &inputVarEntry->value_);
  }

  NN->BookMVA("trainNeuralMtautau", NNconfigFileName.data());

  int numEvents_processed = 0;
  
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
  	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    TFile* inputFile = new TFile(inputFileName->data());
    TTree* tree = dynamic_cast<TTree*>(inputFile->Get(treeName.c_str()));

    int idx = 0;
    for ( vstring::const_iterator inputBranchName = inputBranchNames.begin();
	  inputBranchName != inputBranchNames.end(); ++inputBranchName ) {
      inputVarEntryType* inputVarEntry = inputBranchMap[*inputBranchName];
      std::string treeFormulaName = Form("treeFormula%i", idx);
      inputVarEntry->treeFormula_ = new TTreeFormula(treeFormulaName.data(), inputBranchName->data(), tree);
      ++idx;
    }

//--- define variables needed to run SVfit algorithm for comparisson
    Float_t recLeg1En, recLeg1Px, recLeg1Py, recLeg1Pz;
    Float_t recLeg2En, recLeg2Px, recLeg2Py, recLeg2Pz;
    Float_t recMEtPx, recMEtPy, recMEtVxx, recMEtVxy, recMEtVyy;
    Float_t recSVfitMtautau; 
    if ( runSVfit ) {
      tree->SetBranchAddress("recLeg1En", &recLeg1En);
      tree->SetBranchAddress("recLeg1Px", &recLeg1Px);
      tree->SetBranchAddress("recLeg1Py", &recLeg1Py);
      tree->SetBranchAddress("recLeg1Pz", &recLeg1Pz);
      tree->SetBranchAddress("recLeg2En", &recLeg2En);
      tree->SetBranchAddress("recLeg2Px", &recLeg2Px);
      tree->SetBranchAddress("recLeg2Py", &recLeg2Py);
      tree->SetBranchAddress("recLeg2Pz", &recLeg2Pz);
      tree->SetBranchAddress("recMEtPx", &recMEtPx);
      tree->SetBranchAddress("recMEtPy", &recMEtPy);
      tree->SetBranchAddress("recMEtVxx", &recMEtVxx);
      tree->SetBranchAddress("recMEtVxy", &recMEtVxy);
      tree->SetBranchAddress("recMEtVyy", &recMEtVyy);
      tree->SetBranchAddress("recSVfitMtautau", &recSVfitMtautau);
    }

    Float_t target;
    tree->SetBranchAddress(targetBranchName.data(), &target);

    int numEntries = tree->GetEntries();
    for ( int iEntry = 0; iEntry < numEntries && (numEvents_processed < maxEvents || maxEvents == -1); ++iEntry ) {
      //std::cout << "Event #" << numEvents_processed << ":" << std::endl;
      if ( numEvents_processed > 0 && (numEvents_processed % 1000) == 0 ) {
	std::cout << "processing Event " << numEvents_processed << std::endl;
      }

      tree->GetEntry(iEntry);

      for ( vstring::const_iterator inputBranchName = inputBranchNames.begin();
	    inputBranchName != inputBranchNames.end(); ++inputBranchName ) {
	inputVarEntryType* inputVarEntry = inputBranchMap[*inputBranchName];
	// CV: TTreeFormula::EvalInstance causes segmentation violation
	//     if used with TChains consisting of more than one input file
	inputVarEntry->value_ = inputVarEntry->treeFormula_->EvalInstance();
      }
     
      Float_t genMtautau = target;
      Float_t recMtautau1 = NN->EvaluateRegression("trainNeuralMtautau")[0];
      std::cout << " Mtautau: gen = " << genMtautau << " --> rec (1) = " << recMtautau1 << std::endl;
      histogramRecVsGenMass1->Fill(genMtautau, recMtautau1);     

      reco::Candidate::LorentzVector recLeg1P4(recLeg1Px, recLeg1Py, recLeg1Pz, recLeg1En);
      reco::Candidate::LorentzVector recLeg2P4(recLeg2Px, recLeg2Py, recLeg2Pz, recLeg2En);
      TMatrixD recMEtCov(2, 2);
      recMEtCov(0, 0) = recMEtVxx;
      recMEtCov(0, 1) = recMEtVxy;
      recMEtCov(1, 0) = recMEtVxy;
      recMEtCov(1, 1) = recMEtVyy;
      Float_t recMtautau2 = (*neuralMtautauAlgorithm)(recLeg1P4, recLeg2P4, recMEtPx, recMEtPy, recMEtCov);
      std::cout << " Mtautau: gen = " << genMtautau << " --> rec (2) = " << recMtautau2 << std::endl;
      histogramRecVsGenMass2->Fill(genMtautau, recMtautau2);  
      
      if ( runSVfit ) {
	std::vector<MeasuredTauLepton> tauDecayProduts;
	reco::Candidate::LorentzVector recLeg1P4(recLeg1Px, recLeg1Py, recLeg1Pz, recLeg1En);
	tauDecayProduts.push_back(MeasuredTauLepton(kLepDecay, recLeg1P4));
	reco::Candidate::LorentzVector recLeg2P4(recLeg2Px, recLeg2Py, recLeg2Pz, recLeg2En);
	tauDecayProduts.push_back(MeasuredTauLepton(kHadDecay, recLeg2P4));
	Vector recMEtP3(recMEtPx, recMEtPy, 0.);
	TMatrixD pfMEtSignCovMatrix(2,2);
	pfMEtSignCovMatrix(0,0) = recMEtVxx;
	pfMEtSignCovMatrix(0,1) = recMEtVxy;
	pfMEtSignCovMatrix(1,0) = recMEtVxy;
	pfMEtSignCovMatrix(1,1) = recMEtVyy;
	NSVfitStandaloneAlgorithm svFit(tauDecayProduts, recMEtP3, pfMEtSignCovMatrix);
	svFit.fit();
	double svFitMtautau = ( svFit.isValidSolution() ) ?
	  svFit.mass() : -1.;
	//std::cout << " svFitMtautau = " << svFitMtautau << " (in file = " << recSVfitMtautau << ")" << std::endl;
      }

      ++numEvents_processed;
    }

    for ( vstring::const_iterator inputBranchName = inputBranchNames.begin();
	  inputBranchName != inputBranchNames.end(); ++inputBranchName ) {
      inputVarEntryType* inputVarEntry = inputBranchMap[*inputBranchName];
      delete inputVarEntry->treeFormula_;
    }
    
    delete inputFile;
  }
  
  delete neuralMtautauAlgorithm;

  clock.Show("testNeuralMtautau");

  return 0;
}
