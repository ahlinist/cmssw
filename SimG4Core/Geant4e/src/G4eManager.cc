
#define private public 
#include "G4MagIntegratorStepper.hh"
#define private private
#include "G4Mag_UsualEqRhs.hh"
#include "G4Mag_EqRhs.hh"
#include "G4MagIntegratorDriver.hh"

#include "G4ClassicalRK4.hh"

#include "SimG4Core/Geant4e/interface/G4eManager.hh"

#include "G4EventManager.hh"
#include "SimG4Core/Geant4e/interface/G4eRunManagerKernel.hh"
#include "SimG4Core/Geant4e/interface/G4ePropagatorG4.hh"
#include "SimG4Core/Geant4e/interface/G4eMag_UsualEqRhs.hh"
#include "G4MagneticField.hh" 

#include "G4TransportationManager.hh"
#include "SimG4Core/Geant4e/interface/G4eNavigator.hh"
#include "G4GeometryManager.hh"
#include "G4StateManager.hh"
#include "G4ChordFinder.hh"
#include "G4EquationOfMotion.hh"
#include "G4FieldManager.hh"

G4eManager* G4eManager::theG4eManager = 0;

int G4eManager::theVerbosity;

//-----------------------------------------------------------------------
G4eManager* G4eManager::GetG4eManager()
{
  if( theG4eManager == NULL ) {
    theG4eManager = new G4eManager;
  }

  return theG4eManager;
}


//-----------------------------------------------------------------------
G4eManager::G4eManager()
{
  //----- Initialize a few things
  //o  theG4eManager = this;

  char* g4emverb = getenv("G4EVERBOSE");
  if( !g4emverb ) {
    theVerbosity = 0;
  } else {
    theVerbosity = atoi( g4emverb );
  }

  thePropagator = 0;

  theEquationOfMotion = 0;

  StartG4eRunManagerKernel(); 

  theState = G4eState_PreInit;

  theG4eNavigator = 0;

  StartNavigator(); //navigator has to be initialized at the beggining !?!?!


}

//-----------------------------------------------------------------------
G4eManager::~G4eManager()
{
}


//-----------------------------------------------------------------------
void G4eManager::StartG4eRunManagerKernel()
{
  //----- Initialize G4eRunManagerKernel
  theG4eRunManagerKernel = G4eRunManagerKernel::GetRunManagerKernel();

  if( theG4eRunManagerKernel == 0 ) {
    theG4eRunManagerKernel = new G4eRunManagerKernel();
  }

  //----- User Initialization classes 
  //--- GEANT4e PhysicsList
  if( theVerbosity >= 4 ) G4cout << " G4eManager::StartG4eRunManagerKernel() done " << G4endl;
  //-  theG4eRunManager->SetUserInitialization(new G4ePhysicsList);

}


//-----------------------------------------------------------------------
void G4eManager::StartNavigator()
{
  if( theG4eNavigator == 0 ) {
    G4TransportationManager*transportationManager = G4TransportationManager::GetTransportationManager();
    
    G4Navigator* g4navi = transportationManager->GetNavigatorForTracking();
   
    G4VPhysicalVolume* world = g4navi->GetWorldVolume();
    int verb = g4navi->GetVerboseLevel();
    delete g4navi;

    theG4eNavigator = new G4eNavigator();

    if( world != 0 ) {
      theG4eNavigator->SetWorldVolume( world );
    }
    theG4eNavigator->SetVerboseLevel( verb );   
    
    transportationManager->SetNavigatorForTracking(theG4eNavigator);
    //  G4ThreeVector center(0,0,0);
    //  theG4eNavigator->LocateGlobalPointAndSetup(center,0,false);
    
  }

  if( theVerbosity >= 2 ) G4cout << " theState at StartNavigator " << PrintG4eState() << G4endl;

}
  

//-----------------------------------------------------------------------
void G4eManager::InitGeant4e()
{
  if( theVerbosity >= 1 ) G4cout << "InitGeant4e GEANT4e State= " << PrintG4eState() << " GEANT4 State= " << PrintG4State() << G4endl;
  G4ApplicationState currentState = G4StateManager::GetStateManager()->GetCurrentState();
  //----- Initialize run
  //  if( G4StateManager::GetStateManager()->GetCurrentState() == G4State_PreInit) {
  if( theState == G4eState_PreInit ) {
    if ( currentState == G4State_PreInit || currentState == G4State_Idle) {
      //    G4eRunManager::GetRunManager()->Initialize();
      theG4eRunManagerKernel->InitializeGeometry();
      theG4eRunManagerKernel->InitializePhysics();
    }
    
    InitFieldForBackwards();
    
    //-    G4StateManager::GetStateManager()->SetNewState(G4State_Idle);
    
    if( theVerbosity >= 4 )   G4cout << " bef  theG4eManager->RunInitialization() " <<  G4StateManager::GetStateManager()->GetCurrentState() << G4endl;
    theG4eRunManagerKernel->RunInitialization();
    if( theVerbosity >= 4 ) G4cout << " aft  theG4eManager->RunInitialization() " <<  G4StateManager::GetStateManager()->GetCurrentState() << G4endl;
    
    if( !thePropagator ) thePropagator = new G4ePropagatorG4();  // currently the only propagator possible
    
    InitTrackPropagation();
  } else {
    G4cerr << "G4eManager::InitGeant4e: Illegal application state - "
	   << "G4eManager::InitGeant4e() ignored." << G4endl;
    G4cerr << " GEANT4e State= " << PrintG4eState() 
      //<< " GEANT4 State= " <<  PrintG4State() 
	   << G4endl;
  }
  
  //----- Set the tracking geometry for this propagation
  //t  SetTrackingGeometry();
  //----- Set the physics list for this propagation
  //t  SetPhysicsList();
  //----- Set the field propagation parameters for this propagation
  //t  SetFieldPropagationParameters();
  SetState( G4eState_Init );

  if( theVerbosity >= 2 ) G4cout << "End InitGeant4e GEANT4e State= " << PrintG4eState() << " GEANT4 State= " << PrintG4State() << G4endl;


}


//-----------------------------------------------------------------------
void G4eManager::InitTrackPropagation()
{
  thePropagator->SetStepN( 0 );

  SetState( G4eState_Propagating );

}

//-----------------------------------------------------------------------
bool G4eManager::InitFieldForBackwards()
{

  if( theVerbosity >= 4 ) G4cout << " G4eManager::InitFieldForBackwards() " << G4endl;
  //----- Gets the current equation of motion
  G4FieldManager* fieldMgr= G4TransportationManager::GetTransportationManager()->GetFieldManager();
  //  G4cout << " fieldMgr " << fieldMgr << G4endl;
  //  if( !fieldMgr ) return 0;

  //  G4Field* myfield = fieldMgr->GetDetectorField();
  G4ChordFinder* cf = fieldMgr ->GetChordFinder();
  G4cout << " cf " <<cf << G4endl;
  if( !cf ) return 0;
  G4MagInt_Driver* mid = cf->GetIntegrationDriver();
  G4cout << " mid " << mid << G4endl;
  if( !mid ) return 0;
  G4MagIntegratorStepper* stepper = const_cast<G4MagIntegratorStepper*>(mid->GetStepper());
  G4cout << " stepper " << stepper << G4endl;
  if( !stepper ) return 0;
  G4EquationOfMotion* equation = stepper->GetEquationOfMotion();
  G4cout << " equation " << equation << G4endl;

  //----- Replaces the equation by a G4eMag_UsualEqRhs to handle backwards tracking
  if ( !dynamic_cast<G4eMag_UsualEqRhs*>(equation) ) {

    G4MagneticField* myfield = (G4MagneticField*)fieldMgr->GetDetectorField();
  G4cout << " myfield " << myfield << G4endl;
    
    //    G4Mag_UsualEqRhs* fEquation_usual = dynamic_cast<G4Mag_UsualEqRhs*>(equation);
    if( theEquationOfMotion == 0 ) theEquationOfMotion = new G4eMag_UsualEqRhs(myfield);
 
    //---- Pass the equation of motion to the G4MagIntegratorStepper
//    stepper->SetEquationOfMotion( theEquationOfMotion );
    stepper->fEquation_Rhs = theEquationOfMotion;

    //--- change stepper for speed tests
   G4MagIntegratorStepper* g4eStepper = new G4ClassicalRK4(theEquationOfMotion);
   // G4MagIntegratorStepper* g4eStepper = new G4ExactHelixStepper(theEquationOfMotion);
    
    //---- 
    G4MagneticField* field = static_cast<G4MagneticField*>(const_cast<G4Field*>(fieldMgr->GetDetectorField()));
    G4ChordFinder* pChordFinder = new G4ChordFinder(field, 1.0e-2*mm, g4eStepper);

    fieldMgr->SetChordFinder(pChordFinder);

  }

  return 1;
}


//-----------------------------------------------------------------------
int G4eManager::Propagate( G4eTrajState* currentTS, const G4eTarget* target, G4eMode mode )
{
  thePropagationMode = mode;
  if( !thePropagator ) thePropagator = new G4ePropagatorG4();  // currently the only propagator possible

  return thePropagator->Propagate( currentTS, target, mode );
}


//-----------------------------------------------------------------------
int G4eManager::PropagateOneStep( G4eTrajState* currentTS, G4eMode mode )
{
  thePropagationMode = mode;

  if( !thePropagator ) thePropagator = new G4ePropagatorG4();  // currently the only propagator possible

  return thePropagator->PropagateOneStep( currentTS );
}

//---------------------------------------------------------------------------
void G4eManager::InvokePreUserTrackingAction( G4Track* fpTrack )
{
  const G4UserTrackingAction* fpUserTrackingAction = G4EventManager::GetEventManager()->GetUserTrackingAction();
  if( fpUserTrackingAction != NULL ) {
  const_cast<G4UserTrackingAction*>(fpUserTrackingAction)->PreUserTrackingAction((fpTrack) );
  }

}


//---------------------------------------------------------------------------
void G4eManager::InvokePostUserTrackingAction( G4Track* fpTrack )
{
  const G4UserTrackingAction* fpUserTrackingAction = G4EventManager::GetEventManager()->GetUserTrackingAction();
  if( fpUserTrackingAction != NULL ) {
  const_cast<G4UserTrackingAction*>(fpUserTrackingAction)->PostUserTrackingAction((fpTrack) );
  }

}


//-----------------------------------------------------------------------
bool G4eManager::CloseGeometry()
{
  G4GeometryManager* geomManager = G4GeometryManager::GetInstance();
  geomManager->OpenGeometry();
  if(  G4StateManager::GetStateManager()->GetCurrentState() != G4State_GeomClosed) {
    G4StateManager::GetStateManager()->SetNewState(G4State_Quit);
  }

  return TRUE;
}


//---------------------------------------------------------------------------
void G4eManager::SetUserInitialization(G4VUserDetectorConstruction* userInit)
{
  theG4eRunManagerKernel->SetUserInitialization( userInit); 
}


//---------------------------------------------------------------------------
void G4eManager::SetUserInitialization(G4VPhysicalVolume* userInit)
{ 
  theG4eRunManagerKernel->SetUserInitialization( userInit); 
}
 

//---------------------------------------------------------------------------
void G4eManager::SetUserInitialization(G4VUserPhysicsList* userInit)
{ 
  theG4eRunManagerKernel->SetUserInitialization( userInit); 
}


//---------------------------------------------------------------------------
void G4eManager::SetUserAction(G4UserTrackingAction* userAction)
{
  G4EventManager::GetEventManager()->SetUserAction( userAction ); 
}


//---------------------------------------------------------------------------
void G4eManager::SetUserAction(G4UserSteppingAction* userAction)
{
  G4EventManager::GetEventManager()->SetUserAction( userAction ); 
}


//---------------------------------------------------------------------------
void G4eManager::SetSteppingManagerVerboseLevel()
{
  G4TrackingManager* trkmgr = G4EventManager::GetEventManager()->GetTrackingManager();
  trkmgr->GetSteppingManager()->SetVerboseLevel( trkmgr->GetVerboseLevel() );
}


//---------------------------------------------------------------------------
void G4eManager::EventTermination()
{
  SetState( G4eState_Init );
}


//---------------------------------------------------------------------------
void G4eManager::RunTermination()
{
  SetState( G4eState_PreInit );
  theG4eRunManagerKernel->RunTermination(); 
}


//---------------------------------------------------------------------------
G4String G4eManager::PrintG4eState() 
{
  return PrintG4eState( theState );
}


//---------------------------------------------------------------------------
G4String G4eManager::PrintG4eState( G4eState state ) 
{
  G4String nam = "";
  switch (state){
  case G4eState_PreInit: 
    nam = "G4eState_PreInit"; 
    break;
  case G4eState_Init: 
    nam = "G4eState_Init"; 
    break;
  case G4eState_Propagating:
    nam = "G4eState_Propagating";
    break;
  case G4eState_TargetCloserThanBoundary:
    nam = "G4eState_TargetCloserThanBoundary";
    break;
  case G4eState_StoppedAtTarget:
    nam = "G4eState_StoppedAtTarget";
    break;
  }

  return nam;
}


//---------------------------------------------------------------------------
G4String G4eManager::PrintG4State()
{
  return PrintG4State(G4StateManager::GetStateManager()->GetCurrentState());
}


//---------------------------------------------------------------------------
G4String G4eManager::PrintG4State( G4ApplicationState state )
{
  G4String nam = "";
  switch ( state ){
  case G4State_PreInit:
    nam = "G4State_PreInit";
    break;
  case G4State_Init:
    nam = "G4State_Init";
    break;
  case G4State_Idle:
    nam = "G4State_Idle";
    break;
  case G4State_GeomClosed:
    nam = "G4State_GeomClosed";
    break;
  case G4State_EventProc:
    nam = "G4State_EventProc"; 
    break;
  case G4State_Quit:
    nam = "G4State_Quit";
    break;
  case G4State_Abort:
    nam = "G4State_Abort";
    break;
  }
  
  return nam;

}
