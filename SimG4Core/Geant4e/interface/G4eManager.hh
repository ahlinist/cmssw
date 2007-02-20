//
// class G4eManager
//
// Class description:
//
// This is the class manager of the GEANT4e package 
// It is the main interface for the user to define the setup and start the propagation 
// Initializes GEANT4 for the propagation
//
// Keeps the G4eState, that controls when the propagation has ended
// and the G4eMode, that controls if propagation is forwards or backwards
//

// History:
// - Created. Pedro Arce, February 2001
// --------------------------------------------------------------------
#ifndef G4eManager_h
#define G4eManager_h 1


#include "globals.hh"
#include "G4ApplicationState.hh"

enum G4eState {G4eState_PreInit = 1, G4eState_Init, G4eState_Propagating, G4eState_TargetCloserThanBoundary, G4eState_StoppedAtTarget};
enum G4eMode {G4eMode_PropForwards = 1, G4eMode_PropBackwards, G4eMode_PropTest};

class G4eNavigator;
class G4ePropagator;
class G4eRunManagerKernel;
class G4eTarget;
class G4eTrajState;

class G4VUserDetectorConstruction;
class G4VPhysicalVolume;
class G4VUserPhysicsList;
class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;
class G4UserSteppingAction;
class G4Mag_UsualEqRhs;
class G4Track;


class G4eManager
{
public:
  G4eManager();
    // Initialise data to 0. Starts the G4eRunManagerKernel and G4eNavigator.

public:
  ~G4eManager();

  static G4eManager* GetG4eManager();
    // Get only instance of G4eManager. If it does not exists, creates it

  void EventTermination();
    // Set state to G4eState_Init

  void RunTermination();
    // Set state to G4eState_Init and invoke G4eRunManagerKernel::RunTermination()

  void InitGeant4e();
    // Initializes Geant4 and Geant4e

  void InitTrackPropagation();
    // Set the propagator step number to 0 and the G4eState to Propagating
 
  bool InitFieldForBackwards();
   // Creates the G4eMag_UsualEqRhs, that will control backwards tracking

  int Propagate( G4eTrajState* currentTS, const G4eTarget* target, G4eMode mode = G4eMode_PropForwards );
    //invokes G4ePropagator::Propagate

  int PropagateOneStep( G4eTrajState* currentTS, G4eMode mode = G4eMode_PropForwards );
    //invokes G4ePropagator::PropagateOneStep

  void InvokePreUserTrackingAction( G4Track* fpTrack );
    // Invoke the G4UserTrackingAction::PreUserTrackingAction
  void InvokePostUserTrackingAction( G4Track* fpTrack );
    // Invoke the G4UserTrackingAction::PostUserTrackingAction

  bool CloseGeometry();
    // Close Geant4 geometry

  void SetUserInitialization(G4VUserDetectorConstruction* userInit);
    // Invokes G4eRunManagerKernel to construct detector and set world volume
  void SetUserInitialization(G4VPhysicalVolume* userInit);
    // Invokes G4eRunManagerKernel to  set world volume
   void SetUserInitialization(G4VUserPhysicsList* userInit);
    // Invokes G4eRunManagerKernel to initialize physics

  void SetUserAction(G4UserTrackingAction* userAction);
    // Invokes G4EventManager to set a G4UserTrackingAction
  void SetUserAction(G4UserSteppingAction* userAction);
    // Invokes G4EventManager to set a G4UserSteppingAction

  G4String PrintG4eState();
  G4String PrintG4eState( G4eState state );
    // print Geant4e state
  G4String PrintG4State();
  G4String PrintG4State( G4ApplicationState state );
    // print Geant4 state
 
private:
  void StartG4eRunManagerKernel();
    // Create a G4eRunManagerKernel if it does not exist and set to it the G4ePhysicsList
 
  void StartNavigator();
    // Create a G4eNavigator
 
public:
  // Set and Get methods 
  G4eRunManagerKernel* GetG4eRunManagerKernel() const;

  void SetSteppingManagerVerboseLevel();

  const G4eState GetState() const;
  void SetState( G4eState sta );

  const G4eMode GetMode() const;
  void SetMode( G4eMode mode );

  const G4eTarget* GetTarget( bool mustExist = 0) const;
  void SetTarget( const G4eTarget* target );

  static int verbose();
  static void SetVerbose( int ver );

  G4eNavigator* GetG4eNavigator() const { return theG4eNavigator; }

private:
  static G4eManager* theG4eManager;
  //--- The only instance 

  G4eRunManagerKernel* theG4eRunManagerKernel;
  //--- The G4eRunManagerKernel

  static int theVerbosity;
  //--- the verbosity for all GEANT4e classes

  //--- State of the GEANT4e tracking to the target
  G4eState theState;

  G4eMode thePropagationMode;

  G4eTarget* theFinalTarget;

  G4ePropagator* thePropagator;

  G4Mag_UsualEqRhs* theEquationOfMotion;

  G4eNavigator* theG4eNavigator;

};

#include "G4eManager.icc"

#endif

