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

DQMHistCombiner::cfgEntryHist::cfgEntryHist(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryHist::cfgEntryHist>:" << std::endl;
  
  meNameInputShape_ = cfg.getParameter<std::string>("meNameShape");

  if ( cfg.exists("meNameNorm") && cfg.exists("meNameNormErr") ) {
    meNameInputNorm_ = cfg.getParameter<std::string>("meNameNorm");
    meNameInputNormErr_ = cfg.getParameter<std::string>("meNameNormErr");
  } else {
    meNameInputNorm_ = meNameInputShape_;
    meNameInputNormErr_ = meNameInputShape_;
  }
}

DQMHistCombiner::cfgEntryCombJob::cfgEntryCombJob(const edm::ParameterSet& cfg)
{
  //std::cout << "<cfgEntryCombJob::cfgEntryCombJob>:" << std::endl;

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgInputs = cfg.getParameter<vParameterSet>("inputs");
  for ( vParameterSet::const_iterator cfgInput = cfgInputs.begin();
	cfgInput != cfgInputs.end(); ++cfgInput ) {
    cfgEntriesHist_.push_back(cfgEntryHist(*cfgInput));
  }

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
    vParameterSet cfgCombJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgCombJob = cfgCombJobs.begin();
	  cfgCombJob != cfgCombJobs.end(); ++cfgCombJob ) {
      cfgEntriesCombJob_.push_back(cfgEntryCombJob(*cfgCombJob));
    }
  } else {
    cfgEntriesCombJob_.push_back(cfgEntryCombJob(cfg));
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

void getBinContentErr2(DQMStore& dqmStore, const std::string& meName_shape, unsigned iBinX, unsigned iBinY,
		       const std::string& meName_norm, const std::string& meName_normErr, double& binContent, double& binError2)
{
  bool error = false;
  const TH1* histogramShape = getHistogram(dqmStore, meName_shape, error);

  if ( histogramShape ) {
    double binContent_shape = histogramShape->GetBinContent(iBinX, iBinY);
    double binError_shape = histogramShape->GetBinError(iBinX, iBinY);
    
    if ( meName_norm != "" && meName_normErr != "" ) {
      double norm = getValue(dqmStore, meName_norm, error);
      double normErr = getValue(dqmStore, meName_normErr, error);
   
      binContent = binContent_shape*norm;
      binError2 = (binError_shape*norm)*(binError_shape*norm) + (binContent_shape*normErr)*(binContent_shape*normErr);
    } else {
      binContent = binContent_shape;
      binError2 = binError_shape*binError_shape;
    }
  }

  if ( error ) {
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

  for ( std::vector<cfgEntryCombJob>::const_iterator plot = cfgEntriesCombJob_.begin(); 
	plot != cfgEntriesCombJob_.end(); ++plot ) {

    std::vector<std::string> meNamesInputShape;
    for ( std::vector<cfgEntryHist>::const_iterator cfgEntryHist = plot->cfgEntriesHist_.begin();
	  cfgEntryHist != plot->cfgEntriesHist_.end(); ++cfgEntryHist ) {
      meNamesInputShape.push_back(cfgEntryHist->meNameInputShape_);
    }

    bool dqmError = false;
    std::vector<TH1*> inputShapes = getHistograms(dqmStore, meNamesInputShape, dqmError);

    int numDimensions = -1;
    for ( std::vector<TH1*>::const_iterator inputShape = inputShapes.begin();
	  inputShape != inputShapes.end(); ++inputShape ) {
      int numDimensions_i = (*inputShape)->GetDimension();
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
      unsigned numBins = inputShapes.front()->GetXaxis()->GetNbins();
      float* binEdges = getBinning(inputShapes.front()->GetXaxis());
      meShapeOutput = dqmStore.book1D(outputShapeName, outputShapeName, numBins, binEdges);
      delete[] binEdges;
    } else if ( numDimensions == 2 ) {
      unsigned numBinsX = inputShapes.front()->GetXaxis()->GetNbins();
      float* binEdgesX = getBinning(inputShapes.front()->GetXaxis());
      unsigned numBinsY = inputShapes.front()->GetYaxis()->GetNbins();
      float* binEdgesY = getBinning(inputShapes.front()->GetYaxis());
      meShapeOutput = dqmStore.book2D(outputShapeName, outputShapeName, numBinsX, binEdgesX, numBinsY, binEdgesY);
      delete[] binEdgesX;
      delete[] binEdgesY;
    } 

    if ( !meShapeOutput ) continue;
    
    TH1* outputShape = meShapeOutput->getTH1();

    unsigned numBinsX = outputShape->GetNbinsX();
    for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {

      unsigned numBinsY = outputShape->GetNbinsY();
      for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double outputShapeBinContent = 0.;
	double outputShapeBinInvErr2 = 0.;
	
	for ( std::vector<cfgEntryHist>::const_iterator cfgEntryHist = plot->cfgEntriesHist_.begin();
	      cfgEntryHist != plot->cfgEntriesHist_.end(); ++cfgEntryHist ) {
	  double binContent_i, binErr2_i;
	  getBinContentErr2(dqmStore, cfgEntryHist->meNameInputShape_, iBinX, iBinY,
			    cfgEntryHist->meNameInputNorm_, cfgEntryHist->meNameInputNormErr_, binContent_i, binErr2_i);
	  
	  if ( binErr2_i > 0. ) {
	    outputShapeBinContent += (1./binErr2_i)*binContent_i;
	    outputShapeBinInvErr2 += (1./binErr2_i);
	  }
	}

	if ( outputShapeBinInvErr2 > 0. ) {
	  outputShapeBinContent /= outputShapeBinInvErr2;
	  double outputShapeBinErr2 = 1./outputShapeBinInvErr2;

	  outputShape->SetBinContent(iBinX, iBinY, outputShapeBinContent);
	  outputShape->SetBinError(iBinX, iBinY, TMath::Sqrt(outputShapeBinErr2));
	}
      }
    }

    if ( plot->meNameOutputNorm_ != "" && plot->meNameOutputNormErr_ != "" ) {
      
      double outputShapeIntegral = 0.;
      double outputShapeIntegralErr2 = 0.;

      unsigned numBinsX = outputShape->GetNbinsX();
      for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	
	unsigned numBinsY = outputShape->GetNbinsY();
	for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {

	  outputShapeIntegral += outputShape->GetBinContent(iBinX, iBinY);
	  double outputShapeIntegralErr = outputShape->GetBinError(iBinX, iBinY);
	  outputShapeIntegralErr2 += (outputShapeIntegralErr*outputShapeIntegralErr);
	}
      }

      bookMonitorElement(dqmStore, plot->meNameOutputNorm_, outputShapeIntegral);
      bookMonitorElement(dqmStore, plot->meNameOutputNormErr_, TMath::Sqrt(outputShapeIntegralErr2));

      outputShape->Scale(1./outputShape->Integral());
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistCombiner);
