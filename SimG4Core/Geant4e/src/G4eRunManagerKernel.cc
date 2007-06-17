
#include "G4Navigator.hh"
// On Sun, to prevent conflict with ObjectSpace, G4Timer.hh has to be
// loaded *before* globals.hh...
#include "G4Timer.hh"

#include "SimG4Core/Geant4e/interface/G4eRunManagerKernel.hh"

#include "G4RunManagerKernel.hh"
#include "G4VUserDetectorConstruction.hh"
#include "SimG4Core/Geant4e/interface/G4ePhysicsList.hh"
#include "G4TransportationManager.hh"
#include "G4RunManager.hh"

//-----------------------------------------------------------------------

G4eRunManagerKernel* G4eRunManagerKernel::fRunManagerKernel = 0;

//-----------------------------------------------------------------------
G4eRunManagerKernel* G4eRunManagerKernel::GetRunManagerKernel()
{ return fRunManagerKernel; }

//-----------------------------------------------------------------------
G4eRunManagerKernel::G4eRunManagerKernel()
{
  if(fRunManagerKernel)
  { G4Exception("G4eRunManageKernel constructed twice."); }
  fRunManagerKernel = this;

  //----- Look if somebody has created a G4RunManagerKernel
  theG4RunManagerKernel = G4RunManagerKernel::GetRunManagerKernel();
  if( theG4RunManagerKernel == 0 ) {
    //--- if not create it
    theG4RunManagerKernel = new G4RunManagerKernel();
    G4cout << " creating G4RunManagerKernel " <<  theG4RunManagerKernel << G4endl;
  }
    
  theG4RunManagerKernel->SetVerboseLevel(2);
  theUserPhysicsList = 0;
  theUserWorld = 0;

}

//-----------------------------------------------------------------------
G4eRunManagerKernel::~G4eRunManagerKernel()
{
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::SetUserInitialization(G4VUserDetectorConstruction* userInit)
{ 
  theUserWorld = userInit->Construct();
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::SetUserInitialization(G4VPhysicalVolume* userInit)
{
  theUserWorld = userInit;
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::SetUserInitialization(G4VUserPhysicsList* userInit)
{
  theUserPhysicsList = userInit;
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::InitializeGeometry()
{
  //check if user world has been directly called or someone initialized the world volume already 
  //----- First option: geometry has been defined to GEANT4e 
  if( theUserWorld != 0 ) {
    theG4RunManagerKernel->DefineWorldVolume( theUserWorld );
    
    //----- Second option: geometry has been defined to GEANT4, do nothing GEANT4 should take care 
  } else {
    //  G4cerr << "G4 TM " << G4TransportationManager::GetTransportationManager() 
    //      << " NAV " << G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking() 
    //      << " WORLD " << G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume() << std::endl;
    //--- Check that indeed geometry has been defined to GEANT4
    if ( G4TransportationManager::GetTransportationManager()
	 ->GetNavigatorForTracking()->GetWorldVolume() == 0 ) {
      G4Exception("G4eRunManagerKernel::InitializeGeometry no world defined in your geometry!" );
    }
    
  }
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::InitializePhysics()
{

  G4cout << "  G4eRunManagerKernel::InitializePhysics "  << G4endl;

  //----- First option: physics list has been defined to GEANT4e 
  if( theUserPhysicsList != 0 ) {
    theG4RunManagerKernel->SetPhysics(theUserPhysicsList);
    theG4RunManagerKernel->InitializePhysics();
  }else {
  //----- Second option: physics list has been defined to GEANT4, do nothing GEANT4 should take care 
    if( G4RunManager::GetRunManager() != 0 && G4RunManager::GetRunManager()->GetUserPhysicsList() != 0 ){ 
      //--- Physics should be G4ePhysicsList, else send a warning
      if( static_cast<const G4ePhysicsList*>(G4RunManager::GetRunManager()->GetUserPhysicsList()) != 0 ) {
	G4cerr << " WARNING G4eRunManagerKernel::InitializePhysics() physics list is not G4ePhysicsList. Are you sure? " << G4endl;
      }
    } else {
      //----- Third option: no physics list has been defined, define a G4ePhysicsList
      theG4RunManagerKernel->SetPhysics(new G4ePhysicsList);
      //    theG4RunManagerKernel->SetPhysics(new ExN02PhysicsList);
      theG4RunManagerKernel->InitializePhysics();
    }
  }
 
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::RunInitialization()
{
  theG4RunManagerKernel->RunInitialization();
}


//-----------------------------------------------------------------------
void G4eRunManagerKernel::SetUserAction(G4UserTrackingAction* userAction)
{

  G4EventManager::GetEventManager()->SetUserAction( userAction );
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::SetUserAction(G4UserSteppingAction* userAction)
{
  G4EventManager::GetEventManager()->SetUserAction( userAction );
}

//-----------------------------------------------------------------------
void G4eRunManagerKernel::RunTermination()
{
  theG4RunManagerKernel->RunTermination();
}

