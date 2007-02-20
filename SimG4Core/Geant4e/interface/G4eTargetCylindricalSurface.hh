//
// class G4eTargetCylindricalSurface
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
// 
// class G4eTargetSurface
//
// Class description:
//
//  G4eTarget class: limits step when track reaches this cylindrical surface
//
// History:
// - Created. P. Arce, September 2004

#ifndef G4eTargetCylindricalSurface_h
#define G4eTargetCylindricalSurface_h

#include "globals.hh"
#include "G4eTargetSurface.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Plane3D.hh"

class G4eTargetCylindricalSurface : public G4eTargetSurface
{
public:
  G4eTargetCylindricalSurface( const G4float& radius, const G4ThreeVector& trans=G4ThreeVector(), const G4RotationMatrix& rotm=G4RotationMatrix());
  G4eTargetCylindricalSurface( const G4float& radius, const G4Transform3D& trans3D);

  ~G4eTargetCylindricalSurface(){};

public:

  virtual G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  virtual G4ThreeVector IntersectLocal( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  virtual G4double GetDistanceFromPoint( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  virtual G4double GetDistanceFromPoint( const G4ThreeVector& point ) const;
  virtual G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const;
  virtual void Dump( G4String msg ) const;

 private:
  G4float fradius;
  G4Transform3D ftransform3D;

};

#endif

