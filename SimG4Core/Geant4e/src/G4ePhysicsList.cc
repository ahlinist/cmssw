// no decay
// no annihilation

#include "globals.hh"
#include "G4eMuIonisation.hh"
#include "SimG4Core/Geant4e/interface/G4ePhysicsList.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
 
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip.h>                
#include "G4PhysicsTable.hh"
#include "G4Transportation.hh"

//#include "G4ePhysListEmModel.hh"
#include "SimG4Core/Geant4e/interface/G4eEnergyLossProcess.hh"

G4ePhysicsList::G4ePhysicsList():  G4VUserPhysicsList()
{
  defaultCutValue = 1.0E+9*cm; // set big step so that AlongStep computes all the energy
}

G4ePhysicsList::~G4ePhysicsList()
{
}

void G4ePhysicsList::ConstructParticle()
{
// In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 
  //  gamma
  G4Gamma::GammaDefinition(); 
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // mu+/-
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

}

void G4ePhysicsList::ConstructProcess()
{
  G4Transportation* theTransportationProcess= new G4Transportation();

#ifdef G4VERBOSE
    if (verboseLevel >= 4){
      G4cout << "G4VUserPhysicsList::ConstructProcess()  "<< G4endl;
    }
#endif

  // loop over all particles in G4ParticleTable
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (!particle->IsShortLived()) {
      G4cout << particle << "G4ePhysicsList:: particle process manager " << particle->GetParticleName() << " = " << particle->GetProcessManager() << G4endl;
      // Add transportation process for all particles other than  "shortlived"
      if ( pmanager == 0) {
	// Error !! no process manager
	G4String particleName = particle->GetParticleName();
	G4Exception("G4VUserPhysicsList::AddTransportation","No process manager",
		    RunMustBeAborted, particleName );
      } else {
	// add transportation with ordering = ( -1, "first", "first" )
	pmanager ->AddProcess(theTransportationProcess);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxAlongStep);
	pmanager ->SetProcessOrderingToFirst(theTransportationProcess, idxPostStep);
      }
    } else {
      // shortlived particle case
    }
  }

  ConstructEM();
}

#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"

#include "G4eIonisation.hh"

#include "G4MuBremsstrahlung.hh"
#include "G4MuIonisation.hh"
#include "G4MuPairProduction.hh"

//#include "G4eMuIonisation"

#include "G4PhysicsTable.hh"

#include "G4VeEnergyLoss.hh"
#include "G4VEnergyLoss.hh"
#include "G4VMuEnergyLoss.hh"

#include "G4MuIonisation.hh"

#include "SimG4Core/Geant4e/interface/G4eMagneticFieldLimitsProcess.hh"

void G4ePhysicsList::ConstructEM()
{

  G4eMagneticFieldLimitsProcess* theG4eMagneticFieldLimitsProcess = new G4eMagneticFieldLimitsProcess; 

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
    // gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());      
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-"  || particleName == "e+") {
      pmanager->AddProcess(new G4eIonisation,       -1, 2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,   -1, 3,3);      

    //electron or  positron
      //      G4VProcess* theeIonisation = new G4eMuIonisation(); //!!!
      //
      // add processes
      //t      pmanager->AddProcess(theeIonisation);
      pmanager->AddDiscreteProcess( theG4eMagneticFieldLimitsProcess );     
      //
      // set ordering for AtRestDoIt
      //
      // set ordering for AlongStepDoIt
      //t      pmanager->SetProcessOrdering(theeIonisation, idxAlongStep,1);
      //      pmanager->SetProcessOrdering(theG4eMagneticFieldLimitsProcess, idxAlongStep,2);
      //
      // set ordering for PostStepDoIt
      //t      pmanager->SetProcessOrdering(theeIonisation, idxPostStep,1);
      //      pmanager->SetProcessOrdering(theG4eMagneticFieldLimitsProcess, idxPostStep,2);
  
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      
      G4bool bElossExtrap;
      char* elext = getenv("G4EELOSSEXTRAP");
      if( !elext ) {
	bElossExtrap = 1;
      } else {
	bElossExtrap = atoi( elext );
      }

      if( ! bElossExtrap ) {
	G4cout << "!! G4ePhysicsList setting old G4e physics " << G4endl;
	//muon  
	//    G4VProcess* aMultipleScattering = new G4MultipleScattering();
	G4VProcess* aBremsstrahlung     = new G4MuBremsstrahlung();
	G4VProcess* aPairProduction     = new G4MuPairProduction();
	//G4VProcess* anIonisation        = new G4MuIonisation();
	
	G4VProcess* anIonisation = new G4eMuIonisation();
	
	G4VEnergyLossProcess* io = (G4VEnergyLossProcess*)anIonisation;
	io->SetLossFluctuations( FALSE );
	// add processes
	pmanager->AddProcess(anIonisation,-1,1,1);
	//    pmanager->AddProcess(aMultipleScattering);
	pmanager->AddProcess(aBremsstrahlung,-1,2,2);
	pmanager->AddProcess(aPairProduction,-1,3,3);
	pmanager->AddDiscreteProcess( theG4eMagneticFieldLimitsProcess );      
	//
	// set ordering for AlongStepDoIt
	//    pmanager->SetProcessOrdering(aMultipleScattering, idxAlongStep,1);
	pmanager->SetProcessOrdering(anIonisation, idxAlongStep,1);
	//      pmanager->SetProcessOrdering(theG4eMagneticFieldLimitsProcess, idxAlongStep,2);
	
	//
	// set ordering for PostStepDoIt
	pmanager->SetProcessOrdering(anIonisation, idxPostStep,1);
	//      pmanager->SetProcessOrdering(theG4eMagneticFieldLimitsProcess, idxPostStep,2);
      }else {
	G4cout << "!! G4ePhysicsList setting new G4e physics (G4eEnergyLossProcess) " << G4endl;

	G4VProcess* anEloss = new G4eEnergyLossProcess();
	
	// add processes
	pmanager->AddContinuousProcess(anEloss);
	pmanager->AddDiscreteProcess( theG4eMagneticFieldLimitsProcess );      
	//
	// set ordering for AlongStepDoIt
	//	pmanager->SetProcessOrdering(anEloss, idxAlongStep,1);
	
	// set ordering for PostStepDoIt
	//	pmanager->SetProcessOrdering(theG4eMagneticFieldLimitsProcess, idxPostStep,1);
      }
    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
     // all others charged particles except geantino
      //   G4VProcess* aMultipleScattering = new G4MultipleScattering();
     G4VProcess* anIonisation        = new G4hIonisation();     
     ////G4VProcess*  theUserCuts = new G4UserSpecialCuts();
     
     //
     // add processes
     pmanager->AddProcess(anIonisation);
     //   pmanager->AddProcess(aMultipleScattering);    
     ////pmanager->AddProcess(theUserCuts);
     
     //
     // set ordering for AlongStepDoIt
     //   pmanager->SetProcessOrdering(aMultipleScattering, idxAlongStep,1);
     pmanager->SetProcessOrdering(anIonisation, idxAlongStep,1);
     
     //
     // set ordering for PostStepDoIt
     //   pmanager->SetProcessOrdering(aMultipleScattering, idxPostStep,1);
     pmanager->SetProcessOrdering(anIonisation, idxPostStep,1);
     ////pmanager->SetProcessOrdering(theUserCuts,     idxPostStep,2);
    }
  }
}


void G4ePhysicsList::SetCuts()
{
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value or all particle types 
  SetCutsWithDefault(); 
  // if (verboseLevel>0) 
  //  DumpCutValuesTable();
}

