#ifndef HadronPhysicsLHEP_h
#define HadronPhysicsLHEP_h 1

#include "G4VPhysicsConstructor.hh"

#include "SimG4Core/Packaging/src/G4HadronQEDBuilder.hh"
#include "SimG4Core/Packaging/src/G4StoppingHadronBuilder.hh"
#include "SimG4Core/Packaging/src/G4MiscLHEPBuilder.hh"

#include "SimG4Core/Packaging/src/G4LHEPPiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4PiKBuilder.hh"

#include "SimG4Core/Packaging/src/G4ProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4LHEPProtonBuilder.hh"

#include "SimG4Core/Packaging/src/G4NeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4LHEPNeutronBuilder.hh"

class HadronPhysicsLHEP : public G4VPhysicsConstructor
{
  public: 
    HadronPhysicsLHEP(const G4String& name ="hadron");
    virtual ~HadronPhysicsLHEP();

  public: 
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    G4NeutronBuilder theNeutrons;
    G4LHEPNeutronBuilder theLHEPNeutron;
    
    G4PiKBuilder thePiK;
    G4LHEPPiKBuilder theLHEPPiK;
    
    G4ProtonBuilder thePro;
    G4LHEPProtonBuilder theLHEPPro;
    
    G4MiscLHEPBuilder theMiscLHEP;
    G4StoppingHadronBuilder theStoppingHadron;
    G4HadronQEDBuilder theHadronQED;
};

// 2002 by J.P. Wellisch

#endif

