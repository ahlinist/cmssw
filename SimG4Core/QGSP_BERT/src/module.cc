#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "PluginManager/ModuleDef.h"
#include "QGSP_BERT.hh"

#ifndef G4V7
DEFINE_SEAL_MODULE ();
DEFINE_PHYSICSLIST(QGSP_BERT);
#endif
