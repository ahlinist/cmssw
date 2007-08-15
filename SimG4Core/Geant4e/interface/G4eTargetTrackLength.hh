//
// class G4eTargetTrackLength
//
// Class description:
//
// G4eTarget class: limits step when track length is bigger than theMaximumTrackLength
// It is a G4VDiscreteProcess: limits the step in PostStepGetPhysicalInteractionLength
//
// History:
// - Created. P. Arce, September 2004

#ifndef G4eTargetTrackLength_h
#define G4eTargetTrackLength_h 1

#include "G4VDiscreteProcess.hh"
#include "G4eTarget.hh"

//---------------------------------------------------------------------------- 
class G4eTargetTrackLength : public G4VDiscreteProcess, public G4eTarget
{
public:  // with description

    virtual double GetDistanceFromPoint( const G4ThreeVector&, const G4ThreeVector& ) const    { return DBL_MAX; }   //for targetVolume
    virtual double GetDistanceFromPoint( const G4ThreeVector& ) const   { return DBL_MAX; }   //for targetVolume
  
  G4eTargetTrackLength(const double maxTrkLength );
    // Constructs and add this process to G4ProcessManager

  virtual ~G4eTargetTrackLength(){ };
  
  virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4double   previousStepSize,
                             G4ForceCondition* condition
                            );
    // Checks if the maximum track length has been reached

  /*
  virtual G4VParticleChange* PostStepDoIt(
					  const G4Track& ,
					  const G4Step& );

  */
                       
  virtual  G4double GetMeanFreePath(const class G4Track & track, G4double, G4ForceCondition *);
    // Mean free path = theMaximumTrackLength - track.GetTrackLength();

  void Dump( G4String msg ) const;

private:
  G4double theMaximumTrackLength;
};

 
  
#endif
 
