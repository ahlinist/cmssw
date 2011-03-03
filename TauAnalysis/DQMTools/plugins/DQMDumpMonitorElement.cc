#include "TauAnalysis/DQMTools/plugins/DQMDumpMonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <TH1.h>

#include <iostream>

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpMonitorElement::jobEntryType::jobEntryType(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMDumpMonitorElement::jobEntryType>" << std::endl;

  meName_ = cfg.getParameter<std::string>("meName");
  meName_err_ = ( cfg.exists("meName_err") ) ? cfg.getParameter<std::string>("meName_err") : "";

  label_ = cfg.getParameter<std::string>("label");

  if ( cfg.exists("processes") ) processes_ = cfg.getParameter<vstring>("processes");
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMDumpMonitorElement::DQMDumpMonitorElement(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMDumpMonitorElement::DQMDumpMonitorElement>:" << std::endl;

  //std::cout << "--> configuring dumpJobs..." << std::endl;
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgDumpJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgDumpJob = cfgDumpJobs.begin();
	  cfgDumpJob != cfgDumpJobs.end(); ++cfgDumpJob ) {
      jobEntryType dumpJob(*cfgDumpJob);
      dumpJobs_.push_back(dumpJob);
    }
  } else {
    jobEntryType dumpJob(cfg);
    dumpJobs_.push_back(dumpJob);
  }
  
  //std::cout << "done." << std::endl;
}

DQMDumpMonitorElement::~DQMDumpMonitorElement() 
{
// nothing to be done yet...
}

void DQMDumpMonitorElement::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMDumpMonitorElement::endJob()
{
  std::cout << "<DQMDumpMonitorElement::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be added !!";
    return;
  }

//--- print-out values (and errors) of MonitorElements
  //std::cout << "--> dumping MonitorElements..." << std::endl;
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  for ( std::vector<jobEntryType>::const_iterator dumpJob = dumpJobs_.begin();
	dumpJob != dumpJobs_.end(); ++dumpJob ) {
    if ( dumpJob->meName_.find(processDirKeyword) != std::string::npos ) {

      if ( dumpJob->processes_.size() == 0 ) {
	edm::LogError ("endJob") 
	  << " MonitorElement meName = " << dumpJob->meName_ << " contains keyword '" << processDirKeyword << "'," 
	  << " but no processes have been defined !!";
      }

      std::cout << dumpJob->label_ << ":" << std::endl;

      for ( vstring::const_iterator process = dumpJob->processes_.begin();
	    process != dumpJob->processes_.end(); ++process ) {
	int errorFlag = 0;       
	std::string meName_expanded = replace_string(dumpJob->meName_, processDirKeyword, *process, 0, 1, errorFlag);
	std::string meName_err_expanded = ( dumpJob->meName_err_ != "" ) ?
	  replace_string(dumpJob->meName_err_, processDirKeyword, *process, 0, 1, errorFlag) : "";

	if ( !errorFlag ) dqmDumpMonitorElement(dqmStore, meName_expanded, meName_err_expanded, *process, std::cout);
      }
    } else {
      dqmDumpMonitorElement(dqmStore, dumpJob->meName_, dumpJob->meName_err_, dumpJob->label_, std::cout);
    }
  }

  std::cout << "done." << std::endl; 
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpMonitorElement);
