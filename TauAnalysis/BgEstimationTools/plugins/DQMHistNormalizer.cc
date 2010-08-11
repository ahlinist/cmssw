#include "TauAnalysis/BgEstimationTools/plugins/DQMHistNormalizer.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH1.h>

#include <iostream>

DQMHistNormalizer::jobEntryType::jobEntryType(const edm::ParameterSet& cfg)
  : meName_input_(""),
    meName_output_(""),
    dqmDirectory_input_(""),
    dqmDirectory_output_(""),
    cfgError_(0)
{
  unsigned numConfigs = 0;

  if ( cfg.exists("meNameInput") && cfg.exists("meNameOutput") ) {
    meName_input_ = cfg.getParameter<std::string>("meNameInput");
    meName_output_ = cfg.getParameter<std::string>("meNameOutput");

    ++numConfigs;
  }

  if ( cfg.exists("dqmDirectoryInput") && cfg.exists("dqmDirectoryOutput") ) {
    dqmDirectory_input_ = cfg.getParameter<std::string>("dqmDirectoryInput");
    dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectoryOutput");

    ++numConfigs;
  }

  if ( numConfigs != 1 ) {
    edm::LogError("DQMHistNormalizer") 
      << " Need to specify either Configuration parameter 'meNameInput' and 'meNameOutput' or"
      << " 'dqmDirectoryInput' and 'dqmDirectoryOutput' !!";
    cfgError_ = 1;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistNormalizer::DQMHistNormalizer(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistNormalizer::DQMHistNormalizer>:" << std::endl;

//--- check if multiple histograms or just a single one should be normalized
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgJob = cfgJobs.begin();
	  cfgJob != cfgJobs.end(); ++cfgJob ) {
      jobEntryType jobEntry(*cfgJob);
      jobs_.push_back(jobEntry);
    }
  } else {
    jobEntryType jobEntry(cfg);
    jobs_.push_back(jobEntry);
  } 
  
  norm_ = cfg.exists("norm") ? cfg.getParameter<double>("norm") : 1.;
}

DQMHistNormalizer::~DQMHistNormalizer() 
{
//--- nothing to be done yet
}

void DQMHistNormalizer::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void addNormalizedHistogram(DQMStore& dqmStore, 
			    const std::string& meNameInput_full, const std::string& meNameOutput_full, double norm)
{
  //std::cout << "<addNormalizedHistogram>:" << std::endl;
  //std::cout << " meNameInput = " << meNameInput_full << std::endl;
  //std::cout << " meNameOutput = " << meNameOutput_full << std::endl;

  MonitorElement* meInput = dqmStore.get(meNameInput_full);
  //std::cout << " meInput = " << meInput << std::endl;
  TH1* histoInput = ( meInput ) ? meInput->getTH1() : 0;
  if ( !histoInput ) {
    edm::LogError ("addNormalizedHistogram") 
      << " Failed to retrieve histogram " << meNameInput_full << " from dqmStore !!";
    return;
  }
  
  std::auto_ptr<TH1> histoOutput(dynamic_cast<TH1*>(histoInput->Clone()));
  if ( !histoOutput->GetSumw2N()      ) histoOutput->Sumw2();
  if (  histoOutput->Integral() != 0. ) histoOutput->Scale(norm/histoOutput->Integral());

//--- register normalized histogram as MonitorElement in DQMStore    
  dqmRegisterHistogram(dqmStore, histoOutput.release(), meNameOutput_full);
}

void addNormalizedHistogramRecursively(DQMStore& dqmStore, 
				       const std::string& dqmDirectoryInput, const std::string& dqmDirectoryOutput, double norm)
{
//--- add to the output directory 
//    normalized copies of all histograms in current input directory 
  dqmStore.setCurrentFolder(dqmDirectoryInput);

  std::vector<std::string> meNames = dqmStore.getMEs();
  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    std::string meNameInput_full = dqmDirectoryName(dqmDirectoryInput).append(*meName);
    std::string meNameOutput_full = dqmDirectoryName(dqmDirectoryOutput).append(*meName);

    addNormalizedHistogram(dqmStore, meNameInput_full, meNameOutput_full, norm);
  }

//--- call function recursively for all sub-directories
  dqmStore.setCurrentFolder(dqmDirectoryInput);
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    std::string subDirName = dqmSubDirectoryName(dqmDirectoryInput, *dirName);
    
    std::string dqmDirectoryInput_full = dqmDirectoryName(dqmDirectoryInput).append(subDirName);
    std::string dqmDirectoryOutput_full = dqmDirectoryName(dqmDirectoryOutput).append(subDirName);

    addNormalizedHistogramRecursively(dqmStore, dqmDirectoryInput_full, dqmDirectoryOutput_full, norm);
  }
}

void DQMHistNormalizer::endJob()
{
  std::cout << "<DQMHistNormalizer::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be normalized !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  for ( std::vector<jobEntryType>::const_iterator jobEntry = jobs_.begin();
	jobEntry != jobs_.end(); ++jobEntry ) {

//--- check that configuration parameters contain no errors
    if ( jobEntry->cfgError_ ) {
      edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be normalized !!";
      continue;
    }

    if ( jobEntry->meName_input_ != "" && jobEntry->meName_output_ != "" ) {
      addNormalizedHistogram(dqmStore, jobEntry->meName_input_, jobEntry->meName_output_, norm_);
    } else if ( jobEntry->dqmDirectory_input_ != "" && jobEntry->dqmDirectory_output_ != "" ) {
      addNormalizedHistogramRecursively(dqmStore, jobEntry->dqmDirectory_input_, jobEntry->dqmDirectory_output_, norm_);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistNormalizer);
