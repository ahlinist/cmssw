#include "QGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4DataQuestionaire.hh"

QGSP_BERT_HP::QGSP_BERT_HP(const edm::ParameterSet & p) 
    : PhysicsList(p)
{
    G4DataQuestionaire it(photon);
    std::cout << "You are using the simulation engine: QGSP_BERT_HP 1.0" << std::endl;
  
    RegisterPhysics(new HadronPhysicsQGSP_BERT_HP("hadron"));
    RegisterPhysics(new G4IonPhysics("ion"));
  // EM Physics
  RegisterPhysics( new G4EmStandardPhysics("standard EM"));
  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics("extra EM"));
  // Decays
  RegisterPhysics( new G4DecayPhysics("decay") );
  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics("elastic"));
  // Stopping Physics
  RegisterPhysics( new G4QStoppingPhysics("stopping"));
  // Ion Physics

}

