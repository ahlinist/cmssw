#include "TauAnalysis/DQMTools/plugins/DQMHistRebinner.h"

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

typedef std::vector<double> vdouble;

const double epsilonBinMatching = 1.e-2;

void getBinning(const TH1* histoOriginal, unsigned combineBinsX, unsigned numBinsX, vdouble& xBins, double xMin, double xMax)
{
  if ( combineBinsX != 0 ) {
    int numBins_original = histoOriginal->GetNbinsX();

    if ( (numBins_original % combineBinsX) != 0 ) {
      edm::LogError("DQMHistEffProducer") << " Histogram = " << histoOriginal->GetName() << " has " << numBins_original << " Bins,"
					  << " cannot combined " << combineBinsX << " into 1 !!";
      return;
    }

    const TAxis* xAxis = histoOriginal->GetXaxis();

    int numBins_rebinned = (numBins_original / combineBinsX);

    for ( int iBin = 0; iBin < numBins_rebinned; ++iBin ) {
      xBins[iBin] = xAxis->GetBinLowEdge(iBin*combineBinsX + 1);
    }
    
    xBins[numBins_rebinned] = xAxis->GetBinUpEdge(numBins_rebinned*combineBinsX);
  } else if ( xBins.size() == 0 ) {
    xBins.resize(numBinsX + 1);
    
    double binWidthX = (xMax - xMin)/numBinsX;
    
    for ( unsigned iBin = 0; iBin < numBinsX; ++iBin ) {
      xBins[iBin] = (xMin + iBin*binWidthX);
    }
    
    xBins[numBinsX] = xMax;
  }
}

bool checkBinning(const TH1* histoOriginal, unsigned numBins_rebinned, const vdouble& xBins_rebinned)
{
  bool isBinningCompatible = true;

  const TAxis* xAxis_original = histoOriginal->GetXaxis();

  for ( unsigned iBin_rebinned = 0; iBin_rebinned <= numBins_rebinned; ++iBin_rebinned ) {

    bool bin_isMatched = false;
    
    int numBins_original = histoOriginal->GetNbinsX();
    for ( int iBin_original = 1; iBin_original <= numBins_original; ++iBin_original ) {
      double binEdgeLow = xAxis_original->GetBinLowEdge(iBin_original);
      double binEdgeUp = xAxis_original->GetBinUpEdge(iBin_original);

      if ( TMath::Abs(binEdgeLow - xBins_rebinned[iBin_rebinned]) < epsilonBinMatching ) bin_isMatched = true;
      if ( TMath::Abs(binEdgeUp  - xBins_rebinned[iBin_rebinned]) < epsilonBinMatching ) bin_isMatched = true;
    }

    if ( !bin_isMatched ) {
      edm::LogError("DQMHistEffProducer") << " Failed to match Bin-edge at x = " << xBins_rebinned[iBin_rebinned]
					  << " with Histogram = " << histoOriginal->GetName() << " !!";
      isBinningCompatible = false;
    }
  }

  return isBinningCompatible;
}

TH1F* getRebinnedHistogram(const TH1* histoOriginal, unsigned numBins_rebinned, const vdouble& xBins_rebinned)
{
  std::string histoRebinnedName = std::string(histoOriginal->GetName()).append("rebinned");
  float* xBins_float = new float[numBins_rebinned];
  for ( unsigned iBin = 0; iBin <= numBins_rebinned; ++iBin ) {
    xBins_float[iBin] = xBins_rebinned[iBin];
  }
  TH1F* histoRebinned = new TH1F(histoRebinnedName.data(), histoOriginal->GetTitle(), numBins_rebinned, xBins_float);
  histoRebinned->Sumw2();
  delete xBins_float;

  double binContentSum = 0.;
  double binError2Sum = 0.;

  int lastBinIndex_rebinned = -1;

  int numBins_original = histoOriginal->GetNbinsX();
  for ( int iBin = 1; iBin <= numBins_original; ++iBin ) {
    double binCenter_original = histoOriginal->GetBinCenter(iBin);

    int binIndex_rebinned = histoRebinned->FindBin(binCenter_original);

    if ( binIndex_rebinned != lastBinIndex_rebinned ) {
      if ( lastBinIndex_rebinned != -1 ) {
	histoRebinned->SetBinContent(lastBinIndex_rebinned, binContentSum);
	histoRebinned->SetBinError(lastBinIndex_rebinned, TMath::Sqrt(binError2Sum));
      }

      binContentSum = 0.;
      binError2Sum = 0.;
    }

    binContentSum += histoOriginal->GetBinContent(iBin);
    double binError = histoOriginal->GetBinError(iBin);
    binError2Sum += (binError*binError);

    lastBinIndex_rebinned = binIndex_rebinned;
  }

  histoRebinned->SetBinContent(numBins_rebinned, binContentSum);
  histoRebinned->SetBinError(numBins_rebinned, TMath::Sqrt(binError2Sum));

  return histoRebinned;
}

DQMHistRebinner::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;

  meName_original_ = cfg.getParameter<std::string>("meName_original");
  //std::cout << " meName_original = " << meName_original_ << std::endl;

  meName_rebinned_ = cfg.getParameter<std::string>("meName_rebinned");
  //std::cout << " meName_rebinned = " << meName_rebinned_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistRebinner::DQMHistRebinner(const edm::ParameterSet& cfg)
  :  combineBinsX_(0), numBinsX_(0), xMin_(0.), xMax_(0.)
{
  //std::cout << "<DQMHistRebinner::DQMHistRebinner>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	cfgPlot != cfgPlots.end(); ++cfgPlot ) {
    cfgEntryPlot_.push_back(cfgEntryPlot(*cfgPlot));
  }

  if ( cfg.exists("combineBinsX") ) {
    combineBinsX_ = cfg.getParameter<unsigned>("combineBinsX");
  } else {
    numBinsX_ = cfg.getParameter<unsigned>("numBinsX");
    if ( cfg.exists("xBins") ) {
      xBins_ = cfg.getParameter<vdouble>("xBins");
    } else {
      xMin_ = cfg.getParameter<double>("xMin");
      xMax_ = cfg.getParameter<double>("xMax");
    }
  }

  //std::cout << " combineBinsX = " << combineBinsX_ << std::endl;
  //std::cout << " numBinsX = " << numBinsX_ << std::endl;
  //std::cout << " xBins = " << format_vdouble(xBins_) << std::endl;
  //std::cout << " xMin = " << xMin_ << std::endl;
  //std::cout << " xMax = " << xMax_ << std::endl;
}

DQMHistRebinner::~DQMHistRebinner() 
{
//--- nothing to be done yet
}

void DQMHistRebinner::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMHistRebinner::endJob()
{
  //std::cout << "<DQMHistRebinner::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> rebinned histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryPlot>::const_iterator plot = cfgEntryPlot_.begin(); 
	plot != cfgEntryPlot_.end(); ++plot ) {
    std::string originalHistogramName, originalHistogramDirectory;
    separateMonitorElementFromDirectoryName(plot->meName_original_, originalHistogramName, originalHistogramDirectory);
    MonitorElement* meOriginal = dqmStore.get(std::string(originalHistogramDirectory).append(dqmSeparator).append(originalHistogramName));
    TH1* histoOriginal = ( meOriginal != NULL ) ? meOriginal->getTH1() : NULL;
    
    if ( histoOriginal != NULL ) {
      if ( !histoOriginal->GetSumw2N() ) histoOriginal->Sumw2();

      getBinning(histoOriginal, combineBinsX_, numBinsX_, xBins_, xMin_, xMax_);
      bool isCompatibleBinning = checkBinning(histoOriginal, numBinsX_, xBins_);

      if ( isCompatibleBinning ) {
	TH1F* histoRebinned = getRebinnedHistogram(histoOriginal, numBinsX_, xBins_);

	std::string rebinnedHistogramName, rebinnedHistogramDirectory, dummy;
	separateMonitorElementFromDirectoryName(plot->meName_rebinned_, rebinnedHistogramName, rebinnedHistogramDirectory);
	if ( rebinnedHistogramDirectory == "" ) separateMonitorElementFromDirectoryName(originalHistogramName, dummy, rebinnedHistogramDirectory);
	if ( rebinnedHistogramDirectory != "" ) dqmStore.setCurrentFolder(rebinnedHistogramDirectory);
      
	dqmStore.book1D(rebinnedHistogramName, histoRebinned);
      } else {
	edm::LogError("DQMHistEffProducer") << " Histogram = " << histoOriginal->GetName() << " has Binning not compatible with"
					    << " numBinsX = " << numBinsX_ << ", xBins = " << format_vdouble(xBins_)
					    << " --> skipping !!";
      }
    } else {
      edm::LogError("endJob") << " Failed to produce rebinned Histogram = " << plot->meName_rebinned_ << " !!";
      edm::LogError("endJob") << " original Histogram = " << plot->meName_original_ << " does not exist.";
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistRebinner);
