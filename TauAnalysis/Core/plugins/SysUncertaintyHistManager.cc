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

  cfgHistManagerEntries_ = cfg.getParameter<vParameterSet>("histManagers");
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

  for ( vParameterSet::const_iterator cfgHistManagerEntry = cfgHistManagerEntries_.begin();
	cfgHistManagerEntry != cfgHistManagerEntries_.end(); ++cfgHistManagerEntry ) {
    edm::ParameterSet cfgHistManager = cfgHistManagerEntry->getParameter<edm::ParameterSet>("config");

    std::string histManagerType = cfgHistManager.getParameter<std::string>("pluginType");
    //std::cout << " histManagerType = " << histManagerType << std::endl;

    std::string dqmDirectory_histmanager = cfgHistManager.getParameter<std::string>("dqmDirectory_store");

//--- get list of all systematic uncertainties
//    relevant for this histogram manager
    std::set<std::string> sysNames;
    sysNames.insert(SysUncertaintyService::getNameCentralValue());

    edm::ParameterSet cfgSystematics = cfgHistManagerEntry->getParameter<edm::ParameterSet>("systematics");
    typedef std::vector<std::string> vstring;
    vstring srcNames = cfgSystematics.getParameterNamesForType<vParameterSet>();
    for ( vstring::const_iterator srcName = srcNames.begin(); 
	  srcName != srcNames.end(); ++srcName ) {
      //std::cout << " srcName = " << (*srcName) << std::endl;

      vParameterSet cfgSysDefinitions = cfgSystematics.getParameter<vParameterSet>(*srcName);

      for ( vParameterSet::const_iterator cfgSysDefinition = cfgSysDefinitions.begin();
	    cfgSysDefinition != cfgSysDefinitions.end(); ++cfgSysDefinition ) {
	std::string sysName = cfgSysDefinition->getParameter<std::string>("name");
	//std::cout << "--> adding sysName = " << sysName << std::endl;
	sysNames.insert(sysName);
      }
    }

//--- create one instance of this histogram manager
//    per relevant systematic uncertainty
    for ( std::set<std::string>::const_iterator sysName = sysNames.begin();
	  sysName != sysNames.end(); ++sysName ) {     
      //std::cout << " sysName = " << (*sysName) << std::endl;

      edm::ParameterSet cfgHistManager_systematic(cfgHistManager);

      for ( vstring::const_iterator srcName = srcNames.begin(); 
	    srcName != srcNames.end(); ++srcName ) {
	//std::cout << " srcName = " << (*srcName) << std::endl;

	vParameterSet cfgSysDefinitions = cfgSystematics.getParameter<vParameterSet>(*srcName);
	
	for ( vParameterSet::const_iterator cfgSysDefinition = cfgSysDefinitions.begin();
	      cfgSysDefinition != cfgSysDefinitions.end(); ++cfgSysDefinition ) {
	  std::string sysName_definition = cfgSysDefinition->getParameter<std::string>("name");
	  //std::cout << " sysName_definition = " << sysName_definition << std::endl;
	  edm::InputTag srcValue = cfgSysDefinition->getParameter<edm::InputTag>("src");
	  //std::cout << " srcValue = " << srcValue.label() << std::endl;

	  if ( sysName_definition == (*sysName) ) cfgHistManager_systematic.addParameter<edm::InputTag>(*srcName, srcValue);
	}
      }

      std::string dqmDirectory_systematic = dqmDirectoryName(dqmDirectory_store_).append(*sysName);
      //std::cout << " dqmDirectory_systematic = " << dqmDirectory_systematic << std::endl;
      std::string dqmDirectory_store = dqmDirectoryName(dqmDirectory_systematic).append(dqmDirectory_histmanager);
      //std::cout << " dqmDirectory_store = " << dqmDirectory_store << std::endl;
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
  //std::cout << " currentSystematic = " << currentSystematic << std::endl;

//--- fill collection of histograms dependent upon current systematic
  std::map<std::string, vHistManager>::iterator histManagerList = histManagers_.find(currentSystematic);
  if ( histManagerList != histManagers_.end() ) {
    for ( vHistManager::iterator histManager = histManagerList->second.begin();
	  histManager != histManagerList->second.end(); ++histManager ) {
      (*histManager)->analyze(evt, es, evtWeight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, SysUncertaintyHistManager, "SysUncertaintyHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<SysUncertaintyHistManager> SysUncertaintyAnalyzer;

DEFINE_FWK_MODULE(SysUncertaintyAnalyzer);
