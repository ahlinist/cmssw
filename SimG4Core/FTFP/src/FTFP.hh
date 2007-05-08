#ifndef SimG4Core_FTFP_H
#define SimG4Core_FTFP_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class FTFP: public PhysicsList
{
public:
  FTFP(G4LogicalVolumeToDDLogicalPartMap& map, const edm::ParameterSet & p);
};

#endif



