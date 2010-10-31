#include "TauAnalysis/BgEstimationTools/plugins/DQMHistCombiner.h"

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

DQMHistCombiner::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryPlot::cfgEntryPlot>:" << std::endl;

  meNamesInput_ = cfg.getParameter<vstring>("inputs");

  const edm::ParameterSet& cfgOutput = cfg.getParameter<edm::ParameterSet>("output");
  meNameOutputShape_ = cfgOutput.getParameter<std::string>("meNameShape");
  if ( cfgOutput.exists("meNameNorm") && cfgOutput.exists("meNameNormErr") ) {
    meNameOutputNorm_ = cfgOutput.getParameter<std::string>("meNameNorm");
    meNameOutputNormErr_ = cfgOutput.getParameter<std::string>("meNameNormErr");
  } else {
    meNameOutputNorm_ = "";
    meNameOutputNormErr_ = "";
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistCombiner::DQMHistCombiner(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistCombiner::DQMHistCombiner>:" << std::endl;
  
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	  cfgPlot != cfgPlots.end(); ++cfgPlot ) {
      cfgEntriesPlot_.push_back(cfgEntryPlot(*cfgPlot));
    }
  } else {
    cfgEntriesPlot_.push_back(cfgEntryPlot(cfg));
  }
}

DQMHistCombiner::~DQMHistCombiner() 
{
//--- nothing to be done yet
}

void DQMHistCombiner::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void getBinContentErr2(DQMStore& dqmStore, const std::string& meName_shape, unsigned iBinX, unsigned iBinY,
		       double& binContent, double& binError2)
{
  bool dqmError;
  const TH1* histogramShape = getHistogram(dqmStore, meName_shape, dqmError);

  if ( histogramShape ) {
    binContent = histogramShape->GetBinContent(iBinX, iBinY);
    double binError = histogramShape->GetBinError(iBinX, iBinY);
    binError2 = binError*binError;
  } else {
    edm::LogError("getBinContentErr2") 
      << " Failed to to compute binContent, binError2 for MonitorElement = " << meName_shape << " !!";
    binContent = -1.;
    binError2 = -1.;
  }
} 

void bookMonitorElement(DQMStore& dqmStore, const std::string& meName_full, double meValue)
{
  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_full,  meName, dqmDirectory);
  dqmStore.setCurrentFolder(dqmDirectory);

  MonitorElement* me = dqmStore.bookFloat(meName);
  me->Fill(meValue);
}

void DQMHistCombiner::endJob()
{
  std::cout << "<DQMHistCombiner::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> combined histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryPlot>::const_iterator plot = cfgEntriesPlot_.begin(); 
	plot != cfgEntriesPlot_.end(); ++plot ) {

    bool dqmError = false;
    std::vector<TH1*> inputHistograms = getHistograms(dqmStore, plot->meNamesInput_, dqmError);

    int numDimensions = -1;
    for ( std::vector<TH1*>::const_iterator inputHistogram = inputHistograms.begin();
	  inputHistogram != inputHistograms.end(); ++inputHistogram ) {
      int numDimensions_i = (*inputHistogram)->GetDimension();
      if ( numDimensions == -1 ) numDimensions = numDimensions_i;
      if ( numDimensions != -1 && numDimensions_i != numDimensions ) {
	edm::LogError("DQMHistCombiner::endJob") << " Mismatch in Histogram dimensionality !!";
	std::cout << "(found = " << numDimensions_i << ", expected = " << numDimensions << ")" << std::endl;
      }
    }

    if ( !(numDimensions == 1 || numDimensions == 2) ) {
      edm::LogError("DQMHistCombiner::endJob") << " Unsupported Histogram dimensionality = " << numDimensions << " !!";
      continue;
    }

    std::string outputShapeName, outputShapeDirectory;
    separateMonitorElementFromDirectoryName(plot->meNameOutputShape_, outputShapeName, outputShapeDirectory);
    dqmStore.setCurrentFolder(outputShapeDirectory);

    MonitorElement* meShapeOutput = 0;

    if ( numDimensions == 1 ) {
      unsigned numBins = inputHistograms.front()->GetXaxis()->GetNbins();
      float* binEdges = getBinning(inputHistograms.front()->GetXaxis());
      meShapeOutput = dqmStore.book1D(outputShapeName, outputShapeName, numBins, binEdges);
      delete[] binEdges;
    } else if ( numDimensions == 2 ) {
      unsigned numBinsX = inputHistograms.front()->GetXaxis()->GetNbins();
      float* binEdgesX = getBinning(inputHistograms.front()->GetXaxis());
      unsigned numBinsY = inputHistograms.front()->GetYaxis()->GetNbins();
      float* binEdgesY = getBinning(inputHistograms.front()->GetYaxis());
      meShapeOutput = dqmStore.book2D(outputShapeName, outputShapeName, numBinsX, binEdgesX, numBinsY, binEdgesY);
      delete[] binEdgesX;
      delete[] binEdgesY;
    } 

    if ( !meShapeOutput ) continue;
    
    TH1* outputHistogram = meShapeOutput->getTH1();

    unsigned numBinsX = outputHistogram->GetNbinsX();
    for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {

      unsigned numBinsY = outputHistogram->GetNbinsY();
      for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double outputHistogramBinContent = 0.;
	double outputHistogramBinInvErr2 = 0.;
	
	for ( vstring::const_iterator meNameInput = plot->meNamesInput_.begin();
	      meNameInput != plot->meNamesInput_.end(); ++meNameInput ) {
	  double binContent_i, binErr2_i;
	  getBinContentErr2(dqmStore, *meNameInput, iBinX, iBinY, binContent_i, binErr2_i);
	  
	  if ( binErr2_i > 0. ) {
	    outputHistogramBinContent += (1./binErr2_i)*binContent_i;
	    outputHistogramBinInvErr2 += (1./binErr2_i);
	  }
	}

	if ( outputHistogramBinInvErr2 > 0. ) {
	  outputHistogramBinContent /= outputHistogramBinInvErr2;
	  double outputHistogramBinErr2 = 1./outputHistogramBinInvErr2;

	  outputHistogram->SetBinContent(iBinX, iBinY, outputHistogramBinContent);
	  outputHistogram->SetBinError(iBinX, iBinY, TMath::Sqrt(outputHistogramBinErr2));
	}
      }
    }

    if ( plot->meNameOutputNorm_ != "" && plot->meNameOutputNormErr_ != "" ) {
      
      double outputHistogramIntegral = 0.;
      double outputHistogramIntegralErr2 = 0.;

      unsigned numBinsX = outputHistogram->GetNbinsX();
      for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	
	unsigned numBinsY = outputHistogram->GetNbinsY();
	for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {

	  outputHistogramIntegral += outputHistogram->GetBinContent(iBinX, iBinY);
	  double outputHistogramIntegralErr = outputHistogram->GetBinError(iBinX, iBinY);
	  outputHistogramIntegralErr2 += (outputHistogramIntegralErr*outputHistogramIntegralErr);
	}
      }

      bookMonitorElement(dqmStore, plot->meNameOutputNorm_, outputHistogramIntegral);
      bookMonitorElement(dqmStore, plot->meNameOutputNormErr_, TMath::Sqrt(outputHistogramIntegralErr2));

      //outputHistogram->Scale(1./outputHistogram->Integral());
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistCombiner);
