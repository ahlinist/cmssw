#include "SimG4Core/DummyPhysics/interface/DummyPhysics.h"
#include "SimG4Core/DummyPhysics/interface/GeneralPhysics.h"
#include "SimG4Core/DummyPhysics/interface/DummyEMPhysics.h"

#ifndef G4V7
#include "G4DecayPhysics.hh"
#endif

DummyPhysics::DummyPhysics(const edm::ParameterSet & p) 
    : PhysicsList(p)
{ 
#ifdef G4V7
    RegisterPhysics(new GeneralPhysics("general"));
#else
    RegisterPhysics(new G4DecayPhysics("decay"));
#endif
    if (p.getParameter<bool>("DummyEMPhysics")) 
	RegisterPhysics(new DummyEMPhysics("dummyEM"));
    std::cout << " DummyPhysics constructed " << std::endl;
}

DummyPhysics::~DummyPhysics() {}

