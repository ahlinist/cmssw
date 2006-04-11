#include "SimG4Core/UtilityAction/interface/UtilityActionPluginFactory.h"
 
UtilityActionPluginFactory UtilityActionPluginFactory::s_instance;
 
UtilityActionPluginFactory::UtilityActionPluginFactory () :
    seal::PluginFactory<UtilityActionMakerBase * ()>("CMS Simulation UtilityActionPluginFactory")
{}

UtilityActionPluginFactory::~UtilityActionPluginFactory () {}

UtilityActionPluginFactory  *UtilityActionPluginFactory::get () { return &s_instance; }
