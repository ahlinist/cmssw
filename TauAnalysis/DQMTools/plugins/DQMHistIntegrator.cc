#include "TauAnalysis/DQMTools/plugins/DQMHistIntegrator.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

DQMHistIntegrator::cfgEntryPlot::cfgEntryPlot(const edm::ParameterSet& cfg)
  : me_input_(0),
    me_output_(0),
    cfgError_(0)
{
  meName_input_ = cfg.getParameter<std::string>("meName_input");
  meName_output_ = cfg.getParameter<std::string>("meName_output");

  std::string mode_string = cfg.getParameter<std::string>("integrateFrom");
  if      ( mode_string == "left"  ) mode_ = kIntegrateFromLeft;
  else if ( mode_string == "right" ) mode_ = kIntegrateFromRight;
  else {
    edm::LogError("cfgEntryPlot") << " Invalid 'integrateFrom'Parameter = " << mode_string << "; needs to be either 'left' or 'right' !!";
    cfgError_ = 1;
  }

  //print();
}

void DQMHistIntegrator::cfgEntryPlot::print() const
{
  std::cout << "<cfgEntryPlot::print>:" << std::endl;
  std::cout << " meName_input = " << meName_input_ << std::endl;
  std::cout << " meName_output = " << meName_output_ << std::endl;
  std::cout << " mode = " << mode_ << std::endl;
}

void DQMHistIntegrator::cfgEntryPlot::integrate(DQMStore& dqmStore) 
{
  bool dqmError = false;
  me_input_ = getMonitorElement(dqmStore, meName_input_, dqmError);
   
  if ( dqmError ) {
    edm::LogError ("integrate") << " Failed to access histogram !!";
    cfgError_ = 1;
    return;
  }

  int meType = me_input_->kind();
  if ( !(meType == MonitorElement::DQM_KIND_TH1F ||
	 meType == MonitorElement::DQM_KIND_TH1S) ) {
     edm::LogError ("integrate") << " MonitorElement = " << meName_input_ << " is not a histogram of Type 1d !!";
     cfgError_ = 1;
     return;
  }

  TH1* histogram = me_input_->getTH1();

  std::auto_ptr<TH1> clone(dynamic_cast<TH1*>(histogram->Clone()));
  if ( !clone->GetSumw2N() ) clone->Sumw2();

  int firstBin, lastBin, incBin;
  if ( mode_ == kIntegrateFromLeft ) {
    firstBin = 1;
    lastBin = histogram->GetNbinsX();
    incBin = +1;
  } else if ( mode_ == kIntegrateFromRight ) {
    firstBin = histogram->GetNbinsX();
    lastBin = 1;
    incBin = -1;
  } else assert(0);
  int numBins = TMath::Abs(lastBin - firstBin) + 1;
 
  double binContentSum = 0.;
  double binError2sum = 0.;
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    int binIndex = firstBin + (iBin - 1)*incBin; 

    double binContent = histogram->GetBinContent(binIndex);
    double binError = histogram->GetBinError(binIndex);

    binContentSum += binContent;
    binError2sum += (binError*binError);

    clone->SetBinContent(binIndex, binContentSum);
    clone->SetBinError(binIndex, TMath::Sqrt(binError2sum));
  }

  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_output_, meName, dqmDirectory);
  dqmStore.setCurrentFolder(dqmDirectory);
  std::string meName_full = dqmDirectoryName(dqmDirectory).append(meName);
  dqmRegisterHistogram(dqmStore, clone.release(), meName_full);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistIntegrator::DQMHistIntegrator(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistIntegrator::DQMHistIntegrator>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgPlots = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgPlot = cfgPlots.begin();
	cfgPlot != cfgPlots.end(); ++cfgPlot ) {
    cfgEntryPlots_.push_back(cfgEntryPlot(*cfgPlot));
  }
}

DQMHistIntegrator::~DQMHistIntegrator() 
{
//--- nothing to be done yet
}

void DQMHistIntegrator::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMHistIntegrator::endJob()
{
  //std::cout << "<DQMHistIntegrator::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> efficiency histograms will NOT be produced !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryPlot>::iterator plot = cfgEntryPlots_.begin(); 
        plot != cfgEntryPlots_.end(); ++plot ) {
    if ( !plot->cfgError_ ) plot->integrate(dqmStore);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistIntegrator);
