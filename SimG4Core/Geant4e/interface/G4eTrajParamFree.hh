// 
// class G4eTrajParamFree
//
// Class description:
//
// Holds the 5 independent variables of the trajectory for a G4eTrajStateFree object. It is not used for anything but for printing, but anyhow it is updated everytime the position and momentum are updated 
//
// History:
// - Created. Pedro Arce, September 2002

#ifndef G4eTrajParamFree_h
#define G4eTrajParamFree_h

#include "G4Point3D.hh"
#include "G4Vector3D.hh"

#include "G4Track.hh"

class G4eTrajParamFree
{
public:
  G4eTrajParamFree(){};
  G4eTrajParamFree( const G4Point3D& pos, const G4Vector3D& mom );
    // Build parameters from position and momentum
  virtual ~G4eTrajParamFree(){};

  void Update( const G4Track* aTrack );
    // Update parameters from G4Track

  friend
    std::ostream& operator<<(std::ostream&, const G4eTrajParamFree& ts);
  
  // Set and Get methods 
public:
  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom );

  G4Vector3D GetDirection() const { return fDir;}

  double GetInvP() const { return fInvP; }

  double GetLambda() const { return fLambda; }

  double GetPhi() const { return fPhi; }

  double GetYPerp() const { return fYPerp; }

  double GetZPerp() const { return fZPerp; }

private:
  G4Vector3D fDir; //direction to which YPerp, ZPerp refer
  double fInvP;
  double fLambda; // 90 - theta
  double fPhi;
  double fYPerp;
  double fZPerp;

};

#endif
