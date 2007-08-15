//
// class G4ePhysicsList
//
// Class description:
//  Default physics list for GEANT4e (should not be overridden, unless by experts)
//  No multiple scattering and no production of secondaries.
//  The energy loss process is G4eMuIonisation or G4EnergyLossForExtrapolator 
//  (depending on the value of the enviromental variable G4EELOSSEXTRAP)
//  It also defines the geant4e processes to limit the step: G4eMagneticFieldLimitProcess, G4eStepLimitProcess

// History:
//
// --------------------------------------------------------------------
//
#ifndef G4ePhysicsList_h
#define G4ePhysicsList_h 1

#include "G4VUserPhysicsList.hh"

class G4ePhysicsList: public G4VUserPhysicsList
{
public:
  G4ePhysicsList();
  virtual ~G4ePhysicsList();
  
protected:
  virtual void ConstructParticle();
    // constructs gamma, e+/- and mu+/- 

  virtual void ConstructProcess();
    // construct physical processes

  virtual void SetCuts();  
    // SetCutsWithDefault

protected:
  virtual void ConstructEM();
  
  private:
  //  G4VPhysicsConstructor* emPhysicsList;

};

#endif



