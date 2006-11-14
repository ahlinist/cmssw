#include "SimG4Core/DummyPhysics/interface/GeneralPhysics.h"
#ifdef G4V7

#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"

#include "G4ProcessManager.hh"

GeneralPhysics::GeneralPhysics(const std::string n) : 
    G4VPhysicsConstructor(n),wasActivated(false) {}

GeneralPhysics::~GeneralPhysics() 
{
    if (wasActivated)
    {
	theParticleIterator->reset();
	while ((*theParticleIterator)())
	{
	    G4ParticleDefinition * p = theParticleIterator->value();
	    G4ProcessManager * m = p->GetProcessManager();
	    if (m!=0 && theDecay.IsApplicable(*p)) m->RemoveProcess(&theDecay);
	}
    }
}

void GeneralPhysics::ConstructParticle()
{ 
    G4Geantino::GeantinoDefinition(); 
    G4ChargedGeantino::ChargedGeantinoDefinition(); 
}

void GeneralPhysics::ConstructProcess()
{
    theParticleIterator->reset();
    while ((*theParticleIterator)())
    {
	G4ParticleDefinition * p = theParticleIterator->value();
	G4ProcessManager * m = p->GetProcessManager();
	if (theDecay.IsApplicable(*p)) 
	{ 
	    m->AddProcess(&theDecay);
	    m->SetProcessOrdering(&theDecay,idxPostStep);
	    m->SetProcessOrdering(&theDecay,idxAtRest);
	}
    }
}

#endif
