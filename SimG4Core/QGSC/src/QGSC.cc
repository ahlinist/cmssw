#include "QGSC.hh"
#include "HadronPhysicsQGSC.hh"

#include "SimG4Core/Packaging/src/GeneralPhysics.hh"
#include "SimG4Core/Packaging/src/EMPhysics.hh"
#include "SimG4Core/Packaging/src/MuonPhysics.hh"
#include "SimG4Core/Packaging/src/IonPhysics.hh"
#include "SimG4Core/Packaging/src/G4DataQuestionaire.hh"

QGSC::QGSC(const edm::ParameterSet & p) 
    : PhysicsList(p)
{
    G4DataQuestionaire it(photon);
    std::cout << "You are using the simulation engine: QGSC 2.8" << std::endl;
  
    RegisterPhysics(new GeneralPhysics("general"));
    RegisterPhysics(new EMPhysics("EM"));
    RegisterPhysics(new MuonPhysics("muon"));
    RegisterPhysics(new HadronPhysicsQGSC("hadron"));
    RegisterPhysics(new IonPhysics("ion"));
}

