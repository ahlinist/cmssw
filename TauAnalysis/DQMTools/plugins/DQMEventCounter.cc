#include "TauAnalysis/DQMTools/plugins/DQMEventCounter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>

DQMEventCounter::DQMEventCounter(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    me_(0)
{
  //std::cout << "<DQMEventCounter::DQMEventCounter>:" << std::endl;

  dqmDirectory_ = cfg.exists("dqmDirectory") ?
    cfg.getParameter<std::string>("dqmDirectory") : "";

  meName_ = cfg.getParameter<std::string>("meName");
  
  //std::cout << "done." << std::endl;
}

DQMEventCounter::~DQMEventCounter() 
{
// nothing to be done yet...
}

void DQMEventCounter::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("beginJob") << " Failed to access dqmStore --> counter will NOT be booked !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  dqmStore.setCurrentFolder(dqmDirectory_);

  me_ = dqmStore.bookInt(meName_);
}

void DQMEventCounter::analyze(const edm::Event&, const edm::EventSetup&)
{
  me_->Fill(1);
}

void DQMEventCounter::endJob()
{
  std::cout << "<DQMEventCounter::endJob>:" << std::endl;
  std::cout << " moduleLabel = " << moduleLabel_ << ": number of processed Events = " <<  me_->getIntValue() << std::endl;
  std::cout << "done." << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMEventCounter);
