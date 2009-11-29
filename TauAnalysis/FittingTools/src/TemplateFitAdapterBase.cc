#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/histogramAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <RooAbsBinning.h>

#include <TROOT.h>

#include <iostream>
#include <fstream>

const std::string fluctMode_coherent = "coherent";
const std::string fluctMode_incoherent = "incoherent";

typedef std::pair<double, double> double_pair;

TemplateFitAdapterBase::drawOptionsType::drawOptionsType(const edm::ParameterSet& cfg)
{
  lineColor_ = cfg.getParameter<int>("lineColor");
  lineStyle_ = cfg.getParameter<int>("lineStyle");
  lineWidth_ = cfg.getParameter<int>("lineWidth");
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapterBase::data1dType::data1dType(const std::string& processName, const std::string& varName, 
					       const std::string& meName, const std::vector<fitRangeEntryType>& fitRanges)
  : processName_(processName),
    varName_(varName),
    meName_(meName),
    fitRanges_(fitRanges),
    histogram_(0),
    fluctHistogram_(0),
    error_(0)
{}

TemplateFitAdapterBase::data1dType::~data1dType()
{
  delete histogram_;
  delete fluctHistogram_;
}

void TemplateFitAdapterBase::data1dType::initialize()
{
  std::cout << "<data1dType::initialize>:" << std::endl;
  std::cout << " process = " << processName_ << ", variable = " << varName_ << std::endl;

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  me_ = dqmStore.get(meName_);
  if ( !me_ ) {
    edm::LogError ("data1dType") << " Failed to access dqmMonitorElement = " << meName_ << " !!";
    error_ = 1;
    return;
  }

  std::cout << " integral = " << getIntegral(me_->getTH1()) << std::endl;

  TH1* histogram_subrange = makeSubrangeHistogram(me_->getTH1(), &fitRanges_);
  histogram_ = makeSerializedHistogram(histogram_subrange);
  delete histogram_subrange;
  
  fluctHistogram_ = (TH1*)histogram_->Clone();

  compFittedFraction(fluctHistogram_);
  std::cout << "fitted fraction = " << fittedFraction_ << std::endl;
}

void TemplateFitAdapterBase::data1dType::compFittedFraction(const TH1* histogram) 
{
  integral_ = getIntegral(me_->getTH1());
  
  fittedIntegral_ = getIntegral(histogram, &fitRanges_);

  fittedFraction_ = ( integral_ > 0. ) ? (fittedIntegral_/integral_) : 1.;
}

void TemplateFitAdapterBase::data1dType::fluctuate(bool, bool)
{  
  sampleHistogram_stat(histogram_, fluctHistogram_);
  makeHistogramPositive(fluctHistogram_);
  compFittedFraction(fluctHistogram_);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapterBase::dataNdType::dataNdType()
  : error_(0)
{}

TemplateFitAdapterBase::dataNdType::~dataNdType()
{
  //std::cout << "<dataNdType::~dataNdType>:" << std::endl;

  for ( std::map<std::string, data1dType*>::iterator it = data1dEntries_.begin();
	it != data1dEntries_.end(); ++it ) {
    delete it->second;
  }
}

void TemplateFitAdapterBase::dataNdType::addVar(const std::string& varName, 
						const std::string& meName, const std::vector<fitRangeEntryType>& fitRanges)
{
  varNames_.push_back(varName);
  
  data1dType* data1dEntry = new data1dType("data", varName, meName, fitRanges);
  if ( data1dEntry->error_ ) error_ = 1;
  data1dEntries_[varName] = data1dEntry;
}

void TemplateFitAdapterBase::dataNdType::initialize()
{
  for ( std::map<std::string, data1dType*>::iterator data1dEntry = data1dEntries_.begin();
	data1dEntry != data1dEntries_.end(); ++data1dEntry ) { 
    data1dEntry->second->initialize();
    if ( data1dEntry->second->error_ ) error_ = 1;
  }
}

void TemplateFitAdapterBase::dataNdType::fluctuate(bool, bool)
{
  for ( std::map<std::string, data1dType*>::iterator data1dEntry = data1dEntries_.begin();
	data1dEntry != data1dEntries_.end(); ++data1dEntry ) {
    data1dEntry->second->fluctuate(true, false);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapterBase::model1dType::model1dType(const std::string& processName, const std::string& varName, 
						 const std::string& meName, const std::vector<fitRangeEntryType>& fitRanges)
  : data1dType(processName, varName, meName, fitRanges)
{}

TemplateFitAdapterBase::model1dType::~model1dType()
{}

void TemplateFitAdapterBase::model1dType::initialize()
{
  std::cout << "<model1dType::initialize>:" << std::endl;
  std::cout << " process = " << processName_ << ", variable = " << varName_ << std::endl;

  data1dType::initialize();

  if ( error_ ) return;

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  for ( std::vector<sysErrType>::iterator sysErrFluctuation = sysErrFluctuations_.begin();
	sysErrFluctuation != sysErrFluctuations_.end(); ++sysErrFluctuation ) {
    sysErrFluctuation->me_ = dqmStore.get(sysErrFluctuation->meName_);
    if ( !sysErrFluctuation->me_ ) {
      edm::LogError ("model1dType") << " Failed to access dqmMonitorElement = " << sysErrFluctuation->meName_ << " !!";
      error_ = 1;
      continue;
    }

//--- check that histograms representing systematic uncertainties have the same binning
//    as that representing expectation
    if ( !isCompatibleBinning(histogram_, sysErrFluctuation->me_->getTH1()) ) {
      edm::LogError ("model1dType") << " Incompatible binning of histograms " << meName_ 
				    << " and " << sysErrFluctuation->meName_ << " !!";
      error_ = 1;
      continue;
    }
  }
}

void TemplateFitAdapterBase::model1dType::fluctuate(bool fluctStat, bool fluctSys)
{
  if ( fluctStat ) {
    sampleHistogram_stat(histogram_, fluctHistogram_);
  } else {
    int numBins = histogram_->GetNbinsX();
    for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
      double binContent = histogram_->GetBinContent(iBin);
      double binError = histogram_->GetBinError(iBin);

      fluctHistogram_->SetBinContent(iBin, binContent);
      fluctHistogram_->SetBinError(iBin, binError);
    }
  }

  if ( fluctSys ) {
    for ( std::vector<sysErrType>::const_iterator sysErrFluctuation = sysErrFluctuations_.begin();
	  sysErrFluctuation != sysErrFluctuations_.end(); ++sysErrFluctuation ) {
      sampleHistogram_sys(fluctHistogram_, sysErrFluctuation->me_->getTH1(),  
			  sysErrFluctuation->pullRMS_, sysErrFluctuation->pullMin_, sysErrFluctuation->pullMax_, 
			  sysErrFluctuation->fluctMode_);
    }
  }

  makeHistogramPositive(fluctHistogram_);
  compFittedFraction(fluctHistogram_);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapterBase::modelNdType::modelNdType(const std::string& processName)
  : processName_(processName),
    error_(0)
{}

TemplateFitAdapterBase::modelNdType::~modelNdType()
{
  for ( std::map<std::string, model1dType*>::iterator it = model1dEntries_.begin();
	it != model1dEntries_.end(); ++it ) {
    delete it->second;
  }
}

void TemplateFitAdapterBase::modelNdType::addVar(const std::string& varName, 
						 const std::string& meName, const std::vector<fitRangeEntryType>& fitRanges)
{
  varNames_.push_back(varName);
  
  model1dType* model1dEntry = new model1dType(processName_, varName, meName, fitRanges);
  if ( model1dEntry->error_ ) error_ = 1;
  model1dEntries_[varName] = model1dEntry;
}

void TemplateFitAdapterBase::modelNdType::initialize()
{
  for ( std::map<std::string, model1dType*>::iterator model1dEntry = model1dEntries_.begin();
	model1dEntry != model1dEntries_.end(); ++model1dEntry ) {
    model1dEntry->second->initialize();
    if ( model1dEntry->second->error_ ) error_ = 1;
  }
}

void TemplateFitAdapterBase::modelNdType::fluctuate(bool fluctStat, bool fluctSys)
{
  for ( std::map<std::string, model1dType*>::iterator model1dEntry = model1dEntries_.begin();
	model1dEntry != model1dEntries_.end(); ++model1dEntry ) {
    model1dEntry->second->fluctuate(fluctStat, fluctSys);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::vector<TemplateFitAdapterBase::fitRangeEntryType> getFitRanges(const edm::ParameterSet& cfg)
{
  std::vector<TemplateFitAdapterBase::fitRangeEntryType> fitRanges;

  std::vector<std::string> axisLabels;
  axisLabels.push_back("x");
  axisLabels.push_back("y");
  unsigned numAxisLabels = axisLabels.size();
  for ( unsigned iAxis = 0; iAxis < numAxisLabels; ++iAxis ) {
    const char* axisLabel = axisLabels[iAxis].data();
    if ( cfg.exists(axisLabel) ) {
      edm::ParameterSet cfgFitRange = cfg.getParameter<edm::ParameterSet>(axisLabel);
      TemplateFitAdapterBase::fitRangeEntryType fitRangeEntry;
      fitRangeEntry.min_ = cfgFitRange.getParameter<double>("min");
      fitRangeEntry.max_ = cfgFitRange.getParameter<double>("max");
      fitRanges.push_back(fitRangeEntry);
    } else {
      break;
    }
  }

  return fitRanges;
}

TemplateFitAdapterBase::TemplateFitAdapterBase(const edm::ParameterSet& cfg)
  : isFirstFit_(true),
    error_(0)
{
  std::cout << "<TemplateFitAdapterBase::TemplateFitAdapterBase>:" << std::endl;

  edm::ParameterSet cfgFit = cfg.getParameter<edm::ParameterSet>("fit");

//--- read list of variables to be used in fit
//    (for each variable: name, title and range to be fitted)
  edm::ParameterSet cfgVariables = cfgFit.getParameter<edm::ParameterSet>("variables");
  vstring varNames = cfgVariables.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator varName = varNames.begin(); 
	varName != varNames.end(); ++varName ) {
    edm::ParameterSet cfgVariable = cfgVariables.getParameter<edm::ParameterSet>(*varName);

    std::cout << "--> including variable = " << (*varName) << " in fit." << std::endl;
    varNames_.push_back(*varName);
  }

//--- read configuration parameters specifying signal and background templates
//    fitted to the distributions observed in (pseudo)data
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin(); 
	processName != processNames.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);

    processNames_.push_back(*processName);

    edm::ParameterSet cfgDrawOptions = cfgProcess.getParameter<edm::ParameterSet>("drawOptions");
    drawOptions_[*processName] = new drawOptionsType(cfgDrawOptions);

    modelNdType* modelNdEntry = new modelNdType(*processName);
    
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      edm::ParameterSet cfgTemplates = cfgProcess.getParameter<edm::ParameterSet>("templates");
      if ( !cfgTemplates.exists(*varName) ) {
	edm::LogError ("TemplateFitAdapterBase") << " No Template of variable = " << (*varName) 
						 << " defined for process = " << (*processName) << " !!";
	error_ = 1;
	continue;
      }
      
      edm::ParameterSet cfgTemplate = cfgTemplates.getParameter<edm::ParameterSet>(*varName);
      
      std::string meName = cfgTemplate.getParameter<std::string>("meName");

      edm::ParameterSet cfgVariable = cfgVariables.getParameter<edm::ParameterSet>(*varName);
      std::vector<fitRangeEntryType> fitRanges = getFitRanges(cfgVariable);

      modelNdEntry->addVar(*varName, meName, fitRanges);
    }

    modelNdEntries_[*processName] = modelNdEntry;
  }
  
//--- read configuration parameters specifying distributions observed in (pseudo)data 
//    that are to be fitted
//    (for each variable: name of DQM MonitorElements holding template histogram)
//
//    WARNING: dataNdType::addElement and modelNdType::addElement need to be called
//              with variable names sorted in the **exact** same order !!
//
  edm::ParameterSet cfgData = cfg.getParameter<edm::ParameterSet>("data");

  dataNdEntry_ = new dataNdType();

  for ( vstring::const_iterator varName = varNames_.begin();
        varName != varNames_.end(); ++varName ) {
    edm::ParameterSet cfgDistributions = cfgData.getParameter<edm::ParameterSet>("distributions");
    if ( !cfgDistributions.exists(*varName) ) {
      edm::LogError ("TemplateFitAdapterBase") << " No Template of variable = " << (*varName) << " defined for data !!";
      error_ = 1;
      continue;
    }

    edm::ParameterSet cfgDistribution = cfgDistributions.getParameter<edm::ParameterSet>(*varName);

    std::string meName = cfgDistribution.getParameter<std::string>("meName");

    edm::ParameterSet cfgVariable = cfgVariables.getParameter<edm::ParameterSet>(*varName);
    std::vector<fitRangeEntryType> fitRanges = getFitRanges(cfgVariable);

    dataNdEntry_->addVar(*varName, meName, fitRanges);
  }

//--- read configuration parameters specifying options for making control plots
  edm::ParameterSet cfgControlPlots = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("controlPlots");
  controlPlotsFileName_ = cfgControlPlots.getParameter<std::string>("fileName");

//--- read configuration parameters specifying options for saving fit results in DQMStore
  edm::ParameterSet cfgSaveFitResults = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("fitResults");
  dqmDirectory_fitResult_ = dqmDirectoryName(cfgSaveFitResults.getParameter<std::string>("dqmDirectory"));

//--- read configuration parameters specifying systematic uncertainties 
//    on normalization factors determined by fit
  if ( cfg.exists("estSysUncertainties") ) {
    edm::ParameterSet cfgSysErr = cfg.getParameter<edm::ParameterSet>("estSysUncertainties");
    edm::ParameterSet cfgSysFluctuations = cfgSysErr.getParameter<edm::ParameterSet>("fluctuations");
    vstring sysErrNames = cfgSysFluctuations.getParameterNamesForType<edm::ParameterSet>();
    for ( vstring::const_iterator sysErrName = sysErrNames.begin(); 
	  sysErrName != sysErrNames.end(); ++sysErrName ) {
      edm::ParameterSet cfgSysFluct = cfgSysFluctuations.getParameter<edm::ParameterSet>(*sysErrName);
      
      double pullRMS = cfgSysFluct.getParameter<double>("pullRMS");
      double pullMin = cfgSysFluct.getParameter<double>("pullMin");
      double pullMax = cfgSysFluct.getParameter<double>("pullMax");
      
      std::string fluctMode_string = cfgSysFluct.getParameter<std::string>("mode");
      int fluctMode_int = -1;
      if ( fluctMode_string == fluctMode_coherent ) {
	fluctMode_int = kCoherent;
      } else if ( fluctMode_string == fluctMode_incoherent ) {
	fluctMode_int = kIncoherent;
      } else {
	edm::LogError ("TemplateFitAdapterBase") << " Invalid 'mode' parameter = " << fluctMode_string << " !!";
	error_ = 1;
      }
      
      edm::ParameterSet cfgProcesses = cfgSysFluct.getParameter<edm::ParameterSet>("meNames");

      for ( vstring::iterator processName = processNames_.begin();
	    processName != processNames_.end(); ++processName ) {
	if ( !cfgProcesses.exists(*processName) ) {
	  edm::LogError ("TemplateFitAdapterBase") << " No Estimate of systematic Uncertainty = " << (*sysErrName) 
						   << " defined for process = " << (*processName) << " !!";
	  error_ = 1;
	  continue;
	}
	
	edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);
	
	for ( vstring::const_iterator varName = varNames_.begin();
	      varName != varNames_.end(); ++varName ) {
	  if ( !cfgProcess.exists(*varName) ) {
	    edm::LogError ("TemplateFitAdapterBase") << " No Estimate of systematic Uncertainty = " << (*sysErrName) 
						     << " on variable = " << (*varName) 
						     << " defined for process = " << (*processName) << " !!";
	    error_ = 1;
	  }
	  
	  std::string meName = cfgProcess.getParameter<std::string>(*varName);
	  
	  model1dType::sysErrType sysErr;
	  sysErr.fluctName_ = (*sysErrName);
	  sysErr.meName_ = meName;
	  sysErr.pullRMS_ = pullRMS;
	  sysErr.pullMin_ = pullMin;
	  sysErr.pullMax_ = pullMax;
	  sysErr.fluctMode_ = fluctMode_int;
	  
	  modelNdEntries_[*processName]->model1dEntries_[*varName]->sysErrFluctuations_.push_back(sysErr);
	}
      }
    }
  }
}

TemplateFitAdapterBase::~TemplateFitAdapterBase()
{
  delete dataNdEntry_;

  for ( modelNdEntryMap::iterator it = modelNdEntries_.begin();
	it != modelNdEntries_.end(); ++it ) {
    delete it->second;
  }

  for ( std::map<std::string, drawOptionsType*>::iterator it = drawOptions_.begin();
	it != drawOptions_.end(); ++it ) {
    delete it->second;
  }

  delete fitResult_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TemplateFitAdapterBase::initialize()
{
//--- check that configuration parameters contain no errors,
//    retrieve MonitorElements from DQMStore
//    and check that all DQM MonitorElements have successfully been retrieved
  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    modelNdEntry->second->initialize();
    if ( modelNdEntry->second->error_ ) error_ = 1;
  }

  dataNdEntry_->initialize();
  if ( dataNdEntry_->error_ ) error_ = 1;

  fitResult_ = new fitResultType();
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

const TemplateFitAdapterBase::fitResultType* TemplateFitAdapterBase::fit(int printLevel, int printWarnings)
{
  std::cout << "<TemplateFitAdapterBase::fit>:" << std::endl;

  if ( isFirstFit_ ) {
//--- retrieve template histograms and distributions observed in (pseudo)data from DQMStore
    initialize();
    isFirstFit_ = false;
  }

//--- fit set of signal and background templates
//    to distribution(s) observed in data
//    (implementation specific to actual fitting algorithm)
  fitImp(printLevel, printWarnings);

//--- copy normalization factors determined by fit,
//    signal and background templates and distribution(s) observed in data
//    into fitResultsType object returned by fit function
  unpackFitResults();

  return fitResult_;
}  

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TemplateFitAdapterBase::unpackFitResults()
{
  for ( vstring::const_iterator varName = varNames_.begin();
	varName != varNames_.end(); ++varName ) {
    const data1dType* data1dEntry = dataNdEntry_->data1dEntries_[*varName];

    fitResult_->distributions_[*varName].data_ = data1dEntry->me_->getTH1();

    fitResult_->distributions_[*varName].fitRanges_ = data1dEntry->fitRanges_;

    for ( vstring::const_iterator processName = processNames_.begin();
	  processName != processNames_.end(); ++processName ) {
      const model1dType* model1dEntry = modelNdEntries_[*processName]->model1dEntries_[*varName];
      fitResult_->distributions_[*varName].templates_[*processName] = model1dEntry->me_->getTH1();
    }
  }

  unpackFitResultsImpSpecific();
}

void TemplateFitAdapterBase::saveFitResults() 
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("saveFitResults") << " Failed to access dqmStore --> fit results will NOT be saved !!";
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) { 
    const fitResultType::normEntryType& norm = fitResult_->normalizations_[*processName];
    
    saveFitParameter(dqmStore, dqmDirectory_fitResult_, *processName, std::string("norm"), norm.value_, norm.errUp_, norm.errDown_);
  }

//--- save values of additional parameters determined by fit,
//    specific to actual fitting algorithm being used 
  saveFitResultsImpSpecific();
}

void TemplateFitAdapterBase::print(std::ostream& stream) const
{
  stream << "Fit Parameter:" << std::endl;
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) { 
    const fitResultType::normEntryType& norm = fitResult_->normalizations_[*processName];

    stream << " " << (*processName) << ": normalization = " << norm.value_;

    if ( norm.errUp_ != norm.errDown_ ) {
      stream << " + " << norm.errUp_ << " - " << norm.errUp_;
    } else {
      double err = 0.5*(norm.errUp_ + norm.errDown_);
      stream << " +/- " << err;
    }
    
    stream << std::endl;
  }

//--- print values of additional parameters determined by fit,
//    specific to actual fitting algorithm being used 
  printImpSpecific(stream);
}

void TemplateFitAdapterBase::makeControlPlots() 
{ 
//--- produce control plot of distribution observed in (pseudo)data
//    versus sum of signal and background templates using normalization determined by fit

//--- stop ROOT from opening X-window for canvas output
//    (in order to be able to run in batch mode) 
  gROOT->SetBatch(true);

//--- produce control plots of one and two sigma error contours 
//    showing correlation of estimated normalization factors
  unsigned numProcesses = processNames_.size();
  vstring labels(numProcesses);
  for ( unsigned iX = 0; iX < numProcesses; ++iX ) {
    labels[iX] = std::string("norm").append("_").append(processNames_[iX]);
  }

  makeControlPlotsCovariance(fitResult_->normValues_, fitResult_->normValues_, fitResult_->normCov_, labels, controlPlotsFileName_, "");

//--- produce control plots of distributions observed in (pseudo)data
//    compared to sum of signal and background templates
//    scaled by normalization factors determined by the fit
  makeControlPlotsNdObsDistribution(fitResult_, drawOptions_, controlPlotsFileName_);

//--- produce additional control plots specific to algorithm 
//    being used to perform the actual fitting
  makeControlPlotsImpSpecific();
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(TemplateHistFitAdapterPluginFactory, "TemplateHistFitAdapterPluginFactory");
