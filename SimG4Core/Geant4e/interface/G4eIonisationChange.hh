//
// class G4eIonisationChange
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
//
// Class description:
//
//  Serves to reverse the sign of the energy lost if propagationis backwards
//
// History:
// - Created. P. Arce
// ------------------------------------------------------------
 
#ifndef G4eIonisationChange_h
#define G4eIonisationChange_h 1
 
#include "globals.hh"
class G4ParticleChangeForLoss;
class G4Track;
 
class G4eIonisationChange
{
 public:
 
  G4eIonisationChange(const G4String& processName = "GEANT4eMuIoni"); 

  ~G4eIonisationChange();

  void RecomputeParticleChange( G4ParticleChangeForLoss* fParticleChange, const G4Track& trackData);

};
 
 
#endif

