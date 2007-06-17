//
// --------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      History: September 2001, P. Arce
// --------------------------------------------------------------
 
#include "SimG4Core/Geant4e/interface/G4eIonisationChange.hh"

#include "G4Track.hh"
#include "G4ParticleChangeForLoss.hh"

#include "SimG4Core/Geant4e/interface/G4eManager.hh"

// constructor and destructor
 G4eIonisationChange::G4eIonisationChange(const G4String& )
{ }

G4eIonisationChange::~G4eIonisationChange() 
{ }


void G4eIonisationChange::RecomputeParticleChange( G4ParticleChangeForLoss* fParticleChange, const G4Track& trackData )
{

  //----- Reset number of secondaries to 0
  //  fParticleChange->Clear();

  //----- If propagation is backwards:
  G4eManager* g4emgr =  G4eManager::GetG4eManager();
  if( g4emgr->GetMode() == G4eMode(G4eMode_PropBackwards) ) {
#ifdef G4EVERBOSE
    if( g4emgr->verbose() >= 1) {
      G4cout << "G4eIonisationChange::RecomputeParticleChange: backwards. fParticleChange->GetProposedKineticEnergy() " << fParticleChange->GetProposedKineticEnergy() << G4endl;
    }
#endif
    //---- Reset energy deposited to negative value (gaining energy) 
    //    double eDepoOld = trackData.GetStep()->GetTotalEnergyDeposit();
    double eDepo = fParticleChange->GetLocalEnergyDeposit();
    fParticleChange->ProposeLocalEnergyDeposit( -eDepo );
    //---- Reset energy change to negative value (gaining energy) 
    double energyNew = fParticleChange->GetProposedKineticEnergy();
    double mass = trackData.GetDynamicParticle()->GetDefinition()->GetPDGMass();
    double energyOld = trackData.GetDynamicParticle()->GetTotalEnergy()-mass;
    //energyChange is =0  
    fParticleChange->SetProposedKineticEnergy( energyNew - 2*(energyNew-energyOld) );

    //---- If it has lost all its energy, it is in status StopAndAlive: change it!
    if(fParticleChange->GetTrackStatus() == fStopButAlive ) {
      fParticleChange->ProposeTrackStatus( fAlive );
    }

#ifdef G4EVERBOSE
    if( g4emgr->verbose() >= 4) {
      if( trackData.GetStep() != 0 ) G4cout << "G4eIonisationChange::RecomputeParticleChange energyDeposited new " << eDepo << " old " << trackData.GetStep()->GetTotalEnergyDeposit() << G4endl;
      G4cout << " G4eIonisationChange::RecomputeParticleChangeForIoniPost energyChange new " << energyNew << " old " << energyOld << G4endl;
    }
#endif
  }

  return;

}

