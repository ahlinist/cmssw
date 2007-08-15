//
// class G4ePropagator
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
#ifndef G4ePropagator_h
#define G4ePropagator_h
// 
// class G4ePropagator
//
// Class description:
//   Base classes of propagators 
//
// History:
// - Created. Pedro Arce, June 2001

class G4eTrajState;
class G4eTarget;

#include "G4eManager.hh"
#include "globals.hh"

class G4ePropagator
{
public:
  G4ePropagator(){};
  virtual ~G4ePropagator(){};

  virtual int Propagate( G4eTrajState* currentTS, const G4eTarget* target, G4eMode mode ) = 0;
  virtual int PropagateOneStep( G4eTrajState* currentTS ) = 0;
  // Steers the GEANT4 propagation of a track:
  //  The particle will be extrapolated until theFinalTarget is reached. The final G4Track parameters will be passed to theFinalTrajState


private:
  // set and get methods 
public:
  inline const G4eTrajState* GetInitialTrajState() const;

  G4double GetStepLength() const 
  { return theStepLength; }

  void SetStepLength( const G4double sl )
  { theStepLength = sl; }

  void SetStepN( const int sn ){ 
    theStepN = sn; }

protected:
  G4double theStepLength;
  G4eTrajState* theInitialTrajState;
  G4eTrajState* theFinalTrajState;

  G4int theStepN;

};

#include "G4ePropagator.icc"

#endif

