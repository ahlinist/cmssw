//
#include "SimG4Core/Geant4e/interface/G4eTargetCylindricalSurface.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh" //for verbosity checking
#include "geomdefs.hh"

#include "G4Plane3D.hh"

      // Initialise a single volume, positioned in a frame which is rotated by
      // *pRot and traslated by tlate, relative to the coordinate system of the
      // mother volume pMotherLogical.
      // If pRot=0 the volume is unrotated with respect to its mother.
      // The physical volume is added to the mother's logical volume.
      // Arguments particular to G4PVPlacement:
      //   pMany Currently NOT used. For future use to identify if the volume
      //         is meant to be considered an overlapping structure, or not.
      //   pCopyNo should be set to 0 for the first volume of a given type.
      // This is a very natural way of defining a physical volume, and is
      // especially useful when creating subdetectors: the mother volumes are
      // not placed until a later stage of the assembly program.

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetCylindricalSurface::G4eTargetCylindricalSurface( const G4float& radius, const G4ThreeVector& trans, const G4RotationMatrix& rotm ): fradius(radius)
{
  theType = G4eTarget_CylindricalSurface;

  ftransform3D = G4Transform3D( rotm.inverse(), -trans );
  Dump( " $$$ creating G4eTargetCylindricalSurface ");
}

      // Additional constructor, which expects a G4Transform3D that represents 
      // the direct rotation and translation of the solid (NOT of the frame).  
      // The G4Transform3D argument should be constructed by:
      //  i) First rotating it to align the solid to the system of 
      //     reference of its mother volume *pMotherLogical, and 
      // ii) Then placing the solid at the location Transform3D.getTranslation(),
      //     with respect to the origin of the system of coordinates of the
      //     mother volume.  
      // [ This is useful for the people who prefer to think in terms 
      //   of moving objects in a given reference frame. ]
      // All other arguments are the same as for the previous constructor.

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetCylindricalSurface::G4eTargetCylindricalSurface( const G4float& radius, const G4Transform3D& trans3D): fradius(radius), ftransform3D(trans3D.inverse())
{
  theType = G4eTarget_CylindricalSurface;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4ThreeVector G4eTargetCylindricalSurface::Intersect( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);
  G4ThreeVector localDir = ftransform3D * G4Normal3D(dir);
  return IntersectLocal( localPoint, localDir );

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4ThreeVector G4eTargetCylindricalSurface::IntersectLocal( const G4ThreeVector& localPoint, const G4ThreeVector& localDir ) const
{
  G4double pointPerp = localPoint.perp();
  G4double dirPerp = localDir.perp();

  if( dirPerp == 0. ) {
    G4Exception("G4eTargetCylindricalSurface::Intersect. Direction is perpendicular to cylinder axis ");
  }

  G4double lam = (fradius - pointPerp ) / dirPerp;
  G4ThreeVector inters = localPoint + lam * localDir;

#ifdef G4EVERBOSE
  if(G4eManager::verbose() >= 4 ) { 
    G4cout << " G4eTargetCylindricalSurface::getIntersection " << inters << " " << inters.perp() << G4endl;
  } 
#endif

  return inters;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4double G4eTargetCylindricalSurface::GetDistanceFromPoint( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{

  if( dir.mag() == 0. ) {
    G4Exception("G4eTargetCylindricalSurface::GetDistanceFromPoint: direction is zero ");
  }

  //transform to local coordinates
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt) ;
  G4ThreeVector localDir = ftransform3D * G4Normal3D(dir/dir.mag());

#ifdef G4EVERBOSE
  if(G4eManager::verbose() >= 4 ) { 
    G4cout << " global pt " << pt << " dir " << dir << G4endl;
    G4cout << " transformed to local coordinates pt " << localPoint << " dir " << localDir << G4endl;
    Dump( " cylsurf " );
  }
#endif

  // If parallel to cylinder axis there is no intersection
  if( localDir.perp() == 0 ) {
    return kInfinity;
  }


  G4ThreeVector inters = IntersectLocal( localPoint, localDir );
  G4double lam = (inters - localPoint).mag();
#ifdef G4EVERBOSE
  if(G4eManager::verbose() >= 3 ) {
    G4cout << this << " G4eTargetCylindricalSurface::getDistanceFromPoint " << lam << " point " << pt << " direc " << dir << G4endl;
  }
#endif
  
  return lam;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4double G4eTargetCylindricalSurface::GetDistanceFromPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);

  return fradius - localPoint.perp();
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4Plane3D G4eTargetCylindricalSurface::GetTangentPlane( const G4ThreeVector& pt ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);

  //check that point is at cylinder surface
  if( fabs( localPoint.perp() - fradius ) > 1000.*kCarTolerance ) {
    G4cerr << " !!WARNING  G4eTargetCylindricalSurface::GetTangentPlane: point " << pt << " is not at surface, but it is distant " << localPoint.perp() - fradius << G4endl;
  }

  G4Normal3D normal = localPoint - ftransform3D.getTranslation();
  return G4Plane3D( normal ,pt );

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void G4eTargetCylindricalSurface::Dump( G4String msg ) const
{
  G4cout << msg << " radius " << fradius << " centre " <<  ftransform3D.getTranslation() << " rotation " << ftransform3D.getRotation() << G4endl;

}
