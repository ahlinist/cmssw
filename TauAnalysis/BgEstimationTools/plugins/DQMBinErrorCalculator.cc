#include "TauAnalysis/BgEstimationTools/plugins/DQMBinErrorCalculator.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/RooStatsCms/interface/FeldmanCousinsBinomialInterval.h"

#include <TMath.h>

#include <iostream>

DQMBinErrorCalculator::cfgEntryBinError::cfgEntryBinError(const edm::ParameterSet& cfg)
  : error_(false)
{
  //std::cout << "<cfgEntryBinError::cfgEntryBinError>:" << std::endl;
  
  meName_numerator_   = ( cfg.exists("meName_numerator")   ) ? cfg.getParameter<std::string>("meName_numerator")   : "";
  meName_denominator_ = ( cfg.exists("meName_denominator") ) ? cfg.getParameter<std::string>("meName_denominator") : "";
  
  meName_passed_      = ( cfg.exists("meName_passed")      ) ? cfg.getParameter<std::string>("meName_passed")      : "";
  meName_failed_      = ( cfg.exists("meName_failed")      ) ? cfg.getParameter<std::string>("meName_failed")      : "";
  
  label_ = ( cfg.exists("label") ) ? cfg.getParameter<std::string>("label") : "";
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMBinErrorCalculator::DQMBinErrorCalculator(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMBinErrorCalculator::DQMBinErrorCalculator>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgBinErrors = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgBinError = cfgBinErrors.begin();
	cfgBinError != cfgBinErrors.end(); ++cfgBinError ) {
    cfgEntriesBinError_.push_back(cfgEntryBinError(*cfgBinError));
  }
}

DQMBinErrorCalculator::~DQMBinErrorCalculator() 
{
//--- nothing to be done yet
}

void DQMBinErrorCalculator::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMBinErrorCalculator::endJob()
{
  std::cout << "<DQMBinErrorCalculator::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> rebinned histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryBinError>::const_iterator plot = cfgEntriesBinError_.begin(); 
	plot != cfgEntriesBinError_.end(); ++plot ) {
    double numerator, denominator;
    if ( plot->meName_numerator_ != "" && plot->meName_denominator_ != "" ) {
      numerator = getValue(dqmStore, plot->meName_numerator_, plot->error_);
      denominator = getValue(dqmStore, plot->meName_denominator_, plot->error_);
    } else if ( plot->meName_passed_ != "" && plot->meName_failed_ != "" ) {
      double passed = getValue(dqmStore, plot->meName_passed_, plot->error_);
      double failed = getValue(dqmStore, plot->meName_failed_, plot->error_);
      numerator = passed;
      denominator = (passed + failed);
    } else {
      edm::LogError ("endJob") << " Must specify either 'meName_numerator' and 'meName_denominator'" 
			       << " or 'meName_passed' and 'meName_failed configuration parameters !!";
      continue;
    }
    
    if ( plot->error_ ) continue;

    std::cout << " numerator = " << numerator << std::endl;
    std::cout << " denominator = " << denominator << std::endl;

    FeldmanCousinsBinomialInterval binErrAlgorithm;
    binErrAlgorithm.init(0.32); // compute one-sigma intervals
    binErrAlgorithm.calculate(numerator, denominator);

    double binEffValue = numerator/denominator;
    double binEffErrUp = binErrAlgorithm.upper() - binEffValue;
    double binEffErrDown = binEffValue - binErrAlgorithm.lower();

    std::cout << "--> Efficiency";
    if ( plot->label_ != "" ) std::cout << "(" << plot->label_ << ")";
    std::cout << " = " << binEffValue*100. << " + " << binEffErrUp*100. << " - " << binEffErrDown*100. << " %" << std::endl;

    double N = denominator;
    double p = numerator/denominator;
    double q = (1 - numerator/denominator);
    std::cout << "sqrt(N*p*q)/N = " << (TMath::Sqrt(N*p*q)/N)*100. << " %" << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMBinErrorCalculator);
