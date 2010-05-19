#include "TauAnalysis/DQMTools/plugins/DQMSimpleFileSaver.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>

const std::string dqmDirectory_temp = "tempBACKUP";

DQMSimpleFileSaver::DQMSimpleFileSaver(const edm::ParameterSet& cfg)
  : outputCommands_(0),
    cfgError_(0)
{
  //std::cout << "<DQMSimpleFileSaver::DQMSimpleFileSaver>:" << std::endl;

  if ( cfg.exists("outputCommands") ) {
    typedef std::vector<std::string> vstring;
    vstring outputCommands = cfg.getParameter<vstring>("outputCommands");

    TPRegexp validOutputCommandPattern_line("keep|drop\\s+[a-zA-Z0-9\\*/]+");
    TPRegexp validOutputCommandPattern_dqmDirectory("(keep|drop)\\s+([a-zA-Z0-9\\*/]+)");

    for ( vstring::const_iterator outputCommand = outputCommands.begin();
	  outputCommand != outputCommands.end(); ++outputCommand ) {

//--- check if output command specified in configuration file
//    matches the pattern "keep" or "drop" plus name of DQM directory
      TString outputCommand_tstring = outputCommand->data();
      if ( validOutputCommandPattern_line.Match(outputCommand_tstring) == 1 ) {

//--- match individually "keep" or drop statement and name of DQM directory;
//    require three matches (first match refers to entire line)
	TObjArray* subStrings = validOutputCommandPattern_dqmDirectory.MatchS(outputCommand_tstring);
	assert(subStrings->GetEntries() == 3);

	std::string statement = ((TObjString*)subStrings->At(1))->GetString().Data();
	int statement_int = -1;
	if ( statement == "keep" ) statement_int = kKeep;
	if ( statement == "drop" ) statement_int = kDrop;
	assert(statement_int == kKeep || statement_int == kDrop);
	//std::cout << " statement_int = " << statement_int << std::endl;
	
	std::string dqmDirectory = ((TObjString*)subStrings->At(2))->GetString().Data();
	int errorFlag = 0;
	std::string dqmDirectory_regexp = replace_string(dqmDirectory, "*", "[a-zA-Z0-9/]*", 0, 1000, errorFlag);
//--- match the names of all DQM subdirectories also
	dqmDirectory_regexp += "[a-zA-Z0-9/]*";
	//std::cout << " dqmDirectory_regexp = " << dqmDirectory_regexp << std::endl;

	if ( !outputCommands_ ) outputCommands_ = new std::vector<outputCommandEntry>();
	outputCommands_->push_back(outputCommandEntry(statement_int, TPRegexp(dqmDirectory_regexp.data())));
      } else {
	edm::LogError("DQMSimpleFileSaver") << " Invalid outputCommand = " << (*outputCommand) << " --> histograms will NOT be saved !!";
	cfgError_ = 1;
      }
    }
  }

  outputFileName_ = cfg.getParameter<std::string>("outputFileName");
  if ( outputFileName_ == "" ) {
    edm::LogError("DQMSimpleFileSaver") << " No outputFileName specified --> histograms will NOT be saved !!";
    cfgError_ = 1;
  } else if ( outputFileName_.find(".root") == std::string::npos ) {
    edm::LogError("DQMSimpleFileSaver") << " Invalid outputFileName = " << outputFileName_ << " --> histograms will NOT be saved !!";
    cfgError_ = 1;
  }
}

DQMSimpleFileSaver::~DQMSimpleFileSaver() 
{
  delete outputCommands_;
}

void DQMSimpleFileSaver::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMSimpleFileSaver::endJob()
{
  std::cout << "<DQMSimpleFileSaver::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be saved !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be saved !!";
    return;
  }  

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- in case output commands have been specified in configuration file,
//    copy subset of plots passing "keep" statements into temporary directory ("tempSAVE")
//    and save MonitorElements contained in temporary directory into ROOT file only;
//    else save all MonitorElements into ROOT file
  if ( outputCommands_ ) {
    dqmCopyRecursively(dqmStore, dqmRootDirectory, dqmDirectory_temp, 1.0, 0., 1, true);
    dqmCopyRecursively(dqmStore, dqmDirectory_temp, dqmRootDirectory, 1.0, 0., 1, false, outputCommands_);

    dqmStore.setCurrentFolder(dqmRootDirectory);
    dqmStore.rmdir(dqmDirectory_temp);

    dqmStore.save(outputFileName_); 
  } else {
    dqmStore.save(outputFileName_);  
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMSimpleFileSaver);
