#include "TauAnalysis/FittingTools/plugins/TemplateHistFitter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/RunningMean.h"
#include "TauAnalysis/FittingTools/interface/RunningMedian.h"
#include "TauAnalysis/FittingTools/interface/RunningCovMatrix.h"

#include <TMath.h>

#include <iostream>
#include <fstream>

TemplateHistFitter::TemplateHistFitter(const edm::ParameterSet& cfg)
  : fitAlgorithm_(0),
    fitResult_(0)
{
  std::cout << "<TemplateHistFitter::TemplateHistFitter>:" << std::endl;

  edm::ParameterSet cfgFit = cfg.getParameter<edm::ParameterSet>("fit");
  
  edm::ParameterSet cfgFitAlgorithm = cfgFit.getParameter<edm::ParameterSet>("algorithm");
  std::string fitAlgorithmType = cfgFitAlgorithm.getParameter<std::string>("pluginType");
  fitAlgorithm_ = TemplateHistFitAdapterPluginFactory::get()->create(fitAlgorithmType, cfg);

//--- read list of variables to be used in template fits
  edm::ParameterSet cfgVariables = cfgFit.getParameter<edm::ParameterSet>("variables");
  vstring varNames = cfgVariables.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator varName = varNames.begin(); 
	varName != varNames.end(); ++varName ) {
    varNames_.push_back(*varName);
  }

//--- read list of signal and background processes to be fitted 
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin(); 
	processName != processNames.end(); ++processName ) {
    processNames_.push_back(*processName);
  }

//--- read values of configuration parameters controlling verbosity print-out
//    generated during fitting
  printLevel_ = ( cfgFit.exists("printLevel") ) ? cfgFit.getParameter<int>("printLevel") : 1;
  printWarnings_  = ( cfgFit.exists("printWarnings") ) ? cfgFit.getParameter<bool>("printWarnings") : true;

//--- read configuration parameters specifying options for estimating
//    statistical and systematic uncertainties on normalization factors determined by fit
  if ( cfg.exists("estStatUncertainties") ) {
    edm::ParameterSet cfgStatErr = cfg.getParameter<edm::ParameterSet>("estStatUncertainties");
    statErrNumSamplings_ = cfgStatErr.getParameter<int>("numSamplings");
    statErrChi2redMax_ = cfgStatErr.getParameter<double>("chi2redMax");  
    statErrPrintLevel_ = cfgStatErr.getParameter<edm::ParameterSet>("verbosity").getParameter<int>("printLevel");
    statErrPrintWarnings_ = cfgStatErr.getParameter<edm::ParameterSet>("verbosity").getParameter<bool>("printWarnings");
  } else {
    statErrNumSamplings_ = 0;
  }
  
  if ( cfg.exists("estSysUncertainties") ) {
    edm::ParameterSet cfgSysErr = cfg.getParameter<edm::ParameterSet>("estSysUncertainties");
    sysErrNumSamplings_ = cfgSysErr.getParameter<int>("numSamplings");
    sysErrChi2redMax_ = cfgSysErr.getParameter<double>("chi2redMax");  
    sysErrPrintLevel_ = cfgSysErr.getParameter<edm::ParameterSet>("verbosity").getParameter<int>("printLevel");
    sysErrPrintWarnings_ = cfgSysErr.getParameter<edm::ParameterSet>("verbosity").getParameter<bool>("printWarnings");
  } else {
    sysErrNumSamplings_ = 0;
  }  

//--- read configuration parameters specifying options for making control plots
  edm::ParameterSet cfgControlPlots = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("controlPlots");
  controlPlotsFileName_ = cfgControlPlots.getParameter<std::string>("fileName");

//--- read configuration parameters specifying options for saving fit results in DQMStore
  edm::ParameterSet cfgSaveFitResults = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("fitResults");
  dqmDirectory_fitResult_ = dqmDirectoryName(cfgSaveFitResults.getParameter<std::string>("dqmDirectory"));
}

TemplateHistFitter::~TemplateHistFitter()
{
  delete fitAlgorithm_;
  delete fitResult_;
}

void TemplateHistFitter::endJob()
{
  std::cout << "<TemplateHistFitter::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("TemplateHistFitter::endJob") << " Failed to access dqmStore" 
					       << " --> histogram will NOT be fitted !!";
    return;
  }
  
  if ( fitAlgorithm_->getErrors() ) {
    edm::LogError ("TemplateHistFitter::endJob") << " Error in Configuration ParameterSet" 
						 << " --> histogram will NOT be fitted !!";
    return;
  }

//--- perform actual fit
  const TemplateFitAdapterBase::fitResultType* fitResult_tmp = fitAlgorithm_->fit(printLevel_, printWarnings_);
  fitResult_ = new TemplateFitAdapterBase::fitResultType(*fitResult_tmp);

//--- print-out fit results
  std::cout << ">>> Fit Results <<<" << std::endl;
  std::cout << " fitStatus = " << fitResult_->status_ << std::endl;
  std::cout << " Chi2red = " << compChi2red(fitResult_) << std::endl;
  fitAlgorithm_->print(std::cout);

//--- store fit results in DQMStore
  if ( dqmDirectory_fitResult_ != "" ) fitAlgorithm_->saveFitResults();

//--- produce plot of different signal and background processes
//    using scale factors determined by fit
//    compared to distribution of (pseudo)data
  if ( controlPlotsFileName_ != "" ) fitAlgorithm_->makeControlPlots();

//--- estimate statistical uncertainties
  if ( statErrNumSamplings_ > 0 ) {
    std::cout << ">>> Statistical Uncertainties <<<" << std::endl;
    estimateUncertainties(true, false, statErrNumSamplings_, statErrChi2redMax_, 
			  "estStatUncertainties", statErrPrintLevel_, statErrPrintWarnings_);
  }

//--- estimate systematic uncertainties
  if ( sysErrNumSamplings_ > 0 ) {
    std::cout << ">>> Systematic Uncertainties <<<" << std::endl;
    estimateUncertainties(false, true, sysErrNumSamplings_, sysErrChi2redMax_,
			  "estSysUncertainties", sysErrPrintLevel_, sysErrPrintWarnings_);
  }

//--- estimate total (statistical + systematic) uncertainties
  if ( statErrNumSamplings_ > 0 && sysErrNumSamplings_ > 0 ) {
    std::cout << ">>> Total (statistical + systematic) Uncertainties <<<" << std::endl;
    double chi2redMax = TMath::Max(statErrChi2redMax_, sysErrChi2redMax_);
    int totErrPrintLevel = TMath::Min(statErrPrintLevel_, sysErrPrintLevel_);
    bool totErrPrintWarnings = (statErrPrintWarnings_ && sysErrPrintWarnings_);
    estimateUncertainties(true, true, sysErrNumSamplings_, chi2redMax,
			  "estTotUncertainties", totErrPrintLevel, totErrPrintWarnings);
  }

  std::cout << "done." << std::endl;
}

void TemplateHistFitter::estimateUncertainties(bool fluctStat, bool fluctSys, int numSamplings, 
					       double chi2redMax, const char* type, int printLevel, bool printWarnings)
{
  int numProcesses = fitResult_->normalizations_.size();
  TVectorD fitValues(numProcesses);
  RunningMean mean(numProcesses);
  RunningMedian median(numProcesses);
  RunningCovMatrix cov(numProcesses);

  unsigned numTotFits = 0;
  unsigned numGoodFits = 0;

  for ( int iRndFluct = 0; iRndFluct < numSamplings; ++iRndFluct ) {

    std::cout << "<TemplateHistFitter::estimateUncertainties>: iRndFluct = " << iRndFluct << std::endl;

//--- fluctuate distributions observed in (pseudo)data
    fitAlgorithm_->dataNdEntry_->fluctuate(true, false);

//--- fluctuate template histograms fitted to the (pseudo)data  
//
//    CV: treat statistical uncertainties on template histograms
//        as systematic uncertainties of background estimation
//
    for ( TemplateFitAdapterBase::modelNdEntryMap::iterator modelNdEntry = fitAlgorithm_->modelNdEntries_.begin();
	  modelNdEntry != fitAlgorithm_->modelNdEntries_.end(); ++modelNdEntry ) {
      //modelNdEntry->second->fluctuate(fluctStat, fluctSys);
      modelNdEntry->second->fluctuate(fluctSys, fluctSys);
    }

    const TemplateFitAdapterBase::fitResultType* fitResult_tmp = fitAlgorithm_->fit(printLevel, printWarnings);

    ++numTotFits;
    
    for ( int iProcess = 0; iProcess < numProcesses; ++iProcess ) {
      const std::string& processName = processNames_[iProcess];

      if ( fitResult_tmp->normalizations_.find(processName) == fitResult_tmp->normalizations_.end() ) {
	  edm::LogError ("estimateUncertainties") << " Failed to find normalization for process = " << processName 
						  << " --> skipping !!";
	  return;
      } 

      fitValues(iProcess) = fitResult_tmp->normalizations_.find(processName)->second.value_;
      //std::cout << " fitValue(iProcess = " << iProcess << ", processName = " << processName << ")"
      //	  << " = " << fitValues(iProcess) << std::endl;
    }
    
    double chi2red = compChi2red(fitResult_tmp);
    //std::cout << "Chi2red = " << chi2red << std::endl;
    if ( !(chi2red < chi2redMax) ) continue;
    
    mean.update(fitValues);
    median.update(fitValues);
    cov.update(fitValues);
    
    ++numGoodFits;
  }

  double badFitFraction = (numTotFits - numGoodFits)/((double)numTotFits);
  std::cout << "fraction of Samplings discarded due to bad Fit quality = " << badFitFraction << std::endl;

  std::cout << "Mean:" << std::endl;
  mean.print(std::cout, &processNames_);
  std::cout << "Median:" << std::endl;
  median.print(std::cout, &processNames_);
  std::cout << "Covariance Matrix:" << std::endl;
  cov.print(std::cout, &processNames_);
  
  if ( controlPlotsFileName_ != "" ) {
    //makeControlPlotsCovariance(fitResult_->normValues_, median(), cov(), processNames_, controlPlotsFileName_, type);
    makeControlPlotsCovariance(fitResult_->normValues_, fitResult_->normValues_, cov(), processNames_, controlPlotsFileName_, type);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TemplateHistFitter);
