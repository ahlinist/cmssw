//
// class G4eTargetPlaneSurface
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
// class G4eTargetPlaneSurface
//
// Class description:
//
// G4eTarget class: limits step when track reaches this plane surface
//
//
// History:
// - Created. P. Arce, September 2004

#ifndef G4eTargetPlaneSurface_h
#define G4eTargetPlaneSurface_h

#include "globals.hh"
#include "G4eTargetSurface.hh"
#include "G4ThreeVector.hh"
#include "G4Plane3D.hh"

class G4eTargetPlaneSurface : public G4eTargetSurface, G4Plane3D
{
public:
  G4eTargetPlaneSurface(G4double a=0, G4double b=0, G4double c=0, G4double d=0);
  G4eTargetPlaneSurface(const G4Normal3D &n, const G4Point3D &p);
  G4eTargetPlaneSurface(const G4Point3D &p1, const G4Point3D &p2, const G4Point3D &p3);

  ~G4eTargetPlaneSurface(){};

public:
  virtual G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
    // Intersects the surface with the line given by point + vector

  virtual double GetDistanceFromPoint( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
    // Gets distance from point to surface in the direction of direc

  virtual double GetDistanceFromPoint( const G4ThreeVector& pt ) const;
    // Gets closest distance from point to surface

  virtual G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const;
    // Returns tangent plane as itself

  virtual void Dump( G4String msg ) const;

 private:

};

#endif

