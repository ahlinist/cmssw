#include "TauAnalysis/DQMTools/interface/DQMErrorBandProducerBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

const std::string DQMErrorBandProducerBase::method_sqrt = "sqrt";
const std::string DQMErrorBandProducerBase::method_min_max = "min_max"; 

const std::string DQMErrorBandProducerBase::defaultMethod = method_sqrt;

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMErrorBandProducerBase::cfgEntryProduceJob::cfgEntryProduceJob(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMErrorBandProducerBase::cfgEntryProduceJob>" << std::endl;

  dqmDirectory_inputCentralValue_ = ( cfg.exists("dqmDirectory_inputCentralValue") ) ?
    cfg.getParameter<std::string>("dqmDirectory_inputCentralValue") : "";
  dqmDirectories_inputVariance_ = cfg.getParameter<vstring>("dqmDirectories_inputVariance");
  dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectory_output");
  method_ = ( cfg.exists("method") ) ?
    cfg.getParameter<std::string>("method") : defaultMethod;
}

void DQMErrorBandProducerBase::cfgEntryProduceJob::print() const
{
  std::cout << "<cfgEntryProduceJob::print>:" << std::endl;
  std::cout << " dqmDirectory_inputCentralValue = " << dqmDirectory_inputCentralValue_ << std::endl;
  std::cout << " dqmDirectories_inputVariance = " << format_vstring(dqmDirectories_inputVariance_) << std::endl;
  std::cout << " dqmDirectory_output = " << dqmDirectory_output_ << std::endl;
  std::cout << " method = " << method_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMErrorBandProducerBase::DQMErrorBandProducerBase(const edm::ParameterSet& cfg)
  : verbosity_(0),
    cfgError_(0)	
{
  //std::cout << "<DQMErrorBandProducerBase::DQMErrorBandProducerBase>:" << std::endl;

//--- configure processes  
  //std::cout << "--> configuring produceJobs..." << std::endl;
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgProduceJobs = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgProduceJob = cfgProduceJobs.begin();
	cfgProduceJob != cfgProduceJobs.end(); ++cfgProduceJob ) {
    cfgEntryProduceJob produceJob(*cfgProduceJob);
    if ( verbosity_ ) produceJob.print();
    produceJobs_.push_back(produceJob);
  }
  
  //std::cout << "done." << std::endl;
}

DQMErrorBandProducerBase::~DQMErrorBandProducerBase() 
{
// nothing to be done yet...
}

void DQMErrorBandProducerBase::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMErrorBandProducerBase::computeErrorBand(DQMStore& dqmStore, 
						const std::string& inputDirectory_centralValue, 
						const std::vector<std::string>& inputDirectories_variance, 
						const std::string& outputDirectory, 
						const std::string& dqmSubDirectory, 
						const std::string& method, int nDoF)
{
  //std::cout << "<DQMErrorBandProducer::computeErrorBand>:" << std::endl;
  //std::cout << " inputDirectory_centralValue = " << inputDirectory_centralValue << std::endl;
  //std::cout << " inputDirectories_variance = " << format_vstring(inputDirectories_variance) << std::endl;
  //std::cout << " outputDirectory = " << outputDirectory << std::endl;
  //std::cout << " dqmSubDirectory = " << dqmSubDirectory << std::endl;
  //std::cout << " nDoF = " << nDoF << std::endl;

  std::string dqmDirectory_centralValue = dqmDirectoryName(inputDirectory_centralValue).append(dqmSubDirectory);

  std::vector<std::string> dqmDirectories_variance;
  for ( std::vector<std::string>::const_iterator inputDirectory_variance = inputDirectories_variance.begin();
	inputDirectory_variance != inputDirectories_variance.end(); ++inputDirectory_variance ) {
    std::string dqmDirectory_variance = dqmDirectoryName(*inputDirectory_variance).append(dqmSubDirectory);
    dqmDirectories_variance.push_back(dqmDirectory_variance);
  }

  std::string dqmDirectory_output = dqmDirectoryName(outputDirectory).append(dqmSubDirectory);

//--- call computeErrorBandImp function speficic to different types of MonitorElements
//    (implemented in derived classes)
  computeErrorBandImp(dqmStore, dqmDirectory_centralValue, dqmDirectories_variance, dqmDirectory_output, method, nDoF);

//--- call function recursively for all sub-directories
  dqmStore.setCurrentFolder(dqmDirectory_centralValue);
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    std::string subDirName = dqmSubDirectoryName(dqmDirectory_centralValue, *dirName);

    std::string dqmSubDirectory_descend = dqmDirectoryName(dqmSubDirectory).append(subDirName);

    computeErrorBand(dqmStore, inputDirectory_centralValue, inputDirectories_variance, 
		     outputDirectory, dqmSubDirectory_descend, method, nDoF);
  }
}


void DQMErrorBandProducerBase::endJob()
{
  std::cout << "<DQMErrorBandProducerBase::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet --> error-band histograms will NOT be produced !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> error-band histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- produce error-band histograms
  //std::cout << "--> producing error-band histograms..." << std::endl;
  for ( std::vector<cfgEntryProduceJob>::const_iterator produceJob = produceJobs_.begin();
	produceJob != produceJobs_.end(); ++produceJob ) {
    const std::string& dqmDirectory_inputCentralValue = produceJob->dqmDirectory_inputCentralValue_;
    const vstring& dqmDirectories_inputVariance = produceJob->dqmDirectories_inputVariance_;
    const std::string& dqmDirectory_output = produceJob->dqmDirectory_output_;

    int nDoF = -1;

//--- compute central values
    if ( dqmDirectory_inputCentralValue != "" ) {
      dqmCopyRecursively(dqmStore, dqmDirectory_inputCentralValue, dqmDirectory_output, 1., 0., 1, false);
      nDoF = dqmDirectories_inputVariance.size();
    } else if ( dqmDirectories_inputVariance.size() > 0. ) {
      double scaleFactor = 1./dqmDirectories_inputVariance.size();
      for ( vstring::const_iterator dqmDirectory_input = dqmDirectories_inputVariance.begin();
	    dqmDirectory_input != dqmDirectories_inputVariance.end(); ++dqmDirectory_input ) {
//--- when processing first inputDirectory, check that histograms in outputDirectory do not yet exist;
//    afterwards, add histograms in inputDirectory to those in outputDirectory
        int mode = ( dqmDirectory_input == dqmDirectories_inputVariance.begin() ) ? 1 : 2;
        //std::cout << " mode = " << mode << std::endl;
	dqmCopyRecursively(dqmStore, *dqmDirectory_input, dqmDirectory_output, scaleFactor, 0., mode, false);
      }
      nDoF = dqmDirectories_inputVariance.size() - 1;
    } else {
      edm::LogError ("endJob") 
	<< " Invalid values for Configuration Parameters 'dqmDirectory_inputCentralValue' and 'dqmDirectories_inputVariance'"
	<< " --> error-band histograms will NOT be produced !!";
      continue;
    }

//--- compute sum of variations around central value
    computeErrorBand(dqmStore, dqmDirectory_output, dqmDirectories_inputVariance, 
		     dqmDirectory_output, "", produceJob->method_, nDoF);
  }

  std::cout << "done." << std::endl; 
  if ( verbosity_ ) dqmStore.showDirStructure();
}
