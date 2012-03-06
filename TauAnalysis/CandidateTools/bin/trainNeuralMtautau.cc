
/** \executable trainNeuralMtautau
 *
 * Train neural network for reconstruction of tau+ tau- invariant mass
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: trainNeuralMtautau.cc,v 1.6 2011/09/30 12:26:40 veelken Exp $
 *
 */

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/FWLite/interface/InputSource.h"
#include "DataFormats/FWLite/interface/OutputFiles.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TBenchmark.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) 
{
//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  std::cout << "<trainNeuralMtautau>:" << std::endl;

//--- load framework libraries
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("trainNeuralMtautau");

//--- read python configuration parameters
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") ) 
    throw cms::Exception("trainNeuralMtautau") 
      << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!\n";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfgTrainNeuralMtautau = cfg.getParameter<edm::ParameterSet>("trainNeuralMtautau");

  std::string treeName = cfgTrainNeuralMtautau.getParameter<std::string>("treeName");

  std::string mvaType_string = cfgTrainNeuralMtautau.getParameter<std::string>("mvaType");
  std::cout << " mvaType = " << mvaType_string << std::endl;
  TMVA::Types::EMVA mvaType = TMVA::Types::kVariable;
  if      ( mvaType_string == "MLP" ) mvaType = TMVA::Types::kMLP;
  else if ( mvaType_string == "kNN" ) mvaType = TMVA::Types::kKNN;
  else throw cms::Exception("trainNeuralMtautau") 
    << "Invalid MVA type = " << mvaType_string << " !!\n";

  std::string mvaName = cfgTrainNeuralMtautau.getParameter<std::string>("mvaName");
  std::cout << " mvaName = " << mvaName << std::endl;

  std::string mvaTrainingOptions = cfgTrainNeuralMtautau.getParameter<std::string>("mvaTrainingOptions");
  std::cout << " mvaTrainingOptions = " << mvaTrainingOptions << std::endl;

  typedef std::vector<std::string> vstring;
  vstring inputBranchNames = cfgTrainNeuralMtautau.getParameter<vstring>("inputBranchNames");
  std::string targetBranchName = cfgTrainNeuralMtautau.getParameter<std::string>("targetBranchName");

  fwlite::InputSource inputFiles(cfg); 

  TChain* tree = new TChain(treeName.c_str());
  for ( vstring::const_iterator inputFileName = inputFiles.files().begin();
	inputFileName != inputFiles.files().end(); ++inputFileName ) {
    tree->AddFile(inputFileName->c_str());
  }

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());
  
  TMVA::Tools::Instance();
  TMVA::Factory* factory = new TMVA::Factory("trainNeuralMtautau", &fs.file(), "!V:!Silent:Color:DrawProgressBar");
  
  Float_t evtWeight = 1.0;
  factory->AddRegressionTree(tree, evtWeight);

  for ( vstring::const_iterator branchName = inputBranchNames.begin();
	branchName != inputBranchNames.end(); ++branchName ) {
    factory->AddVariable(branchName->c_str(), 'F'); 
  }
  factory->AddTarget(targetBranchName.c_str(), 'F');

  TCut cut = "";
  
  factory->PrepareTrainingAndTestTree(cut, "nTrain_Regression=0:nTest_Regression=0:SplitMode=Random:NormMode=NumEvents:!V");
  factory->BookMethod(mvaType, mvaName.c_str(), mvaTrainingOptions.c_str());
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();  
  
  delete factory;

  clock.Show("trainNeuralMtautau");

  return 0;
}
