//
// class G4eMuIonisation
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
#define private public
#include "G4MuIonisation.hh"
#define private private

// class G4eManager
//
// Class description:
//
//  Manages the energy loss processes: suppresses fluctuations and takes care that energy is gained instead of lost if propagation is backwards (by calling G4eIonisationChange methods)
//
// History:
// - Created. Patricia Mendez
// - Adapted to geant4.6.2.p01: P.Arce - Aug 04
//

#ifndef G4eMuIonisation_h
#define G4eMuIonisation_h 1
#define private public
#define private private

#include "G4eIonisationChange.hh"
class G4Track;
class G4Step;


class G4eMuIonisation:public G4MuIonisation, public G4eIonisationChange 
{

public: 
  G4eMuIonisation(const G4String& name = "G4eMuIoni");
 
  ~G4eMuIonisation();

  /*   virtual std::vector<G4Track*>* SecondariesAlongStep(
                             const G4Step&,
			           G4double&,
			           G4double&,
                                   G4double&);
  */
  std::vector<G4DynamicParticle*>* SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*,
                                   G4double& tcut);
 
 
  G4VParticleChange* AlongStepDoIt( const G4Track& trackData,
  			    const G4Step&  stepData);
/* not needed becuase step is huge: this way AlongStepDoIt computes all the energy loss, if not the creation of secondaries will account for some energy loss, in a random way
  virtual G4double AlongStepGetPhysicalInteractionLength(
					 const G4Track&,
					 G4double  ,
					 G4double  ,
					 G4double& ,
					 G4GPILSelection*);
  G4double PostStepGetPhysicalInteractionLength(    
			     const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition );
*/
 
};

#endif
