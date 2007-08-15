
#include "SimG4Core/Geant4e/interface/G4eTrajParamOnSurface.hh"
#include "G4Track.hh" 
#include <iomanip>

//------------------------------------------------------------------------
G4eTrajParamOnSurface::G4eTrajParamOnSurface( const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW )
{
  SetParameters( pos, mom, vecV, vecW );
}

//------------------------------------------------------------------------
G4eTrajParamOnSurface::G4eTrajParamOnSurface( const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane )
{
  SetParameters( pos, mom, plane );
}


//------------------------------------------------------------------------
void G4eTrajParamOnSurface::SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane )
{
  //--- Get two perpendicular vectors: first parallel X (unless normal is parallel to X, then take Y)
  G4ThreeVector Xvec(1.,0.,0.);
  G4Vector3D vecV = -Xvec.cross(plane.normal());
  if( vecV.mag() < kCarTolerance ) {
    G4ThreeVector Zvec(0.,0.,1.);
    vecV = Zvec.cross(plane.normal());
  }

  G4Vector3D vecW = plane.normal().cross( vecV );

  SetParameters( pos, mom, vecV, vecW );
}


//------------------------------------------------------------------------
void G4eTrajParamOnSurface::SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW )
{
  if( mom.mag() > 0. ) {
    fDir = mom;
    fDir /= mom.mag();
  } else {
    fDir = G4Vector3D(0.,0.,0.);
  }
  fVectorV = vecV / vecV.mag();
  fVectorW = vecW / vecW.mag();
  fInvP = 1./mom.mag();
  G4ThreeVector posv(pos);
  //check 3 vectors are ortogonal and right handed

  fPV = mom*vecV;
  fPW = mom*vecW;

  fV = pos*vecV;
  fW = pos*vecW;

}


//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const G4eTrajParamOnSurface& tp)
{
  //  long mode = out.setf(std::ios::fixed,std::ios::floatfield);
  
  //  out << tp.theType;
  //  out << std::setprecision(5) << std::setw(10);
  out << " InvP= " << tp.fInvP << " PV= " << tp.fPV << " PW= " << tp.fPW << " V= " << tp.fV << " W= " << tp.fW << G4endl;
  out << " vectorV direction= " << tp.fVectorV << " vectorW direction= " << tp.fVectorW << G4endl;
    
  return out;
}
