#ifndef HadronPhysicsLHEP_h
#define HadronPhysicsLHEP_h 1

//#define G4v7
#include "G4VPhysicsConstructor.hh"

#ifdef G4v7
#include "SimG4Core/Packaging/src/G4HadronQEDBuilder.hh"
#include "SimG4Core/Packaging/src/G4StoppingHadronBuilder.hh"
#include "SimG4Core/Packaging/src/G4MiscLHEPBuilder.hh"

#include "SimG4Core/Packaging/src/G4LHEPPiKBuilder.hh"
#include "SimG4Core/Packaging/src/G4PiKBuilder.hh"

#include "SimG4Core/Packaging/src/G4ProtonBuilder.hh"
#include "SimG4Core/Packaging/src/G4LHEPProtonBuilder.hh"

#include "SimG4Core/Packaging/src/G4NeutronBuilder.hh"
#include "SimG4Core/Packaging/src/G4LHEPNeutronBuilder.hh"
#else
#include "G4StoppingHadronBuilder.hh"
#include "G4MiscLHEPBuilder.hh"
 
#include "G4LHEPPiKBuilder.hh"
#include "G4PiKBuilder.hh"
 
#include "G4ProtonBuilder.hh"
#include "G4LHEPProtonBuilder.hh"
 
#include "G4NeutronBuilder.hh"
#include "G4LHEPNeutronBuilder.hh"
#endif 

class HadronPhysicsLHEP : public G4VPhysicsConstructor
{
  public: 
    HadronPhysicsLHEP(const G4String& name ="hadron");
    virtual ~HadronPhysicsLHEP();

  public: 
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
#ifdef G4v7
    G4NeutronBuilder theNeutrons;
    G4LHEPNeutronBuilder theLHEPNeutron;
    
    G4PiKBuilder thePiK;
    G4LHEPPiKBuilder theLHEPPiK;
    
    G4ProtonBuilder thePro;
    G4LHEPProtonBuilder theLHEPPro;
    
    G4MiscLHEPBuilder theMiscLHEP;
    G4StoppingHadronBuilder theStoppingHadron;
    G4HadronQEDBuilder theHadronQED;
#else
    void CreateModels();
    G4NeutronBuilder * theNeutrons;
    G4LHEPNeutronBuilder * theLHEPNeutron;
     
    G4PiKBuilder * thePiK;
    G4LHEPPiKBuilder * theLHEPPiK;
     
    G4ProtonBuilder * thePro;
    G4LHEPProtonBuilder * theLHEPPro;
     
    G4MiscLHEPBuilder * theMiscLHEP;
    G4StoppingHadronBuilder * theStoppingHadron;
#endif
};

// 2002 by J.P. Wellisch

#endif

