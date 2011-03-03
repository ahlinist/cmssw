#include "TauAnalysis/DQMTools/plugins/DQMHistErrorBandProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

DQMHistErrorBandProducer::DQMHistErrorBandProducer(const edm::ParameterSet& cfg)
  : DQMErrorBandProducerBase(cfg)
{
// nothing to be done yet...
}

DQMHistErrorBandProducer::~DQMHistErrorBandProducer() 
{
// nothing to be done yet...
}

void DQMHistErrorBandProducer::computeErrorBandImp(DQMStore& dqmStore, 
						   const std::string& dqmDirectory_centralValue, 
						   const std::vector<std::string>& dqmDirectories_variance, 
						   const std::string& dqmDirectory_output, 
						   const std::string& method, int nDoF)
{
  //std::cout << "<DQMHistErrorBandProducer::computeErrorBand>:" << std::endl;
  //std::cout << " inputDirectory_centralValue = " << inputDirectory_centralValue << std::endl;
  //std::cout << " inputDirectories_variance = " << format_vstring(inputDirectories_variance) << std::endl;
  //std::cout << " outputDirectory = " << outputDirectory << std::endl;
  //std::cout << " nDoF = " << nDoF << std::endl;

  dqmStore.setCurrentFolder(dqmDirectory_centralValue);

//--- compute error-band for all MonitorElements stored in current DQM directory
  std::vector<std::string> meNames = dqmStore.getMEs();
  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    bool dqmError = false;
    
    std::string meName_centralValue = dqmDirectoryName(dqmDirectory_centralValue).append(*meName);
    TH1* histogram_centralValue = getHistogram(dqmStore, meName_centralValue, dqmError);
    
    if ( histogram_centralValue ) {
      
      std::vector<TH1*> histograms_variance;
      for ( std::vector<std::string>::const_iterator dqmDirectory_variance = dqmDirectories_variance.begin();
	    dqmDirectory_variance != dqmDirectories_variance.end(); ++dqmDirectory_variance ) {
	std::string meName_variance = dqmDirectoryName(*dqmDirectory_variance).append(*meName);
	TH1* histogram_variance = getHistogram(dqmStore, meName_variance, dqmError);
	histograms_variance.push_back(histogram_variance);
      }
      
      std::string meName_output = dqmDirectoryName(dqmDirectory_output).append(*meName);
      TH1* histogram_output = getHistogram(dqmStore, meName_output, dqmError);
      
      if ( dqmError ) {
	edm::LogWarning ("computeErrorBandImp") 
	  << " Failed to retrieve histograms for meName = " << (*meName) << " --> skipping !!";
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
		edm::LogWarning ("computeErrorBandImp") 
		  << " Mismatch in histogram binning for meName = " << (*meName) << " --> skipping !!";
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
	      edm::LogWarning ("computeErrorBandImp") 
		<< " Unsupported method = " << method << " for computing error-band --> skipping !!";
	    }
	  }
	}
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistErrorBandProducer);
