#include "SimG4Core/Geant4e/interface/G4eTrajStateOnSurface.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh"

#include "G4Field.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "CLHEP/Matrix/Matrix.h"
#include <iomanip>


//------------------------------------------------------------------------
G4eTrajStateOnSurface::G4eTrajStateOnSurface( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4Vector3D& vecU, const G4Vector3D& vecV, const G4eTrajError& errmat) : G4eTrajState( partType, pos, mom, errmat )
{
  Init();
  fTrajParam = G4eTrajParamOnSurface( pos, mom, vecU, vecV );
}

//------------------------------------------------------------------------
G4eTrajStateOnSurface::G4eTrajStateOnSurface( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4Plane3D& plane, const G4eTrajError& errmat ): G4eTrajState( partType, pos, mom, errmat )
{
  Init();
  fTrajParam = G4eTrajParamOnSurface( pos, mom, plane );

}


//------------------------------------------------------------------------
G4eTrajStateOnSurface::G4eTrajStateOnSurface( G4eTrajStateFree& tpSC, const G4Plane3D& plane ): G4eTrajState( tpSC.GetParticleType(), tpSC.GetPosition(), tpSC.GetMomentum() )
{
  //  fParticleType = tpSC.GetParticleType();
  //  fPosition = tpSC.GetPosition();
  //  fMomentum = tpSC.GetMomentum();
  fTrajParam = G4eTrajParamOnSurface( fPosition, fMomentum, plane );
  Init();

  //----- Get the error matrix in SC coordinates
  BuildErrorMatrix( tpSC, GetVectorV(), GetVectorW() );
}

//------------------------------------------------------------------------
G4eTrajStateOnSurface::G4eTrajStateOnSurface( G4eTrajStateFree& tpSC, const G4Vector3D& vecU, const G4Vector3D& vecV ) : G4eTrajState( tpSC.GetParticleType(), tpSC.GetPosition(), tpSC.GetMomentum() )
{
  fTrajParam = G4eTrajParamOnSurface( fPosition, fMomentum, vecU, vecV );
  theTSType = G4eTS_OS;
  //----- Get the error matrix in SC coordinates
  BuildErrorMatrix( tpSC, vecU, vecV );
}


//------------------------------------------------------------------------
void G4eTrajStateOnSurface::BuildErrorMatrix( G4eTrajStateFree& tpSC, const G4Vector3D&, const G4Vector3D& )
{
  double sclambda = tpSC.GetParameters().GetLambda();
  double scphi = tpSC.GetParameters().GetPhi();
  if( G4eManager::GetG4eManager()->GetMode() == G4eMode_PropBackwards ){
    sclambda *= -1;
    scphi += M_PI;
  }
  double cosLambda = cos( sclambda );
  double sinLambda = sin( sclambda );
  double sinPhi = sin( scphi );
  double cosPhi = cos( scphi );

#ifdef G4EVERBOSE
  if( iverbose >= 4) G4cout << " PM " << fMomentum.mag() << " pLambda " << sclambda << " pPhi " << scphi << G4endl;
#endif

  G4ThreeVector vTN( cosLambda*cosPhi, cosLambda*sinPhi,sinLambda );
  G4ThreeVector vUN( -sinPhi, cosPhi, 0. );
  G4ThreeVector vVN( -vTN.z()*vUN.y(),vTN.z()*vUN.x(), cosLambda );
  
#ifdef G4EVERBOSE
  if( iverbose >= 4) std::cout << " SC2SD: vTN " << vTN << " vUN " << vUN << " vVN " << vVN << std::endl;
#endif
  double UJ = vUN*GetVectorV();
  double UK = vUN*GetVectorW();
  double VJ = vVN*GetVectorV();
  double VK = vVN*GetVectorW();


  //--- Get transformation first
  CLHEP::HepMatrix transfM(5, 5, 0 );
  //--- Get magnetic field
  const G4Field* field = G4TransportationManager::GetTransportationManager()->GetFieldManager()->GetDetectorField();

  G4Vector3D vectorU = GetVectorV().cross( GetVectorW() );

#ifdef G4EVERBOSE
  if( iverbose >= 4) std::cout << "vectors " << vectorU << " " <<  GetVectorV() << " " << GetVectorW() << std::endl;
#endif
  double T1R = 1. / ( vTN * vectorU );

  if( fCharge != 0 && field ) {
    G4double pos[3]; pos[0] = fPosition.x()*cm; pos[1] = fPosition.y()*cm; pos[2] = fPosition.z()*cm;
    G4double Hd[3];
    field->GetFieldValue( pos, Hd );
    G4ThreeVector H = G4ThreeVector( Hd[0], Hd[1], Hd[2] ) / tesla *10.;  //in kilogauus
    G4double magH = H.mag();
    G4double invP = 1./(fMomentum.mag()/GeV);
    G4double magHM = magH * invP;
    if( magH != 0. ) {
      G4double magHM2 = fCharge / magH;
      G4double Q = -magHM * c_light/ (km/ns);
#ifdef G4EVERBOSE
      if( iverbose >= 4) std::cout << GeV <<  " Q " << Q << " magHM " << magHM << " c_light/(km/ns) " << c_light/(km/ns) << std::endl;      
#endif

      G4double sinz = -H*vUN * magHM2;
      G4double cosz =  H*vVN * magHM2;
      double T3R = Q * pow(T1R,3);
      double UI = vUN * vectorU;
      double VI = vVN * vectorU;
#ifdef G4EVERBOSE
      if( iverbose >= 4) {
	G4cout << " T1R " << T1R << " T3R " << T3R << G4endl;
	G4cout << " UI " << UI << " VI " << VI << " vectorU " << vectorU << G4endl;
	G4cout << " UJ " << UJ << " VJ " << VJ << G4endl;
	G4cout << " UK " << UK << " VK " << VK << G4endl;
      }
#endif

      transfM[1][3] = -UI*( VK*cosz-UK*sinz)*T3R;
      transfM[1][4] = -VI*( VK*cosz-UK*sinz)*T3R;
      transfM[2][3] = UI*( VJ*cosz-UJ*sinz)*T3R;
      transfM[2][4] = VI*( VJ*cosz-UJ*sinz)*T3R;
    }
  }

  double T2R = T1R * T1R;
  transfM[0][0] = 1.;
  transfM[1][1] = -UK*T2R;
  transfM[1][2] = VK*cosLambda*T2R;
  transfM[2][1] = UJ*T2R;
  transfM[2][2] = -VJ*cosLambda*T2R;
  transfM[3][3] = VK*T1R;
  transfM[3][4] = -UK*T1R;
  transfM[4][3] = -VJ*T1R;
  transfM[4][4] = UJ*T1R;

#ifdef G4EVERBOSE
  if( iverbose >= 4) G4cout << " SC2SD transf matrix A= " << transfM << G4endl;
#endif
  fError = G4eTrajError( tpSC.GetError().similarity( transfM ) );

#ifdef G4EVERBOSE
  if( iverbose >= 1) std::cout << "G4EP: error matrix SC2SD " << fError << std::endl;
  if( iverbose >= 4) G4cout << "G4eTrajStateOnSurface from SC " << *this << G4endl;
#endif

}

//------------------------------------------------------------------------
void G4eTrajStateOnSurface::Init()
{
 theTSType = G4eTS_OS;
 BuildCharge();
}


//------------------------------------------------------------------------
void G4eTrajStateOnSurface::Dump( std::ostream& out ) const
{
  out << *this;
}


//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const G4eTrajStateOnSurface& ts)
{
  out.setf(std::ios::fixed,std::ios::floatfield);
  
  ts.DumpPosMomError( out );
 
  out << " G4eTrajStateOnSurface: Params: " << ts.fTrajParam << G4endl;
  return out;
}

