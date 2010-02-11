#include "TauAnalysis/Skimming/plugins/EwkTauValidation.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include <stdlib.h>

const std::string runKeyword = "r";

const std::string dqmSeparator = "/";

EwkTauValidation::EwkTauValidation(const edm::ParameterSet& cfg)
  : cfgChannels_(cfg.getParameter<vParameterSet>("channels")),
    dqmError_(0)
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("EwkTauValidation") << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!";
    dqmError_ = 1;
    return;
  }

  maxNumWarnings_ = cfg.exists("maxNumWarnings") ? cfg.getParameter<int>("maxNumWarnings") : 1;
}

EwkTauValidation::~EwkTauValidation()
{
  for ( std::map<edm::RunNumber_t, vEwkValHistManagerBase>::iterator histManagers_run = histManagers_.begin();
	histManagers_run != histManagers_.end(); ++histManagers_run ) {
    for ( vEwkValHistManagerBase::iterator histManager_run = histManagers_run->second.begin();
	  histManager_run != histManagers_run->second.end(); ++histManager_run ) {
      delete (*histManager_run);
    }
  }
}

void EwkTauValidation::beginJob()
{
  if ( dqmError_ ) return;
}

void EwkTauValidation::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  if ( dqmError_ ) return;

  edm::RunNumber_t runNumber = evt.id().run();

//--- check if histogram manager plugins already exists for current run;
//    if not, create them
  if ( histManagers_.find(runNumber) == histManagers_.end() ) {
    bookHistograms(runNumber);
  }

//--- fill histograms for current run
  for ( std::map<edm::RunNumber_t, vEwkValHistManagerBase>::iterator histManagers_run = histManagers_.begin();
	histManagers_run != histManagers_.end(); ++histManagers_run ) {
    for ( vEwkValHistManagerBase::iterator histManager_run = histManagers_run->second.begin();
	  histManager_run != histManagers_run->second.end(); ++histManager_run ) {
      (*histManager_run)->fillHistograms(evt, es);
    }
  }
}

void EwkTauValidation::endJob()
{
  if ( dqmError_ ) return;

  for ( std::map<edm::RunNumber_t, vEwkValHistManagerBase>::iterator histManagers_run = histManagers_.begin();
	histManagers_run != histManagers_.end(); ++histManagers_run ) {
    for ( vEwkValHistManagerBase::iterator histManager_run = histManagers_run->second.begin();
	  histManager_run != histManagers_run->second.begin(); ++histManager_run ) {
      (*histManager_run)->finalizeHistograms();
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void EwkTauValidation::bookHistograms(edm::RunNumber_t runNumber)
{
  for ( vParameterSet::const_iterator cfgChannel = cfgChannels_.begin();
	cfgChannel != cfgChannels_.end(); ++cfgChannel ) {
    std::string pluginName = cfgChannel->getParameter<std::string>("pluginName");

    std::string dqmDirectory = cfgChannel->getParameter<std::string>("dqmDirectory");

    std::ostringstream dqmDirectory_run;
    dqmDirectory_run << runKeyword << runNumber << dqmSeparator << dqmDirectory;

    edm::LogInfo ("EwkTauValidation") 
      << "Booking " << pluginName << " histograms for run = " << runNumber << ", dqmDirectory = " << dqmDirectory_run.str();
    
    edm::ParameterSet cfgChannel_run(*cfgChannel);
    cfgChannel_run.addParameter<std::string>("dqmDirectory", dqmDirectory_run.str());
    cfgChannel_run.addParameter<int>("maxNumWarnings", maxNumWarnings_);

    std::string pluginType = cfgChannel->getParameter<std::string>("pluginType");
    EwkValHistManagerBase* histManager_run = EwkValHistManagerPluginFactory::get()->create(pluginType, cfgChannel_run);
    histManager_run->bookHistograms();
    histManagers_[runNumber].push_back(histManager_run);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(EwkTauValidation);
