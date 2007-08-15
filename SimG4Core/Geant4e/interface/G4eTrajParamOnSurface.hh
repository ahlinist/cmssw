// class G4eTrajParamOnSurface
//
// Class description:
//
//  Holds the 5 independent variables of the trajectory for a G4eTrajStateOnSurface object.  It is not used for anything but for printing, but anyhow it is updated everytime the position and momentum are updated 
//
// History:
// - Created. P. Arce

#ifndef G4eTrajParamOnSurface_h
#define G4eTrajParamOnSurface_h

#include "G4Point3D.hh"
#include "G4Vector3D.hh"
#include "G4Plane3D.hh"
#include "G4ThreeVector.hh"

#include "globals.hh"

class G4eTrajParamOnSurface
{
public:
  G4eTrajParamOnSurface(){};
  G4eTrajParamOnSurface( const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW );
  G4eTrajParamOnSurface( const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane );
  virtual ~G4eTrajParamOnSurface(){};

  friend
    std::ostream& operator<<(std::ostream&, const G4eTrajParamOnSurface& ts);
  
  // set and get methods 
public:
  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW );
  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane );

  G4Vector3D GetDirection() const { return fDir;}
  G4Vector3D GetPlaneNormal() const { return fVectorV.cross(fVectorW);}
  G4Vector3D GetVectorV() const { return fVectorV;}
  G4Vector3D GetVectorW() const { return fVectorW;}
  double GetPV() const{ return fPV; }
  double GetPW() const{ return fPW; }
  double GetV() const{ return fV; }
  double GetW() const{ return fW; }

private:
  G4ThreeVector fDir;
  G4Vector3D fVectorV; //one of the vectors defining the plane
  G4Vector3D fVectorW; //one of the vectors defining the plane
  double fInvP;
  double fPV;
  double fPW;
  double fV;
  double fW;

};

#endif


