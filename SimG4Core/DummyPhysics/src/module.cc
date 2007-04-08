#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"

#include "FWCore/PluginManager/interface/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_PHYSICSLIST (DummyPhysics);

