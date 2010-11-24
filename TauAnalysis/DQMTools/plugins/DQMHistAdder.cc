#include "TauAnalysis/DQMTools/plugins/DQMHistAdder.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

const double defaultScaleFactor = 1.;

const int verbosity = 0;

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistAdder::cfgEntryAddJob::cfgEntryAddJob(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<DQMHistAdder::cfgEntryAddJob>" << std::endl;

  unsigned numConfigs = 0;

  if ( cfg.exists("input") && cfg.exists("output") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgInputs = cfg.getParameter<vParameterSet>("input");
    for ( vParameterSet::const_iterator cfgInput = cfgInputs.begin();
	  cfgInput != cfgInputs.end(); ++cfgInput ) {
      meNames_input_.push_back(cfgInput->getParameter<std::string>("meName"));
      std::string meName_inputErr = ( cfgInput->exists("meName_err") ) ? cfgInput->getParameter<std::string>("meName_err") : "";
      meNames_inputErr_.push_back(meName_inputErr);
    }

    edm::ParameterSet cfgOutput = cfg.getParameter<edm::ParameterSet>("output");
    meName_output_ = cfgOutput.getParameter<std::string>("meName");
    meName_outputErr_ = ( cfgOutput.exists("meName_err") ) ? cfgOutput.getParameter<std::string>("meName_err") : "";

    ++numConfigs;
  }

  if ( cfg.exists("dqmDirectories_input") && cfg.exists("dqmDirectory_output") ) {
    dqmDirectories_input_ = cfg.getParameter<vstring>("dqmDirectories_input");
    dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectory_output");

    ++numConfigs;
  }

//--- check that either:
//     o individual MonitorElement
//    or
//     o DQM directory
//    configuration parameters are defined for input and output
  if ( numConfigs != 1 ) {
    edm::LogError("DQMHistAdder") 
      << " Need to specify either Configuration parameter 'input' and 'output' or"
      << " 'dqmDirectories_input' and 'dqmDirectory_output' !!";
    cfgError_ = 1;
  } 
  
  if ( verbosity ) print();
}

void DQMHistAdder::cfgEntryAddJob::print() const
{
  std::cout << "<cfgEntryAddJob::print>:" << std::endl;
  std::cout << " meNames_input = " << format_vstring(meNames_input_) << std::endl;
  std::cout << " meNames_inputErr = " << format_vstring(meNames_inputErr_) << std::endl;
  std::cout << " meName_output = " << meName_output_ << std::endl;
  std::cout << " meName_outputErr = " << meName_outputErr_ << std::endl;
  std::cout << " dqmDirectories_input = " << format_vstring(dqmDirectories_input_) << std::endl;
  std::cout << " dqmDirectory_output = " << dqmDirectory_output_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistAdder::DQMHistAdder(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<DQMHistAdder::DQMHistAdder>:" << std::endl;

//--- configure processes  
  //std::cout << "--> configuring addJobs..." << std::endl;
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgAddJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgAddJob = cfgAddJobs.begin();
	  cfgAddJob != cfgAddJobs.end(); ++cfgAddJob ) {
      cfgEntryAddJob addJob(*cfgAddJob);
      if ( addJob.cfgError_ ) cfgError_ = 1;
      addJobs_.push_back(addJob);
    }
  } else {
    std::vector<std::string> cfgAddJobNames = cfg.getParameterNamesForType<edm::ParameterSet>();
    for ( std::vector<std::string>::const_iterator cfgAddJobName = cfgAddJobNames.begin(); 
	  cfgAddJobName != cfgAddJobNames.end(); ++cfgAddJobName ) {
      edm::ParameterSet cfgAddJob = cfg.getParameter<edm::ParameterSet>(*cfgAddJobName);
      cfgEntryAddJob addJob(cfgAddJob);
      if ( addJob.cfgError_ ) cfgError_ = 1;
      addJobs_.push_back(addJob);
    }
  }
  
  //std::cout << "done." << std::endl;
}

DQMHistAdder::~DQMHistAdder() 
{
// nothing to be done yet...
}

void DQMHistAdder::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMHistAdder::endJob()
{
  std::cout << "<DQMHistAdder::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be added !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be added !!";
    return;
  }

//--- add histograms
  //std::cout << "--> adding histograms..." << std::endl;
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  for ( std::vector<cfgEntryAddJob>::const_iterator addJob = addJobs_.begin();
	addJob != addJobs_.end(); ++addJob ) {
    if ( addJob->meNames_input_.size() > 0 && addJob->meName_output_ != "" ) {

//--- check that all MonitorElements used as input are of the same type
      int meType_input = -1;
      for ( vstring::const_iterator meName_input = addJob->meNames_input_.begin();
	    meName_input != addJob->meNames_input_.end(); ++meName_input ) {
	MonitorElement* meInput = dqmStore.get(*meName_input);

	if ( !meInput ) {
	  edm::LogError ("endJob") 
	    << " Failed to retrieve MonitorElement " << (*meName_input) << " from dqmStore !!";
	  return;
	}

	if ( meName_input == addJob->meNames_input_.begin() ) meType_input = meInput->kind();

	if ( meType_input != meInput->kind() ) {
	  edm::LogError ("endJob") 
	    << " All MonitorElements to be added need to be of the same type !!";
	  return;
	}
      }

      if ( meType_input == MonitorElement::DQM_KIND_TH1F      ||
	   meType_input == MonitorElement::DQM_KIND_TH1S      ||
	   meType_input == MonitorElement::DQM_KIND_TH2F      ||
	   meType_input == MonitorElement::DQM_KIND_TH2S      ||
	   meType_input == MonitorElement::DQM_KIND_TH3F      ||
	   meType_input == MonitorElement::DQM_KIND_TPROFILE  ||
	   meType_input == MonitorElement::DQM_KIND_TPROFILE2D ) {
	std::auto_ptr<TH1> histoOutput;

	for ( vstring::const_iterator meName_input = addJob->meNames_input_.begin();
	      meName_input != addJob->meNames_input_.end(); ++meName_input ) {
	  MonitorElement* meInput = dqmStore.get(*meName_input);
	  TH1* histoInput = meInput->getTH1();
	  
	  if ( !histoOutput.get() ) {
	    histoOutput = std::auto_ptr<TH1>(dynamic_cast<TH1*>(histoInput->Clone()));
	    if ( !histoOutput->GetSumw2N() ) histoOutput->Sumw2();
	  } else {
	    histoOutput->Add(histoInput);
	  }
	}

//--- register output histogram as MonitorElement in DQMStore
	dqmRegisterHistogram(dqmStore, histoOutput.release(), addJob->meName_output_);
      } else if ( meType_input == MonitorElement::DQM_KIND_REAL ) {
	double sumValue = 0.;
	double sumErr2 = 0.;
	
	assert(addJob->meNames_input_.size() == addJob->meNames_inputErr_.size());
	
	bool errorFlag;

	unsigned numInputs = addJob->meNames_input_.size();
	for ( unsigned iInput = 0; iInput < numInputs; ++iInput ) {
	  const std::string& meName_input = addJob->meNames_input_[iInput];
	  double meValue_input = getValue(dqmStore, meName_input, errorFlag);
	  sumValue += meValue_input;
	  
	  const std::string& meName_inputErr = addJob->meNames_inputErr_[iInput];
	  if ( meName_inputErr != "" ) {
	    double meValue_inputErr = getValue(dqmStore, meName_inputErr, errorFlag);
	    sumErr2 += (meValue_inputErr*meValue_inputErr);
	  }
	}

//--- register output value and error sums as MonitorElement in DQMStore
	dqmRegisterFloat(dqmStore, sumValue, addJob->meName_output_);
	if ( addJob->meName_outputErr_ != "" ) dqmRegisterFloat(dqmStore, TMath::Sqrt(sumErr2), addJob->meName_outputErr_);
      } else {
	edm::LogError ("endJob") 
	  << " MonitorElements to be added of unknown or unsupported type" << " --> skipping !!";
	return;
      }
    } else if ( addJob->dqmDirectories_input_.size() > 0 && addJob->dqmDirectory_output_ != "" ) {
      const std::string& dqmDirectory_output = addJob->dqmDirectory_output_;
      for ( vstring::const_iterator dqmDirectory_input = addJob->dqmDirectories_input_.begin();
	    dqmDirectory_input != addJob->dqmDirectories_input_.end(); ++dqmDirectory_input ) {
	
	std::string inputDirectory = dqmDirectoryName(*dqmDirectory_input);
	std::cout << " inputDirectory = " << inputDirectory << std::endl;
	std::string outputDirectory = dqmDirectoryName(dqmDirectory_output);
	std::cout << " outputDirectory = " << outputDirectory << std::endl;
	
//--- when processing first inputDirectory, check that histograms in outputDirectory do not yet exist;
//    afterwards, add histograms in inputDirectory to those in outputDirectory
	int mode = ( dqmDirectory_input == addJob->dqmDirectories_input_.begin() ) ? 1 : 2;
	//std::cout << " mode = " << mode << std::endl;
	dqmCopyRecursively(dqmStore, inputDirectory, outputDirectory, 1., 0., mode, false);
      }
    }
  }

  std::cout << "done." << std::endl; 
  if ( verbosity ) dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistAdder);
