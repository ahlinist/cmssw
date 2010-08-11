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
  : errorFlag_(false)
{
  //std::cout << "<cfgEntryBinError::cfgEntryBinError>:" << std::endl;
  
  meName_numerator_      = ( cfg.exists("meName_numerator")      ) ? cfg.getParameter<std::string>("meName_numerator")      : "";
  meName_numeratorErr_   = ( cfg.exists("meName_numeratorErr")   ) ? cfg.getParameter<std::string>("meName_numeratorErr")   : "";
  meName_denominator_    = ( cfg.exists("meName_denominator")    ) ? cfg.getParameter<std::string>("meName_denominator")    : "";
  meName_denominatorErr_ = ( cfg.exists("meName_denominatorErr") ) ? cfg.getParameter<std::string>("meName_denominatorErr") : "";
  
  meName_passed_         = ( cfg.exists("meName_passed")         ) ? cfg.getParameter<std::string>("meName_passed")         : "";
  meName_passedErr_      = ( cfg.exists("meName_passedErr")      ) ? cfg.getParameter<std::string>("meName_passedErr")      : "";
  meName_failed_         = ( cfg.exists("meName_failed")         ) ? cfg.getParameter<std::string>("meName_failed")         : "";
  meName_failedErr_      = ( cfg.exists("meName_failedErr")      ) ? cfg.getParameter<std::string>("meName_failedErr")      : "";
  
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
    double numerator, denominator, efficiency;
    double efficiencyErr = 0.;
    if ( plot->meName_numerator_ != "" && plot->meName_denominator_ != "" ) {
      numerator = getValue(dqmStore, plot->meName_numerator_, plot->errorFlag_);
      denominator = getValue(dqmStore, plot->meName_denominator_, plot->errorFlag_);

      efficiency = numerator/denominator;

      if ( plot->meName_numeratorErr_ != "" && plot->meName_denominatorErr_ != "" ) {
	double numeratorErr = getValue(dqmStore, plot->meName_numeratorErr_, plot->errorFlag_);
	double denominatorErr = getValue(dqmStore, plot->meName_denominatorErr_, plot->errorFlag_);

	std::cout << " numerator = " << numerator << " +/- " << numeratorErr << std::endl;
	std::cout << " denominator = " << denominator << " +/- " << denominatorErr << std::endl; 

	double relErr2 = 0.;
	if ( numerator   != 0. ) relErr2 += TMath::Power(numeratorErr/numerator, 2);
	if ( denominator != 0. ) relErr2 += TMath::Power(denominatorErr/denominator, 2);
	efficiencyErr = efficiency*TMath::Sqrt(relErr2);
      } else {
	std::cout << " numerator = " << numerator << std::endl;
	std::cout << " denominator = " << denominator << std::endl;      
      }      
    } else if ( plot->meName_passed_ != "" && plot->meName_failed_ != "" ) {
      double passed = getValue(dqmStore, plot->meName_passed_, plot->errorFlag_);
      double failed = getValue(dqmStore, plot->meName_failed_, plot->errorFlag_);

      numerator = passed;
      denominator = (passed + failed);

      efficiency = numerator/denominator;

      if ( plot->meName_passedErr_ != "" && plot->meName_failedErr_ != "" ) {
	double passedErr = getValue(dqmStore, plot->meName_passedErr_, plot->errorFlag_);
	double failedErr = getValue(dqmStore, plot->meName_failedErr_, plot->errorFlag_);

	std::cout << " passed = " << passed << " +/- " << passedErr << std::endl;
	std::cout << " failed = " << failed << " +/- " << failedErr << std::endl;

	if ( denominator != 0. ) 
	  efficiencyErr = TMath::Sqrt(TMath::Power(failed*passedErr, 2) + TMath::Power(passed*failedErr, 2))/denominator;
      } else {
	std::cout << " passed = " << passed << std::endl;
	std::cout << " failed = " << failed << std::endl;  
      }
    } else {
      edm::LogError ("endJob") 
	<< " Must specify either 'meName_numerator' and 'meName_denominator'" 
	<< " or 'meName_passed' and 'meName_failed configuration parameters !!";
      continue;
    }
    
    if ( plot->errorFlag_ ) continue;

    FeldmanCousinsBinomialInterval binErrAlgorithm;
    binErrAlgorithm.init(0.32); // compute one-sigma intervals
    binErrAlgorithm.calculate(numerator, denominator);

    double efficiency_binErrUp = binErrAlgorithm.upper() - efficiency;
    double efficiency_binErrDown = efficiency - binErrAlgorithm.lower();

    std::cout << "--> efficiency";
    if ( plot->label_ != "" ) std::cout << "(" << plot->label_ << ")";
    std::cout << " = " << efficiency*100. 
	      << " + " << efficiency_binErrUp*100. << " - " << efficiency_binErrDown*100. << " ('stat.')";
    if ( efficiencyErr > 0. ) std::cout << " +/- " << efficiencyErr << " ('sys.')";
    std::cout << " %" << std::endl;

    double N = denominator;
    double p = efficiency;
    double q = (1 - efficiency);
    std::cout << "sqrt(N*p*q)/N = " << (TMath::Sqrt(N*p*q)/N)*100. << " %" << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMBinErrorCalculator);
