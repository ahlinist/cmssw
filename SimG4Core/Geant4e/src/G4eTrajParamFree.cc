
#include "SimG4Core/Geant4e/interface/G4eTrajParamFree.hh"
#include "globals.hh" 
#include "G4ThreeVector.hh"
#include <iomanip>

//------------------------------------------------------------------------
G4eTrajParamFree::G4eTrajParamFree( const G4Point3D& pos, const G4Vector3D& mom )
{
  SetParameters( pos, mom );
}


//------------------------------------------------------------------------
void G4eTrajParamFree::SetParameters( const G4Point3D& pos, const G4Vector3D& mom )
{
  fDir = mom;
  fInvP = 1./mom.mag();
  fLambda = 90.*deg - mom.theta();
  fPhi = mom.phi();
  G4Vector3D vxPerp(0.,0.,0.);
 if( mom.mag() > 0.) {
   vxPerp = mom/mom.mag();
 }
  G4Vector3D vyPerp = G4Vector3D( -vxPerp.y(), vxPerp.x(), 0.);
  G4Vector3D vzPerp = vxPerp.cross( vyPerp );
  // check if right handed
  //  fXPerp = pos.proj( mom );
  G4ThreeVector posv(pos);
  if( vyPerp.mag() != 0. ) {
    fYPerp = posv.project( vyPerp ).mag();
    fZPerp = posv.project( vzPerp ).mag();
  } else {
    fYPerp = 0.;
    fZPerp = 0.;
  }
}

//------------------------------------------------------------------------
void G4eTrajParamFree::Update( const G4Track* aTrack )
{
  SetParameters( aTrack->GetPosition(), aTrack->GetMomentum() );

}


//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const G4eTrajParamFree& tp)
{
  //  long mode = out.setf(std::ios::fixed,std::ios::floatfield);
  
  //  out << tp.theType;
  //  out << std::setprecision(5) << std::setw(10);
  out << std::setprecision(8) << " InvP= " << tp.fInvP << " Theta= " << tp.fLambda << " Phi= " << tp.fPhi << " YPerp= " << tp.fYPerp << " ZPerp= " << tp.fZPerp << G4endl;
  out << " momentum direction= " << tp.fDir << G4endl;
    
  return out;
}
