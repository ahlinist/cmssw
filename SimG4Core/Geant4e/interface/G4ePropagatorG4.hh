//
// class G4ePropagatorG4
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
//
#ifndef G4ePropagatorG4_h
#define G4ePropagatorG4_h
// 
// class G4ePropagatorG4
//
// Class description:
//
//   Manages the propagation of tracks by GEANT4. Creates a G4Track, asks GEANT4 to propagate it and takes also care to propagate the errors. Stops the track when GEANT4 stops it or a G4eTarget is reached
//
// History:
// - Created. Pedro Arce, June 2001

#include "G4ePropagator.hh"
#include "G4TrackingManager.hh"

class G4eTrajState;
class G4eErrorMatrix;
class G4Track;
class G4eTrajState;
class G4eTrajStateFree;

class G4ePropagatorG4 : public G4ePropagator
{
public:
  G4ePropagatorG4();
  virtual ~G4ePropagatorG4(){};

  G4Track* InitG4Track( G4eTrajState& initialTS );
  virtual int Propagate( G4eTrajState* currentTS, const G4eTarget* target, G4eMode mode = G4eMode_PropForwards);
  virtual int PropagateOneStep( G4eTrajState* currentTS );
  int MakeOneStep( G4eTrajStateFree* currentTS_FREE );
  // Creates theCurrentTS_FREE (transforms the user G4eTrajStateOnSurface or copies the G4eTrajStateFree)
  G4eTrajStateFree* InitFreeTrajState( G4eTrajState* currentTS );
  //--- After steps are done, convert the G4eTrajStateFree used for error propagation to the class of origin (G4eTrajStateFree or G4eTrajStatOnSurface)
  void GetFinalTrajState( G4eTrajState* currentTS, G4eTrajStateFree* currentTS_FREE, const G4eTarget* target );

private:
  int MakeSteps( G4eTrajStateFree* currentTS_FREE );

  G4bool CheckIfLastStep( G4Track* aTrack );

  void SetTargetToNavigator( const G4eTarget* target );
    // Set the target to G4eNavigator. Called at beginning of Propagate and PropagateOneStep (as user is allowed to change target to his will)

private:
  G4Track* theG4Track;

  G4SteppingManager* fpSteppingManager;

  G4int verbose;

  //t  G4VPhysicslVolume* theTrackingGeometry;
  //t  static G4VPhysicslVolume* theCurrentTrackingGeometry;

  G4bool thePropIsInitialized;

};

#endif
