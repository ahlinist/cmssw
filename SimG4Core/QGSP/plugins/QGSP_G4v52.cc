#include "QGSP_G4v52.hh"
#ifndef G4V7

#include "G4DecayPhysics.hh"
#include "SimG4Core/QGSP/src/EmStandardPhysics52.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"

#include "G4DataQuestionaire.hh"
#include "SimG4Core/QGSP/src/HadronPhysicsQGSP.hh"

QGSP_G4v52::QGSP_G4v52(const edm::ParameterSet & p)
    : PhysicsList(p)
{

  G4DataQuestionaire it(photon);
  std::cout << "<<< Geant4 Physics List simulation engine: QGSP_G4v52 3.1" << std::endl;

  int    ver = p.getUntrackedParameter<int>("Verbosity",0);

  // EM Physics
  RegisterPhysics( new EmStandardPhysics52("standard EM G4v52",ver));

  // Decays
  RegisterPhysics( new G4DecayPhysics("decay",ver) );

  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics("elastic",ver,false));

  // Hadron Physics
  RegisterPhysics( new HadronPhysicsQGSP("hadron"));
  
  // Stopping Physics
  RegisterPhysics( new G4QStoppingPhysics("stopping"));

  // Ion Physics
  RegisterPhysics( new G4IonPhysics("ion"));

}
#endif
