#include "TauAnalysis/DQMTools/plugins/DQMHistScaler.h"

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

#include <iostream>

const int verbosity = 0;

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistScaler::DQMHistScaler(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistScaler::DQMHistScaler>:" << std::endl;

  cfgError_ = 0;

  cfgScaleFactor_ = ( cfg.exists("scaleFactor") ) ? cfg.getParameter<double>("scaleFactor") : -1.;
  //std::cout << "cfgScaleFactor = " << cfgScaleFactor_ << std::endl;

  if ( cfg.exists("numerator") && cfg.exists("denominator") && cfg.exists("type") ) {
    numerator_ = cfg.getParameter<std::string>("numerator");
    denominator_ = cfg.getParameter<std::string>("denominator");
    type_ = cfg.getParameter<std::string>("type");
  } else {
    numerator_ = "";
    denominator_ = "";
    type_ =  "";
  }
  //std::cout << "numerator = " << numerator_ << std::endl;
  //std::cout << "denominator = " << denominator_ << std::endl;
  //std::cout << "type = " << type_ << std::endl;

  dqmDirectory_input_ = cfg.getParameter<std::string>("dqmDirectory_input");
  if ( cfg.exists("dqmSubDirectories_input") ) dqmSubDirectories_input_ = cfg.getParameter<vstring>("dqmSubDirectories_input");

  dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectory_output");

//--- check that either:
//     o scaleFactor
//    or
//     o numerator + denominator
//    configuration parameters are defined 
  unsigned numScales = 0;
  if ( cfgScaleFactor_ != -1. ) ++numScales;
  if ( numerator_ != "" && denominator_ != "" && type_ != "" ) ++numScales;
  if ( numScales != 1 ) {
    edm::LogError("DQMHistScaler") << " Need to specify either Configuration parameter 'scaleFactor'"
				   << " or 'numerator', 'denominator' and 'type' !!";
    cfgError_ = 1;
  } 

  //std::cout << "done." << std::endl;
}

DQMHistScaler::~DQMHistScaler() 
{
// nothing to be done yet...
}

void DQMHistScaler::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

std::string dqmDirectoryName_full(const std::string& dqmDirectory, const std::string& dqmSubDirectory)
{
  std::string dqmDirectoryName_full = dqmDirectoryName(std::string(dqmRootDirectory)).append(dqmDirectory);
  dqmDirectoryName_full = dqmDirectoryName(dqmDirectoryName_full).append(dqmSubDirectory);
  return dqmDirectoryName_full;
}


double getMonitorElementNorm(DQMStore& dqmStore, const std::string& meName, const std::string& meType, int& errorFlag)
{
  MonitorElement* me = dqmStore.get(meName);

  if ( meType == "real" ) {
    return me->getFloatValue();
  } else if ( meType == "int" ) {
    return me->getIntValue();
  } else if ( meType == "hist" ) {
    if ( me->getTH1() ) {
      return me->getTH1()->Integral();
    } else {
      edm::LogError("getMonitorElementNorm") << " No histogram associated to Monitor Element = " << meName << " !!";
      errorFlag = 1;
      return -1.;
    } 
  } else {
    edm::LogError("getMonitorElementNorm") << " Undefined type = " << meType << " !!";
    errorFlag = 1;
    return -1.;
  }
}

void DQMHistScaler::endJob()
{
  std::cout << "<DQMHistScaler::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be scaled !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be scaled !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
//--- compute scale factor
  double scaleFactor = 0.;
  if ( cfgScaleFactor_ != -1. ) {
    scaleFactor = cfgScaleFactor_;
  } else {
    int errorFlag = 0;
    double normNumerator = getMonitorElementNorm(dqmStore, numerator_, type_, errorFlag);
    double normDenominator = getMonitorElementNorm(dqmStore, denominator_, type_, errorFlag);
    if ( errorFlag ) {
      edm::LogError ("endJob") << " Failed to access numerator and denominator Monitor Elements"
			       << " --> histograms will NOT be scaled !!";
      return;
    }

    scaleFactor = normNumerator/normDenominator;
  }
  std::cout << " scaleFactor = " << scaleFactor << std::endl;

//--- scale histograms;
//    in case specific subdirectories have been specified in configuration parameters, 
//    copy the specified subdirectories only, else copy all subdirectories 
//    from dqmDirectory_input to dqmDirectory_output
  //std::cout << "--> scaling histograms..." << std::endl;
  if ( dqmSubDirectories_input_.size() > 0 ) {
    for ( vstring::const_iterator dqmSubDirectory = dqmSubDirectories_input_.begin();
	  dqmSubDirectory != dqmSubDirectories_input_.end(); ++dqmSubDirectory ) {

      std::string inputDirectory = dqmDirectoryName_full(dqmDirectory_input_, *dqmSubDirectory);
      //std::cout << " inputDirectory = " << inputDirectory << std::endl;
      std::string outputDirectory = dqmDirectoryName_full(dqmDirectory_output_, *dqmSubDirectory);
      //std::cout << " outputDirectory = " << outputDirectory << std::endl;

      dqmCopyRecursively(dqmStore, inputDirectory, outputDirectory, scaleFactor, 1, false);
    }
  } else {
    std::string inputDirectory = dqmDirectoryName(std::string(dqmRootDirectory));
    //std::cout << " inputDirectory = " << inputDirectory << std::endl;
    std::string outputDirectory = dqmDirectoryName(std::string(dqmRootDirectory));
    //std::cout << " outputDirectory = " << outputDirectory << std::endl;
      
    dqmCopyRecursively(dqmStore, inputDirectory, outputDirectory, scaleFactor, 1, false);
  }

  std::cout << "done." << std::endl; 
  if ( verbosity ) dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistScaler);
