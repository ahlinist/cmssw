#include "TauAnalysis/DQMTools/plugins/DQMDumpHistogram.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <iostream>

DQMDumpHistogram::DQMDumpHistogram(const edm::ParameterSet& cfg)
  : cfgError_(0)
{
  //std::cout << "<DQMDumpHistogram::DQMDumpHistogram>:" << std::endl;

  meNames_ = cfg.getParameter<vstring>("meNames");
  if ( meNames_.size() == 0 ) {
    edm::LogError("DQMDumpHistogram") 
      << " Configuration Parameter meNames = " << format_vstring(meNames_)
      << " contains no Entries --> skipping !!";
    cfgError_ = 1;
  }
}

DQMDumpHistogram::~DQMDumpHistogram() 
{
//--- nothing to be done yet
}

void DQMDumpHistogram::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMDumpHistogram::endJob()
{
  std::cout << "<DQMDumpHistogram::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob") << " Error in Configuration ParameterSet --> histogram bin-contents will NOT be printed-out !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histogram bin-contents will NOT be printed-out !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( vstring::const_iterator meName = meNames_.begin();
	meName != meNames_.end(); ++meName ) {
    std::cout << " meName = " << (*meName) << std::endl;

    MonitorElement* me = dqmStore.get(*meName);
    if ( !me ) {
      edm::LogError ("endjob") 
	<< " Failed to access meName = " << (*meName) << " in DQMStore --> skipping !!";
      continue;
    }
    
    if ( !(me->kind() == MonitorElement::DQM_KIND_TH1F      ||
	   me->kind() == MonitorElement::DQM_KIND_TH1S      ||
	   me->kind() == MonitorElement::DQM_KIND_TH2F      ||
	   me->kind() == MonitorElement::DQM_KIND_TH2S      ||
	   me->kind() == MonitorElement::DQM_KIND_TH3F      ||
	   me->kind() == MonitorElement::DQM_KIND_TPROFILE  ||
	   me->kind() == MonitorElement::DQM_KIND_TPROFILE2D) ) {
      edm::LogError ("endjob") 
	<< " MonitorElement meName = " << (*meName) << " is NOT a histogramm --> skipping !!";
      continue;
    }
    
    TH1* histogram = me->getTH1();
    if ( !histogram ) {
      edm::LogError ("endjob") 
	<< " Failed to access histogram associated to meName = " << (*meName) << " in DQMStore --> skipping !!";
      continue;
    }
    
    int numBinsX = histogram->GetNbinsX();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double binCenterX = histogram->GetXaxis()->GetBinCenter(iBinX);
      
      int numBinsY = histogram->GetNbinsY();
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binCenterY = histogram->GetYaxis()->GetBinCenter(iBinY);
	
	int numBinsZ = histogram->GetNbinsZ();
	for ( int iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	  double binCenterZ = histogram->GetZaxis()->GetBinCenter(iBinZ);
	  
	  int binNumber = histogram->FindBin(binCenterX, binCenterY, binCenterZ);
	  double binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	  double binError = histogram->GetBinError(iBinX, iBinY, iBinZ);
	  
	  std::cout << " bin " << std::setw(2) << binNumber << " (center:";
	  std::cout << " X = " << binCenterX;
	  if ( histogram->GetDimension() >= 2 ) std::cout << ", Y = " << binCenterY;
	  if ( histogram->GetDimension() >= 3 ) std::cout << ", Z = " << binCenterZ;
	  std::cout << "):";
	  std::cout << std::setprecision(3) << std::fixed << binContent << " +/- " << binError << std::endl;
	}
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpHistogram);
