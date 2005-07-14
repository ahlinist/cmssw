#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"
#include "SimG4Core/DummyPhysics/interface/GeneralPhysics.h"
#include "SimG4Core/DummyPhysics/interface/DummyEMPhysics.h"

DummyPhysics::DummyPhysics(const edm::ParameterSet & p) : PhysicsList(p)
{ 
    RegisterPhysics(new GeneralPhysics("general"));
    if (p.getParameter<bool>("DummyEMPhysics")) 
	RegisterPhysics(new DummyEMPhysics("dummyEM"));
    std::cout << " DummyPhysics constructed " << std::endl;
}

DummyPhysics::~DummyPhysics() {}

