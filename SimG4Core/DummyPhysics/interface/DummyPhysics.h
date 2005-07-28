#ifndef SimG4Core_DummyPhysics_H
#define SimG4Core_DummyPhysics_H

#include "SimG4Core/Physics/interface/PhysicsList.h"
 
#include "SealKernel/Component.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class DummyPhysics : public PhysicsList
{
    DECLARE_SEAL_COMPONENT;
public:
    DummyPhysics(seal::Context * c, const edm::ParameterSet & p);
    virtual ~DummyPhysics();
};
 
#endif
