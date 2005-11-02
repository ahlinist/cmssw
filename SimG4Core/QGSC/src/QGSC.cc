#include "QGSC.hh"
#include "HadronPhysicsQGSC.hh"

#include "SimG4Core/Packaging/src/GeneralPhysics.hh"
#include "SimG4Core/Packaging/src/EMPhysics.hh"
#include "SimG4Core/Packaging/src/MuonPhysics.hh"
#include "SimG4Core/Packaging/src/IonPhysics.hh"
#include "SimG4Core/Packaging/src/G4DataQuestionaire.hh"

DEFINE_SEAL_COMPONENT (QGSC, "SimG4Core/Physics/QGSC");

QGSC::QGSC(seal::Context * c, const edm::ParameterSet & p) 
    : PhysicsList(c,p)
{
    G4DataQuestionaire it(photon);
    std::cout << "You are using the simulation engine: QGSC 2.8" << std::endl;
  
    RegisterPhysics(new GeneralPhysics("general"));
    RegisterPhysics(new EMPhysics("EM"));
    RegisterPhysics(new MuonPhysics("muon"));
    RegisterPhysics(new HadronPhysicsQGSC("hadron"));
    RegisterPhysics(new IonPhysics("ion"));
}

QGSC::~QGSC() {}
