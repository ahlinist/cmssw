#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"
#include "SimG4Core/DummyPhysics/interface/GeneralPhysics.h"
#include "SimG4Core/DummyPhysics/interface/DummyEMPhysics.h"

DummyPhysics::DummyPhysics() : PhysicsList()
{ 
    RegisterPhysics(new GeneralPhysics("general"));
    bool dummyEMPhysics = true;    
    if (dummyEMPhysics) RegisterPhysics(new DummyEMPhysics("dummyEM"));
    std::cout << " DummyPhysics constructed " << std::endl;
}

DummyPhysics::~DummyPhysics() {}

