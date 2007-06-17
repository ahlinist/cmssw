#include "SimG4Core/Geant4e/interface/G4eTrajState.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh"
#include "G4StateManager.hh"

#include <iomanip>


//--------------------------------------------------------------------------
G4eTrajState::G4eTrajState( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom, const G4eTrajError& errmat): fParticleType(partType), fPosition(pos), fMomentum(mom), fError(errmat)
{
  iverbose = G4eManager::verbose();

}


//--------------------------------------------------------------------------
void G4eTrajState::UpdatePosMom( const G4Point3D& pos, const G4Vector3D& mom )
{
  fPosition = pos;
  fMomentum = mom;
}

//--------------------------------------------------------------------------
void G4eTrajState::SetData( const G4String& partType, const G4Point3D& pos, const G4Vector3D& mom )
{
  fParticleType = partType;
  BuildCharge();
  fPosition = pos;
  fMomentum = mom;
}

//--------------------------------------------------------------------------
void G4eTrajState::BuildCharge()
{
  if( G4StateManager::GetStateManager()->GetCurrentState() == G4State_PreInit ) {
    G4Exception("G4eTrajState::BuildCharge() GEANT4 is not initialized yet. Try first G4eManager::GetG4eManager()->InitGeant4e()");
  }
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(fParticleType); 
  if( particle == 0) { 
    G4Exception( "!!!EXITING: G4eTrajState::BuildCharge: particle type not defined: " + fParticleType );
  }else {
    fCharge = particle->GetPDGCharge();
  }
}

//------------------------------------------------------------------------
void G4eTrajState::DumpPosMomError( std::ostream& out ) const
{
  out << *this;
}

//--------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const G4eTrajState& ts)
{
  //  long mode = out.setf(std::ios::fixed,std::ios::floatfield);
  out  
    << " G4eTrajState of type " << ts.theTSType << " : partycle: " << ts.fParticleType << "  position: " << std::setw(6) << ts.fPosition
    << "              momentum: " << ts.fMomentum
    << "   error matrix ";
  G4cout << ts.fError << G4endl;

  return out;
}

