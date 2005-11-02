#ifndef SimG4Core_HadronPhysicsFTFP_H
#define SimG4Core_HadronPhysicsFTFP_H

#include "G4VPhysicsConstructor.hh"

#include "SimG4Core/Packaging/src/G4HadronQEDBuilder.hh"
#include "SimG4Core/Packaging/src/G4StoppingHadronBuilder.hh"
#include "SimG4Core/Packaging/src/G4MiscLHEPBuilder.hh"

#include "SimG4Core/Packaging/src/G4PiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPPiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4FTFPPiKBuilder.hh"

#include "SimG4Core/Packaging/src/G4ProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4FTFPProtonBuilder.hh"

#include "SimG4Core/Packaging/src/G4NeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4LEPNeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4FTFPNeutronBuilder.hh"

class HadronPhysicsFTFP : public G4VPhysicsConstructor
{
  public: 
    HadronPhysicsFTFP(const G4String& name ="hadron");
    virtual ~HadronPhysicsFTFP();

  public: 
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    G4NeutronBuilder theNeutrons;
    G4LEPNeutronBuilder theLEPNeutron;
    G4FTFPNeutronBuilder theFTFPNeutron;
    
    G4PiKBuilder thePiK;
    G4LEPPiKBuilder theLEPPiK;
    G4FTFPPiKBuilder theFTFPPiK;
    
    G4ProtonBuilder thePro;
    G4LEPProtonBuilder theLEPPro;
    G4FTFPProtonBuilder theFTFPPro;    
    
    G4MiscLHEPBuilder theMiscLHEP;
    G4StoppingHadronBuilder theStoppingHadron;
    G4HadronQEDBuilder theHadronQED;
};

// 2002 by J.P. Wellisch

#endif

