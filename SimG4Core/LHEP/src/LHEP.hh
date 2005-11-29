#ifndef SimG4Core_LHEP_H
#define SimG4Core_LHEP_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class LHEP: public PhysicsList
{
public:
    LHEP(const edm::ParameterSet & p);
};

#endif



