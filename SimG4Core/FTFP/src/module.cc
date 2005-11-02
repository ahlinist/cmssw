#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "SimG4Core/FTFP/src/FTFP.hh"

#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (PhysicsListFactory, 
		    FTFP,FTFP::classContextLabel ());
