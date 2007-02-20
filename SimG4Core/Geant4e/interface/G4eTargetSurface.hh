//
// class G4eTargetSurface
//
// Class description:
//
// Base class for targets that are surfaces

//
// History:
// - Created. P. Arce, September 2004

#ifndef G4eTargetSurface_h
#define G4eTargetSurface_h

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4eTargetWithTangentPlane.hh"
#include "G4Plane3D.hh"


class G4eTargetSurface : public G4eTargetWithTangentPlane
{
public:
  G4eTargetSurface(){  };
  virtual ~G4eTargetSurface(){};

public:
  virtual double GetDistanceFromPoint( const G4ThreeVector& point, const G4ThreeVector& direc ) const = 0;

  virtual double GetDistanceFromPoint( const G4ThreeVector& point ) const = 0;

  virtual G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const = 0;

  virtual void Dump( G4String msg ) const = 0;

 private:
  virtual G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const = 0;  
    // Intersects the surface with the line given by point + vector
};

#endif


