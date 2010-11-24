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

DQMSimpleFileSaver::DQMSimpleFileSaver(const edm::ParameterSet& cfg)
  : outputCommands_(0),
    cfgError_(0)
{
  //std::cout << "<DQMSimpleFileSaver::DQMSimpleFileSaver>:" << std::endl;

  if ( cfg.exists("outputCommands") ) {
    typedef std::vector<std::string> vstring;
    vstring outputCommands = cfg.getParameter<vstring>("outputCommands");

    TPRegexp validOutputCommandPattern_line("keep|drop\\s+[a-zA-Z0-9_\\*/]+");
    TPRegexp validOutputCommandPattern_dqmDirectory("(keep|drop)\\s+([a-zA-Z0-9_\\*/]+)");

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
	
	std::string dqmNamePattern = ((TObjString*)subStrings->At(2))->GetString().Data();
	int errorFlag = 0;
	std::string dqmNamePattern_regexp = "[/]*";
	dqmNamePattern_regexp += replace_string(dqmNamePattern, "*", "[a-zA-Z0-9_/]*", 0, 1000, errorFlag);
//--- match the names of all DQM subdirectories also
	dqmNamePattern_regexp += "[a-zA-Z0-9_/]*";
	//std::cout << " dqmNamePattern_regexp = " << dqmNamePattern_regexp << std::endl;

	if ( !outputCommands_ ) outputCommands_ = new std::vector<outputCommandEntry>();
	outputCommands_->push_back(outputCommandEntry(statement_int, TPRegexp(dqmNamePattern_regexp.data())));
      } else {
	edm::LogError("DQMSimpleFileSaver") 
	  << " Invalid outputCommand = " << (*outputCommand) << " --> histograms will NOT be saved !!";
	cfgError_ = 1;
      }
    }
  }

  outputFileName_ = cfg.getParameter<std::string>("outputFileName");
  if ( outputFileName_ == "" ) {
    edm::LogError("DQMSimpleFileSaver") 
      << " No outputFileName specified --> histograms will NOT be saved !!";
    cfgError_ = 1;
  } else if ( outputFileName_.find(".root") == std::string::npos ) {
    edm::LogError("DQMSimpleFileSaver") 
      << " Invalid outputFileName = " << outputFileName_ << " --> histograms will NOT be saved !!";
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

int dqmPruneRecursively(DQMStore& dqmStore, const std::string& dqmDirectory, 
			std::vector<outputCommandEntry>& outputCommands)
{
  //std::cout << "<dqmPruneRecursively>:" << std::endl;
  //std::cout << " dqmDirectory = " << inputDirectory << std::endl;
  //std::cout << " outputCommands = " << format_vstring(outputCommands) << std::endl;
  //std::string state_string = ( state = kKeep ) ? "keep" : "drop";
  //std::cout << " state = " << state_string << std::endl;

  int dqmDirectoryState = kDrop;

  dqmStore.setCurrentFolder(dqmDirectory);
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    std::string subDirName = dqmSubDirectoryName(dqmDirectory, *dirName);
    
    std::string subDirName_full = dqmDirectoryName(dqmDirectory).append(subDirName);
    
    int subDirState = dqmPruneRecursively(dqmStore, subDirName_full, outputCommands);
    if ( subDirState == kKeep ) {
      dqmDirectoryState = kKeep;
    } else {
      dqmStore.rmdir(subDirName_full);
    }
  }
  
  dqmStore.setCurrentFolder(dqmDirectory);
  std::vector<std::string> meNames = dqmStore.getMEs();
  //std::cout << " #meNames = " << meNames.size() << std::endl;
  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    std::string meName_full = dqmDirectoryName(dqmDirectory).append(*meName);
    //std::cout << " meName_full = " << meName_full << std::endl;

    bool meState = kKeep;

    for ( std::vector<outputCommandEntry>::iterator outputCommand = outputCommands.begin();
	  outputCommand != outputCommands.end(); ++outputCommand ) {
      int statement = outputCommand->first;
      std::string statement_string = ( statement == kKeep ) ? "keep" : "drop";
      //std::cout << " statement = " << statement_string << std::endl;
      
      TPRegexp& dqmNamePattern_regexp = outputCommand->second;
      //std::cout << " dqmNamePattern_regexp = " << dqmNamePattern_regexp.GetPattern() << std::endl;
      
      TString meName_tstring = meName_full.data();
      
      //std::cout << "--> dqmNamePattern_regexp.Match(meName_tstring) = "
      //          << dqmNamePattern_regexp.Match(meName_tstring) << std::endl;
      
      if ( dqmNamePattern_regexp.Match(meName_tstring) == 1 ) meState = statement;
    }
    
    if ( meState == kKeep ) {
      dqmDirectoryState = kKeep;
    } else {
      dqmStore.removeElement(*meName);
    }
  }

  return dqmDirectoryState;
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
//    recursively browser through DQM directory structure
//    and determine which DQM MonitorElements to keep and which to drop
  if ( outputCommands_ ) dqmPruneRecursively(dqmStore, dqmRootDirectory, *outputCommands_);

//--- save (remaining) DQM MonitorElements into ROOT file
  dqmStore.save(outputFileName_);  
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMSimpleFileSaver);
