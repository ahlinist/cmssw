#include "SimG4Core/Geant4e/interface/G4eTargetG4Volume.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh" //for verbosity checking
#include "G4Point3D.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4eTargetG4Volume::G4eTargetG4Volume( const G4String& name )
{
  theType = G4eTarget_G4Volume;
  theName = name; 
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bool G4eTargetG4Volume::TargetReached( const G4Step* aStep )
{
  if( aStep->GetTrack()->GetNextVolume()->GetName() == theName ){
    return 1;
  }else {
    return 0;
  }
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G4Plane3D G4eTargetG4Volume::GetTangentPlane( const G4ThreeVector& point ) const
{
  return G4Plane3D( G4Normal3D(1,0.,0), G4Point3D(0,0,0) );
  //  return SurfaceNormal*this;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void G4eTargetG4Volume::Dump( G4String msg ) const
{
  //  G4cout << msg << " point = " << point() << " normal = " << normal() << G4endl;

}
