// 
// class G4eNavigator
//
// Class description:
//  This class serves to do the double navigation, in the detector geometry and in the target surface, by overwriting the ComputeStep and ComputeSafety methods
//
// History:
// - Created. P. Arce
//

#ifndef G4eNavigator_H
#define G4eNavigator_H 1

#define private protected
#include "G4Navigator.hh"
#define private private

class G4eTargetSurface;
class G4edd;
#include "G4ThreeVector.hh"
class G4eTargetSurface;

#include <iostream>

class G4eNavigator : public G4Navigator
{
  public:

  G4eNavigator();
  ~G4eNavigator();
  
  G4double ComputeStep (const G4ThreeVector &pGlobalPoint,
			const G4ThreeVector &pDirection,
			const G4double pCurrentProposedStepLength,
			G4double &pNewSafety);
    // calls the navigation in the detector geometry and then checks if the distance to surface is smaller than the proposed step

  G4double ComputeSafety(const G4ThreeVector &globalpoint,
                         const G4double pProposedMaxLength = DBL_MAX);
    // calls the navigation in the detector geometry and then checks if the distance to surface is smaller than the proposed safety

private:
  //  G4edd* theTarget; // any data makes it crash??!!
};

#endif

