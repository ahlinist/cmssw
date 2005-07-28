#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"

#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (PhysicsListFactory, 
		    DummyPhysics,DummyPhysics::classContextLabel ());
