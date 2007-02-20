//
// class G4eMagneticFieldLimitsMessenger
//
// Class description:
//
// Messenger class for G4eMagneticFieldLimitsProcess
//

// History:
// 
//-----------------------------------------------------------------

#ifndef G4eMagneticFieldLimitsMessenger_h
#define G4eMagneticFieldLimitsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4eMagneticFieldLimitsProcess;

//-----------------------------------------------------------------

class G4eMagneticFieldLimitsMessenger: public G4UImessenger
{
public:
  G4eMagneticFieldLimitsMessenger(G4eMagneticFieldLimitsProcess*);
  ~G4eMagneticFieldLimitsMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:

  G4eMagneticFieldLimitsProcess* myAction;
  
  G4UIdirectory*             myDir;

  G4UIcmdWithADoubleAndUnit* StepLimitCmd;
};

#endif

