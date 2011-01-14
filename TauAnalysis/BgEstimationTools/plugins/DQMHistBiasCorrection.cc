#include "TauAnalysis/BgEstimationTools/plugins/DQMHistBiasCorrection.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

DQMHistBiasCorrection::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;

  meName_uncorrected_ = cfg.getParameter<std::string>("meName_uncorrected");
  meName_corrected_ = cfg.getParameter<std::string>("meName_corrected");
  meName_corrNumerator_ = cfg.getParameter<std::string>("meName_corrNumerator");
  meName_corrDenominator_ = cfg.getParameter<std::string>("meName_corrDenominator");

  //print();
}

void DQMHistBiasCorrection::cfgEntryPlot::print() const
{
  std::cout << "<cfgEntryPlot::print>:" << std::endl;
  std::cout << " meName_uncorrected = " << meName_uncorrected_ << std::endl;
  std::cout << " meName_corrected = " << meName_corrected_ << std::endl;
  std::cout << " meName_corrNumerator = " << meName_corrNumerator_ << std::endl;
  std::cout << " meName_corrDenominator = " << meName_corrDenominator_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistBiasCorrection::DQMHistBiasCorrection(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistBiasCorrection::DQMHistBiasCorrection>:" << std::endl;
  
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	  cfgPlot != cfgPlots.end(); ++cfgPlot ) {
      cfgEntryPlots_.push_back(cfgEntryPlot(*cfgPlot));
    }
  } else {
    cfgEntryPlots_.push_back(cfgEntryPlot(cfg));
  }
}

DQMHistBiasCorrection::~DQMHistBiasCorrection() 
{
//--- nothing to be done yet
}

void DQMHistBiasCorrection::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

bool isCompatibeBinning1d(
       const TAxis* axisUncorrectedHisto, const TAxis* axisCorrNumeratorHisto, const TAxis* axisCorrDenominatorHisto)
{
  if ( axisUncorrectedHisto->GetNbins() != axisCorrNumeratorHisto->GetNbins()   ) return false;
  if ( axisUncorrectedHisto->GetNbins() != axisCorrDenominatorHisto->GetNbins() ) return false;
  if ( axisUncorrectedHisto->GetXmin()  != axisCorrNumeratorHisto->GetXmin()    ) return false;
  if ( axisUncorrectedHisto->GetXmin()  != axisCorrDenominatorHisto->GetXmin()  ) return false;
  if ( axisUncorrectedHisto->GetXmax()  != axisCorrNumeratorHisto->GetXmax()    ) return false;
  if ( axisUncorrectedHisto->GetXmax()  != axisCorrDenominatorHisto->GetXmax()  ) return false;
  return true;
}

std::pair<double, double> compCorrBinContentErr(
                            double binContentUncorr, double binErrUncorr, 
			    double binContentCorrNum, double binErrCorrNum, double binContentCorrDenom, double binErrCorrDenom)
{
  double binContentCorr = 0.;
  if ( binErrCorrDenom != 0. ) {
    binContentCorr = binContentUncorr*(binContentCorrNum/binContentCorrDenom);
  } else {
    if ( binErrCorrDenom == 0. && binContentCorrNum == 0. )
      edm::LogWarning ("compCorrBinContentErr") 
	<< " Bin-contents of numerator and denominator histograms are zero --> setting binContentCorr = binContentUncorr !!";
    else
      edm::LogWarning ("compCorrBinContentErr") 
	<< " Bin-content of denominator histogram is zero, but numerator is non-zero --> setting binContentCorr = binContentUncorr !!";
    binContentCorr = binContentUncorr;
  }

  double binRelErrCorr2 = 0.;
  if ( binContentUncorr    != 0. ) binRelErrCorr2 += TMath::Power(binErrUncorr/binContentUncorr, 2.);
  if ( binContentCorrNum   != 0. ) binRelErrCorr2 += TMath::Power(binErrCorrNum/binContentCorrNum, 2.);
  if ( binContentCorrDenom != 0. ) binRelErrCorr2 += TMath::Power(binErrCorrDenom/binContentCorrDenom, 2.);
  
  double binErrCorr = binContentCorr*TMath::Sqrt(binRelErrCorr2);
  
  return std::pair<double, double>(binContentCorr, binErrCorr);
}

void DQMHistBiasCorrection::endJob()
{
  std::cout << "<DQMHistBiasCorrection::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> efficiency histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryPlot>::const_iterator plot = cfgEntryPlots_.begin(); 
        plot != cfgEntryPlots_.end(); ++plot ) {

    bool dqmError = false;
    TH1* histUncorr = getHistogram(dqmStore, plot->meName_uncorrected_, dqmError);
    TH1* histCorrNum = getHistogram(dqmStore, plot->meName_corrNumerator_, dqmError);
    TH1* histCorrDenom = getHistogram(dqmStore, plot->meName_corrDenominator_, dqmError);
    if ( dqmError ) {
      edm::LogError("DQMHistBiasCorrection::endJob") 
	<< " Failed to retrieve histograms from dqmStore --> skipping !!";
      continue;
    }

    std::string corrHistName, corrHistDirectory;
    separateMonitorElementFromDirectoryName(plot->meName_corrected_, corrHistName, corrHistDirectory);
    dqmStore.setCurrentFolder(corrHistDirectory);

    int numDimensions = histCorrNum->GetDimension();
    if ( !(numDimensions == 1 || numDimensions == 2) ) {
      edm::LogError("DQMHistBiasCorrection::endJob") 
	<< " Unsupported Histogram dimensionality = " << numDimensions << " !!";
      continue;
    }

    if ( numDimensions == 1 ) {
//--- check that binning of uncorrected, coorNumerator and corrDenominator histograms is compatible
      if ( !isCompatibeBinning1d(histUncorr->GetXaxis(), histCorrNum->GetXaxis(), histCorrDenom->GetXaxis()) ) {
	edm::LogError("DQMHistBiasCorrection::endJob") 
	  << " Incompatible binning of histograms --> skipping !!";
	continue;
      }

      unsigned numBins = histCorrNum->GetXaxis()->GetNbins();
      float* binEdges = getBinning(histCorrNum->GetXaxis());
      //std::cout << "--> booking meCorrected = " << corrHistName << std::endl;
      MonitorElement* meCorrected = dqmStore.book1D(corrHistName, corrHistName, numBins, binEdges);
      TH1* histCorrected = meCorrected->getTH1();
      for ( unsigned iBin = 1; iBin <= numBins; ++iBin ) {
	std::pair<double, double> binContentErrCorrected = 
	  compCorrBinContentErr(histUncorr->GetBinContent(iBin), histUncorr->GetBinError(iBin), 
				histCorrNum->GetBinContent(iBin), histCorrNum->GetBinError(iBin), 
				histCorrDenom->GetBinContent(iBin), histCorrDenom->GetBinError(iBin));
	
	histCorrected->SetBinContent(iBin, binContentErrCorrected.first);
	histCorrected->SetBinError(iBin, binContentErrCorrected.second);
      }
      delete[] binEdges;
    } else if ( numDimensions == 2 ) {
//--- check that binning of uncorrected, coorNumerator and corrDenominator histograms is compatible
      if ( !(isCompatibeBinning1d(histUncorr->GetXaxis(), histCorrNum->GetXaxis(), histCorrDenom->GetXaxis()) &&
	     isCompatibeBinning1d(histUncorr->GetYaxis(), histCorrNum->GetYaxis(), histCorrDenom->GetYaxis())) ) {
	edm::LogError("DQMHistBiasCorrection::endJob") 
	  << " Incompatible binning of histograms --> skipping !!";
	continue;
      }
      
      unsigned numBinsX = histCorrNum->GetXaxis()->GetNbins();
      float* binEdgesX = getBinning(histCorrNum->GetXaxis());
      unsigned numBinsY = histCorrNum->GetYaxis()->GetNbins();
      float* binEdgesY = getBinning(histCorrNum->GetYaxis());
      MonitorElement* meCorrected = dqmStore.book2D(corrHistName, corrHistName, numBinsX, binEdgesX, numBinsY, binEdgesY);
      TH1* histCorrected = meCorrected->getTH1();
      for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  std::pair<double, double> binContentErrCorrected = 
	    compCorrBinContentErr(histUncorr->GetBinContent(iBinX, iBinY), histUncorr->GetBinError(iBinX, iBinY), 
				  histCorrNum->GetBinContent(iBinX, iBinY), histCorrNum->GetBinError(iBinX, iBinY), 
				  histCorrDenom->GetBinContent(iBinX, iBinY), histCorrDenom->GetBinError(iBinX, iBinY));
	
	  histCorrected->SetBinContent(iBinX, iBinY, binContentErrCorrected.first);
	  histCorrected->SetBinError(iBinX, iBinY, binContentErrCorrected.second);
	}
      }
      delete[] binEdgesX;
      delete[] binEdgesY;
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistBiasCorrection);
