#ifndef SimG4Core_UtilityAction_UtilityActionPluginFactory_H
#define SimG4Core_UtilityAction_UtilityActionPluginFactory_H
 
#include "SimG4Core/UtilityAction/interface/UtilityAction.h"
#include "SimG4Core/UtilityAction/interface/UtilityActionMaker.h"

#include "SealKernel/Component.h"
#include "FWCore/PluginManager/interface/PluginFactory.h"
 
typedef edmplugin::PluginFactory<UtilityActionMakerBase *()> UtilityActionPluginFactory ;
 
#define DEFINE_UTILITYACTION(type) \
 DEFINE_EDM_PLUGIN(UtilityActionPluginFactory,UtilityActionMaker<type>,"SimG4Core/UtilityAction/" #type)
#endif
