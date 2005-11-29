#include "LHEP.hh"
#include "HadronPhysicsLHEP.hh"

#include "SimG4Core/Packaging/src/GeneralPhysics.hh"
#include "SimG4Core/Packaging/src/EMPhysics.hh"
#include "SimG4Core/Packaging/src/MuonPhysics.hh"
#include "SimG4Core/Packaging/src/IonPhysics.hh"
#include "SimG4Core/Packaging/src/G4DataQuestionaire.hh"

LHEP::LHEP(const edm::ParameterSet & p) : PhysicsList(p)
{
    G4DataQuestionaire it(photon);
    std::cout << "You are using the simulation engine: LHEP 3.7" << std::endl;
  
    RegisterPhysics(new GeneralPhysics("general"));
    RegisterPhysics(new EMPhysics("EM"));
    RegisterPhysics(new MuonPhysics("muon"));
    RegisterPhysics(new HadronPhysicsLHEP("hadron"));
    RegisterPhysics(new IonPhysics("ion"));
}

