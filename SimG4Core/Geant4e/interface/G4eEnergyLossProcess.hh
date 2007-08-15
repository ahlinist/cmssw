//
// class G4eEnergyLossProcess
//
// Class description:
//
// Description: Continuous Process to calcualte energy loss through G4EnergyLossForExtrapolator

// History:
//
// --------------------------------------------------------------------
//

#ifndef G4eEnergyLossProcess_h
#define G4eEnergyLossProcess_h 1

#include "G4VContinuousProcess.hh"
class G4EnergyLossForExtrapolator;

/////////////////////
// Class Definition
/////////////////////

class G4eEnergyLossProcess : public G4VContinuousProcess
{

public: 

	G4eEnergyLossProcess(const G4String& processName = "G4eEnergyLossProcess");

	~G4eEnergyLossProcess();	

public:

  G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
    // Returns true -> 'is applicable', for all charged particles.

  G4double GetContinuousStepLimit(const G4Track& aTrack,
				  G4double  ,
				  G4double  ,
				  G4double& );
    // Returns DBL_MAX as continuous step limit

  G4VParticleChange* AlongStepDoIt(const G4Track& aTrack, 
				   const G4Step&  aStep);
    // This is the method implementing the energy loss process.

private:
  static void InstantiateEforExtrapolator();
    // Create the G4EnergyLossForExtrapolator

private:
 static G4EnergyLossForExtrapolator* theELossForExtrapolator;

};

////////////////////
// Inline methods
////////////////////

inline 
G4bool G4eEnergyLossProcess::IsApplicable(const G4ParticleDefinition& aParticleType)
{
   return (aParticleType.GetPDGCharge() != 0);
}

#endif /* G4eEnergyLossProcess_h */
