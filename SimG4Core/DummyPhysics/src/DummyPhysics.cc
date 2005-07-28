#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"
#include "SimG4Core/DummyPhysics/interface/GeneralPhysics.h"
#include "SimG4Core/DummyPhysics/interface/DummyEMPhysics.h"

DEFINE_SEAL_COMPONENT (DummyPhysics, "SimG4Core/Physics/DummyPhysics");
 
DummyPhysics::DummyPhysics(seal::Context * c, const edm::ParameterSet & p) 
    : PhysicsList(c,p)
{ 
    RegisterPhysics(new GeneralPhysics("general"));
    if (p.getParameter<bool>("DummyEMPhysics")) 
	RegisterPhysics(new DummyEMPhysics("dummyEM"));
    std::cout << " DummyPhysics constructed " << std::endl;
}

DummyPhysics::~DummyPhysics() {}

