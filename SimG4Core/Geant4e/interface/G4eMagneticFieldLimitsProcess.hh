//
// class G4eMagneticFieldLimitsProcess
//
// Class description:
//
//  Limits the step length if change of magnetic field is too big (user defined limit)

// History:
// - Created. P. Arce, September 2004
//
//-----------------------------------------------------------------

#ifndef G4eMagneticFieldLimitsProcess_h
#define G4eMagneticFieldLimitsProcess_h 1

#include "G4VDiscreteProcess.hh"
class G4eMagneticFieldLimitsMessenger;

//-----------------------------------------------------------------
 
class G4eMagneticFieldLimitsProcess : public G4VDiscreteProcess
{
public:  // with description
  
  G4eMagneticFieldLimitsProcess(const G4String& processName ="G4eMagneticFieldLimitsProcess");
  
  ~G4eMagneticFieldLimitsProcess();
  
  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );
    // returns the step limit

  virtual  G4double GetMeanFreePath(const class G4Track &, G4double, enum G4ForceCondition *);
    // Never called, but needed as it is an abstract method

  virtual G4VParticleChange* PostStepDoIt(
					  const G4Track& ,
					  const G4Step& );
    // No action, but retrieving the G4VParticleChange extracted from the G4Track
                             
  // Get and Set methods
  G4double GetStepLimit() const { return theStepLimit; }

  void SetStepLimit( G4double val ) {
    theStepLimit = val;
    //    G4cout << " G4eMagneticFieldLimitsProcess set theStepLimit " << theStepLimit << G4endl; 
  }

private:
  G4double theStepLimit;
  G4eMagneticFieldLimitsMessenger* theMessenger;
};

#endif
 
