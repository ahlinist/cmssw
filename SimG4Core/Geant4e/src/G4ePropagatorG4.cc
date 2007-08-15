#include "SimG4Core/Geant4e/interface/G4ePropagatorG4.hh"
#include "globals.hh" 
#include "G4ThreeVector.hh" 
#include "SimG4Core/Geant4e/interface/G4eManager.hh"
#include "SimG4Core/Geant4e/interface/G4eTrajStateFree.hh"
#include "SimG4Core/Geant4e/interface/G4eTrajStateOnSurface.hh"
#include "SimG4Core/Geant4e/interface/G4eTargetG4Volume.hh"
#include "SimG4Core/Geant4e/interface/G4eNavigator.hh"

#include "G4DynamicParticle.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTable.hh"
#include "G4UnitsTable.hh"
#include "G4StateManager.hh"

#include <vector>


//---------------------------------------------------------------------------
G4ePropagatorG4::G4ePropagatorG4(): G4ePropagator()
{
  verbose =  G4eManager::verbose();
#ifdef G4EVERBOSE
   if(verbose >= 5) G4cout << "G4ePropagatorG4 " << this << G4endl;
#endif
  //t  theTrackingGeometry = 0; //by default set it to 0, and when propagation it will be set to the world

  theG4Track = 0;

  fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();

  thePropIsInitialized = false;

}


//-----------------------------------------------------------------------
int G4ePropagatorG4::Propagate( G4eTrajState* currentTS, const G4eTarget* target, G4eMode mode )
{

  // to start ierror is set to 1 (= OK)
  int ierr = 1;

  G4eManager* g4emgr = G4eManager::GetG4eManager();
  if( g4emgr->GetState() == G4eState_PreInit ) g4emgr->InitGeant4e();

  g4emgr->SetSteppingManagerVerboseLevel();

  //t  if( !thePropIsInitialized ) InitPropagation( target ); // allows several propagation with same field, geometry and target  !!! IT CRASHES

  g4emgr->InitTrackPropagation();

  //--- Do not propagate zero or too low energy particles
  if( currentTS->GetMomentum().mag() < 1.E-9*MeV ) {
    G4cerr << " !! G4ePropagatorG4::Propagate: energy too low to be propagated " << G4BestUnit(currentTS->GetMomentum().mag(),"Energy") << G4endl;
    return -3; 
  }

  g4emgr->SetMode( mode );

#ifdef G4EVERBOSE
  if( verbose >= 1 ) G4cout << " =====> starting GEANT4E tracking for " << currentTS->GetParticleType() << "  Forwards= " << g4emgr->GetMode() << G4endl;
  if(verbose >= 1 ) G4cout << G4endl << "@@@@@@@@@@@@@@@@@@@@@@@@@ NEW STEP " << G4endl;

  if( verbose >= 3 ){
    G4cout << " G4ePropagatorG4::Propagate initialTS ";
    //    initialTS.Dump();
    G4cout << *currentTS << G4endl;
    target->Dump(G4String(" to target "));
  }
#endif

  g4emgr->SetTarget( target );
  //  SetTargetToNavigator( target );

  //----- Create a track
  if( theG4Track != 0 ) delete theG4Track;
  theG4Track = InitG4Track( *currentTS );

  //----- Create a G4eTrajStateFree
  G4eTrajStateFree* currentTS_FREE = InitFreeTrajState( currentTS );

  //----- Track the particle
  ierr = MakeSteps( currentTS_FREE );

  //------ Tracking ended, check if target has been reached
  // if target not found
  if( g4emgr->GetState() != G4eState_StoppedAtTarget ){
    if( theG4Track->GetKineticEnergy() > 0. ) {
      ierr = -ierr - 10;
    } else {
      ierr = -ierr - 20;
    }
    *currentTS = *currentTS_FREE;
    if(verbose >= 0 ) G4cerr << " !!ERROR G4ePropagatorG4: particle does not reach target " << *currentTS << G4endl;
  } else {
    GetFinalTrajState( currentTS, currentTS_FREE, target );
  }

#ifdef G4EVERBOSE
  if( verbose >= 1 ) G4cout << " G4ePropagator: propagation ended " << G4endl;
  if( verbose >= 2 ) G4cout << " Current TrajState " << currentTS << G4endl;
#endif

  g4emgr->InvokePostUserTrackingAction( theG4Track );  

  // Inform end of tracking to physics processes 
  theG4Track->GetDefinition()->GetProcessManager()->EndTracking();

  g4emgr->EventTermination();

  return ierr;
}


//-----------------------------------------------------------------------
int G4ePropagatorG4::PropagateOneStep( G4eTrajState* currentTS )
{
  G4eManager* g4emgr = G4eManager::GetG4eManager();
  if( g4emgr->GetState() == G4eState_PreInit ) g4emgr->InitGeant4e();

  g4emgr->SetSteppingManagerVerboseLevel();

  if( g4emgr->GetState() == G4eState_PreInit || G4StateManager::GetStateManager()->GetCurrentState() != G4State_GeomClosed) {
    //G4cout << g4emgr << " G4eState " <<  g4emgr->GetState() << " <> " << G4eState_Propagating 
    //	   << " G4State " << G4StateManager::GetStateManager()->GetCurrentState()<< " <> " << G4State_GeomClosed << G4endl;
    G4Exception("!!! G4ePropagatorG4::PropagateOneStep called before initialization is done for this track, please call G4eManager::InitGeant4e() " );
  }

  // to start ierror is set to 0 (= OK)
  int ierr = 0;

  //--- Do not propagate zero or too low energy particles
  if( currentTS->GetMomentum().mag() < 1.E-9*MeV ) {
    G4cerr << " !! G4ePropagatorG4::PropagateOneStep: energy too low to be propagated " << G4BestUnit(currentTS->GetMomentum().mag(),"Energy") << G4endl;
    return -3;   
  }

#ifdef G4EVERBOSE
  if( verbose >= 1 ) G4cout << " =====> starting GEANT4E tracking for " << currentTS->GetParticleType() << "  Forwards= " << g4emgr->GetMode() << G4endl;

  if( verbose >= 3 ){
    G4cout << " G4ePropagatorG4::Propagate initialTS ";
    G4cout << *currentTS << G4endl;
  }
#endif

  //  SetTargetToNavigator( (G4eTarget*)0 );

  //----- If it is the first step, create a track
  if( theStepN == 0 ) theG4Track = InitG4Track( *currentTS );  // set to 0 by the initialization in G4eManager
  theStepN++;

  //----- Create a G4eTrajStateFree
  G4eTrajStateFree* currentTS_FREE = InitFreeTrajState( currentTS );

  //----- Track the particle one step
  ierr = MakeOneStep( currentTS_FREE );

  //----- Get the state on target
  GetFinalTrajState( currentTS, currentTS_FREE, g4emgr->GetTarget() );

  return ierr;
}


//---------------------------------------------------------------------------
G4Track* G4ePropagatorG4::InitG4Track( G4eTrajState& initialTS )
{
  if( verbose >= 5 ) G4cout << "InitG4Track " << G4endl;

  //----- Close geometry
  //-  bool geometryToBeOptimized = true;
  //  if(verboseLevel>1)
  //-  G4GeometryManager* geomManager = G4GeometryManager::GetInstance();
  //-  geomManager->OpenGeometry();
  //- geomManager->CloseGeometry(geometryToBeOptimized);

  //----- Create Particle
  const G4String partType = initialTS.GetParticleType();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(partType); 
  if( particle == 0) { 
    G4Exception( "!!! G4eManager::InitializeTrack: particle type not defined " + partType );
  } else {

  }
 
  G4DynamicParticle* DP = 
     new G4DynamicParticle(particle,initialTS.GetMomentum());
  DP->SetPolarization(0.,0.,0.);
  // Set Charge
  //    if (abs(primaryParticle->GetCharge()-DP->GetPDGCharge())>eplus) {
  //      DP->SetCharge(primaryParticle->GetCharge());
  if( particle->GetPDGCharge() < 0 ) {
    DP->SetCharge(-eplus);
  } else {
    DP->SetCharge(eplus);
  }
  // Set decay products to the DynamicParticle
  //??    SetDecayProducts( primaryParticle, DP );
  
  //----- Create Track 
  theG4Track = new G4Track(DP, 0., initialTS.GetPosition() );
  theG4Track->SetParentID(0);
#ifdef G4EVERBOSE
  if(verbose >= 3) G4cout << " G4eManager new track of energy: " << theG4Track->GetKineticEnergy() << G4endl;
#endif
  
  //---- Reproduce G4TrackingManager::ProcessOneTrack initialization
  G4eManager* g4emgr = G4eManager::GetG4eManager();
  g4emgr->InvokePreUserTrackingAction( theG4Track );  

  if( fpSteppingManager == 0 ) {
    //    G4cerr << " event manager " << G4EventManager::GetEventManager() << G4endl;
    G4Exception( "G4ePropagator::InitG4Track.  GEANT4e error: G4SteppingManager not initialized yet " );
  } else {
    fpSteppingManager->SetInitialStep(theG4Track);
  }

  // Give SteppingManger the maximum number of processes 
  fpSteppingManager->GetProcessNumber();

  // Give track the pointer to the Step
  theG4Track->SetStep(fpSteppingManager->GetStep());

  // Inform beginning of tracking to physics processes 
  theG4Track->GetDefinition()->GetProcessManager()->StartTracking(theG4Track);

  initialTS.SetG4Track( theG4Track );

  return theG4Track;
}


//-----------------------------------------------------------------------
int G4ePropagatorG4::MakeSteps( G4eTrajStateFree* currentTS_FREE )
{
  int ierr = 0;
  //----- Track the particle Step-by-Step while it is alive
  theStepLength = 0.;
  
  while( (theG4Track->GetTrackStatus() == fAlive) ||
         (theG4Track->GetTrackStatus() == fStopButAlive) ){
    ierr = MakeOneStep( currentTS_FREE );
    if( ierr != 0 ) break;
    //----- Check if last step for GEANT4e
    if( CheckIfLastStep( theG4Track ) ) {
      if( verbose >= 5 ) G4cout << "!!!! Last Step reached " << G4endl;
      break;
    }
  }

  return ierr;

}

//-----------------------------------------------------------------------
int G4ePropagatorG4::MakeOneStep( G4eTrajStateFree* currentTS_FREE )
{
  G4eManager* g4emgr = G4eManager::GetG4eManager();
  int ierr = 0;

  //---------- Track one step
#ifdef G4EVERBOSE
  if(verbose >= 2 ) G4cout << G4endl << "@@@@@@@@@@@@@@@@@@@@@@@@@ NEW STEP " << G4endl;
#endif
  
  theG4Track->IncrementCurrentStepNumber();
  //-  G4StepStatus stepStatus = 
  fpSteppingManager->Stepping(); //t
  
  //---------- Check if Target has been reached (and then set G4eState)
#ifdef G4EVERBOSE
  if(verbose >= 5 ) G4cout << " process = " << theG4Track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << "  g4estate " << g4emgr->PrintG4eState() << G4endl;
#endif
  
  // G4eNavigator limits the step if target is closer than boundary (but the winner process is always "Transportation": then geant4e will stop the track
  if( theG4Track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation" ){
    if( g4emgr->GetState() == G4eState(G4eState_TargetCloserThanBoundary) ){  // target or step length reached
      
#ifdef G4EVERBOSE
      if(verbose >= 5 ) G4cout << " transportation determined by geant4e " << G4endl;
#endif
      
      g4emgr->SetState( G4eState_StoppedAtTarget );
      /*t      } else if( theFinalTarget->GetType() == G4eTarget_Volume ) {
	if( static_cast<G4eTargetG4Volume*>( theFinalTarget )->TargetReached( theG4Track->GetStep() ) ) {
	g4emgr->SetState( G4eState_StoppedAtTarget ); 
	} */
    }
  }else if( theG4Track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "G4eTargetTrackLength" ){
    g4emgr->SetState( G4eState_StoppedAtTarget );
  }

  /*if( g4emgr->GetState() == G4eState_StoppedAtTarget ) {
    G4cout << " Run termination " << g4emgr->GetState() << G4endl;
    g4emgr->RunTermination();
    }*/
  

  //---------- Propagate error  
#ifdef G4EVERBOSE
  if(verbose >= 2 ) G4cout << " propagating error " << *currentTS_FREE << G4endl;
#endif
  const G4Track* cTrack = const_cast<G4Track*>(theG4Track);
  ierr = currentTS_FREE->PropagateError( cTrack );
  
#ifdef G4EVERBOSE
  if(verbose >= 3 ) G4cout << " PropagateError returns " << ierr << G4endl;
#endif

  currentTS_FREE->Update( cTrack );
  
  theStepLength += theG4Track->GetStepLength();
   
  if(ierr != 0 ) {
    G4cerr << "!!! G4ePropagatorG4:MakeOneStep returns an error " << ierr << G4endl;
    G4cerr << "!!! GEANT4 tracking will be stopped " << G4endl;
  }

  return ierr; 
}


//-----------------------------------------------------------------------
G4eTrajStateFree* G4ePropagatorG4::InitFreeTrajState( G4eTrajState* currentTS )
{
  G4eTrajStateFree* currentTS_FREE;
  //----- Transform the TrajState to Free coordinates if it is OnSurface
  if( currentTS->GetTSType() == G4eTS_OS ){
    G4eTrajStateOnSurface* tssd = static_cast<G4eTrajStateOnSurface*>(currentTS);
    //t     if( theCurrentTS_FREE != 0 ) delete theCurrentTS_FREE;
    currentTS_FREE = new G4eTrajStateFree( *tssd );
  }else if( currentTS->GetTSType() == G4eTS_FREE ){
    currentTS_FREE = static_cast<G4eTrajStateFree*>(currentTS);
  } else {
    G4Exception("G4ePropagatorG4::InitFreeTrajState WRONG TrajState " + currentTS->GetTSType());
  }

  return currentTS_FREE;
}



//-----------------------------------------------------------------------
void G4ePropagatorG4::GetFinalTrajState( G4eTrajState* currentTS, G4eTrajStateFree* currentTS_FREE, const G4eTarget* target ) 
{
  G4eManager* g4emgr = G4eManager::GetG4eManager();
 
#ifdef G4EVERBOSE
  if(verbose >= 1 ) G4cout << " G4ePropagatorG4::Propagate: final state " << int(g4emgr->GetState()) << " TSType " << currentTS->GetTSType() << G4endl;
#endif

  if( currentTS->GetTSType() == G4eTS_FREE || 
      ! g4emgr->GetState() == G4eState_StoppedAtTarget ){
    currentTS = currentTS_FREE;
  } else if( currentTS->GetTSType() == G4eTS_OS ){
    if( target->GetType() == G4eTarget_TrkL ){
      G4Exception("G4ePropagatorG4:GetFinalTrajState !! Using a G4eTrajStateOnSurface with a target of type G4eTargetTrackLength ");
    }
    //-    G4eTrajStateOnSurface* tssd = static_cast<G4eTrajStateOnSurface*>(currentTS);
    //    delete currentTS;
    const G4eTargetWithTangentPlane* targetWTP = static_cast<const G4eTargetWithTangentPlane*>(target);
    *currentTS = G4eTrajStateOnSurface( *(static_cast<G4eTrajStateFree*>(currentTS_FREE)), targetWTP->GetTangentPlane( currentTS_FREE->GetPosition() ) );
#ifdef G4EVERBOSE
    if(verbose >= 1 ) G4cout << currentTS << " returning tssd " << *currentTS << G4endl;
#endif
    delete currentTS_FREE;
  }
}


//-------------------------------------------------------------------------
G4bool G4ePropagatorG4::CheckIfLastStep( G4Track* aTrack )
{
  G4bool exception = 0;
  G4bool lastG4eStep = false;
  G4eManager* g4emgr = G4eManager::GetG4eManager();

  if( verbose >= 4 ) G4cout << " G4ePropagatorG4::CheckIfLastStep G4eState= " << int(g4emgr->GetState()) << G4endl;
  
  //----- Check if this is the last step: track has reached the target or the end of world
  if(g4emgr->GetState() == G4eState(G4eState_StoppedAtTarget) ) {
    lastG4eStep = true;    
#ifdef G4EVERBOSE
    if(verbose >= 5 ) G4cout << " G4ePropagatorG4::CheckIfLastStep " << lastG4eStep << " " << int(g4emgr->GetState()) << G4endl;
#endif
  } else if( aTrack->GetNextVolume() == 0 ) {
    //----- If particle is out of world, without finding the G4eTarget, give a n error/warning
    lastG4eStep = true;
    if( exception ){
      G4cerr << " !!!EXITING:  G4ePropagatorG4::CheckIfLastSte: track extrapolated until end of World without finding the defined target " << G4endl;
      exit(1);
    } else {
      if( verbose >= 1 ) G4cerr << " !!!WARNING:  G4ePropagatorG4::CheckIfLastSte: track extrapolated until end of World without finding the defined target " << G4endl;
    }
  //----- not last step from G4e, but track is stopped (energy exhausted)
  } else if( aTrack->GetTrackStatus() == fStopAndKill ) { 
    if( exception ){
      G4cerr << " !!!EXITING:  G4ePropagatorG4::CheckIfLastSte: track extrapolated until energy is exhausted without finding the defined target " << G4endl;
      exit(1);
    } else {
      if( verbose >= 1 ) G4cerr << " !!!WARNING:  G4ePropagatorG4::CheckIfLastSte track extrapolated until energy is exhausted without finding the defined target " << G4endl;
      lastG4eStep = 1;
    }
  }

  if( verbose >= 5 ) G4cout << " return CheckIfLastStep " << lastG4eStep << G4endl;
  return  lastG4eStep;
}


//-------------------------------------------------------------------------
void G4ePropagatorG4::SetTargetToNavigator( const G4eTarget* target )
{
  G4eManager* g4emgr = G4eManager::GetG4eManager();

  if( target == 0 ){
    target = g4emgr->GetTarget();
  }

  //  g4emgr->GetG4eNavigator()->SetTarget( target );

}

    

