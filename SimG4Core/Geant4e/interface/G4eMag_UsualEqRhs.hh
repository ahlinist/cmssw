//
// class G4eMag_UsualEqRhs
//
// Class description:
//
// Serves to reverse the magnetic field when propagation is backwards
//

// History:
//
// --------------------------------------------------------------------

#ifndef G4EMAG_USUAL_EQRHS
#define G4EMAG_USUAL_EQRHS

#include "G4Mag_UsualEqRhs.hh"

class G4eMag_UsualEqRhs : public G4Mag_UsualEqRhs
{
   public:  // with description

     G4eMag_UsualEqRhs( G4MagneticField* MagField )
       : G4Mag_UsualEqRhs( MagField ) {;}
    ~G4eMag_UsualEqRhs() {;}

  void EvaluateRhsGivenB( const G4double y[],
			  const G4double B[3],
			  G4double dydx[] ) const;
    // reverses dedx if propagation is backwards
};

#endif /* G4MAG_USUAL_EQRHS */
