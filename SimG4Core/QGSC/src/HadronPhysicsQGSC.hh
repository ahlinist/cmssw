#ifndef SimG4Core_HadronPhysicsQGSC_H
#define SimG4Core_HadronPhysicsQGSC_H

#include "G4VPhysicsConstructor.hh"

#include "SimG4Core/Packaging/src/G4HadronQEDBuilder.hh"
#include "SimG4Core/Packaging/src/G4StoppingHadronBuilder.hh"
#include "SimG4Core/Packaging/src/G4MiscLHEPBuilder.hh"

#include "SimG4Core/Packaging/src/G4PiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPPiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4QGSCPiKBuilder.hh"

#include "SimG4Core/Packaging/src/G4ProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4QGSCProtonBuilder.hh"

#include "SimG4Core/Packaging/src/G4NeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPNeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4QGSCNeutronBuilder.hh"

class HadronPhysicsQGSC : public G4VPhysicsConstructor
{
  public: 
    HadronPhysicsQGSC(const G4String& name ="hadron");
    virtual ~HadronPhysicsQGSC();

  public: 
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    G4NeutronBuilder theNeutrons;
    G4LEPNeutronBuilder theLEPNeutron;
    G4QGSCNeutronBuilder theQGSCNeutron;
    
    G4PiKBuilder thePiK;
    G4LEPPiKBuilder theLEPPiK;
    G4QGSCPiKBuilder theQGSCPiK;
    
    G4ProtonBuilder thePro;
    G4LEPProtonBuilder theLEPPro;
    G4QGSCProtonBuilder theQGSCPro;    
    
    G4MiscLHEPBuilder theMiscLHEP;
    G4StoppingHadronBuilder theStoppingHadron;
    G4HadronQEDBuilder theHadronQED;
};

// 2002 by J.P. Wellisch

#endif

