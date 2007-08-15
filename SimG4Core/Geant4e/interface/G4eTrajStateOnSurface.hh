//
// class G4eTrajStateOnSurface
//
// Class description:
//
// Represents a trajectory state on a surface
// It can be represented by the 5 variables
//      1/p, v', w', v, w
//  where v'=dv/du and w'=dw/du in an orthonormal coordinate system with
//  axis u, v and w
//
// History:
//
// --------------------------------------------------------------------
//
#ifndef G4eTrajStateOnSurface_hh
#define G4eTrajStateOnSurface_hh


#include "G4eTrajState.hh"
#include "G4eTrajParamOnSurface.hh"
#include "G4eTrajStateFree.hh"

#include "G4Point3D.hh"
#include "G4Vector3D.hh"
#include "G4Plane3D.hh"


class G4eTrajStateOnSurface : public G4eTrajState
{

public:
  //  G4eTrajStateOnSurface(){}; //- ??

  G4eTrajStateOnSurface( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane, const G4eTrajError& errmat = G4eTrajError(5,0) );
    // Construct by providing particle, position, momentum and G4Plane3D surface

  G4eTrajStateOnSurface( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW, const G4eTrajError& errmat = G4eTrajError(5,0) );
    // Construct by providing particle, position, momentum and two vectors on surface

  G4eTrajStateOnSurface( G4eTrajStateFree& tpSC, const G4Plane3D& plane );
    // Construct by providing G4eTrajStateFree and G4Plane3D surface

  G4eTrajStateOnSurface( G4eTrajStateFree& tpSC, const G4Vector3D& vecV, const G4Vector3D& vecW );
    // Construct by providing G4eTrajStateFree and two vectors on surface

  void BuildErrorMatrix( G4eTrajStateFree& tpSC, const G4Vector3D& vecV, const G4Vector3D& vecW );

  ~G4eTrajStateOnSurface(){};

  virtual void Dump( std::ostream& out = G4cout ) const;
    // Dump class parameters
  friend
    std::ostream& operator<<(std::ostream&, const G4eTrajStateOnSurface& ts);

  //--- Set and Get methods 
  virtual void SetPosition( const G4Point3D pos ) {
    SetParameters( pos, fMomentum, GetVectorV(), GetVectorW() ); }

  virtual void SetMomentum( const G4Vector3D& mom ) {
    SetParameters( fPosition, mom, GetVectorV(), GetVectorW() ); }

  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecV, const G4Vector3D& vecW ){
    fPosition = pos;
    fMomentum = mom;
    fTrajParam.SetParameters( pos, mom, vecV, vecW ); }

  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane ){
    fPosition = pos;
    fMomentum = mom;
    fTrajParam.SetParameters( pos, mom, plane ); }
    
  G4eTrajParamOnSurface GetParameters() const { return fTrajParam; }

  G4Vector3D GetVectorV() const { return fTrajParam.GetVectorV();}

  G4Vector3D GetVectorW() const { return fTrajParam.GetVectorW();}

private:
  void Init();
    // Define TS type and build charge

private:
  G4eTrajParamOnSurface fTrajParam;

};

#endif

