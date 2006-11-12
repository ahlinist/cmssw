#ifndef SimG4Core_UtilityAction_UtilityActionPluginFactory_H
#define SimG4Core_UtilityAction_UtilityActionPluginFactory_H
 
#include "SimG4Core/UtilityAction/interface/UtilityAction.h"
#include "SimG4Core/UtilityAction/interface/UtilityActionMaker.h"

#include "SealKernel/Component.h"
#include "PluginManager/PluginFactory.h"
 
class UtilityActionPluginFactory : 
    public seal::PluginFactory<UtilityActionMakerBase *()>
{
public:
    static  UtilityActionPluginFactory* get();
    virtual ~UtilityActionPluginFactory();
private:
    static UtilityActionPluginFactory s_instance;
    UtilityActionPluginFactory();
};
 
#define DEFINE_UTILITYACTION(type) \
 DEFINE_SEAL_PLUGIN(UtilityActionPluginFactory,UtilityActionMaker<type>,"SimG4Core/UtilityAction/" #type)
#endif
