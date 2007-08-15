// 
// class G4eTrajStateFree
//
// Class description:
//
// Represents a free trG4eTrajState
// It can be represented by the 5 variables
//     1/p, lambda, phi, y_perp, z_perp
// where lambda and phi are the dip and azimuthal angles related
// to the momentum components in the following way:
//            p_x = p cos(lambda) cos(phi)  ! lambda = 90 - theta
//            p_y = p cos(lambda) sin(phi)
//            p_z = p sin(lambda)
// y_perp and z_perp are the coordinates of the trajectory in a
// local orthonormal reference frame with the x_perp axis along the
// particle direction, the y_perp being parallel to the x-y plane.
//
// History:
//
//-------------------------------------------------------------------
#ifndef G4eTrajStateFree_hh
#define G4eTrajStateFree_hh


#include "G4eTrajState.hh"
#include "G4eTrajParamFree.hh"

#include "G4Point3D.hh"
#include "G4Vector3D.hh"
#include "CLHEP/Matrix/Matrix.h"
class G4eTrajStateOnSurface;

class G4eTrajStateFree : public G4eTrajState
{
public:
  G4eTrajStateFree(){ }; //- ??
  G4eTrajStateFree( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4eTrajError& errmat = G4eTrajError(5,0) );
    // Construct by providing particle, position and momentum 
  G4eTrajStateFree( const G4eTrajStateOnSurface& tpOS );
    // Construct by providing G4eTrajStateOnSurface

  ~G4eTrajStateFree(){};

  virtual int Update( const G4Track* aTrack );
    // Update parameters from G4Track

  virtual int PropagateError( const G4Track* aTrack );
    // Propagate the error along the step

  virtual void Dump( std::ostream& out = G4cout ) const;
    // Dump TrajState parameters
  friend
    std::ostream& operator<<(std::ostream&, const G4eTrajStateFree& ts);

  // Set and Get methods 
  virtual void SetPosition( const G4Point3D pos ) {
    SetParameters( pos, fMomentum ); }

  virtual void SetMomentum( const G4Vector3D& mom ) {
    SetParameters( fPosition, mom ); }

  void SetParameters( const G4Point3D& pos, const G4Vector3D& mom ){
    fPosition = pos;
    fMomentum = mom;
    fTrajParam.SetParameters( pos, mom ); }

private:  
  void Init();
    // Define TS type and build charge

  int PropagateErrorMSC( const G4Track* aTrack );
    // Add the error associated to multiple scattering

  void CalculateEffectiveZandA( const G4Material* mate, double& effZ, double& effA );
    // Calculate effective Z and A (needed by PropagateErrorMSC)

  int PropagateErrorIoni( const G4Track* aTrack );
    // Add the error associated to ionization energy loss

  // Set and Get methods 
public:
  G4eTrajParamFree GetParameters() const { return fTrajParam; }

private:
  G4eTrajParamFree fTrajParam;

  CLHEP::HepMatrix theTransfMat;

  bool theFirstStep; // to count if transf mat is updated or initialized
};

#endif

