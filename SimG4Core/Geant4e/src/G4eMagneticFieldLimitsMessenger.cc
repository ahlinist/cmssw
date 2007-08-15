
#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsMessenger.hh"
#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsProcess.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4eMagneticFieldLimitsMessenger::G4eMagneticFieldLimitsMessenger(G4eMagneticFieldLimitsProcess* myAct)
:myAction(myAct)
{ 

  myDir = new G4UIdirectory("/geant4e/");
  myDir->SetGuidance("GEANT4e control commands");
  
  StepLimitCmd = new G4UIcmdWithADoubleAndUnit("/geant4e/stepLimit",this);  
  StepLimitCmd->SetGuidance("Limit the length of an step");
  StepLimitCmd->SetDefaultUnit("mm");
  //-   StepLimitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //  G4cout << " G4eMagneticFieldLimitsMessenger::G4eMagneticFieldLimitsMessenger " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4eMagneticFieldLimitsMessenger::~G4eMagneticFieldLimitsMessenger()
{
  delete StepLimitCmd;
  delete myDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eMagneticFieldLimitsMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{   
  if( command == StepLimitCmd ) { 
    //    G4cout << " G4eMagneticFieldLimitsMessenger::SetNewValue " << newValue << G4endl;
    myAction->SetStepLimit(StepLimitCmd->GetNewDoubleValue(newValue));}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
