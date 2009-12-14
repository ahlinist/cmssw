#ifndef CONDCORE_PLUGINSYSTEM_PROXYFACTORY_H
#define CONDCORE_PLUGINSYSTEM_PROXYFACTORY_H
// -*- C++ -*-
//
// Package:     ESSources
// Class  :     ProxyFactory
// 
/**\class ProxyFactory ProxyFactory.h CondCore/ESSources/interface/ProxyFactory.h
   
Description: <one line class summary>

Usage:
<usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Sat Jul 23 19:14:06 EDT 2005
//
#include "FWCore/PluginManager/interface/PluginFactory.h"
#include<string>

namespace cond {
  class DataProxyWrapperBase;
  class DbSession;

  typedef edmplugin::PluginFactory< cond::DataProxyWrapperBase* ( cond::DbSession&, std::pair< std::string, std::string> const & ) > ProxyFactory;

   const char* pluginCategory();
}

#endif /* CONDCORE_PLUGINSYSTEM_PROXYFACTORY_H */
