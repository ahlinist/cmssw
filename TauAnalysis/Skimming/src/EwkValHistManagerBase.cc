#include "TauAnalysis/Skimming/interface/EwkValHistManagerBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include <TMath.h>

#include <iostream>
#include <iomanip>

EwkValHistManagerBase::EwkValHistManagerBase(const edm::ParameterSet& cfg) 
  : dqmDirectory_(cfg.getParameter<std::string>("dqmDirectory")),
    numEventsAnalyzed_(0),
    numEventsSelected_(0),
    maxNumWarnings_(cfg.getParameter<int>("maxNumWarnings")),
    cfgError_(0)
{
//--- CV: no protection against case of DQMStore unavailable necessary here,
//        as protection is already implemented in EwkTauValidation class
//        creating instances of plugin classes derrived from EwkValHistManagerBase
  dqmStore_ = &(*edm::Service<DQMStore>());
}

void EwkValHistManagerBase::printFilterStatistics()
{
  edm::LogInfo ("EwkValHistManagerBase") 
    << "Filter-Statistics Summary:" << std::endl
    << " Events analyzed = " << numEventsAnalyzed_ << std::endl
    << " Events selected = " << numEventsSelected_;
  if ( numEventsAnalyzed_ > 0 ) {
    double eff = numEventsSelected_/(double)numEventsAnalyzed_;
    edm::LogInfo ("") 
      << "Overall efficiency = " << std::setprecision(4) << eff*100. 
      << " +/- " << std::setprecision(4) << TMath::Sqrt(eff*(1 - eff)/numEventsAnalyzed_)*100. << ")%";
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(EwkValHistManagerPluginFactory, "EwkValHistManagerPluginFactory");




