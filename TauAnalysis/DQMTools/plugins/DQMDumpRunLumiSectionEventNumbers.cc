#include "TauAnalysis/DQMTools/plugins/DQMDumpRunLumiSectionEventNumbers.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <map>

DQMDumpRunLumiSectionEventNumbers::DQMDumpRunLumiSectionEventNumbers(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMDumpRunLumiSectionEventNumbers::DQMDumpRunLumiSectionEventNumbers>:" << std::endl;

  cfgError_ = 0;

  dqmDirectories_ = cfg.getParameter<vstring>("dqmDirectories");
  if ( dqmDirectories_.size() == 0 ) {
    edm::LogError("DQMDumpRunLumiSectionEventNumbers")
      << " Configuration Parameter dqmDirectories = " << format_vstring(dqmDirectories_)
      << " contains no Entries --> skipping !!";
    cfgError_ = 1;
  }

  runLumiSectionEventNumberFileName_ = cfg.getParameter<std::string>("runLumiSectionEventNumberFileName");
  if ( runLumiSectionEventNumberFileName_ == "" ) {
    edm::LogError("DQMDumpRunLumiSectionEventNumbers")
      << " Configuration Parameter runLumiSectionEventNumberFileName_ = " << runLumiSectionEventNumberFileName_
      << " invalid --> skipping !!";
    cfgError_ = 1;
  }

  separator_ = cfg.exists("separator") ?
    cfg.getParameter<std::string>("separator") : " ";
}

DQMDumpRunLumiSectionEventNumbers::~DQMDumpRunLumiSectionEventNumbers()
{
//--- nothing to be done yet
}

void DQMDumpRunLumiSectionEventNumbers::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMDumpRunLumiSectionEventNumbers::endJob()
{
  std::cout << "<DQMDumpRunLumiSectionEventNumbers::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob")
      << " Error in Configuration ParameterSet --> run + event Numbers will NOT be written !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob")
      << " Failed to access dqmStore --> run + event Numbers will NOT be written !!";
    return;
  }

  TPRegexp regexpParser_triplet("r[[:digit:]]+ls[[:digit:]]+ev[[:digit:]]+");
  TPRegexp regexpParser_number("r([[:digit:]]+)ls([[:digit:]]+)ev([[:digit:]]+)");

  typedef std::set<edm::EventNumber_t> eventNumberSet;
  typedef std::map<edm::LuminosityBlockNumber_t, eventNumberSet> lumiSectionEventNumberMap;
  std::map<edm::RunNumber_t, lumiSectionEventNumberMap> runLumiSectionEventNumbers;

//--- decode run + event numbers from names of MonitorElements
  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( vstring::const_iterator dqmDirectory = dqmDirectories_.begin();
	dqmDirectory != dqmDirectories_.end(); ++dqmDirectory ) {
    std::cout << "dqmDirectory = " << (*dqmDirectory);

    dqmStore.setCurrentFolder(*dqmDirectory);
    std::vector<std::string> meNames = dqmStore.getMEs();
    std::cout << " contains " << meNames.size() << " monitor elements. " << std::endl;
    for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	  meName != meNames.end(); ++meName ) {
      //std::cout << "meName = " << (*meName) << std::endl;

      bool parseError = false;

      TString meName_tstring = meName->data();
//--- check if name of MonitorElement matches run + event + luminosity section number format
      if ( regexpParser_triplet.Match(meName_tstring) == 1 ) {

//--- match individual run, event and luminosity section numbers;
//    require four matches (first match refers to entire line)
	TObjArray* subStrings = regexpParser_number.MatchS(meName_tstring);
	int numSubStrings = subStrings->GetEntries();
	if ( numSubStrings == 4 ) {
          //std::cout << ((TObjString*)subStrings->At(1))->GetString() << std::endl;
	  //std::cout << ((TObjString*)subStrings->At(2))->GetString() << std::endl;
	  //std::cout << ((TObjString*)subStrings->At(3))->GetString() << std::endl;

	  edm::RunNumber_t runNumber = ((TObjString*)subStrings->At(1))->GetString().Atoll();
	  edm::LuminosityBlockNumber_t lumiSectionNumber = ((TObjString*)subStrings->At(2))->GetString().Atoll();
	  edm::EventNumber_t eventNumber = ((TObjString*)subStrings->At(3))->GetString().Atoll();

	  std::cout << "--> adding Run# = " << runNumber << ","
		    << " Luminosity Section# " << lumiSectionNumber << ","
		    << " Event# " << eventNumber << std::endl;
	  runLumiSectionEventNumbers[runNumber][lumiSectionNumber].insert(eventNumber);
	} else {
	  parseError = true;
	}
      } else {
	parseError = true;
      }

      if ( parseError ) {
	edm::LogError ("endJob")
	  << " Failed to decode run + luminosity section + event Number from meName = " << (*meName) << " --> skipping !!";
      }
    }
  }

//--- write run + event numbers decoded from names of MonitorElements into ASCII file
  ofstream runLumiSectionEventNumbersFile(runLumiSectionEventNumberFileName_.data(), std::ios::out);
  int numRunLumiSectionEventNumbersWritten = 0;
  for ( std::map<edm::RunNumber_t, lumiSectionEventNumberMap>::const_iterator run = runLumiSectionEventNumbers.begin();
	run != runLumiSectionEventNumbers.end(); ++run ) {
    for ( lumiSectionEventNumberMap::const_iterator lumiSection = run->second.begin();
	  lumiSection != run->second.end(); ++lumiSection ) {
      for ( eventNumberSet::const_iterator event = lumiSection->second.begin();
	    event != lumiSection->second.end(); ++event ) {
	runLumiSectionEventNumbersFile << run->first << separator_ << lumiSection->first << separator_ << (*event) << std::endl;
	++numRunLumiSectionEventNumbersWritten;
      }
    }
  }

  edm::LogInfo ("endjob") << " Number of Events selected = " << numRunLumiSectionEventNumbersWritten << ".";
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpRunLumiSectionEventNumbers);
