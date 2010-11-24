#include "TauAnalysis/FittingTools/plugins/DQMEffXsecCalculator.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>
#include <iomanip>

DQMEffXsecCalculator::DQMEffXsecCalculator(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  //std::cout << "<DQMEffXsecCalculator::DQMEffXsecCalculator>:" << std::endl;

  dataIntLumi_ = cfg.getParameter<double>("dataIntLumi");

  if ( cfg.exists("channels") ) {
    edm::ParameterSet cfgChannels = cfg.getParameter<edm::ParameterSet>("channels");
    typedef std::vector<std::string> vstring;
    vstring channelNames = cfgChannels.getParameterNamesForType<edm::ParameterSet>();
    for ( vstring::const_iterator channelName = channelNames.begin();
	  channelName != channelNames.end(); ++channelName ) {
      edm::ParameterSet cfgChannel = cfgChannels.getParameter<edm::ParameterSet>(*channelName);
      channelEntryType* channelEntry = new channelEntryType(*channelName, cfgChannel, dataIntLumi_);
      channels_.push_back(channelEntry);
    }
  } else {
    channelEntryType* channelEntry = new channelEntryType("undefined", cfg, dataIntLumi_);
    channels_.push_back(channelEntry);
  }
}

DQMEffXsecCalculator::~DQMEffXsecCalculator()
{
  for ( std::vector<channelEntryType*>::iterator it = channels_.begin();
	it != channels_.end(); ++it ) {
    delete (*it);
  }
}

void DQMEffXsecCalculator::endJob()
{
  std::cout << "<DQMEffXsecCalculator::endJob>:" << std::endl;
  std::cout << " moduleLabel = " << moduleLabel_ << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be plotted !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());  

  for ( std::vector<channelEntryType*>::iterator channel = channels_.begin();
	channel != channels_.end(); ++channel ) {
    std::cout << (*channel)->name_ << ": " << (*channel)->getEffXsec(dqmStore) << std::endl;
  }

  std::cout << "done." << std::endl;
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMEffXsecCalculator);
