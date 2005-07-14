#ifndef SimG4Core_DummyPhysics_H
#define SimG4Core_DummyPhysics_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
 
class DummyPhysics : public PhysicsList
{
public:
    DummyPhysics();
    virtual ~DummyPhysics();
};
 
#endif
