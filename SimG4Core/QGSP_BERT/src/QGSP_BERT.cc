#include "QGSP_BERT.hh"
#ifndef G4V7

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"

#include "G4DataQuestionaire.hh"
#include "HadronPhysicsQGSP_BERT.hh"

QGSP_BERT::QGSP_BERT(const edm::ParameterSet & p)
    : PhysicsList(p)
{

  G4DataQuestionaire it(photon);
  std::cout << "<<< Geant4 Physics List simulation engine: QGSP_BERT 3.1" << std::endl;

  int    ver = p.getUntrackedParameter<int>("Verbosity",0);

  // EM Physics
  RegisterPhysics( new G4EmStandardPhysics("standard EM",ver));

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics("extra EM"));

  // Decays
  RegisterPhysics( new G4DecayPhysics("decay",ver) );

  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics("elastic",ver,false));

  // Hadron Physics
  RegisterPhysics( new HadronPhysicsQGSP_BERT("hadron"));
  
  // Stopping Physics
  RegisterPhysics( new G4QStoppingPhysics("stopping"));

  // Ion Physics
  RegisterPhysics( new G4IonPhysics("ion"));

}
#endif
