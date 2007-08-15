#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsProcess.hh"
#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4eMagneticFieldLimitsProcess::G4eMagneticFieldLimitsProcess(const G4String& processName)
  : G4VDiscreteProcess (processName) 
{
  theMessenger = new G4eMagneticFieldLimitsMessenger(this);
  theStepLimit = 1000.*mm;// kInfinity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4eMagneticFieldLimitsProcess::~G4eMagneticFieldLimitsProcess()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 

G4double G4eMagneticFieldLimitsProcess::PostStepGetPhysicalInteractionLength(
								      //G4double G4eMagneticFieldLimitsProcess::PostStepGPIL(
			     const G4Track& ,
			     G4double ,
			     G4ForceCondition* condition )
{
  *condition = NotForced;
  //-  G4cout << "G4eMagneticFieldLimitsProcess::PostStepGetPhysicalInteractionLength " << theStepLimit << G4endl;
  return theStepLimit; 
  //return kInfinity;
}

G4double G4eMagneticFieldLimitsProcess::GetMeanFreePath(const class G4Track &, G4double, enum G4ForceCondition *)
{
  //-  G4cout << "G4eMagneticFieldLimitsProcess GetMeanFreePath " << theStepLimit << G4endl;
 return theStepLimit;
}

G4VParticleChange* G4eMagneticFieldLimitsProcess::PostStepDoIt(
						       const G4Track& aTrack ,
						       const G4Step& )
{
  G4ParticleChange* aParticleChange = new G4ParticleChange;
  aParticleChange->Initialize(aTrack);
  return aParticleChange; 
  
}


