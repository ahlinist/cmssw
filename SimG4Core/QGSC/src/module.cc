#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "SimG4Core/QGSC/src/QGSC.hh"

#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (PhysicsListFactory, 
		    QGSC,QGSC::classContextLabel ());
