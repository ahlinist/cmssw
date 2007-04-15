#include "SimG4Core/Physics/interface/PhysicsListFactory.h"
#include "QGSP.hh"
#include "QGSP_EMV.hh"
#include "QGSP_G4v52.hh"

DEFINE_PHYSICSLIST(QGSP);
#ifndef G4V7
DEFINE_PHYSICSLIST(QGSP_EMV);
DEFINE_PHYSICSLIST(QGSP_G4v52);
#endif
