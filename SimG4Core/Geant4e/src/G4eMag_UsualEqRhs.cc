//
#include "SimG4Core/Geant4e/interface/G4eMag_UsualEqRhs.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh"


void
G4eMag_UsualEqRhs::EvaluateRhsGivenB( const G4double y[],
				      const G4double B[3],
				      G4double dydx[] ) const
{

  G4Mag_UsualEqRhs::EvaluateRhsGivenB(y, B, dydx );
  
  if(G4eManager::GetG4eManager()->GetMode() == G4eMode_PropBackwards){
    G4double momentum_mag_square = sqr(y[3]) + sqr(y[4]) + sqr(y[5]);
    G4double inv_momentum_magnitude = 1.0 / sqrt( momentum_mag_square );
    
    G4double cof = FCof()*inv_momentum_magnitude;

    dydx[3] = cof*(y[4]*(-B[2]) - y[5]*(-B[1])) ;
    dydx[4] = cof*(y[5]*(-B[0]) - y[3]*(-B[2])) ;
    dydx[5] = cof*(y[3]*(-B[1]) - y[4]*(-B[0])) ;  
    
  }
  
  return ;
}





