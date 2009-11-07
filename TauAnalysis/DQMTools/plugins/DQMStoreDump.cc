#include "TauAnalysis/DQMTools/plugins/DQMStoreDump.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

DQMStoreDump::DQMStoreDump(const edm::ParameterSet& cfg)
{
// nothing to be done yet...
}

DQMStoreDump::~DQMStoreDump()
{
// nothing to be done yet...
}

void DQMStoreDump::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMStoreDump::endJob()
{
  std::cout << "<DQMStoreDump::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMStoreDump);
