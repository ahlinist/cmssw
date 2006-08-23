#ifndef SimG4Core_DummyPhysics_H
#define SimG4Core_DummyPhysics_H

#include "SimG4Core/Physics81/interface/PhysicsList.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class DummyPhysics : public PhysicsList
{
public:
    DummyPhysics(const edm::ParameterSet & p);
    virtual ~DummyPhysics();
};
 
#endif
