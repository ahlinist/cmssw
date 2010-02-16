#include "TauAnalysis/Core/plugins/SysUncertaintyBinner.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include "TauAnalysis/Core/interface/BinnerBase.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/Core/interface/sysUncertaintyAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <iostream>
#include <iomanip>

SysUncertaintyBinner::SysUncertaintyBinner(const edm::ParameterSet& cfg) 
{
  //std::cout << "<SysUncertaintyBinner::SysUncertaintyBinner>:" << std::endl;

  cfgBinnerPlugins_ = cfg.getParameter<vParameterSet>("binnerPlugins");

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  vstring cfgSystematics = cfg.getParameter<vstring>("systematics");
  for ( vstring::const_iterator sysName = cfgSystematics.begin();
	sysName != cfgSystematics.end(); ++sysName ) {
    vstring sysNames_expanded = expandSysName(*sysName);
    systematics_.insert(systematics_.end(), sysNames_expanded.begin(), sysNames_expanded.end());
  }

  systematics_.insert(systematics_.begin(), SysUncertaintyService::getNameCentralValue());
  //std::cout << " systematics = " << format_vstring(systematics_) << std::endl;
  
  for ( vstring::const_iterator sysName = systematics_.begin();
	sysName != systematics_.end(); ++sysName ) {
    for ( vParameterSet::const_iterator cfgBinnerPlugin = cfgBinnerPlugins_.begin();
	  cfgBinnerPlugin != cfgBinnerPlugins_.end(); ++cfgBinnerPlugin ) {
      std::string binnerPluginType = cfgBinnerPlugin->getParameter<std::string>("pluginType");
      //std::cout << " binnerPluginType = " << binnerPluginType << std::endl;

      std::string dqmSubDirectory_binner = cfgBinnerPlugin->getParameter<std::string>("dqmDirectory_store");
      std::string dqmDirectory_binner = dqmDirectoryName(dqmDirectory_store_).append(dqmSubDirectory_binner);
      std::string dqmDirectory_systematic = dqmDirectoryName(dqmDirectory_binner).append(*sysName);
      //std::cout << " dqmDirectory_systematic = " << dqmDirectory_systematic << std::endl;

      edm::ParameterSet cfgBinnerPlugin_systematic(*cfgBinnerPlugin);
      cfgBinnerPlugin_systematic.addParameter<std::string>("dqmDirectory_store", dqmDirectory_systematic);
      
      BinnerBase* binnerPlugin = BinnerPluginFactory::get()->create(binnerPluginType, cfgBinnerPlugin_systematic);
      binnerPlugins_[*sysName].push_back(binnerPlugin);
    }
  }
}

SysUncertaintyBinner::~SysUncertaintyBinner()
{
  for ( std::map<std::string, vBinnerPlugins>::iterator binnerPluginList = binnerPlugins_.begin();
	binnerPluginList != binnerPlugins_.end(); ++binnerPluginList ) {
    for ( vBinnerPlugins::iterator binnerPlugin = binnerPluginList->second.begin();
	  binnerPlugin != binnerPluginList->second.end(); ++binnerPlugin ) {
      delete (*binnerPlugin);
    }
  }
}

void SysUncertaintyBinner::endJob() 
{ 
  for ( std::map<std::string, vBinnerPlugins>::iterator binnerPluginList = binnerPlugins_.begin();
	binnerPluginList != binnerPlugins_.end(); ++binnerPluginList ) {
    std::cout << "sysName = " << binnerPluginList->first << ":" << std::endl;
    
    for ( vBinnerPlugins::iterator binnerPlugin = binnerPluginList->second.begin();
	  binnerPlugin != binnerPluginList->second.end(); ++binnerPlugin ) {
      (*binnerPlugin)->endJob();
    }
    
    std::cout << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void SysUncertaintyBinner::bin(const edm::Event& evt, const edm::EventSetup& es, double evtWeight) 
{
 if ( !edm::Service<SysUncertaintyService>().isAvailable() ) {
    edm::LogError ("bin") << " Failed to access SysUncertaintyService --> binning results will NOT be filled !!";
    return;
  }

  const SysUncertaintyService* sysUncertaintyService = &(*edm::Service<SysUncertaintyService>());

  const std::string& currentSystematic = sysUncertaintyService->getCurrentSystematic();

  std::map<std::string, vBinnerPlugins>::iterator binnerPluginList = binnerPlugins_.find(currentSystematic);
  if ( binnerPluginList != binnerPlugins_.end() ) {
    for ( vBinnerPlugins::iterator binnerPlugin = binnerPluginList->second.begin();
	  binnerPlugin != binnerPluginList->second.end(); ++binnerPlugin ) {
      (*binnerPlugin)->bin(evt, es, evtWeight);
    }
  } else {
    edm::LogError ("bin") << " No binning results defined for systematic = " << currentSystematic << " !!";
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, SysUncertaintyBinner, "SysUncertaintyBinner");
DEFINE_EDM_PLUGIN(BinnerPluginFactory, SysUncertaintyBinner, "SysUncertaintyBinner");



