#include "TauAnalysis/Core/plugins/SysUncertaintyHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <iostream>
#include <iomanip>

SysUncertaintyHistManager::SysUncertaintyHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<SysUncertaintyHistManager::SysUncertaintyHistManager>:" << std::endl;

  cfgHistManagers_ = cfg.getParameter<vParameterSet>("histManagers");

  systematics_ = cfg.getParameter<vstring>("systematics");
}

SysUncertaintyHistManager::~SysUncertaintyHistManager()
{
  for ( std::map<std::string, vHistManager>::iterator histManagerList = histManagers_.begin();
	histManagerList != histManagers_.end(); ++histManagerList ) {
    for ( vHistManager::iterator histManager = histManagerList->second.begin();
	  histManager != histManagerList->second.end(); ++histManager ) {
      delete (*histManager);
    }
  }
}

void SysUncertaintyHistManager::bookHistogramsImp()
{
  //std::cout << "<SysUncertaintyHistManager::bookHistogramsImp>:" << std::endl;

  for ( vstring::const_iterator sysName = systematics_.begin();
	sysName != systematics_.end(); ++sysName ) {
    std::string dqmDirectory_systematic = dqmDirectoryName(dqmDirectory_store_).append(*sysName);
    //std::cout << " dqmDirectory_systematic = " << dqmDirectory_systematic << std::endl;

    for ( vParameterSet::const_iterator cfgHistManager = cfgHistManagers_.begin();
	  cfgHistManager != cfgHistManagers_.end(); ++cfgHistManager ) {
      std::string histManagerType = cfgHistManager->getParameter<std::string>("pluginType");
      //std::cout << " histManagerType = " << histManagerType << std::endl;

      std::string dqmDirectory_histmanager = cfgHistManager->getParameter<std::string>("dqmDirectory_store");
      std::string dqmDirectory_store = dqmDirectoryName(dqmDirectory_systematic).append(dqmDirectory_histmanager);
      //std::cout << " dqmDirectory_store = " << dqmDirectory_store << std::endl;

      edm::ParameterSet cfgHistManager_systematic(*cfgHistManager);
      cfgHistManager_systematic.addParameter<std::string>("dqmDirectory_store", dqmDirectory_store);

      HistManagerBase* histManager = HistManagerPluginFactory::get()->create(histManagerType, cfgHistManager_systematic);
      histManager->beginJob();
      histManagers_[*sysName].push_back(histManager);
    }
  }
}

void SysUncertaintyHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<SysUncertaintyHistManager::fillHistogramsImp>:" << std::endl; 

  if ( !edm::Service<SysUncertaintyService>().isAvailable() ) {
    edm::LogError ("fillHistogramsImp") << " Failed to access SysUncertaintyService --> histograms will NOT be filled !!";
    return;
  }

  const SysUncertaintyService* sysUncertaintyService = &(*edm::Service<SysUncertaintyService>());

  const std::string& currentSystematic = sysUncertaintyService->getCurrentSystematic();

//--- fill collection of histograms corresponding to current systematic;
//    print error message if no histogram managers defined for current systematic
  std::map<std::string, vHistManager>::iterator histManagerList = histManagers_.find(currentSystematic);
  if ( histManagerList == histManagers_.end() ) {
    edm::LogError ("fillHistogramsImp") << " No histogram Managers defined for systematic = " << currentSystematic
					<< " --> skipping !!";
    return;
  }

  for ( vHistManager::iterator histManager = histManagerList->second.begin();
	histManager != histManagerList->second.end(); ++histManager ) {
    (*histManager)->analyze(evt, es, evtWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, SysUncertaintyHistManager, "SysUncertaintyHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<SysUncertaintyHistManager> SysUncertaintyAnalyzer;

DEFINE_FWK_MODULE(SysUncertaintyAnalyzer);
