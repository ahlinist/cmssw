#ifndef SimG4Core_GeneralPhysics_h
#define SimG4Core_GeneralPhysics_h

#ifdef G4V7

#include "G4VPhysicsConstructor.hh"

#include "G4Decay.hh"

class GeneralPhysics : public G4VPhysicsConstructor
{
public: 
    GeneralPhysics(const std::string name = "general");
    virtual ~GeneralPhysics();
    virtual void ConstructParticle();
    virtual void ConstructProcess();
protected:
    bool wasActivated;
private:
    G4Decay theDecay;
};

#endif
#endif







