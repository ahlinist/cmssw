#ifndef SimG4Core_FTFP_H
#define SimG4Core_FTFP_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class FTFP: public PhysicsList
{
public:
    FTFP(const edm::ParameterSet & p);
};

#endif



