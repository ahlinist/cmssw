#include "TauAnalysis/DQMTools/plugins/DQMHistErrorBandProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

const std::string method_sqrt = "sqrt";
const std::string method_min_max = "min_max"; 

const std::string defaultMethod = method_sqrt;

const int verbosity = 0;

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistErrorBandProducer::cfgEntryProduceJob::cfgEntryProduceJob(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistErrorBandProducer::cfgEntryProduceJob>" << std::endl;

  dqmDirectory_inputCentralValue_ = ( cfg.exists("dqmDirectory_inputCentralValue") ) ?
    cfg.getParameter<std::string>("dqmDirectory_inputCentralValue") : "";
  dqmDirectories_inputVariance_ = cfg.getParameter<vstring>("dqmDirectories_inputVariance");
  dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectory_output");
  method_ = ( cfg.exists("method") ) ?
    cfg.getParameter<std::string>("method") : defaultMethod;

  if ( verbosity ) print();
}

void DQMHistErrorBandProducer::cfgEntryProduceJob::print() const
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

DQMHistErrorBandProducer::DQMHistErrorBandProducer(const edm::ParameterSet& cfg)
  : cfgError_(0)	
{
  //std::cout << "<DQMHistErrorBandProducer::DQMHistErrorBandProducer>:" << std::endl;

//--- configure processes  
  //std::cout << "--> configuring produceJobs..." << std::endl;
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgProduceJobs = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgProduceJob = cfgProduceJobs.begin();
	cfgProduceJob != cfgProduceJobs.end(); ++cfgProduceJob ) {
    produceJobs_.push_back(cfgEntryProduceJob(*cfgProduceJob));
  }
  
  //std::cout << "done." << std::endl;
}

DQMHistErrorBandProducer::~DQMHistErrorBandProducer() 
{
// nothing to be done yet...
}

void DQMHistErrorBandProducer::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

MonitorElement* getMonitorElement(DQMStore& dqmStore, const std::string& dqmDirectory, const std::string& meName, bool& dqmError) 
{
  std::string meName_full = dqmDirectoryName(dqmDirectory).append(meName);
  MonitorElement* me = dqmStore.get(meName_full);
  if ( !me ) {
    edm::LogError ("getMonitorElement") << " Failed to access meName = " << meName_full << " in DQMStore !!";
    dqmError = true;
  }
  return me;
}

TH1* getHistogram(MonitorElement* me, bool& dqmError)
{
  TH1* histogram = me->getTH1();
  if ( !histogram ) {
    edm::LogError ("getHistogram") << " Failed to access histogram associated to meName = " << me->getName() << " in DQMStore !!";
    dqmError = true;
  }
  return histogram;
}

void computeErrorBand(DQMStore& dqmStore, const std::string& inputDirectory_centralValue, 
		      const std::vector<std::string>& inputDirectories_variance, 
		      const std::string& outputDirectory, 
		      const std::string& dqmSubDirectory, const std::string& method, int nDoF)
{
  //std::cout << "<computeErrorBand>:" << std::endl;
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

  dqmStore.setCurrentFolder(dqmDirectory_centralValue);

//--- compute error-band for all MonitorElements stored in current DQM directory
  std::vector<std::string> meNames = dqmStore.getMEs();
  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    bool dqmError = false;

    dqmStore.setCurrentFolder(dqmDirectory_centralValue);
    MonitorElement* me_centralValue = getMonitorElement(dqmStore, dqmDirectory_centralValue, *meName, dqmError);
    if ( !me_centralValue ) continue;

    if ( me_centralValue->kind() == MonitorElement::DQM_KIND_TH1F      ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TH1S      ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TH2F      ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TH2S      ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TH3F      ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TPROFILE  ||
	 me_centralValue->kind() == MonitorElement::DQM_KIND_TPROFILE2D ) {
      TH1* histogram_centralValue = getHistogram(me_centralValue, dqmError);

      std::vector<TH1*> histograms_variance;
      for ( std::vector<std::string>::const_iterator dqmDirectory_variance = dqmDirectories_variance.begin();
	    dqmDirectory_variance != dqmDirectories_variance.end(); ++dqmDirectory_variance ) {
	dqmStore.setCurrentFolder(*dqmDirectory_variance);
	MonitorElement* me_variance = getMonitorElement(dqmStore, *dqmDirectory_variance, *meName, dqmError);
	if ( me_variance ) {
	  TH1* histogram_variance = getHistogram(me_variance, dqmError);
	  histograms_variance.push_back(histogram_variance);
	}
      }

      dqmStore.setCurrentFolder(dqmDirectory_output);
      MonitorElement* me_output = getMonitorElement(dqmStore, dqmDirectory_output, *meName, dqmError);
      TH1* histogram_output = 0;
      if ( me_output ) {
	histogram_output = getHistogram(me_output, dqmError);
      }
      
      if ( dqmError ) {
	edm::LogWarning ("computeErrorBand") << " Failed to retrieve histograms for meName = " << (*meName) << " --> skipping !!";
	continue;
      }

      unsigned numBinsX = histogram_centralValue->GetNbinsX();
      for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	unsigned numBinsY = histogram_centralValue->GetNbinsY();
	for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  unsigned numBinsZ = histogram_centralValue->GetNbinsZ();
	  for ( unsigned iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	    int binNumber = histogram_centralValue->GetBin(iBinX, iBinY, iBinZ);

	    double binContent_centralValue = histogram_centralValue->GetBinContent(binNumber);
	    
	    double variance = 0.;

	    double binContent_min = 0.;
	    double binContent_max = 0.;
	    bool isFirstHistogram = true;

	    for ( std::vector<TH1*>::const_iterator histogram_variance = histograms_variance.begin();
		  histogram_variance != histograms_variance.end(); ++histogram_variance ) {
	      if ( !(histogram_centralValue->GetNbinsX() == (*histogram_variance)->GetNbinsX() &&
		     histogram_centralValue->GetNbinsX() == histogram_output->GetNbinsX() &&
		     histogram_centralValue->GetNbinsY() == (*histogram_variance)->GetNbinsY() &&
		     histogram_centralValue->GetNbinsY() == histogram_output->GetNbinsY() &&
		     histogram_centralValue->GetNbinsZ() == (*histogram_variance)->GetNbinsZ() &&
		     histogram_centralValue->GetNbinsZ() == histogram_output->GetNbinsZ()) ) {
		edm::LogWarning ("computeErrorBand") << " Mismatch in histogram binning for meName = " << (*meName) << " --> skipping !!";
		std::cout << " central value: numBinsX = " << histogram_centralValue->GetNbinsX() << ","
			  << " numBinsY = " << histogram_centralValue->GetNbinsY() << ","
			  << " numBinsZ = " << histogram_centralValue->GetNbinsZ() << std::endl;
		std::cout << " variance: numBinsX = " << (*histogram_variance)->GetNbinsX() << ","
			  << " numBinsY = " << (*histogram_variance)->GetNbinsY() << ","
			  << " numBinsZ = " << (*histogram_variance)->GetNbinsZ() << std::endl;
		std::cout << " output: numBinsX = " << histogram_output->GetNbinsX() << ","
			  << " numBinsY = " << histogram_output->GetNbinsY() << ","
			  << " numBinsZ = " << histogram_output->GetNbinsZ() << std::endl;
	      }

	      double binContent = (*histogram_variance)->GetBinContent(binNumber);

	      double diffBinContent = binContent - binContent_centralValue;
	      variance += diffBinContent*diffBinContent;

	      if ( binContent < binContent_min || isFirstHistogram ) binContent_min = binContent;
	      if ( binContent > binContent_max || isFirstHistogram ) binContent_max = binContent;
	      isFirstHistogram = false;
	    }
	    
	    if ( method == method_sqrt ) {
	      double binError = ( nDoF > 0 ) ? TMath::Sqrt(variance/nDoF) : 0.;
	      histogram_output->SetBinError(binNumber, binError);
	    } else if ( method == method_min_max ) {
	      double binContent_centralValue = 0.5*(binContent_max + binContent_min);
	      histogram_output->SetBinContent(binNumber, binContent_centralValue);

	      double binError = 0.5*(binContent_max - binContent_min);
	      histogram_output->SetBinError(binNumber, binError);
	    } else {
	      edm::LogWarning ("computeErrorBand") << " Unsupported method = " << method << " for computing error-band --> skipping !!";
	    }
	  }
	}
      }
    }
  }

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

void DQMHistErrorBandProducer::endJob()
{
  std::cout << "<DQMHistErrorBandProducer::endJob>:" << std::endl;

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
      dqmCopyRecursively(dqmStore, dqmDirectory_inputCentralValue, dqmDirectory_output, 1., 1, false);
      nDoF = dqmDirectories_inputVariance.size();
    } else if ( dqmDirectories_inputVariance.size() > 0. ) {
      double scaleFactor = 1./dqmDirectories_inputVariance.size();
      for ( vstring::const_iterator dqmDirectory_input = dqmDirectories_inputVariance.begin();
	    dqmDirectory_input != dqmDirectories_inputVariance.end(); ++dqmDirectory_input ) {
//--- when processing first inputDirectory, check that histograms in outputDirectory do not yet exist;
//    afterwards, add histograms in inputDirectory to those in outputDirectory
        int mode = ( dqmDirectory_input == dqmDirectories_inputVariance.begin() ) ? 1 : 2;
        //std::cout << " mode = " << mode << std::endl;
	dqmCopyRecursively(dqmStore, *dqmDirectory_input, dqmDirectory_output, scaleFactor, mode, false);
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
  if ( verbosity ) dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistErrorBandProducer);
