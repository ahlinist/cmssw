// 
// class G4eTargetWithTangentPlane
//
// Class description:
//
// Base class for G4eTarget classes for which a tangent plane is defined
//
// History:
// - Created. P. Arce, September 2004
#ifndef G4eTargetWithTangentPlane_HH
#define G4eTargetWithTangentPlane_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4eTarget.hh"
#include "G4Plane3D.hh"

class G4eTargetWithTangentPlane : public G4eTarget
{
public:
  G4eTargetWithTangentPlane(){};
  virtual ~G4eTargetWithTangentPlane(){};

public:
  virtual G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const = 0;

  virtual void Dump( G4String msg ) const = 0;

 private:

};

#endif


