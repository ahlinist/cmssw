//
// class G4eTargetG4Volume
//
// Class description:
//
//
//

// History:
//
// --------------------------------------------------------------------
#ifndef G4eTargetG4Volume_HH
#define G4eTargetG4Volume_HH
//
// Class description:
//
// G4eTarget class: limits step when volume is reached.
// !! NOT IMPLEMENTED YET
//
// History:
// - Created. P. Arce, September 2004

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4eTargetWithTangentPlane.hh"
#include "G4Plane3D.hh"
  
class G4Step;
class G4String;

class G4eTargetG4Volume : public G4eTargetWithTangentPlane
{
public:
  G4eTargetG4Volume( const G4String& name );
  virtual ~G4eTargetG4Volume(){};

public:
  virtual G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const = 0;  
  virtual G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const;
  virtual bool TargetReached(const G4Step* aStep);
  virtual void Dump( G4String msg ) const;

//access methods
 private:
  G4String  theName;

};

#endif


