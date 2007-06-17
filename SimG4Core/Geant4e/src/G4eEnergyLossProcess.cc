////////////////////////////////////////////////////////////////////////
// Energy Loss for extrapolator Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        G4eEnergyLossProcess.cc 
// Description: Continuous Process -- energy loss for extrapolator
// Version:     2.1
// Created:     2005-12-22
// Author:      Pedro Arce
// Updated:     
//
// mail:        pedro.arce@cern.ch
//
////////////////////////////////////////////////////////////////////////

#include "SimG4Core/Geant4e/interface/G4eEnergyLossProcess.hh"
#include "SimG4Core/Geant4e/interface/G4eManager.hh"
#include "G4EnergyLossForExtrapolator.hh"

//--------------------------------------------------------------------------

G4EnergyLossForExtrapolator* G4eEnergyLossProcess::theELossForExtrapolator = 0;

//--------------------------------------------------------------------------
G4eEnergyLossProcess::G4eEnergyLossProcess(const G4String& processName)
           : G4VContinuousProcess(processName)
{
  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }

  G4eEnergyLossProcess::InstantiateEforExtrapolator();

}


//--------------------------------------------------------------------------
void G4eEnergyLossProcess::InstantiateEforExtrapolator()
{

  if( theELossForExtrapolator == 0 ) {
    theELossForExtrapolator = new G4EnergyLossForExtrapolator;
  }
}


//--------------------------------------------------------------------------
G4eEnergyLossProcess::~G4eEnergyLossProcess() 
{
}


//--------------------------------------------------------------------------
G4VParticleChange*
G4eEnergyLossProcess::AlongStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);

  G4eManager* g4emgr =  G4eManager::GetG4eManager();

  G4double kinEnergyStart = aTrack.GetKineticEnergy();
  G4double step_length  = aStep.GetStepLength();
  //  step_length = 10*mm;
  const G4Material* aMaterial = aTrack.GetMaterial();
  const G4ParticleDefinition* aParticleDef = aTrack.GetDynamicParticle()->GetDefinition();
  G4double kinEnergyEnd;

  //  G4cout << " mode " << g4emgr->GetMode()<< " " << G4eMode_PropForwards  << std::endl;
 
  if( g4emgr->GetMode() == G4eMode(G4eMode_PropBackwards) ) {
    kinEnergyEnd = G4eEnergyLossProcess::theELossForExtrapolator->EnergyBeforeStep( kinEnergyStart, step_length, aMaterial, aParticleDef );
    G4double kinEnergyHalfStep = kinEnergyStart - (kinEnergyStart-kinEnergyEnd)/2.;
    //-    G4cout << " G4eEnergyLossProcess FWD  end " << kinEnergyEnd << " halfstep " << kinEnergyHalfStep << G4endl;

    //    /* b2
    //--- rescale to energy lost at 1/2 step
    kinEnergyEnd = G4eEnergyLossProcess::theELossForExtrapolator->EnergyBeforeStep( kinEnergyHalfStep, step_length, aMaterial, aParticleDef );
    kinEnergyEnd = kinEnergyStart - (kinEnergyHalfStep - kinEnergyEnd );
    //  b2 */
  }else if( g4emgr->GetMode() == G4eMode(G4eMode_PropForwards) ) {
    kinEnergyEnd = G4eEnergyLossProcess::theELossForExtrapolator->EnergyAfterStep( kinEnergyStart, step_length, aMaterial, aParticleDef );
    G4double kinEnergyHalfStep = kinEnergyStart - (kinEnergyStart-kinEnergyEnd)/2.;
    //-    G4cout << " G4eEnergyLossProcess BCKD  end " << kinEnergyEnd << " halfstep " << kinEnergyHalfStep << G4endl;

    //    /* b2
    //--- rescale to energy lost at 1/2 step
    kinEnergyEnd = G4eEnergyLossProcess::theELossForExtrapolator->EnergyAfterStep( kinEnergyHalfStep, step_length, aMaterial, aParticleDef );
    kinEnergyEnd = kinEnergyStart - (kinEnergyHalfStep - kinEnergyEnd );
    //    b2 */

  }

  G4double edepo = kinEnergyEnd - kinEnergyStart;

  if( G4eManager::verbose() >= 2 ) G4cout << "G4eEnergyLossProcess::AlongStepDoIt Estart= " << kinEnergyStart << " Eend " << kinEnergyEnd << " Ediff " << kinEnergyStart-kinEnergyEnd << " step= " << step_length << " mate= " << aMaterial->GetName() << " particle= " << aParticleDef->GetParticleName() << G4endl;

  aParticleChange.ClearDebugFlag();
  aParticleChange.ProposeLocalEnergyDeposit( edepo );
  aParticleChange.SetNumberOfSecondaries(0);
 
  aParticleChange.ProposeEnergy( kinEnergyEnd );

  /*?????
  //---- If it has lost all its energy, it is in status StopAndAlive: change it!
  if(fParticleChange->GetTrackStatus() == fStopButAlive ) {
  fParticleChange->ProposeTrackStatus( fAlive );
  }
  */	
  
  return &aParticleChange;
}


//--------------------------------------------------------------------
G4double G4eEnergyLossProcess::GetContinuousStepLimit(const G4Track&,
				   G4double,
				   G4double,
                                   G4double& )
{
  return DBL_MAX;
}
