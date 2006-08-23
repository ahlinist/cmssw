#include "SimG4Core/DummyPhysics81/interface/DummyPhysics.h"
#include "SimG4Core/DummyPhysics81/interface/DummyEMPhysics.h"

#include "G4DecayPhysics.hh"

DummyPhysics::DummyPhysics(const edm::ParameterSet & p) 
    : PhysicsList(p)
{ 
    RegisterPhysics(new G4DecayPhysics("decay"));
    if (p.getParameter<bool>("DummyEMPhysics")) 
	RegisterPhysics(new DummyEMPhysics("dummyEM"));
    std::cout << " DummyPhysics constructed " << std::endl;
}

DummyPhysics::~DummyPhysics() {}

