#include "TauAnalysis/DQMTools/plugins/DQMHistEffProducer.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH1.h>

#include <iostream>
#include <iomanip>

typedef std::vector<std::string> vstring;

DQMHistEffProducer::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;

  meName_numerator_ = cfg.getParameter<std::string>("meName_numerator");
  //std::cout << " meName_numerator = " << meName_numerator_ << std::endl;

  meName_denominator_ = cfg.getParameter<std::string>("meName_denominator");
  //std::cout << " meName_denominator = " << meName_denominator_ << std::endl;

  meName_efficiency_ = cfg.getParameter<std::string>("meName_efficiency");
  //std::cout << " meName_efficiency = " << meName_efficiency_ << std::endl;
}

DQMHistEffProducer::cfgEntryPlot::cfgEntryPlot(const std::string& meName_numerator, const std::string& meName_denominator, const std::string& meName_efficiency)
  : meName_numerator_(meName_numerator), meName_denominator_(meName_denominator), meName_efficiency_(meName_efficiency)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;
  //std::cout << " meName_numerator = " << meName_numerator_ << std::endl;
  //std::cout << " meName_denominator = " << meName_denominator_ << std::endl;
  //std::cout << " meName_efficiency = " << meName_efficiency_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistEffProducer::DQMHistEffProducer(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistEffProducer::DQMHistEffProducer>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	cfgPlot != cfgPlots.end(); ++cfgPlot ) {
    if ( cfgPlot->exists("parameter") ) {
      std::string meName_numerator = cfgPlot->getParameter<std::string>("meName_numerator");
      std::string meName_denominator = cfgPlot->getParameter<std::string>("meName_denominator");
      std::string meName_efficiency = cfgPlot->getParameter<std::string>("meName_efficiency");

      vstring parameter = cfgPlot->getParameter<vstring>("parameter");
      
      for ( vstring::const_iterator parameter_i = parameter.begin();
	    parameter_i != parameter.end(); ++parameter_i ) {

	int errorFlag = 0;
	std::string modNumerator = replace_string(meName_numerator, parKeyword, *parameter_i, 1, 1, errorFlag);
	std::string modDenominator = replace_string(meName_denominator, parKeyword, *parameter_i, 1, 1, errorFlag);
	std::string modEfficiency = replace_string(meName_efficiency, parKeyword, *parameter_i, 1, 1, errorFlag);

	if ( !errorFlag ) {
	  cfgEntryPlot_.push_back(cfgEntryPlot(modNumerator, modDenominator, modEfficiency));
	} else {
	  edm::LogError("DQMHistEffProducer") << " Failed to decode histogram names --> skipping !!";
	}
      }
    } else {
      cfgEntryPlot_.push_back(cfgEntryPlot(*cfgPlot));
    }
  }
}

DQMHistEffProducer::~DQMHistEffProducer() 
{
//--- nothing to be done yet
}

void DQMHistEffProducer::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

float* getBinning(const TAxis* axis)
{
  unsigned numBins = axis->GetNbins();

  float* binEdges_float = new float[numBins + 1];

  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    binEdges_float[iBin] = axis->GetBinLowEdge(iBin + 1);
  }

  binEdges_float[numBins] = axis->GetBinUpEdge(numBins);

  return binEdges_float;
}

void DQMHistEffProducer::endJob()
{
  //std::cout << "<DQMHistEffProducer::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> efficiency histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryPlot>::const_iterator plot = cfgEntryPlot_.begin(); 
        plot != cfgEntryPlot_.end(); ++plot ) {
    std::string numeratorHistogramName, numeratorHistogramDirectory;
    separateMonitorElementFromDirectoryName(plot->meName_numerator_, numeratorHistogramName, numeratorHistogramDirectory);
    MonitorElement* meNumerator = dqmStore.get(std::string(numeratorHistogramDirectory).append(dqmSeparator).append(numeratorHistogramName));
    TH1* histoNumerator = ( meNumerator != NULL ) ? meNumerator->getTH1() : NULL;
    
    std::string denominatorHistogramName, denominatorHistogramDirectory;
    separateMonitorElementFromDirectoryName(plot->meName_denominator_, denominatorHistogramName, denominatorHistogramDirectory);
    MonitorElement* meDenominator = dqmStore.get(std::string(denominatorHistogramDirectory).append(dqmSeparator).append(denominatorHistogramName));
    TH1* histoDenominator = ( meDenominator != NULL ) ? meDenominator->getTH1() : NULL;
    
    if ( histoNumerator != NULL && histoDenominator != NULL ) {
      if ( !histoNumerator->GetSumw2N()   ) histoNumerator->Sumw2();
      if ( !histoDenominator->GetSumw2N() ) histoDenominator->Sumw2();
      
      std::string effHistogramName, effHistogramDirectory, dummy;
      separateMonitorElementFromDirectoryName(plot->meName_efficiency_, effHistogramName, effHistogramDirectory);
      if ( effHistogramDirectory == "" ) separateMonitorElementFromDirectoryName(numeratorHistogramName, dummy, effHistogramDirectory);
      if ( effHistogramDirectory != "" ) dqmStore.setCurrentFolder(effHistogramDirectory);
      
      if ( histoNumerator->GetDimension() == 1 && histoDenominator->GetDimension() == 1 ) {
	unsigned numBins = histoNumerator->GetXaxis()->GetNbins();
	float* binEdges = getBinning(histoNumerator->GetXaxis());
	MonitorElement* histoEfficiency = dqmStore.book1D(effHistogramName, effHistogramName, 
							  numBins, binEdges);
	histoEfficiency->getTH1F()->Divide(histoNumerator, histoDenominator, 1., 1., "B");
	delete[] binEdges;
      } else if ( histoNumerator->GetDimension() == 2 && histoDenominator->GetDimension() == 2 ) {
	unsigned numBinsX = histoNumerator->GetXaxis()->GetNbins();
	float* binEdgesX = getBinning(histoNumerator->GetXaxis());
	unsigned numBinsY = histoNumerator->GetYaxis()->GetNbins();
	float* binEdgesY = getBinning(histoNumerator->GetYaxis());
	MonitorElement* histoEfficiency = dqmStore.book2D(effHistogramName, effHistogramName, 
							  numBinsX, binEdgesX, numBinsY, binEdgesY);
	histoEfficiency->getTH2F()->Divide(histoNumerator, histoDenominator, 1., 1., "B");
	delete[] binEdgesX;
	delete[] binEdgesY;
      } else {
	edm::LogError("endJob") << " Unsupported dimensionality of numerator = " << histoNumerator->GetDimension()
				<< " and denominator = " << histoDenominator->GetDimension() << " Histograms !!";
      }
    } else {
      std::ostringstream message;
      message << " Failed to produce efficiency Histogram = " << plot->meName_efficiency_ << ";";
      if ( histoNumerator   == NULL ) message << " numerator = " << plot->meName_numerator_ << " does not exist";
      if ( histoNumerator   == NULL && histoDenominator == NULL ) message << ",";
      if ( histoDenominator == NULL ) message << " denominator = " << plot->meName_denominator_ << " does not exist";
      message << " !!";
      edm::LogError("endJob") << message.str();
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistEffProducer);
