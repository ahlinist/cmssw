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
#include <TArrayF.h>

#include <iostream>

typedef std::vector<double> vdouble;

const double epsilonBinMatching = 1.e-2;

bool checkBinning(const TAxis* axis_original, unsigned numBins_rebinned, const vdouble& binEdges_rebinned)
{  
  bool isBinningCompatible = true;

  for ( unsigned iBin_rebinned = 0; iBin_rebinned <= numBins_rebinned; ++iBin_rebinned ) {

    bool bin_isMatched = false;
    
    int numBins_original = axis_original->GetNbins();
    for ( int iBin_original = 1; iBin_original <= numBins_original; ++iBin_original ) {
      double binEdgeLow = axis_original->GetBinLowEdge(iBin_original);
      double binEdgeUp = axis_original->GetBinUpEdge(iBin_original);

      if ( TMath::Abs(binEdgeLow - binEdges_rebinned[iBin_rebinned]) < epsilonBinMatching ) bin_isMatched = true;
      if ( TMath::Abs(binEdgeUp  - binEdges_rebinned[iBin_rebinned]) < epsilonBinMatching ) bin_isMatched = true;
    }

    if ( !bin_isMatched ) {
      edm::LogError("checkBinning") 
	<< " Failed to match Bin-edge at position = " << binEdges_rebinned[iBin_rebinned]
	<< " with Axis = " << axis_original->GetName() << " !!";
      isBinningCompatible = false;
    }
  }

  return isBinningCompatible;
}

TArrayF* getBinning(const TAxis* axis_original, unsigned combineBins, unsigned numBins, const vdouble& binEdges)
{
  TArrayF* binEdges_float = 0;

  if ( combineBins > 0 ) {
    int numBins_original = axis_original->GetNbins();

    if ( (numBins_original % combineBins) != 0 ) {
      edm::LogError("DQMHistEffProducer") 
	<< " Axis = " << axis_original->GetName() << " has " << numBins_original << " Bins,"
	<< " cannot combined " << combineBins << " into 1 !!";
      return binEdges_float;
    }

    int numBins_rebinned = (numBins_original / combineBins);
    binEdges_float = new TArrayF(numBins_rebinned + 1);

    //std::cout << " numBins_original = " << numBins_original << " --> numBins_rebinned = " << numBins_rebinned << std::endl;

    for ( int iBin = 0; iBin < numBins_rebinned; ++iBin ) {
      (*binEdges_float)[iBin] = axis_original->GetBinLowEdge(iBin*combineBins + 1);
      //std::cout << "binEdges_float[" << iBin << "] = " << binEdges_float->At(iBin) << std::endl;
    }
    
    (*binEdges_float)[numBins_rebinned] = axis_original->GetBinUpEdge(numBins_rebinned*combineBins);
    //std::cout << "binEdges_float[" << numBins_rebinned << "] = " << binEdges_float->At(numBins_rebinned) << std::endl;
  } else {
    binEdges_float = new TArrayF(numBins + 1);
    for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
      (*binEdges_float)[iBin] = binEdges[iBin];
    }
  }

  return binEdges_float;
}

TH1F* getRebinnedHistogram1d(const TH1* histoOriginal, 
			     unsigned combineBins, unsigned numBins_rebinned, const vdouble& binEdges_rebinned)
{
  std::string histoRebinnedName = std::string(histoOriginal->GetName()).append("rebinned");
  TArrayF* binEdges_float = getBinning(histoOriginal->GetXaxis(), combineBins, numBins_rebinned, binEdges_rebinned);
  TH1F* histoRebinned = new TH1F(histoRebinnedName.data(), histoOriginal->GetTitle(), 
				 binEdges_float->GetSize() - 1, binEdges_float->GetArray());
  histoRebinned->Sumw2();
  delete binEdges_float;

  TAxis* axis_original = histoOriginal->GetXaxis();

  int numBins_original = axis_original->GetNbins();
  for ( int iBin = 1; iBin <= numBins_original; ++iBin ) {
    double binContent_original = histoOriginal->GetBinContent(iBin);
    double binError_original = histoOriginal->GetBinError(iBin);

    double binCenter_original = axis_original->GetBinCenter(iBin);

    int binIndex_rebinned = histoRebinned->FindBin(binCenter_original);

    double binContent_rebinned = histoRebinned->GetBinContent(binIndex_rebinned);
    binContent_rebinned += binContent_original;
    histoRebinned->SetBinContent(binIndex_rebinned, binContent_rebinned);
    
    double binError_rebinned = histoRebinned->GetBinError(binIndex_rebinned);
    binError_rebinned = TMath::Sqrt(binError_rebinned*binError_rebinned + binError_original*binError_original);
    histoRebinned->SetBinError(binIndex_rebinned, binError_rebinned);
  }

  return histoRebinned;
}

TH2F* getRebinnedHistogram2d(const TH1* histoOriginal, 
			     unsigned combineBinsX, unsigned numBinsX_rebinned, const vdouble& binEdgesX_rebinned,
			     unsigned combineBinsY, unsigned numBinsY_rebinned, const vdouble& binEdgesY_rebinned)
{
  std::string histoRebinnedName = std::string(histoOriginal->GetName()).append("rebinned");
  TArrayF* binEdgesX_float = getBinning(histoOriginal->GetXaxis(), combineBinsX, numBinsX_rebinned, binEdgesX_rebinned);
  TArrayF* binEdgesY_float = getBinning(histoOriginal->GetYaxis(), combineBinsY, numBinsY_rebinned, binEdgesY_rebinned);
  TH2F* histoRebinned = new TH2F(histoRebinnedName.data(), histoOriginal->GetTitle(), 
				 binEdgesX_float->GetSize() - 1, binEdgesX_float->GetArray(), 
				 binEdgesY_float->GetSize() - 1, binEdgesY_float->GetArray());
  histoRebinned->Sumw2();
  delete binEdgesX_float;
  delete binEdgesY_float;

  TAxis* xAxis_original = histoOriginal->GetXaxis();
  TAxis* yAxis_original = histoOriginal->GetYaxis();

  int numBinsX_original = xAxis_original->GetNbins();
  for ( int iBinX = 1; iBinX <= numBinsX_original; ++iBinX ) {

    int numBinsY_original = yAxis_original->GetNbins();
    for ( int iBinY = 1; iBinY <= numBinsY_original; ++iBinY ) {
      double binContent_original = histoOriginal->GetBinContent(iBinX, iBinY);
      double binError_original = histoOriginal->GetBinError(iBinX, iBinY);
      
      double binCenterX_original = xAxis_original->GetBinCenter(iBinX);
      double binCenterY_original = yAxis_original->GetBinCenter(iBinY);

      int binIndex_rebinned = histoRebinned->FindBin( binCenterX_original, binCenterY_original);

      double binContent_rebinned = histoRebinned->GetBinContent(binIndex_rebinned);
      binContent_rebinned += binContent_original;
      histoRebinned->SetBinContent(binIndex_rebinned, binContent_rebinned);
    
      double binError_rebinned = histoRebinned->GetBinError(binIndex_rebinned);
      binError_rebinned = TMath::Sqrt(binError_rebinned*binError_rebinned + binError_original*binError_original);
      histoRebinned->SetBinError(binIndex_rebinned, binError_rebinned);
    }
  }

  return histoRebinned;
}
   
//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistRebinner::axisEntryType::axisEntryType(const edm::ParameterSet& cfg)
  : combineBins_(0),
    numBins_(0)    
{
  if ( cfg.exists("combineBins") ) {
    combineBins_ = cfg.getParameter<unsigned>("combineBins");
  } else {    
    if ( cfg.exists("binEdges") ) {
      binEdges_ = cfg.getParameter<vdouble>("binEdges");
      if ( !(binEdges_.size() >= 1) ) {
	edm::LogError("axisEntryType") 
	  << " Configuration parameter 'binEdges' must not be empty list !!";
	assert(0);
      }
    } else {
      numBins_ = cfg.getParameter<unsigned>("numBins");
      binEdges_.resize(numBins_ + 1);

      double min = cfg.getParameter<double>("min");
      double max = cfg.getParameter<double>("max");
      
      double binWidth = (max - min)/numBins_;
    
      for ( unsigned iBin = 0; iBin <= numBins_; ++iBin ) {
	binEdges_[iBin] = (min + iBin*binWidth);
      }
    }
  }

  //print();
}

void DQMHistRebinner::axisEntryType::print() const
{
  std::cout << "<axisEntryType::print>:" << std::endl;
  std::cout << " combineBins = " << combineBins_ << std::endl;
  std::cout << " numBins = " << numBins_ << std::endl;
  std::cout << " binEdges = " << format_vdouble(binEdges_) << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistRebinner::plotEntryType::plotEntryType(const edm::ParameterSet& cfg)
{
  meName_original_ = cfg.getParameter<std::string>("meName_original");
  meName_rebinned_ = cfg.getParameter<std::string>("meName_rebinned");

  //print();
}

void DQMHistRebinner::plotEntryType::print() const
{
  std::cout << "<plotEntryType::print>:" << std::endl;
  std::cout << " meName_original = " << meName_original_ << std::endl;
  std::cout << " meName_rebinned = " << meName_rebinned_ << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistRebinner::DQMHistRebinner(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistRebinner::DQMHistRebinner>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	cfgPlot != cfgPlots.end(); ++cfgPlot ) {
    plotEntries_.push_back(plotEntryType(*cfgPlot));
  }

  edm::ParameterSet cfgBinning = cfg.getParameter<edm::ParameterSet>("binning");
  std::vector<std::string> axisLabels;
  axisLabels.push_back("x");
  axisLabels.push_back("y");
  unsigned numAxisLabels = axisLabels.size();
  for ( unsigned iAxis = 0; iAxis < numAxisLabels; ++iAxis ) {
    const char* axisLabel = axisLabels[iAxis].data();
    if ( cfgBinning.exists(axisLabel) ) 
      axisEntries_.push_back(axisEntryType(cfgBinning.getParameter<edm::ParameterSet>(axisLabel)));
    else
      break;
  }

  if ( axisEntries_.size() == 0 ) axisEntries_.push_back(axisEntryType(cfgBinning));
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

  for ( std::vector<plotEntryType>::const_iterator plot = plotEntries_.begin(); 
	plot != plotEntries_.end(); ++plot ) {
    std::string originalHistogramName, originalHistogramDirectory;
    separateMonitorElementFromDirectoryName(plot->meName_original_, originalHistogramName, originalHistogramDirectory);
    MonitorElement* meOriginal = dqmStore.get(std::string(originalHistogramDirectory).append(dqmSeparator).append(originalHistogramName));
    TH1* histoOriginal = ( meOriginal != NULL ) ? meOriginal->getTH1() : NULL;

    if ( histoOriginal != NULL ) {
      if ( !histoOriginal->GetSumw2N() ) histoOriginal->Sumw2();

      if ( histoOriginal->GetDimension() != (int)axisEntries_.size() ) {
	edm::LogError ("endJob") 
	  << " Mismatch between dimensionality of Histogram = " << histoOriginal->GetDimension() 
	  << " and number of Axis definitions = " << axisEntries_.size() << " --> skipping !!";
	continue;
      }

      bool isCompatibleBinning = true;
      for ( int iDimension = 0; iDimension < histoOriginal->GetDimension(); ++iDimension ) {
	const TAxis* axis = 0;
	switch ( iDimension ) {
	  case 0: 
	    axis = histoOriginal->GetXaxis();
	    break;
	  case 1: 
	    axis = histoOriginal->GetYaxis();
	    break; 
	}

	if ( !axis ) {
	  edm::LogError("endJob") 
	    << "Failed to access Histogram axis for dimension = " << iDimension << "--> skipping !!";
	  continue;
	}

        const axisEntryType& axisEntry = axisEntries_[iDimension];

	if ( axisEntry.combineBins_ > 0 ) {
	  isCompatibleBinning &= ((axis->GetNbins() % axisEntry.combineBins_) == 0);
	} else {
	  isCompatibleBinning &= checkBinning(axis, axisEntry.numBins_, axisEntry.binEdges_);
	}
      }

      if ( isCompatibleBinning ) {
	std::string rebinnedHistogramName, rebinnedHistogramDirectory, dummy;
	separateMonitorElementFromDirectoryName(plot->meName_rebinned_, rebinnedHistogramName, rebinnedHistogramDirectory);
	if ( rebinnedHistogramDirectory == "" ) 
	  separateMonitorElementFromDirectoryName(originalHistogramName, dummy, rebinnedHistogramDirectory);
	std::string rebinnedHistogramName_full = dqmDirectoryName(rebinnedHistogramDirectory).append(rebinnedHistogramName);
	
	if ( histoOriginal->GetDimension() == 1 ) {
	  const axisEntryType& axisEntry = axisEntries_[0];
	  std::auto_ptr<TH1> histoRebinned = 
	    std::auto_ptr<TH1>(getRebinnedHistogram1d(histoOriginal, 
						      axisEntry.combineBins_, axisEntry.numBins_, axisEntry.binEdges_));
	  dqmRegisterHistogram(dqmStore, histoRebinned.release(), rebinnedHistogramName_full);
	} else if ( histoOriginal->GetDimension() == 2 ) {
	  const axisEntryType& axisEntryX = axisEntries_[0];
	  const axisEntryType& axisEntryY = axisEntries_[1];
	  std::auto_ptr<TH1> histoRebinned =
	    std::auto_ptr<TH1>(getRebinnedHistogram2d(histoOriginal, 
						      axisEntryX.combineBins_, axisEntryX.numBins_, axisEntryX.binEdges_,
						      axisEntryY.combineBins_, axisEntryY.numBins_, axisEntryY.binEdges_));
          dqmRegisterHistogram(dqmStore, histoRebinned.release(), rebinnedHistogramName_full);		       
	}
      } else {
	edm::LogError("DQMHistEffProducer") 
	  << " Histogram = " << histoOriginal->GetName() << " has Binning not compatible"
	  << " with Axis definitions --> skipping !!";
      }
    } else {
      edm::LogError("endJob") 
	<< " Failed to produce rebinned Histogram = " << plot->meName_rebinned_ << ";"
	<< " original Histogram = " << plot->meName_original_ << " does not exist !!";
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistRebinner);
