
#define private public
#include "G4VEnergyLossProcess.hh"
#define private private
#include "SimG4Core/Geant4e/interface/G4eMuIonisation.hh"
#include "G4VMuEnergyLoss.hh"
#include "SimG4Core/Geant4e/interface/G4eIonisationChange.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleChange.hh"

#include "SimG4Core/Geant4e/interface/G4eManager.hh"

G4eMuIonisation::G4eMuIonisation(const G4String& name)
  : G4MuIonisation(name)
{
}

G4eMuIonisation::~G4eMuIonisation(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
/*inline std::vector<G4Track*>* G4eMuIonisation::SecondariesAlongStep(
                           const G4Step& ,
	             	         G4double& ,
			         G4double& ,
                                 G4double& )
{
  std::vector<G4Track*>* neqwp = 0;

  return newp;
}*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4VParticleChange* G4eMuIonisation::AlongStepDoIt( const G4Track& trackData,
                                                 const G4Step&  stepData)
{  

  G4double energyStart = trackData.GetKineticEnergy();

  //  G4VParticleChange* fParticleChange = G4VEnergyLossProcessAlongStepDoIt( trackData, stepData );
  G4VParticleChange* fParticleChange = G4MuIonisation::AlongStepDoIt( trackData, stepData );
  G4ParticleChangeForLoss* fParticleChangen = static_cast<G4ParticleChangeForLoss*>(fParticleChange);

  RecomputeParticleChange( fParticleChangen, trackData );

  G4cout << " G4eMuIonisation::AlongStepDoIt1( energyEnd " << fParticleChangen->GetProposedKineticEnergy() << G4endl;

  //b2  fParticleChangen->ClearDebugFlag(); return fParticleChangen;

  //-------------------------------------
  //--- rescale to energy lost at 1/2 step

  G4double energyEnd = fParticleChangen->GetProposedKineticEnergy();
//   G4cout << " G4eMuIonisation::AlongStepDoIt( energyStart " << energyStart << " energyEnd " << energyEnd << " diff " << energyEnd-energyStart << G4endl;

  G4double energyHalfStep = energyStart - (energyStart-energyEnd)/2.;
  G4Track trknew = trackData;
  trknew.SetKineticEnergy(energyHalfStep);  

  preStepKinEnergy = energyHalfStep;
  preStepScaledEnergy = preStepKinEnergy*massRatio;

  //  G4cout << " G4eMuIonisation::AlongStepDoIt( preStepKinEnergy set " << preStepKinEnergy << " -trknew.GetKineticEnergy() " << preStepKinEnergy-trknew.GetKineticEnergy() << G4endl;

  fParticleChange = G4MuIonisation::AlongStepDoIt( trknew, stepData );
  
  fParticleChangen = static_cast<G4ParticleChangeForLoss*>(fParticleChange);
  RecomputeParticleChange( fParticleChangen, trknew );

  // G4cout << " G4eMuIonisation::AlongStepDoIt( energyEnd new " << fParticleChangen->GetProposedKineticEnergy() << " diff " << trknew.GetKineticEnergy() - fParticleChangen->GetProposedKineticEnergy() << G4endl;

  //recover the 1/2 energy substracted above
  fParticleChangen->SetProposedKineticEnergy( fParticleChangen->GetProposedKineticEnergy() + (energyStart-energyEnd)/2. );  

  //t  preStepKinEnergy = oldPreStepKinEnergy;

  //-  delete condition;
  fParticleChangen->ClearDebugFlag();

  G4cout << " G4eMuIonisation::AlongStepDoIt2( energyEnd " << fParticleChangen->GetProposedKineticEnergy() << G4endl;
  return fParticleChangen;

}


/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4eMuIonisation::AlongStepGetPhysicalInteractionLength(
					 const G4Track& track,
                             G4double previousStepSize,	
                             G4double currentMinimumStep,
                             G4double& currentSafety,
                             G4GPILSelection* selection)
{

  //  G4VContinuousDiscreteProcess::AlongStepGetPhysicalInteractionLength(

  G4MuIonisation::AlongStepGetPhysicalInteractionLength(
				      track, previousStepSize, currentMinimumStep, currentSafety, selection );

  return DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4eMuIonisation::PostStepGetPhysicalInteractionLength(
			     const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition )
{

  G4VContinuousDiscreteProcess::PostStepGetPhysicalInteractionLength(
  			      track, previousStepSize, condition );

  *condition = Forced;
  return DBL_MAX;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
inline std::vector<G4DynamicParticle*>* G4eMuIonisation::SecondariesPostStep(
                                   G4VEmModel*,
                             const G4MaterialCutsCouple*,
                             const G4DynamicParticle*,
                                   G4double&)
{
  std::vector<G4DynamicParticle*>* empty = 0;
  return empty;
}

/*
  G4double stepLengthCm = aTrack->GetStep()->GetStepLength()/cm;
  G4double DEDX2;
  if( stepLengthCm < 1.E-7 ) {
    DEDX2=0.;
  }
  //  *     Calculate xi factor (KeV).
  G4Material* mate = aTrack->GetVolume()->GetLogicalVolume()->GetMaterial();
  G4double effZ, effA;
  CalculateEffectiveZandA( mate, effZ, effA );

  G4double Etot = aTrack->GetTotalEnergy()/GeV;
  G4double beta = aTrack->GetMomentum().mag()/GeV / Etot;
  G4double mass = aTrack->GetDynamicParticle()->GetMass() / GeV;
  G4double gamma = Etot / mass;
  
  // *     Calculate xi factor (KeV).
  G4double XI = 153.5*effZ*stepLengthCm*(mate->GetDensity()/mg*mole) / 
    (effA*beta*beta);

#ifdef G4EVERBOSE
  if( iverbose >= 2 ){
    std::cout << "G4EP:IONI: XI " << XI << " beta " << beta << " gamma " << gamma << std::endl;
    std::cout << " density " << (mate->GetDensity()/mg*mole) << " effA " << effA << " step " << stepLengthCm << std::endl;
  }
#endif
  // *     Maximum energy transfer to atomic electron (KeV).
  G4double eta = beta*gamma;
  G4double etasq = eta*eta;
  G4double eMass = 0.51099906/GeV;
  G4double massRatio = eMass / mass;
  G4double F1 = 2*eMass*etasq;
  G4double F2 = 1. + 2. * massRatio * gamma + massRatio * massRatio;
  G4double Emax = 1.E+6*F1/F2;

  //  * *** and now sigma**2  in GeV
  G4double dedxSq = XI*Emax*(1.-(beta*beta/2.))*1.E-12;

}


*/
