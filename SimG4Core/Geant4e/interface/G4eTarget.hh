//
// class G4eTarget
//
// Class description:
//
//  Base class for all targets
// 
// History:
// - Created. P. Arce, September 2004

#ifndef G4eTarget_h
#define G4eTarget_h

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4Step;

enum G4eTargetType{ G4eTarget_PlaneSurface,  G4eTarget_CylindricalSurface, G4eTarget_G4Volume, G4eTarget_TrkL };


class G4eTarget
{
public:
  G4eTarget(){};
  virtual ~G4eTarget(){};

public:
    virtual double GetDistanceFromPoint( const G4ThreeVector&, const G4ThreeVector& ) const    { return DBL_MAX; }   //for targetVolume
    virtual double GetDistanceFromPoint( const G4ThreeVector& ) const   { return DBL_MAX; }   //for targetVolume

  virtual bool TargetReached(const G4Step*){ return 0; } //for TargetSurface and TargetTrackLength

  virtual void Dump( G4String msg ) const = 0;

//access methods
  G4eTargetType GetType() const { return theType; }

 protected:

  G4eTargetType theType;
};

#endif


