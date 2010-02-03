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
#include <TMath.h>

#include <iostream>
#include <iomanip>

typedef std::vector<std::string> vstring;

DQMHistEffProducer::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;

  if ( cfg.exists("meNames_numerator") && cfg.exists("meNames_denominator") ) {
    typedef std::vector<std::string> vstring;
    meNames_numerator_ = cfg.getParameter<vstring>("meNames_numerator");
    meNames_denominator_ = cfg.getParameter<vstring>("meNames_denominator");
    if ( meNames_numerator_.size() != meNames_denominator_.size()  ) {
      edm::LogError("DQMHistEffProducer") 
	<< " Mismatch in number of numerator = " << meNames_numerator_.size() 
	<< " and denominator = " <<  meNames_denominator_.size() << " histograms !!"; 
      cfgError_ = 1;
    }
  } else {
    meNames_numerator_.push_back(cfg.getParameter<std::string>("meName_numerator"));
    meNames_denominator_.push_back(cfg.getParameter<std::string>("meName_denominator"));
  }

  meName_efficiency_ = cfg.getParameter<std::string>("meName_efficiency");

  //print();
}

DQMHistEffProducer::cfgEntryPlot::cfgEntryPlot(const std::string& meName_numerator, 
					       const std::string& meName_denominator, 
					       const std::string& meName_efficiency)
  : meName_efficiency_(meName_efficiency)
{
  meNames_numerator_.push_back(meName_numerator);
  meNames_denominator_.push_back(meName_denominator);

  //print();
}

void DQMHistEffProducer::cfgEntryPlot::print() const
{
  std::cout << "<cfgEntryPlot::print>:" << std::endl;
  std::cout << " meNames_numerator = " << format_vstring(meNames_numerator_) << std::endl;
  std::cout << " meNames_denominator = " << format_vstring(meNames_denominator_) << std::endl;
  std::cout << " meName_efficiency = " << meName_efficiency_ << std::endl;
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

std::vector<TH1*> getHistograms(DQMStore& dqmStore, const std::vector<std::string>& meNames, bool& dqmError)
{
  std::vector<TH1*> histograms;

  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    std::string histogramName, histogramDirectory;
    separateMonitorElementFromDirectoryName(*meName, histogramName, histogramDirectory);

    MonitorElement* me = dqmStore.get(std::string(histogramDirectory).append(dqmSeparator).append(histogramName));

    TH1* histogram = ( me != NULL ) ? me->getTH1() : NULL;
    //std::cout << "meName = " << (*meName) << ": integral = " << histogram->Integral() << std::endl;

    if ( histogram ) {
      if ( !histogram->GetSumw2N() ) histogram->Sumw2();

      histograms.push_back(histogram);
    } else {
      edm::LogError("DQMHistEffProducer") << " Failed to retrieve MonitorElement = " << (*meName) << " --> skipping !!";
      dqmError = true;
    }
  } 

  return histograms;
}

TH1* computeHistogramProd(const std::vector<TH1*>& histograms, bool& isOwned)
{
  if ( histograms.size() == 0 ) {
    edm::LogError("computeHistogramProd") << " List of Histograms passed as function Argument is empty !!";
    isOwned = false;
    return 0;
  } else if ( histograms.size() == 1 ) {
    isOwned = false;
    return histograms.front();
  } else {
    unsigned numBinsX = histograms.front()->GetNbinsX();
    unsigned numBinsY = histograms.front()->GetNbinsY();
    unsigned numBinsZ = histograms.front()->GetNbinsZ();
    
    for ( std::vector<TH1*>::const_iterator histogram = histograms.begin();
	  histogram != histograms.end(); ++histogram ) {
      unsigned numBinsX_i = (*histogram)->GetNbinsX();
      unsigned numBinsY_i = (*histogram)->GetNbinsY();
      unsigned numBinsZ_i = (*histogram)->GetNbinsZ();
      
      if ( !(numBinsX_i == numBinsX && numBinsY_i == numBinsY && numBinsZ_i == numBinsZ) ) {
	edm::LogError("computeHistogramProd") << " Mismatch in Histogram binning !!";
	std::cout << "(X: found = " << numBinsX_i << ", expected = " << numBinsX << ";"
		  << " Y: found = " << numBinsY_i << ", expected = " << numBinsY << ";"
		  << " Z: found = " << numBinsZ_i << ", expected = " << numBinsZ << ")" << std::endl;
	return 0;
      }
    }
    
    TH1* histogramProd = (TH1*)histograms.front()->Clone();
    
    for ( unsigned iBinX = 0; iBinX <= (numBinsX + 1); ++iBinX ) {
      for ( unsigned iBinY = 0; iBinY <= (numBinsY + 1); ++iBinY ) {
	for ( unsigned iBinZ = 0; iBinZ <= (numBinsZ + 1); ++iBinZ ) {
	  double binContent_prod = 1.;
	  double fractionalSigma_sum2 = 0.;
      
	  for ( std::vector<TH1*>::const_iterator histogram = histograms.begin();
		histogram != histograms.end(); ++histogram ) {
	    double binContent_i = (*histogram)->GetBinContent(iBinX, iBinY, iBinZ);
	    double binError_i = (*histogram)->GetBinError(iBinX, iBinY, iBinZ);

	    binContent_prod *= binContent_i;
	    // CV: compute uncertainty on product of bin-contents;
	    //     for 
	    //      prod := a*b*c 
	    //     the uncertainty is 
	    //      sigma_prod^2 = (b*c*sigma_a)^2 + (a*c*sigma_b)^2 + (a*b*sigma_c)^2
	    //                   = prod^2*((sigma_a/a)^2 + (sigma_b/b)^2 + (sigma_c/c)^2)
	    if ( binContent_i > 0. ) {
	      double fractionalSigma = binError_i/binContent_i;
	      fractionalSigma_sum2 += fractionalSigma*fractionalSigma;
	    }
	  }

	  double binError_prod = binContent_prod * TMath::Sqrt(fractionalSigma_sum2);

	  histogramProd->SetBinContent(iBinX, iBinY, iBinZ, binContent_prod);
	  histogramProd->SetBinError(iBinX, iBinY, iBinZ, binError_prod);
	}
      }
    }

    isOwned = true;
    return histogramProd;
  }
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
    bool dqmError = false;

    std::vector<TH1*> histogramsNumerator = getHistograms(dqmStore, plot->meNames_numerator_, dqmError);
    std::vector<TH1*> histogramsDenominator = getHistograms(dqmStore, plot->meNames_denominator_, dqmError);
    
    if ( !dqmError ) {
      std::string effHistogramName, effHistogramDirectory, dummy;
      separateMonitorElementFromDirectoryName(plot->meName_efficiency_, effHistogramName, effHistogramDirectory);
      dqmStore.setCurrentFolder(effHistogramDirectory);

      int numDimensions = -1;
      for ( std::vector<TH1*>::const_iterator histogramNumerator = histogramsNumerator.begin();
	    histogramNumerator != histogramsNumerator.end(); ++histogramNumerator ) {
	int numDimensions_i = (*histogramNumerator)->GetDimension();
	if ( numDimensions == -1 ) numDimensions = numDimensions_i;
	if ( numDimensions != -1 && numDimensions_i != numDimensions ) {
	  edm::LogError("endJob") << " Mismatch in Histogram dimensionality !!";
	  std::cout << "(found = " << numDimensions_i << ", expected = " << numDimensions << ")" << std::endl;
	}
      }

      bool histogramProdNumerator_isOwned;
      TH1* histogramProdNumerator = computeHistogramProd(histogramsNumerator, histogramProdNumerator_isOwned);
      bool histogramProdDenominator_isOwned;
      TH1* histogramProdDenominator = computeHistogramProd(histogramsDenominator, histogramProdDenominator_isOwned);
      
      if ( numDimensions == 1 ) {
	unsigned numBins = histogramProdNumerator->GetXaxis()->GetNbins();
	float* binEdges = getBinning(histogramProdNumerator->GetXaxis());
	MonitorElement* histogramEfficiency = dqmStore.book1D(effHistogramName, effHistogramName, numBins, binEdges);
	histogramEfficiency->getTH1F()->Divide(histogramProdNumerator, histogramProdDenominator, 1., 1., "B");
	delete[] binEdges;
      } else if ( numDimensions == 2 ) {
	unsigned numBinsX = histogramProdNumerator->GetXaxis()->GetNbins();
	float* binEdgesX = getBinning(histogramProdNumerator->GetXaxis());
	unsigned numBinsY = histogramProdNumerator->GetYaxis()->GetNbins();
	float* binEdgesY = getBinning(histogramProdNumerator->GetYaxis());
	MonitorElement* histogramEfficiency = dqmStore.book2D(effHistogramName, effHistogramName, numBinsX, binEdgesX, numBinsY, binEdgesY);
	histogramEfficiency->getTH2F()->Divide(histogramProdNumerator, histogramProdDenominator, 1., 1., "B");
	delete[] binEdgesX;
	delete[] binEdgesY;
      } 

      if ( histogramProdNumerator_isOwned   ) delete histogramProdNumerator;
      if ( histogramProdDenominator_isOwned ) delete histogramProdDenominator;
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistEffProducer);
