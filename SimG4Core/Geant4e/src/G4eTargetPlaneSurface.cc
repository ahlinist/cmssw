#include "SimG4Core/Geant4e/interface/G4eTargetPlaneSurface.hh"
#include "G4Point3D.hh"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetPlaneSurface::G4eTargetPlaneSurface(G4double a, G4double b, G4double c, G4double d)
  : G4Plane3D( a, b, c, d ) 
{
  theType = G4eTarget_PlaneSurface;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetPlaneSurface::G4eTargetPlaneSurface(const G4Normal3D &n, const G4Point3D &p)
  : G4Plane3D( n, p ) 
{
  theType = G4eTarget_PlaneSurface;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetPlaneSurface::G4eTargetPlaneSurface(const G4Point3D &p1, const G4Point3D &p2, const G4Point3D &p3) : G4Plane3D( p1, p2, p3 )
{
  theType = G4eTarget_PlaneSurface;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//G4Point3D G4eTargetPlaneSurface::Intersect( const G4Point3D& pt, const G4ThreeVector& dir ) const
G4ThreeVector G4eTargetPlaneSurface::Intersect( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  double lam = GetDistanceFromPoint( pt, dir); 
  G4Point3D inters = pt + lam * dir;
  return inters;

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//double G4eTargetPlaneSurface::GetDistanceFromPoint( const G4Point3D& pt, const G4ThreeVector& dir ) const
G4double G4eTargetPlaneSurface::GetDistanceFromPoint( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  if( fabs( dir.mag() -1. ) > 1.E-6 ) G4cerr << "!!!WARNING  G4eTargetPlaneSurface::GetDistanceFromPoint: direction is not a unit vector " << dir << G4endl;
  double lam = -(a_ * pt.x() + b_ * pt.y() + c_ * pt.z() + d_) / 
(a_ * dir.x() + b_ * dir.y() + c_ * dir.z() );

#ifdef G4EVERBOSE
  if(G4eManager::verbose() >= 5 ) {
    G4cout << " G4eTargetPlaneSurface::getDistanceFromPoint " << lam << " point " << pt << " direc " << dir << G4endl;
    G4cout << " a_ " << a_ << " b_ " << b_ << " c_ " << c_ << " d_ " << d_ << G4endl;
  }
#endif
  
  return lam;

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4double G4eTargetPlaneSurface::GetDistanceFromPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector vec = point() - pt;
  double alpha = acos( vec * normal() / vec.mag() / normal().mag() );
  double dist = fabs(vec.mag() * cos( alpha ));
  
  return dist;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4Plane3D G4eTargetPlaneSurface::GetTangentPlane( const G4ThreeVector& ) const
{
  return *this;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void G4eTargetPlaneSurface::Dump( G4String msg ) const
{
  G4cout << msg << " point = " << point() << " normal = " << normal() << G4endl;

}
