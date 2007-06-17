
#include "SimG4Core/Geant4e/interface/G4eTargetTrackLength.hh"
#include "G4UnitsTable.hh"
#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsMessenger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"

//----------------------------------------------------------------------------
G4eTargetTrackLength::G4eTargetTrackLength(const double maxTrkLength )
  : G4VDiscreteProcess ("G4eTargetTrackLength"), theMaximumTrackLength( maxTrkLength )
{
  theType = G4eTarget_TrkL;

   G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
  // loop over all particles in G4ParticleTable
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (!particle->IsShortLived()) {
      // Add transportation process for all particles other than  "shortlived"
      if ( pmanager == 0) {
        // Error !! no process manager
        G4String particleName = particle->GetParticleName();
        G4Exception("G4VUserPhysicsList::AddTransportation","No process manager",
                    RunMustBeAborted, particleName );
      } else {
	G4ProcessVector* procvec = pmanager->GetProcessList();
	uint isiz = procvec->size();
	G4bool processAlreadyDefined = false;
	//-	G4eTargetTrackLength* tlproc = 0;
	for( uint ii=0; ii < isiz; ii++ ){
	  if( ((*procvec)[ii])->GetProcessName() == "G4eTargetTrackLength") {
	   pmanager->RemoveProcess( (*procvec)[ii] );
	   processAlreadyDefined = true;
	  }
	}
	pmanager ->AddDiscreteProcess(this);
	isiz = procvec->size();
      }
    } else {
      // shortlived particle case
    }
  }

}

//-----------------------------------------------------------------------
G4double G4eTargetTrackLength::PostStepGetPhysicalInteractionLength(
			     const G4Track& track,
			     G4double,
			     G4ForceCondition* condition )
{
  *condition = NotForced;
  return GetMeanFreePath( track, 0., condition );

}


//-----------------------------------------------------------------------
G4double G4eTargetTrackLength::GetMeanFreePath(const class G4Track & track, G4double, enum G4ForceCondition *)
{
  return theMaximumTrackLength - track.GetTrackLength();
}


/*
//-----------------------------------------------------------------------
G4VParticleChange* G4eTargetTrackLength::PostStepDoIt(
						       const G4Track& aTrack ,
						       const G4Step& )
{
  G4ParticleChange* aParticleChange = new G4ParticleChange;
  aParticleChange->Initialize(aTrack);
  return aParticleChange; 
  
}
*/


//-----------------------------------------------------------------------
void G4eTargetTrackLength::Dump( G4String msg ) const
{
  G4cout << msg << "G4eTargetTrackLength: max track length = " << theMaximumTrackLength << G4endl;

}
