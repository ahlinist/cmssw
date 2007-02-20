//
// class G4eTrajError
//
// Class description:
//
// Trajectory error. Implemented for the moment as a CLHEP HepSymMatrix, until other possibly faster implementations are studied
//
// History:
// - Created. P. Arce
#ifndef G4eTrajError_h
#define G4eTrajError_h

#include "CLHEP/Matrix/SymMatrix.h"

typedef CLHEP::HepSymMatrix G4eTrajError;

#endif

