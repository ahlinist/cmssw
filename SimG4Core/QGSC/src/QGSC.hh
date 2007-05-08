#ifndef SimG4Core_QGSC_H
#define SimG4Core_QGSC_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class QGSC: public PhysicsList
{
public:
  QGSC(G4LogicalVolumeToDDLogicalPartMap& map, const edm::ParameterSet & p);
};

#endif

